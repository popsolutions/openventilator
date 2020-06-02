/*
 * Menu.h
 * Class definition of a menu.
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

#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "MenuItem.h"

class Menu : public MenuItem
{
  public:
    Menu( const char* text_PSTR, MenuItem* items[] );
    // Note the string will not be copied, and neither will the list of items! Only the pointer will be stored! So don't feed a temporary object.
    // The last item should always be a NULL entry.

    byte getNumItems();
    MenuItem* getItem( byte index );

    bool generateText( char* buf, byte maxLength );
    bool isEditable() { return false; }
    bool performAction( MenuItemAction action );
    bool onEnter() {};
    bool onLeave() {};
        
  private:
    const char* _text_PSTR; // The strings are in program memory
    MenuItem** _items;
};

#endif