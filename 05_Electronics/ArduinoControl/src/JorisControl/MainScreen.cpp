/*
 * MainScreen.cpp
 * Class implementation of the main screen.
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

#include <Arduino.h>
#include "MainScreen.h"

MainScreen::MainScreen()
:
  _mode( MSM_HALF_GRAPH_AND_VALUES ), _editLine( -1 )
{
  _measSel[0] = M_p;
  _measSel[1] = M_PEEP;
  _measSel[2] = M_RR;
  _measSel[3] = M_EI;
  _graphCompression = 4;
}

void MainScreen::process()
{
  Key pressedKey = keySc.getKey();
  char rotMove = rotEnc.getIncrPos();
  
  switch( _mode ) {
    case MSM_FULL_GRAPH:
      switch( pressedKey ) {
        case KEY_ENTER:
          switchScreen( menuScreen );
          break;
        case KEY_4:
          _mode = (MainScreenMode) (( _mode + 1 ) % MSM_NUM_MODES );
          break;
        default:
          if(  rotMove > 0 && _graphCompression > 1 )
            _graphCompression --;
          else if(  rotMove < 0 && _graphCompression < circBuf.getNumRows() / 20 )
            _graphCompression ++;
      }
      break;

    case MSM_HALF_GRAPH_AND_VALUES:
    case MSM_VALUES_AND_SETPOINTS:
      switch( pressedKey ) {
        case KEY_ENTER:
          if( _editLine >= 0 ) {
            _editLine = -1;
          }
          else {
            switchScreen( menuScreen );
          }
          break;
        case KEY_0:
        case KEY_1:
        case KEY_2:
        case KEY_3:
          if ( _editLine != pressedKey - KEY_0 ) {
            // Check if this line has a setting
            Meas meas = _measSel[ pressedKey - KEY_0 ];
            Sett linkedSetting = findMeasSett( meas );
            if( linkedSetting ) {
              _mode = MSM_VALUES_AND_SETPOINTS; // change screen mode always to setpoint visibility
              _editLine = pressedKey - KEY_0;
            }
          }
          else {
            _editLine = -1;
          }
          break;
        case KEY_4:
          _mode = (MainScreenMode) (( _mode + 1 ) % MSM_NUM_MODES );
          _editLine = -1;
          break;
        default:
          if ( _editLine < 0 ) {
            // Change the flexible line
            int newMeasSel = (int) _measSel[0] + rotMove;
            newMeasSel = newMeasSel > M_NONE ? newMeasSel < M_NUM_MEAS ? newMeasSel : M_NUM_MEAS - 1 : 1;
            _measSel[0] = (Meas) newMeasSel;
          }
          else { // if editing
            Meas meas = _measSel[_editLine];
            Sett linkedSetting = findMeasSett( meas );
            if( linkedSetting ) {
              // Read settings properties from PROGMEM
              FloatProps settProps = getSettingsProps( linkedSetting );

              // Adjust the value
              settings[linkedSetting] = coerce_float( settings[linkedSetting] + settProps.stepSize * rotMove, settProps.lowLimit, settProps.highLimit );
            }
            // TODO: limits!
          }
      }
  }
}

void MainScreen::onEnter()
{
  vgraph.prepare();
  Serial.println( F("MainScreen::onEnter()") );
}

void MainScreen::onLeave()
{
  Serial.println( F("MainScreen::onLeave()") );
}

void MainScreen::draw()
{
  char buf[30];
  float col[20];
  int num_rows;
  long start_pos;
  byte cursorY;

  bool blinker = ( ( (byte) circBuf.getHeadPos() & 7 ) < 5);

  switch( _mode ) {
    case MSM_FULL_GRAPH:
      // Draw graph of history
      num_rows = 19 * _graphCompression;
      start_pos = (( circBuf.getHeadPos() - num_rows ) / _graphCompression ) * _graphCompression; // calculate startpos with multiples of _graphCompression to prevent strange-looking scrolling
      circBuf.getCompressedColumn( 0, start_pos, num_rows, _graphCompression, col );
      vgraph.drawMultiple( 19, col, 0.0, 40.0, 0, 3, 0 );
      
      // Draw single-widthbar with current pressure
      circBuf.getColumn( 0, -1, 1, col );
      vgraph.draw( col[0], 0.0, 40.0, 19, 3, 0 );
      
      break;
      
    case MSM_HALF_GRAPH_AND_VALUES:
    case MSM_VALUES_AND_SETPOINTS:
      byte x = 0;
      lcd.noBlink();
      if( _mode == MSM_HALF_GRAPH_AND_VALUES ) {
        // Draw graph of history
        num_rows = 7 * _graphCompression;
        start_pos = (( circBuf.getHeadPos() - num_rows ) / _graphCompression ) * _graphCompression; // calculate startpos with multiples of _graphCompression to prevent strange-looking scrolling
        circBuf.getCompressedColumn( 0, start_pos, num_rows, _graphCompression, col );
        vgraph.drawMultiple( 7, col, 0.0, 40.0, x, 3, 0 );
        x+=7;
      }
      // Draw single-widthbar with current pressure
      circBuf.getColumn( 0, -1, 1, col );
      vgraph.draw( col[0], 0.0, 40.0, x, 3, 0 );
      x++;

      // Show measurements
      for( byte y=0; y<4; y++ ) {
        // Which measurement to display?
        Meas meas = _measSel[y];

        buf[0] = ' ';

        // Copy string from PROGMEM
        strcpy_P( buf+1, (char *)pgm_read_word( &(measStrings_P[meas]) ) );
        strpad( buf+1, ' ', 6 );
 
        // Read precision from PROGMEM
        byte prec = pgm_read_byte( &(measPrecisions_P[meas]) );

        // Format the value
        format_float( buf+6, measValues[meas], 5, prec, true, true );
        strpad( buf+6, ' ', 19-6 );

        // Show alarm indicator *
        if( blinker ) {
          buf[11] = '*';
        } else {
          buf[11] = ' ';
        }

        if( _mode == MSM_VALUES_AND_SETPOINTS ) {
          // Show linked setting
          Sett linkedSetting = findMeasSett( meas );
          if( linkedSetting ) {
            // Read settings properties from PROGMEM
            FloatProps settProps = getSettingsProps( linkedSetting );

            format_float( buf+14, settings[linkedSetting], 5, settProps.precision, true, true );

            // Now that we have the data, determine where edit cursor should be put
            if( _editLine == y ) {
              
              char precisionShift = settProps.precision > 0 ? settProps.precision + 1 : 0;
              char digitToModify;

              // Make a simple log10-like function. The real one uses 400 program bytes or so...
              if( settProps.stepSize < 0.1 ) { digitToModify = -3; }
              else if( settProps.stepSize < 1 ) { digitToModify = -2; }
              else if( settProps.stepSize < 10 ) { digitToModify = 0; }
              else if( settProps.stepSize < 100 ) { digitToModify = 1; }
              else { digitToModify = 2; }

              cursorY = 19 - digitToModify - precisionShift;
            }
          }
          buf[19] = 0; // terminator    WHY?
        }
        else {
          buf[12] = 0; // terminator
        }
        lcd.printxy( x, y, buf );
      }
      break;
  }
  if ( _editLine >= 0 ) {
    lcd.setCursor( cursorY, _editLine );
    lcd.blink();
  }
  else {
    //lcd.noBlink();
  }
}

void MainScreen::setMode( MainScreenMode mode )
{
  _mode = mode;
}
