/*
 * KeyScanner.h
 * Class definition of a scanner for keys.
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

/*
 * This class can scan key connected to one or multiple input pins. It can also scan a matrix of keys and a combination of these.
 * The keys that are directly connected to the input pins are called directKeys. 
 *
 * The input pins are called the X pins. DirectKeys can be connected directly from the switch to ground. The microcontroller will provide a pull-up.
 *
 *     +-----------o input  X0..Xn
 *     |
 *     |
 *     o
 *      \
 *     o
 *     |
 *     |
 *    === GND
 *
 * The keys that are connected in a matrix, to both output pins and input pins are called matrixKeys.
 * The output pins are called the Y pins. MatrixKeys kave to be connected to the Y pins with a diode. 
 *
 *                 |                           Y0 o---|<---@--@--@
 *                 |                                       |  |  |
 *   Y line ---+---------                      Y1 o---|<---@--@--@
 *             |   |            |                          |  |  |
 *             o   |       =  --@--            Y2 o---|<---@--@--@
 *              \  |            |                          |  |  |
 *             o   |                           Y3 o---|<---@--@--@
 *             |   |                                       |  |  |
 *             +---+                               diodes  |  |  |
 *                 |                               1N4148  |  |  |
 *                 |                                       o  o  o
 *             X line                                      X0 X1 X2
 *
 *
 */
 
#ifndef KEYSCANNER_H
#define KEYSCANNER_H

#include <Arduino.h>
 
typedef enum : byte { KEY_NONE=0, KEY_ENTER, KEY_ESCAPE, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, KEY_0 = 48, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9 } Key;

class KeyScanner
{
  public:
    KeyScanner( byte num_X_Pins, byte num_Y_Pins, uint8_t X_Pins[], uint8_t Y_Pins[], Key directKeys[], Key matrixKeys[] );
    // Construct with a list of pins as shown above, and a list of Key codes that will be returned when the corresponding key is pressed.
    // The pins nor the keys will be copied, and a pointer is stored! So don't feed a temporary object.
    void init(); // Makes sure the pins are in the correct mode
    Key getKey();
  private:
    byte _num_X_Pins;
    byte _num_Y_Pins;
    uint8_t* _X_Pins;
    uint8_t* _Y_Pins;
    Key* _directKeys;           // The amount should be _num_X_Pins, fill unused ones with KEY_NONE
    Key* _matrixKeys;           // The amount should be _num_X_Pins * _num_Y_Pins, fill unused ones with KEY_NONE
    Key _prevKey;
};

#endif
