/*
 * FloatMenuItem.h
 * Class definition of a menu item displaying a float.
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

#ifndef FLOATMENUITEM_H
#define FLOATMENUITEM_H

#include <Arduino.h>
#include "MenuItem.h"

typedef struct {
  byte precision;
  float lowLimit;
  float highLimit;
  float stepSize;
} FloatMenuItemData;

class FloatMenuItem : public MenuItem
{
  public:
    FloatMenuItem( const char* text_PSTR, float& value, bool editable, const FloatMenuItemData* PData );
    bool generateText( char* buf, byte maxLength );
    byte getEditCursorPos( byte maxLength );
    bool isEditable() { return true; }
    bool performAction( MenuItemAction action );
  private:
    const char* _text_PSTR;
    float& _value;
    float _prevValue;
    bool _editable;
    const FloatMenuItemData* _PData;
    FloatMenuItemData _getPData();
};

#endif
