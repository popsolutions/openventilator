/*
 * Menu.cpp
 * Class implementation of a menu.
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

#include "Menu.h"
#include "globals.h"

Menu* activeMenu;

Menu::Menu( const char* text_P, MenuItem* items[] )
:
  _text_P( text_P ), _items( items )
{}

byte Menu::getNumItems()
{
  byte n = 0;
  while( _items[n] != NULL ) n++;
  return n;
}

MenuItem* Menu::getItem( byte index )
{
  byte n;
  // Check that the requested item is not further than the length of the item list
  for( n=0; _items[n] != NULL && n < index; n++ );
  // Might be NULL, or contain the requested item
  return _items[n];
}

void Menu::getName( char* buf, byte maxLength )
{
  strncpy_P( buf, _text_P, maxLength );
  buf[maxLength] = 0;
}

void Menu::generateText( char* buf, byte maxLength )
{
  strncpy_P( buf, _text_P, maxLength );
  buf[maxLength] = 0;
  strpad( buf, ' ', maxLength );
  buf[maxLength-3] = '.';
  buf[maxLength-2] = '.';
  buf[maxLength-1] = '.';
}

bool Menu::performAction( MenuItemAction action )
{
  switch( action ) {
    case MIA_VALUEUP:
    case MIA_VALUEDOWN:
    case MIA_MOVERIGHT:
    case MIA_ENTER:
      menuScreen->switchMenu( this );
      break;
  }
  return false;
}
