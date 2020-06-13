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

FloatMenuItem pMaxMI( PSTR("Pmax"), settings[S_pMax], true, &settingsProps_P[S_pMax] );
FloatMenuItem PEEPDeviationMI( PSTR("PEEP deviation"), settings[S_PEEPDeviation], true, &settingsProps_P[S_PEEPDeviation] );
FloatMenuItem RRDeviationMI( PSTR("RR deviation %"), settings[S_RRDeviation], true, &settingsProps_P[S_RRDeviation] );
FloatMenuItem EIDeviationMI( PSTR("EI deviation %"), settings[S_EIDeviation], true, &settingsProps_P[S_EIDeviation] );

MenuItem * alarmMenuList[] = {&pMaxMI, &PEEPDeviationMI, &RRDeviationMI, NULL};
Menu alarmMenu( PSTR("Alarm"), alarmMenuList );
/*
BoolMenuItem assistEnableMI( PSTR("Assist Control"), assistEnabled, true );
FloatMenuItem assistThresholdMI( PSTR("Assist Threshold"), settings[S_AssistThreshold], true, &settingsProps_P[S_AssistThreshold] );
FloatMenuItem assistMaxRRMI( PSTR("Assist Max RR"), settings[S_AssistMaxRR], true, &settingsProps_P[S_AssistMaxRR] );
*/
MenuItem * settingsMenuList[] = {/*&assistEnableMI, &assistThresholdMI, &assistMaxRRMI,*/ NULL};
Menu settingsMenu( PSTR("Settings"), settingsMenuList );

ActionMenuItem calibMI( PSTR("Calibrate"), activateCalibrationScreen );
/*
FloatMenuItem PoffsetMI( PSTR("pOffset"), settings[S_pOffset], false, &settingsProps_P[S_pOffset] );
FloatMenuItem QoffsetMI( PSTR("pQoffset"), settings[S_pQoffset], false, &settingsProps_P[S_pQoffset] );
FloatMenuItem VsupFactorMI( PSTR("VsupplyFactor"), settings[S_VsupFactor], true, &settingsProps_P[S_VsupFactor] );
FloatMenuItem ImotShuntConductanceMI( PSTR("ImotShuntCond"), settings[S_ImotShuntConductance], true, &settingsProps_P[S_ImotShuntConductance] );
FloatMenuItem ImotOffsetMI( PSTR("ImotOffset"), settings[S_ImotOffset], true, &settingsProps_P[S_ImotOffset] );
*/
FloatMenuItem Ri0MI( PSTR("Ri0"), settings[S_Ri0], true, &settingsProps_P[S_Ri0] );
FloatMenuItem RiIdepMI( PSTR("RiIdep"), settings[S_RiIdep], true, &settingsProps_P[S_RiIdep] );
FloatMenuItem KvMI( PSTR("Kv"), settings[S_Kv], true, &settingsProps_P[S_Kv] );
ActionMenuItem FactoryDefaultsMI( PSTR("Factory defaults"), setDefaultSettings );

MenuItem * calibrationMenuList[] = {&calibMI, /* &PoffsetMI, &QoffsetMI, &VsupFactorMI, &ImotShuntConductanceMI, &ImotOffsetMI, */ &Ri0MI, &RiIdepMI, &KvMI, &FactoryDefaultsMI, NULL};
Menu calibrationMenu( PSTR("Calibration"), calibrationMenuList );

MenuItem * mainMenuList[] = {&settingsMenu, &alarmMenu, &calibrationMenu, NULL};
Menu mainMenu( PSTR("Menu"), mainMenuList );

#define CH_TRIANGLE_LEFT 1
#define CH_TRIANGLE_RIGHT 2
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
          // value up
          for( char n=0; n<rotMove; n++ )
            item->performAction( MIA_VALUEUP );
        }
        if ( rotMove < 0 ) {
          // value down
          for( char n=0; n>rotMove; n-- )
            item->performAction( MIA_VALUEDOWN );
        }
      }
  }
}

void MenuScreen::draw()
{
  char buf[21];

  for ( byte y = 0; y < 4; y++ )
  {
    byte i = y + _scrollPos;
    if ( i == 0 ) {

      // Let menu generate its text as menu name
      _activeMenu->getName( buf, 20 );

      // Fill string to 20 characters
      strpad( buf, ' ', 20 );
      if ( _selection == i ) {
        buf[18] = CH_TRIANGLE_LEFT;
        buf[19] = CH_TRIANGLE_LEFT;
      }
    }
    else {
      // Get menu item
      MenuItem* item = _activeMenu->getItem(i - 1);

      if ( _selection == i ) {
        buf[0] = CH_TRIANGLE_RIGHT;
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

    // Put it on LCD
    lcd.printxy( 0, y, buf );
  }

  // Place cursor if in edit mode
  MenuItem* item = _selection > 0 ? _activeMenu->getItem( _selection - 1 ) : NULL;

  if ( item ) {
    byte x = item->getEditCursorPos( 19 );
    byte y = _selection - _scrollPos;
    if ( _editing && x >= 0 ) {
      lcd.setCursor( x + 1, y );
      lcd.blink();
    }
    else {
      lcd.noBlink();
    }
  }
}

void MenuScreen::switchMenu( Menu* newMenu )
{
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
  if( _activeMenu == NULL ) {
    switchMenu( &mainMenu ); // If you set a menu before going to the menu screen, that will be shown.
  }
  memcpy_P( buf, triangle_left, 8 );
  lcd.createChar( CH_TRIANGLE_LEFT, buf );
  memcpy_P( buf, triangle_right, 8 );
  lcd.createChar( CH_TRIANGLE_RIGHT, buf );
}

void MenuScreen::onLeave()
{
  lcd.noBlink();
  switchMenu( NULL );
  saveSettingsIntoEEPROM();
}
