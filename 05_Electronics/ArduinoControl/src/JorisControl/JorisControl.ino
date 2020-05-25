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
#include <LiquidCrystal.h>
#include "Graphs.h"
#include "CircularBuffer.h"
#include "RespirationAnalysis.h"
#include "ADCReader.h"


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

#define DOUT_LCD_EN 9
#define DOUT_LCD_RS 8
#define DOUT_LCD_DB4 7
#define DOUT_LCD_DB5 6
#define DOUT_LCD_DB6 5
#define DOUT_LCD_DB7 4
#define DIN_ROTENC_A 2  // Needs to be on this pin because it needs interrupt on change
#define DIN_ROTENC_B 3  // Needs to be on this pin because it needs interrupt on change
#define DOUT_KEYS_X0 8   // These key pins are shared with the LCD
#define DOUT_KEYS_X1 7   // On all of them, a diode needs to be connected with 
#define DOUT_KEYS_X2 6   // the cathode towards the pin, and the anode towards
#define DOUT_KEYS_X3 5   // the switch and then the KEYS_Y0 pin.
#define DOUT_KEYS_X4 4
#define DIN_KEYS_Y0 10  // This pin reads the key status. It is internally pulled up.
#define DOUT_MOTOR_PWM 11 // This PWM pin is on Timer 2
#define DIN_MOTOR_PARK 12
#define DOUT_BUZZER 13

#define AIN_VSUPPLY A0
#define AIN_IMOTOR A1
#define AIN_LUNGPRES A2
#define AIN_LUNGFLOW A3 // Differential pressure to detect flow

// Free pins: A4 (=SDA) and A5 (=SCK)

LiquidCrystal lcd( DOUT_LCD_RS, DOUT_LCD_EN, DOUT_LCD_DB4, DOUT_LCD_DB5, DOUT_LCD_DB6, DOUT_LCD_DB7 );
VerticalGraph graph( lcd );
CircularBuffer circBuf;
RespirationAnalysis an;
byte ADCPins[4] = { AIN_VSUPPLY, AIN_IMOTOR, AIN_LUNGPRES, AIN_LUNGFLOW };
//ADCReader ADCR( 4, ADCPins );
ADCReader ADCR( AIN_LUNGPRES );
long next_tick_ts = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ADCR.init( 2000 ); // This sets averaging. On Arduino Uno (on 16 MHz), you will get 125 samples per 26 seconds (sorry I couldn't get a nicer fraction). That's one every 52 ms.

  lcd.begin( 20, 4 );
  lcd.noAutoscroll();
  //lcd.noCursor();
  graph.prepare();
  circBuf.init( 2, 100 );
}

void loop() {

  float VA0, p0;

  // The timing of this loop has to be based on the availability of data. Each cycle should not take longer than the sample time (which actually is an averaged sample).
  while( !ADCR.isSampleReady() ) // this also works on carry-over
    delay( 1 );
  
  long t = millis();
  long s = ADCR.getSample();
  
  VA0 = 0.004883 * s / ADCR.getAveraging();
  p0 = (VA0 - 0.2) / 0.04413;

  char fStr[12];
  char buf[40];

  float row[2];
  row[0] = t;
  row[1] = p0;
  circBuf.appendRow( row );

  an.processData( p0, 0 );

  // TODO: move all screen stuff to separate files

/*
  dtostrf( p0, 7, 2, fStr );
  sprintf( buf, "p0=%s ", fStr );
  lcd.setCursor( 10, 0 );
  lcd.print( buf );

  dtostrf( VA0, 6, 4, fStr );
  sprintf( buf, "VA0=%s ", fStr );
  lcd.setCursor( 10, 1 );
  lcd.print( buf );
*/

  float pMin = an.getPEEP();
  float pMax = an.getPP();
  float RR = an.getRR();
  float EI = an.getEI();

  dtostrf( pMin, 5, 1, fStr );
  sprintf( buf, "PEEP %s ", fStr );
  lcd.setCursor( 10, 0 );
  lcd.print( buf );

  dtostrf( pMax, 5, 1, fStr );
  sprintf( buf, "Ppl  %s ", fStr );
  lcd.setCursor( 10, 1 );
  lcd.print( buf );

  dtostrf( RR, 5, 1, fStr );
  sprintf( buf, "RR   %s ", fStr );
  lcd.setCursor( 10, 2 );
  lcd.print( buf );

  dtostrf( EI, 3, 1, fStr );
  sprintf( buf, "I/E  1:%s ", fStr );
  lcd.setCursor( 10, 3 );
  lcd.print( buf );

  Serial.println();

  float col[10];
  circBuf.getColumn( 1, -10, 10, col );

  for( int x = 0; x < 10; x++ ) {
    graph.draw( col[x], 0.0, 40.0, x, 3, 0 );
  }
/*
  const int dataWidth = 80;
  const byte graphWidth = 10;
  float col[dataWidth];
  circBuf.getColumn( 1, -dataWidth, dataWidth, col );
  
  int i = 0;
  for( int x = 0; x < graphWidth; x++ ) {
    float sum = 0;
    
    for( int n = 0; n < dataWidth/graphWidth; n++ ) {
      sum += col[i++];
    }
    float avg = sum / (dataWidth/graphWidth);
    graph.draw( avg, 0.0, 40.0, x, 3, 0 );
  }
*/
  next_tick_ts += 100;
}
