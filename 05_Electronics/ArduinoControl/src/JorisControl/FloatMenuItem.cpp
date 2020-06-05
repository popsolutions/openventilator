/*
 * FloatMenuItem.cpp
 * Class implementation of a menu item displaying a float.
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

#include "FloatMenuItem.h"
#include "globals.h"

FloatMenuItem::FloatMenuItem( const char* text_PSTR, float& value, bool editable, const FloatMenuItemData *PData )
:
  _text_PSTR( text_PSTR ), _value( value ), _prevValue( NAN ), _editable( editable ), _PData( PData )
{}

FloatMenuItemData FloatMenuItem::_getPData()
{
  FloatMenuItemData PData;
  memcpy_P( &PData, _PData, sizeof(FloatMenuItemData) );
  return PData;
}

bool FloatMenuItem::generateText( char* buf, byte maxLength )
{
  bool changed = false;
  
  FloatMenuItemData data = _getPData();

  char fStr[40]; // Use a large buffer just to be sure, because there's no easy way to limit the length
  dtostrf( _value, 1, data.precision, fStr );
  int len = strlen( fStr );
  
  strncpy_P( buf, _text_PSTR, maxLength );
  buf[maxLength] = 0;
  strpad( buf, ' ', maxLength );
  if( len < maxLength ) {
    strcpy( buf+maxLength-len, fStr );
  }
  buf[maxLength] = 0;
  if( _prevValue != _value ) {
    changed = true;
    _prevValue = _value;
  }
  return changed; // return changed indicator: assume text has not changed, maybe the float
}

byte FloatMenuItem::getEditCursorPos( byte maxLength )
{
  if( !_editable ) return -1;
  
  FloatMenuItemData data = _getPData();
   
  char precisionShift = data.precision > 0 ? data.precision + 1 : 0;
  char digitToModify; 
  
  // Make a simple log10-like function. The real one uses 400 program bytes or so...
  if( data.stepSize < 0.001 ) { digitToModify = -5; }
  else if( data.stepSize < 0.01 ) { digitToModify = -4; }
  else if( data.stepSize < 0.1 ) { digitToModify = -3; }
  else if( data.stepSize < 1 ) { digitToModify = -2; }
  else if( data.stepSize < 10 ) { digitToModify = 0; }
  else if( data.stepSize < 100 ) { digitToModify = 1; }
  else if( data.stepSize < 1000 ) { digitToModify = 2; }
  else if( data.stepSize < 10000 ) { digitToModify = 3; }
  else if( data.stepSize < 100000 ) { digitToModify = 4; }
  else { digitToModify = 5; }
  return min( maxLength - 1, maxLength - 1 - precisionShift - digitToModify );
}

bool FloatMenuItem::performAction( MenuItemAction action )
{
  if( !_editable ) return false;

  FloatMenuItemData data = _getPData();
   
  switch( action ) {
    case MIA_ENTER:
      return true;
      break;
    case MIA_ACCEPT:
      return false;
      break;
    case MIA_VALUEUP:
      _value += data.stepSize;
      if( _value > data.highLimit ) _value = data.highLimit;
      break;
    case MIA_VALUEDOWN:
      _value -= data.stepSize;
      if( _value < data.lowLimit ) _value = data.lowLimit;
      break;
  }
  return true;
}
