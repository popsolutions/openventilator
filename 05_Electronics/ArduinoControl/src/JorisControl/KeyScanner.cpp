/*
 * KeyScanner.cpp
 * Class implementation of a scanner for keys.
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

#include "KeyScanner.h"

KeyScanner::KeyScanner( byte num_X_Pins, byte num_Y_Pins, uint8_t X_Pins[], uint8_t Y_Pins[], Key directKeys[], Key matrixKeys[] )
// Construct with a list of pins as shown above, and a list of Key codes that will be returned when the corresponding key is pressed.
// The pins nor the keys will be copied, and only a pointer is stored! So don't feed a temporary object.
:
  _num_X_Pins( num_X_Pins ), _num_Y_Pins( num_Y_Pins ), _X_Pins( X_Pins ), _Y_Pins( Y_Pins ), _directKeys( directKeys ), _matrixKeys( matrixKeys )
{
}

void KeyScanner::init()
{
  for( byte x = 0; x < _num_X_Pins; x++ ) {
    pinMode( _X_Pins[x], INPUT_PULLUP );
  }
  for( byte y = 0; y < _num_Y_Pins; y++ ) {
    pinMode( _Y_Pins[y], OUTPUT );
  }
}

Key KeyScanner::getKey()
{
  Key foundKey = KEY_NONE;
  for( byte x = 0; x < _num_X_Pins; x++ ) {
    if( !digitalRead( _X_Pins[x] )) { // if low
        foundKey = _directKeys[x];
        if( foundKey != KEY_NONE )
          break;
    }
  }
  if( foundKey == KEY_NONE ) {
    // TODO: scan matrix keys
  }
  if( foundKey == _prevKey ) {
    return KEY_NONE;
  }
  Serial.print( "Key pressed: " );
  Serial.println( foundKey );
  _prevKey = foundKey;
  return foundKey;
}
