/*
 * MenuItem.h
 * Class definition of a menu item.
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

#ifndef MENUITEM_H
#define MENUITEM_H

#include <Arduino.h>

typedef enum : byte { MIA_NONE, MIA_ENTER, MIA_ACCEPT, MIA_VALUEUP, MIA_VALUEDOWN, MIA_MOVELEFT, MIA_MOVERIGHT } MenuItemAction;

class MenuItem
{
  public:
    virtual void generateText( char* buf, byte maxLength ) = 0; // return true if modified since last call; always supply a buffer of at least maxLength+1 bytes
    virtual byte getEditCursorPos( byte maxLength ) { return 255; }; // return 255 from this function to disable the cursor
    virtual bool performAction( MenuItemAction action ) { return true; } // return code indicates whether we are (still) editing
};

#endif
