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
#include <LiquidCrystal.h>
#include "MenuScreen.h"
#include "Menu.h"
#include "MenuItem.h"
#include "FloatMenuItem.h"
#include "BoolMenuItem.h"
#include "globals.h"

/*
  // Observations
  float pressure;
  //float flow;
  float Vmotor;
  float Imotor;
  float Pmotor;

  // Alarms
  float PmaxAlarm;
  float PEEPDeviationAlarm;
  float RRDeviationPercentageAlarm;
*/

// Default PSTR dos not allow using it at object static initialisation
#undef PSTR
#define PSTR(s) ([]{ static const char c[] PROGMEM = (s); return &c[0]; }())

// Define the menus here
// Always use PSTR for the strings, the class assumes that the strings are in program memory.

FloatMenuItem pMaxAlarmVoltageMI( PSTR("Pmax"), pMaxAlarm, true, 0, 10, 50, 1 );
FloatMenuItem PEEPDeviationAlarmMI( PSTR("PEEP deviation"), PEEPDeviationAlarm, true, 1, 0, 10, 0.5 );
FloatMenuItem RRDeviationAlarmMI( PSTR("RR deviation %"), RRDeviationAlarm, true, 0, 0, 20, 1 );

MenuItem * alarmMenuList[] = {&pMaxAlarmVoltageMI, &PEEPDeviationAlarmMI, &RRDeviationAlarmMI, NULL};
Menu alarmMenu( PSTR("Alarm"), alarmMenuList );

BoolMenuItem assistEnableMI( PSTR("Assist Control"), assistEnabled, true );
FloatMenuItem assistThresholdMI( PSTR("Assist Threshold"), assistThreshold, true, 1, 1, 10, 0.2 );
FloatMenuItem assistMaxRRMI( PSTR("Assist Max RR"), assistMaxRR, true, 0, 10, 30, 1 );

MenuItem * settingsMenuList[] = {&assistEnableMI, &assistThresholdMI, &assistMaxRRMI, NULL};
Menu settingsMenu( PSTR("Settings"), settingsMenuList );

FloatMenuItem pressureMI( PSTR("Pressure"), measValues[M_p], false, 1, 0, 0, 0 );
//FloatMenuItem flowMI( PSTR("Flow"), measValues[M_Q], false, 1, 0, 0, 0 ); // l/s
FloatMenuItem VsupplyMI( PSTR("Vsupply"), measValues[M_Vsup], false, 1, 0, 0, 0 );

MenuItem * observationMenuList[] = {&pressureMI, &VsupplyMI, NULL};
Menu observationMenu( PSTR("Observations"), observationMenuList );

MenuItem * mainMenuList[] = {&settingsMenu, &alarmMenu, &observationMenu, NULL};
Menu mainMenu( PSTR("Menu"), mainMenuList );

MenuScreen::MenuScreen()
  :
  _activeMenu( NULL )
{}

void MenuScreen::process()
{
  char buf[40];
  MenuItem* item = _selection > 0 ? _activeMenu->getItem( _selection - 1 ) : NULL;

  Key pressedKey = keySc.getKey();
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
    default:
      char rotMove = rotEnc.getIncrPos();
      if ( !_editing ) {
        if ( rotMove > 0 ) {
          // scroll down
          if ( _selection < _activeMenu->getNumItems() ) {
            _selection ++;
            _forceRedraw = true;
            if ( _selection > _scrollPos + 2 ) {
              _scrollPos ++;
            }
          }
        }
        else if ( rotMove < 0 ) {
          // scroll up
          if ( _selection > 0 ) {
            _selection --;
            _forceRedraw = true;
            if ( _selection < _scrollPos + 1 && _scrollPos > 0 ) {
              _scrollPos --;
            }
          }
        }
      }
      else { // if editing
        if ( rotMove > 0 ) {
          // scroll down
          item->performAction( MIA_VALUEUP );
        }
        if ( rotMove < 0 ) {
          // scroll up
          item->performAction( MIA_VALUEDOWN );
        }
      }
  }
}

void MenuScreen::draw()
{
  char buf[21];
  bool doDraw;

  for ( byte y = 0; y < 4; y++ )
  {
    doDraw = _forceRedraw;
    byte i = y + _scrollPos;
    if ( i == 0 ) {

      // Let menu generate its text as menu name
      doDraw |= _activeMenu->generateText( buf, 20 );

      // Fill string to 20 characters
      strpad( buf, ' ', 20 );
      if ( _selection == i ) {
        buf[18] = '<';
        buf[19] = '<';
      }
    }
    else {
      // Get menu item
      MenuItem* item = _activeMenu->getItem(i - 1);

      if ( _selection == i ) {
        buf[0] = '>';
      } else {
        buf[0] = ' ';
      }
      if ( item != NULL ) {
        // Let menu item generate its text
        doDraw |= item->generateText( buf + 1, 19 );
      }
      else {
        buf[0] = 0; // start with zero length string
      }
    }

    // Fill string to 20 characters
    strpad( buf, ' ', 20 );

    // Put it on LCD if needed
    if ( doDraw ) {
      Serial.print( y );
      Serial.print( ": " );
      Serial.println( buf );
      lcd.setCursor( 0, y );
      lcd.print( buf );
    }
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
    else {
      //lcd.noCursor();
      lcd.noBlink();
    }
  }
  _forceRedraw = false; // redraw complete
}

void MenuScreen::switchMenu( Menu* newMenu )
{
  Serial.println( F("MenuScreen::switchMenu()") );
  if ( _activeMenu != newMenu ) {
    if ( _activeMenu != NULL ) {
      _activeMenu->onLeave();
    }
    _activeMenu = newMenu;
    if ( _activeMenu != NULL ) {
      _activeMenu->onEnter();
    }
  }
  _selection = 1;
  _scrollPos = 0;
  _editing = 0;
  _forceRedraw = true;
}

void MenuScreen::onEnter()
{
  Serial.println( F("MenuScreen::onEnter()") );
  switchMenu( &mainMenu );
}

void MenuScreen::onLeave()
{
  Serial.println( F("MenuScreen::onLeave()") );
  lcd.noCursor();
  switchMenu( NULL );
}
