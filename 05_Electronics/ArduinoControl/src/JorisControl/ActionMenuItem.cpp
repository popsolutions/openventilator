/*
 * ActionMenuItem.cpp
 * Class implementation of a menu item that calls a function when activated.
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

#include "ActionMenuItem.h"
#include "globals.h"

ActionMenuItem::ActionMenuItem( const char* text_P, void (*func)() )
:
  _text_P( text_P ), _func( func )
{}

void ActionMenuItem::generateText( char* buf, byte maxLength )
{
  strncpy_P( buf, _text_P, maxLength );
  buf[maxLength] = 0;
  strpad( buf, ' ', maxLength );
  buf[maxLength-3] = '.';
  buf[maxLength-2] = '.';
  buf[maxLength-1] = '.';
}

bool ActionMenuItem::performAction( MenuItemAction action )
{
  switch( action ) {
    case MIA_ENTER:
    case MIA_MOVERIGHT:
      if( _func != NULL ) {
        _func();
      }
      break;
  }
  return false; // We don't keep control
}
