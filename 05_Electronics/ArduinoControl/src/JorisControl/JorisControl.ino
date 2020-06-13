/*
Copyright (c) 2020, Joris Robijn
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following condition is met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Arduino.h>
#include <pins_arduino.h>
#include "BufferedLiquidCrystal.h"
#include "RotaryEncoder.h"
#include "VerticalGraph.h"
#include "RespirationAnalysis.h"
#include "CircularBuffer.h"
#include "ADCReader.h"
#include "KeyScanner.h"
#include "MainScreen.h"
#include "MenuScreen.h"
#include "MemoryFree.h"

/*
Timer output  Arduino output  Chip pin  Pin name
OC0A  6 12  PD6
OC0B  5 11  PD5
OC1A  9 15  PB1
OC1B  10  16  PB2
OC2A  11  17  PB3
OC2B  3 5 PD3

https://etechnophiles.com/change-frequency-pwm-pins-arduino-uno/
TCCR0B = TCCR0B & B11111000 | B00000011; // for PWM frequency of 976.56 Hz (The DEFAULT) Needed for millis to work correctly!
TCCR1B = TCCR1B & B11111000 | B00000011; // for PWM frequency of 490.20 Hz (The DEFAULT)
TCCR2B = TCCR2B & B11111000 | B00000100; // for PWM frequency of 490.20 Hz (The DEFAULT)
*/

#define DOUT_LCD_EN 9     // Usually pin 6 on LCD
#define DOUT_LCD_RS 8     // Usually pin 4 on LCD
#define DOUT_LCD_DB4 7    // Usually pin 11 on LCD
#define DOUT_LCD_DB5 6    // Usually pin 12 on LCD
#define DOUT_LCD_DB6 5    // Usually pin 13 on LCD
#define DOUT_LCD_DB7 4    // Usually pin 14 on LCD
#define DIN_ROTENC_A 2    // Needs to be on this pin because it needs interrupt on change
#define DIN_ROTENC_B 3    // Needs to be on this pin because it needs interrupt on change
#define DIN_KEYS_X0 12    // This pin reads the key status. It is internally pulled up.
#define DOUT_KEYS_Y0 7    // These 5 key pins are shared with the LCD
#define DOUT_KEYS_Y1 6    // On all of them, a diode needs to be connected with 
#define DOUT_KEYS_Y2 5    // the cathode towards the pin, and the anode towards
#define DOUT_KEYS_Y3 4    // the switch. The other side of the switch should be connected to the KEYS_X0 pin.
#define DOUT_KEYS_Y4 8    // Finally, the Enter switch (on the rotary encoder) is directly connected from KEY_X0 to ground.
#define DOUT_MOTOR_PWM 10 // This PWM pin is output B on Timer 1. From its two outputs, only this output is usable if we want to program the top value ourselves.
#define DIN_MOTOR_PARK 11
#define DOUT_BEEPER 13    // A beeper that will beep if it gets voltage. A GND pin is right next to pin 13.

#define AIN_VSUPPLY A0    // Detects the supply voltage with a resistor divider network of e.g. 10k and 3k3. 
                          // Values are not critical, and can be calibrated from the menu.
#define AIN_IMOTOR A1     // Detects the current through the motor from supply, with a shunt resistor on ground side of 0.05 ohm.
                          // Can be calibrated from the menu.
#define AIN_LUNGPRES A2   // Connected to MPX(V)5010 for detecting pressure.
#define AIN_LUNGFLOW A3   // Connected to MPX(V)7002 for detecting differential pressure to detect flow.
                          // If your Arduino is 3V3, you can best choose the 3V3 version of the sensors. They have a 3 in their prefix.

// Free pins: A4 (=SDA) and A5 (=SCK)

BufferedLiquidCrystal lcd( DOUT_LCD_RS, DOUT_LCD_EN, DOUT_LCD_DB4, DOUT_LCD_DB5, DOUT_LCD_DB6, DOUT_LCD_DB7 );
RotaryEncoder rotEnc( DIN_ROTENC_A, DIN_ROTENC_B ); 
VerticalGraph vgraph;
CircularBuffer circBuf;
RespirationAnalysis an;
byte ADCPins[4] = { AIN_VSUPPLY, AIN_IMOTOR, AIN_LUNGPRES, AIN_LUNGFLOW };
ADCReader ADCR( 4, ADCPins );

//#define VSUPPLYRATIO ((10000.0/3300.0)+1) // 10k and 3k3 resistor
//#define IMOTORCONDUCTANCE (1/0.04)

uint8_t X_pinList[] = { DIN_KEYS_X0 };
uint8_t Y_pinList[] = { DOUT_KEYS_Y0, DOUT_KEYS_Y1, DOUT_KEYS_Y2, DOUT_KEYS_Y3, DOUT_KEYS_Y4 };
Key directKeyList[] = { KEY_ENTER};
Key matrixKeyList[] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4 };
KeyScanner keySc( sizeof(X_pinList), sizeof(Y_pinList), X_pinList, Y_pinList, directKeyList, matrixKeyList );

long park_ts = 0;
long peak_ts = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 115200 );
  if( circBuf.init( 1, 160 ) != 0 ) {
    Serial.println( "Buffer allocation failed" );
  }
  ADCR.init( 125 ); // This sets averaging. On Arduino Uno (on 16 MHz), you will get 250 samples per 13 seconds (sorry I couldn't get a nicer fraction). That's one every 52 ms exactly.
  keySc.init();
  lcd.begin( 20, 4 );
  lcd.noAutoscroll();
  rotEnc.init();
  switchScreen( mainScreen );

  analogWrite( DOUT_MOTOR_PWM, 0 );
  OCR1A = 249;        // Program top to 249 i.o. 255
  TCCR1A = B00100011; // Only use OC1B (to Arduino pin 10) output, PWM mode 11 (this is for timer 1)
  TCCR1B = B00010001; // for PWM frequency of 32kHz, and PWM mode 11           (this is for timer 1)
  //TCCR1A = B10000001; // Only use OC2B (to Arduino pin 3), PWM mode 5        (this is for timer 2)
  //TCCR1B = B00001001; // for PWM frequency of 32kHz, and PWM mode 5          (this is for timer 2)

  pinMode( DIN_MOTOR_PARK, INPUT_PULLUP );

  Serial.print( freeMemory() );
  Serial.println( F(" bytes free") );
  
  loadSettingsFromEEPROM();
}

void get_inputs()
{
  float VA[4], Vsup, Imot, p, pQ, Q, Vmot;
  bool park;

  int a = ADCR.getAveraging();
  for( byte ch=0; ch<4; ch++ ) {
    VA[ch] = 0.004883 * ((float)ADCR.getSample( ch ) / a);
  }
  ADCR.signalSamplesRead();

  Vsup = VA[0] * settings[S_VsupFactor];
  Imot = VA[1] * settings[S_ImotShuntConductance] + settings[S_ImotOffset];
  p    = (VA[2] - 0.2) * 22.66 - settings[S_pOffset];  // assuming MPX(V)5010
  pQ   = (VA[3] - 2.5) * 10.20 - settings[S_pQoffset]; // assuming MPX(V)7002
  Q    = pQ; // TODO: determine conversion formula
  //    Vmot = ( PWM * Vsup + (250-PWM) * -Vschottky ) / 250
  // => Vmot = ( PWM * (Vsup+Vschottky) + 250 * -Vschottky ) / 250
  Vmot = ( OCR1B * (Vsup+0.4) - 100 ) / 250; // This is calculated one cycle after setting it, to have all the measured values be from the same moment in time
  park = digitalRead( DIN_MOTOR_PARK );

/*
  Serial.print( " VA[0]=" );
  Serial.print( VA[0] );
  Serial.print( " VA[1]=" );
  Serial.print( VA[1] );
*/

  // Add data to circular buffer
  float row[1];
  //row[0] = t;
  row[0] = p;
  int ar = circBuf.appendRow( row );

  // Process park switch
  if( measValues[M_Park] == 0 && park == 1 ) {
    // Park has just become high
    long new_park_ts = millis();
    measValues[M_tCycl] = ( new_park_ts - park_ts ) / 1000.0;
    park_ts = millis();
    measValues[M_Pos] = 0;
  }
  else {
    // Calculate current position
    // We know that exactly 52 ms has passed since last time
    //       position = 360 * 0.052 * motorSpeedSetpoint / 60;
    if( isnan(VmotOverrule) ) 
      measValues[M_Pos] += 0.312 * motorSpeedSetpoint;
    else
      measValues[M_Pos] = NAN; // We don't know the position when motor voltage is overruled
  }

  // Store values
  measValues[M_p]    = p;
  measValues[M_pQ]   = pQ;
  measValues[M_Q]    = Q;
  measValues[M_Vsup] = Vsup;
  measValues[M_Vmot] = Vmot;
  measValues[M_Imot] = Imot;
  measValues[M_Pmot] = Vmot * Imot;
  measValues[M_Park] = park;

  // Perform respiratory analysis with new data
  an.processData( p, 0 );
  measValues[M_pPk] = an.getPP();
  measValues[M_PEEP] = an.getPEEP();
  measValues[M_RR] = an.getRR();
  measValues[M_EI] = an.getEI();
}

void set_outputs()
{
  float Vsup = measValues[M_Vsup];

  // Calculate new speed setpoint
  motorSpeedSetpoint = settings[S_RR];

  // Is voltage overrule set?
  float Vmot = VmotOverrule;
  if( isnan(Vmot) ) {
    // No overrule, calculate normal voltage based on wanted motor speed
    Vmot = motorSpeedSetpoint / settings[S_Kv] + measValues[M_Imot] * (settings[S_Ri0] + measValues[M_Imot] * settings[S_RiIdep] );
  }
  // Calculate PWM ratio based on wanted voltage, Schottky diode voltage (0.4V) and supply voltage
  //    Vmot = ( PWM * Vsup + (250-PWM) * -Vschottky ) / 250
  // => Vmot = ( PWM * Vsup - 250 * Vschottky + PWM * Vschottky ) / 250
  // => 250 * Vmot = PWM * Vsup - 250 * Vschottky + PWM * Vschottky
  // => -PWM * Vsup = -250 * Vmot - 250 * Vschottky + PWM * Vschottky
  // =>  PWM * -Vsup = -250 * Vmot - 250 * Vschottky + PWM * Vschottky
  // =>  PWM * -Vsup - PWM * Vschottky = -250 * Vmot - 250 * Vschottky
  // =>  PWM * (-Vschottky-Vsup) = -250 * Vmot - 250 * Vschottky
  // =>  PWM  = (-250 * Vmot - 250 * Vschottky) / (-Vschottky-Vsup)
  // =>  PWM  = (250 * Vmot + 250 * Vschottky) / (Vsup+Vschottky)
  // =>  PWM = (250 * (Vmot + Vschottky)) / (Vsup + Vschottky)
  byte PWM = Vmot == 0 ? 0 : coerce_int( (249 * (Vmot + 0.4)) / (Vsup + 0.4), 0, 249 );
  analogWrite( DOUT_MOTOR_PWM, PWM );

/*
  // Debug output: current values (this takes a while, that's why it's done after setting output)
  Serial.print( " Vsup=" );
  Serial.print( measValues[M_Vsup] );
  Serial.print( " Imot=" );
  Serial.print( measValues[M_Imot] );
  Serial.print( " p=" );
  Serial.print( measValues[M_p] );
  Serial.print( " Q=" );
  Serial.print( measValues[M_Q] );
  Serial.print( " park=" );
  Serial.print( measValues[M_Park] );
  Serial.print( " Vmot=" );
  Serial.print( measValues[M_Vmot] );
  Serial.println();
*/
}

void loop() {

  // The timing of this loop has to be based on the availability of data. Each cycle should not take longer than the sample time (which actually is an averaged sample).
  while( !ADCR.areSamplesReady() ) // this also works on carry-over
    delay( 1 );

  unsigned int t = millis(); // We only use the upper 16 bits

  // Read all inputs
  get_inputs();
  
  // Set outputs (e.g. motor)
  set_outputs();

  // Let the currently active screen process a key press, dial rotation or other inputs
  activeScreen->process();

  // Let the currently active screen (which might just have changed) draw its screen
  activeScreen->draw();

  // Debug output: used time
/*  
  Serial.print( F("t end=") );
  Serial.println( (unsigned int) millis() - t );
*/
}
