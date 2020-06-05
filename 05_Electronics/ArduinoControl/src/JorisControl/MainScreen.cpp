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
          switchScreen( menuScreen );
          break;
        case KEY_0:
        case KEY_1:
        case KEY_2:
        case KEY_3:
          if ( _editLine < 0 ) {
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
          break;
        default:
          if ( _editLine < 0 ) {
            int newMeasSel = (int) _measSel[0] + rotMove;
            newMeasSel = newMeasSel > M_NONE ? newMeasSel < M_NUM_MEAS ? newMeasSel : M_NUM_MEAS - 1 : 1;
            _measSel[0] = (Meas) newMeasSel;
          }
          else { // if editing
          }
      }
  }
}

void MainScreen::onEnter()
{
}

void MainScreen::onLeave()
{
}

void MainScreen::draw()
{
  char measStr[20];
  char fStr[20];
  char fStr2[20];
  char buf[40];
  float col[20];
  int num_rows;
  long start_pos;

  switch( _mode ) {
    case MSM_FULL_GRAPH:
      num_rows = 20 * _graphCompression;
      start_pos = (( circBuf.getHeadPos() - num_rows ) / _graphCompression ) * _graphCompression; // calculate startpos with multiples of _graphCompression to prevent strange-looking scrolling
      circBuf.getCompressedColumn( 0, start_pos, num_rows, _graphCompression, col );

      vgraph.drawMultiple( 20, col, 0.0, 40.0, 0, 3, 0 );
      
      break;
      
    case MSM_HALF_GRAPH_AND_VALUES:
      num_rows = 10 * _graphCompression;
      start_pos = (( circBuf.getHeadPos() - num_rows ) / _graphCompression ) * _graphCompression; // calculate startpos with multiples of _graphCompression to prevent strange-looking scrolling
      circBuf.getCompressedColumn( 0, start_pos, num_rows, _graphCompression, col );
    
      vgraph.drawMultiple( 10, col, 0.0, 40.0, 0, 3, 0 );

      for( byte y=0; y<4; y++ ) {
        Meas meas = _measSel[y];

        strcpy( measStr, measStrings[meas] );

        dtostrf( measValues[meas], 4, measPrecisions[meas], fStr );

        sprintf( buf, "%-5.5s%4.4s ", measStr, fStr );
        lcd.setCursor( 10, y );
        lcd.print( buf );
      }
      break;
      
    case MSM_VALUES_AND_SETPOINTS:
      circBuf.getColumn( 0, -1, 1, col );
      vgraph.draw( col[0], 0.0, 40.0, 0, 3, 0 );

      for( byte y=0; y<4; y++ ) {
        Meas meas = _measSel[y];

        strcpy( measStr, measStrings[meas] );
        
        dtostrf( measValues[meas], 4, measPrecisions[meas], fStr );

        Sett linkedSetting = findMeasSett( meas );
        fStr2[0] = 0; // empty string
        if( linkedSetting ) {
          dtostrf( settings[linkedSetting], 4, measPrecisions[meas], fStr2 );
        }
        sprintf( buf, " %-5.5s  %4.4s   %4.4s", measStr, fStr, fStr2 );
        lcd.setCursor( 1, y );
        lcd.print( buf );
      }
      break;
  }
}

void MainScreen::setMode( MainScreenMode mode )
{
  _mode = mode;
}
