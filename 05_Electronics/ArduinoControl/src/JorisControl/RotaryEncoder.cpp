/*
 * RotaryEncoder.cpp
 * Class implementation to track the position of a rotary encoder.
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
#include "RotaryEncoder.h"

uint8_t rotEnc_A_Pin;
uint8_t rotEnc_B_Pin;
volatile byte rotEnc_state;
volatile int rotEnc_pos;
int rotEnc_prevPos;

void rotaryEncoder_interruptHandler()
{
  byte newState = digitalRead( rotEnc_A_Pin ) | digitalRead( rotEnc_B_Pin ) << 1;
  // Convert from grey code to normal counts
  newState ^= ( ( newState & 2 ) >> 1 );
  byte change = (newState - rotEnc_state ) & 0x03;

  // Only if we haven't missed a step update the position
  if( change == 1 ) {
    rotEnc_pos ++;
  } else if( change == 3 ) {
    rotEnc_pos --;
  }
  rotEnc_state = newState;
}

RotaryEncoder::RotaryEncoder( uint8_t A_Pin, uint8_t B_Pin )
{
  rotEnc_A_Pin = A_Pin;
  rotEnc_B_Pin = B_Pin;
  rotEnc_pos = 0;
  rotEnc_state = 0;
  rotEnc_prevPos = 0;
}

void RotaryEncoder::init()
{
  pinMode( rotEnc_A_Pin, INPUT_PULLUP );
  pinMode( rotEnc_B_Pin, INPUT_PULLUP );
  attachInterrupt( digitalPinToInterrupt( rotEnc_A_Pin ), rotaryEncoder_interruptHandler, CHANGE );
  attachInterrupt( digitalPinToInterrupt( rotEnc_B_Pin ), rotaryEncoder_interruptHandler, CHANGE );
  rotEnc_pos = 0;
}

int RotaryEncoder::getPos()
{
  return rotEnc_pos;
}

void RotaryEncoder::setPos( int newPos )
{
  rotEnc_pos = newPos; // This will mess up getIncrPos, but we assume user uses either of the 2 reading methods
}

int RotaryEncoder::getIncrPos()
{
  int pos = rotEnc_pos; // read first once because it may be changed during execution
  int difference = pos - rotEnc_prevPos;
  rotEnc_prevPos = pos;
  return difference;
}
