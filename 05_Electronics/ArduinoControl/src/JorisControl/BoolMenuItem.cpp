/*
 * BoolMenuItem.cpp
 * Class implementation of a menu item displaying a bool.
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

#include "BoolMenuItem.h"
#include "globals.h"

BoolMenuItem::BoolMenuItem( const char* text_PSTR, bool& value, bool editable )
:
  _text_PSTR( text_PSTR ), _value( value ), _editable( editable )
{}

void BoolMenuItem::generateText( char* buf, byte maxLength )
{
  bool changed = false;
  
  strncpy_P( buf, _text_PSTR, maxLength );
  buf[maxLength] = 0;
  strpad( buf, ' ', maxLength-1 );
  buf[maxLength-1] = _value ? 'Y' : 'N';
  buf[maxLength] = 0;
}

byte BoolMenuItem::getEditCursorPos( byte maxLength )
{
  return maxLength - 2;
}

bool BoolMenuItem::performAction( MenuItemAction action )
{
  if( !_editable ) return false;
  
  switch( action ) {
    case MIA_ENTER:
    case MIA_VALUEUP:
    case MIA_VALUEDOWN:
      _value = !_value;
      break;
  }
  return false; // Booleans do not need to keep control
}
