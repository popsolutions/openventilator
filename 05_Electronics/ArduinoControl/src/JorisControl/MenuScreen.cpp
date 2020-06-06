/*
   MenuScreen.cpp
   Class implementation of the menu screen.
*/

/*
  Copyright (c) 2020, Joris Robijn
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following condition is met:
      Redistributions of source code must retain the above copyright
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
#include "MenuScreen.h"
#include "Menu.h"
#include "MenuItem.h"
#include "ActionMenuItem.h"
#include "BoolMenuItem.h"
#include "FloatMenuItem.h"
#include "globals.h"

// Define the menus here
// Always use PSTR for the strings, the class assumes that the strings are in program memory.

FloatMenuItem pMaxMI( PSTR("Pmax"), settings[S_pMax], true, PDATA( FloatMenuItemData, { 0, 10, 50, 1 } ) );
FloatMenuItem PEEPDeviationMI( PSTR("PEEP deviation"), settings[S_PEEPDeviation], true, PDATA( FloatMenuItemData, { 1, 0, 10, 0.5 } ) );
FloatMenuItem RRDeviationMI( PSTR("RR deviation %"), settings[S_RRDeviation], true, PDATA( FloatMenuItemData, { 0, 0, 20, 1 } ) );

MenuItem * alarmMenuList[] = {&pMaxMI, &PEEPDeviationMI, &RRDeviationMI, NULL};
Menu alarmMenu( PSTR("Alarm"), alarmMenuList );

BoolMenuItem assistEnableMI( PSTR("Assist Control"), assistEnabled, true );
FloatMenuItem assistThresholdMI( PSTR("Assist Threshold"), settings[S_AssistThreshold], true, PDATA( FloatMenuItemData, { 1, 1, 10, 0.2 } ) );
FloatMenuItem assistMaxRRMI( PSTR("Assist Max RR"), settings[S_AssistMaxRR], true, PDATA( FloatMenuItemData, { 0, 10, 30, 1 } ) );

MenuItem * settingsMenuList[] = {&assistEnableMI, &assistThresholdMI, &assistMaxRRMI, NULL};
Menu settingsMenu( PSTR("Settings"), settingsMenuList );

FloatMenuItem pressureMI( PSTR("Pressure"), measValues[M_p], false, PDATA( FloatMenuItemData, { 1, 0, 0, 0 } ) );
//FloatMenuItem flowMI( PSTR("Flow"), measValues[M_Q], false, PDATA( FloatMenuItemData, { 1, 0, 0, 0 } ) ); // l/s
FloatMenuItem VsupplyMI( PSTR("Vsupply"), measValues[M_Vsup], false, PDATA( FloatMenuItemData, { 1, 0, 0, 0 } ) );
FloatMenuItem ImotorMI( PSTR("Imotor"), measValues[M_Imot], false, PDATA( FloatMenuItemData, { 2, 0, 0, 0 } ) );

MenuItem * measurementMenuList[] = {&pressureMI, &VsupplyMI, &ImotorMI, NULL};
Menu measurementMenu( PSTR("Measurements"), measurementMenuList );

ActionMenuItem calibMI( PSTR("Calibrate"), activateCalibrationScreen );
FloatMenuItem PoffsetMI( PSTR("Poffset"), settings[S_pOffset], false, PDATA( FloatMenuItemData, { 1, -4, +4, 0.1 } ) );
FloatMenuItem QoffsetMI( PSTR("Qoffset"), settings[S_Qoffset], false, PDATA( FloatMenuItemData, { 1, -4, +4, 0.1 } ) );
FloatMenuItem VsupFacMI( PSTR("Vsupply_factor"), settings[S_VsupFac], false, PDATA( FloatMenuItemData, { 1, 3, 10, 0.1 } ) );
FloatMenuItem RiMI( PSTR("Ri"), settings[S_Ri], false, PDATA( FloatMenuItemData, { 1, 0.1, 10, 0.1 } ) );
FloatMenuItem KvMI( PSTR("Kv"), settings[S_Kv], false, PDATA( FloatMenuItemData, { 1, 2, 20, 0.1 } ) );

MenuItem * calibrationMenuList[] = {&calibMI, &PoffsetMI, &QoffsetMI, &VsupFacMI, &RiMI, &KvMI, NULL};
Menu calibrationMenu( PSTR("Calibration"), calibrationMenuList );

FloatMenuItem VmotMI( PSTR("VmotOverrule"), settings[S_VmotOverrule], true, PDATA( FloatMenuItemData, { 1, 0, 10, 0.1 } ) );
FloatMenuItem ParkMI( PSTR("Park"), measValues[M_Park], false, PDATA( FloatMenuItemData, { 0, 0, 0, 0 } ) );

MenuItem * mainMenuList[] = {&VmotMI, &ImotorMI, &ParkMI, &calibrationMenu, &settingsMenu, &alarmMenu, &measurementMenu, NULL};
Menu mainMenu( PSTR("Menu"), mainMenuList );

const byte triangle_left[8] PROGMEM = {
  B00000,
  B00001,
  B00111,
  B11111,
  B00111,
  B00001,
  B00000,
  B00000
};

const byte triangle_right[8] PROGMEM = {
  B00000,
  B10000,
  B11100,
  B11111,
  B11100,
  B10000,
  B00000,
  B00000
};

void activateCalibrationScreen()
{
  switchScreen( calibrationScreen );
}

MenuScreen::MenuScreen()
:
  _activeMenu( NULL )
{}

void MenuScreen::process()
{
  char buf[40];
  MenuItem* item = _selection > 0 ? _activeMenu->getItem( _selection - 1 ) : NULL;

  Key pressedKey = keySc.getKey();
  char rotMove = rotEnc.getIncrPos();
  switch ( pressedKey ) {
    case KEY_ENTER:
      if ( _selection == 0 ) {
        switchScreen( mainScreen );
      } else {
        if ( !_editing ) {
          _editing = item->performAction( MIA_ENTER );
          // If the item doesn't do editing, it will return true here already.
        }
        else {
          _editing = item->performAction( MIA_ACCEPT );
        }
      }
      break;
    case KEY_0:
    case KEY_1:
    case KEY_2:
    case KEY_3:
        if ( _editing ) {
          // independent of which key is pressed
          _editing = item->performAction( MIA_ACCEPT );
        }
        else {
          // Check if the button corresponds to a menu item (or the menu header for Back)
          if( (pressedKey - KEY_0) + _scrollPos <= _activeMenu->getNumItems() ) {
            _selection = _scrollPos + (pressedKey - KEY_0);
            if( _selection == 0 ) {
              switchScreen( mainScreen );
            }
            else {
              item = _activeMenu->getItem( _selection - 1 );
              _editing = item->performAction( MIA_ENTER );
              // If the item doesn't do editing, it will return true here already.
            }
          }
        }
      break;
    case KEY_4:
      switchScreen( mainScreen );
      break;
    default:
      if ( !_editing ) {
        if ( rotMove > 0 ) {
          // scroll down
          if ( _selection < _activeMenu->getNumItems() ) {
            _selection ++;
            if ( _selection > _scrollPos + 2 ) {
              _scrollPos ++;
            }
          }
        }
        else if ( rotMove < 0 ) {
          // scroll up
          if ( _selection > 0 ) {
            _selection --;
            if ( _selection < _scrollPos + 1 && _scrollPos > 0 ) {
              _scrollPos --;
            }
          }
        }
      }
      else { // if editing
        if ( rotMove > 0 ) {
          // scroll down
          for( char n=0; n<rotMove; n++ )
            item->performAction( MIA_VALUEUP );
        }
        if ( rotMove < 0 ) {
          // scroll up
          for( char n=0; n>rotMove; n-- )
            item->performAction( MIA_VALUEDOWN );
        }
      }
  }
}

void MenuScreen::draw()
{
  char buf[21];

  lcd.noBlink();
  for ( byte y = 0; y < 4; y++ )
  {
    byte i = y + _scrollPos;
    if ( i == 0 ) {

      // Let menu generate its text as menu name
      _activeMenu->getName( buf, 20 );

      // Fill string to 20 characters
      strpad( buf, ' ', 20 );
      if ( _selection == i ) {
        buf[18] = 1; // left arrow
        buf[19] = 1;
      }
    }
    else {
      // Get menu item
      MenuItem* item = _activeMenu->getItem(i - 1);

      if ( _selection == i ) {
        buf[0] = 2; // right arrow
      } else {
        buf[0] = ' ';
      }
      if ( item != NULL ) {
        // Let menu item generate its text
        item->generateText( buf + 1, 19 );
      }
      else {
        buf[0] = 0; // start with zero length string
      }
    }

    // Fill string to 20 characters
    strpad( buf, ' ', 20 );

    // Put it on LCD if needed
    lcd.setCursor( 0, y );
    lcd.print( buf );
  }

  // Place cursor if in edit more
  MenuItem* item = _selection > 0 ? _activeMenu->getItem( _selection - 1 ) : NULL;

  if ( item ) {
    byte x = item->getEditCursorPos( 19 );
    byte y = _selection - _scrollPos;
    if ( _editing && x >= 0 ) {
      lcd.setCursor( x + 1, y );
      lcd.blink();
    }
  }
}

void MenuScreen::switchMenu( Menu* newMenu )
{
  Serial.println( F("MenuScreen::switchMenu()") );
  if ( _activeMenu != newMenu ) {
    _activeMenu = newMenu;
  }
  _selection = 1;
  _scrollPos = 0;
  _editing = 0;
}

void MenuScreen::onEnter()
{
  byte buf[8];
  Serial.println( F("MenuScreen::onEnter()") );
  if( _activeMenu == NULL ) {
    switchMenu( &mainMenu ); // If you set a menu before going to the menu screen, that will be shown.
  }
  memcpy_P( buf, triangle_left, 8 );
  lcd.createChar( 1, buf );
  memcpy_P( buf, triangle_right, 8 );
  lcd.createChar( 2, buf );
}

void MenuScreen::onLeave()
{
  Serial.println( F("MenuScreen::onLeave()") );
  lcd.noBlink();
  switchMenu( NULL );
}
