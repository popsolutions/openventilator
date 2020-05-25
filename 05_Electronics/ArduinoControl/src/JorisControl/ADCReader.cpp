/*
 * ADCReader.cpp
 * Class implementation to continuously read from the ADC and average (or actally sum) the samples, creating one sample with more resolution.
 */

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
#include "ADCReader.h"

ADCReader::ADCReader( uint8_t pin ) //  byte numPins, uint8_t ADCpins[]
{
  _pin = pin;
}

void ADCReader::init( int averaging )
// Works for at least Arduino UNO
// http://yaab-arduino.blogspot.com/2015/02/fast-sampling-from-analog-input.html
{
  _averaging = averaging;
  _growthCount = 0;       // counter of amount of ADC samples taken for a summed sample
  _growingSample = 0;     // summed sample as it is growing
  _storedSample = 0;      // final summed sample data, will be overwritten when next sample is ready
  _storedCounter = 0;     // will be increased each time a new average sample is ready
  _prevStoredCounter = 0; // used on the reading side to check if a new sample has arrived

  // Hardware setup
  ADMUX = 
  ( _pin - A0 )   // set analog input pin (0 = A0)
  | (1 << REFS0)  // set reference voltage to 5V (only S0 set, S1 cleared)
  | (0 << REFS1)  // set reference voltage to 5V (only S0 set, S1 cleared)
  | (0 << ADLAR); // if 1, left align ADC value to 8 bits from ADCH register

  // Sampling rate is [ADC clock] / [prescaler] / [conversion clock cycles]
  // For Arduino Uno ADC clock is 16 MHz and a conversion takes 13 clock cycles.
  // Prescaler 128 will yield full 10 bit resolution. With higher speeds, it will go down. Going faster than 150 kHz yields unreliable samples. Read ATmega documentation for more info.

  ADCSRB = 0;             // free running mode

  ADCSRA = 
  (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)    // 128 prescaler for 9.615 KHz
  // (1 << ADPS2) | (1 << ADPS1)    // 64 prescaler for 19.231 KHz
  // (1 << ADPS2) | (1 << ADPS0)    // 32 prescaler for 38.462 KHz
  // (1 << ADPS2)                   // 16 prescaler for 76.923 KHz
  // (1 << ADPS1) | (1 << ADPS0)    // 8 prescaler for 153.8 KHz
  | (1 << ADATE) // enable auto trigger
  | (1 << ADIE)  // enable interrupts when measurement complete
  | (1 << ADEN)  // enable ADC
  | (1 << ADSC);  // start ADC measurements
}

bool ADCReader::isSampleReady()
{
  return _storedCounter - _prevStoredCounter > 0; // also works on wrap-around
}

long ADCReader::getSample()
{
  noInterrupts();
  _prevStoredCounter ++;
  long s = _storedSample;
  interrupts();
  return s;
}

void ADCReader::addRawSample( byte channel, int a )
{
  // Should be called by ISR, and therefore have interrupts disabled
  _growingSample += a;
  _growthCount ++;
  if( _growthCount >= _averaging ) {
    _storedSample = _growingSample;
    _growingSample = 0;
    _growthCount = 0;
    _storedCounter ++;
  }
}

ISR(ADC_vect) {
// Called when new ADC value is ready
  int a = ADCL | ((int)ADCH << 8);
  ADCR.addRawSample( 0, a );
}
