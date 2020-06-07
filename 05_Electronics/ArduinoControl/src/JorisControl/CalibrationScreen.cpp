/*
 * CalibrationScreen.cpp
 * Class implementation of the calibration screen.
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
#include "CalibrationScreen.h"
#include "MenuScreen.h"

CalibrationScreen::CalibrationScreen()
:
  _step( CSM_ASKCONTINUE )
{}

void CalibrationScreen::process()
{
  Key pressedKey = keySc.getKey();
  char rotMove = rotEnc.getIncrPos();
  
  switch( _step ) {
    case CSM_ASKCONTINUE:
      switch( pressedKey ) {
        case KEY_3:
          VmotOverrule = 0; // Stop the motor
          _step = (CalibrationScreenMode) ( (byte)_step + 1 );
          break;
        case KEY_0:
        case KEY_4:
          switchScreen( mainScreen );
          break;
      }
      break;

    case CSM_VOLTAGE:
      if( _prevStep != _step ) { // First time in this case
        _editValue = measValues[M_Vsup];
        _subStep = 0;
      }
      switch( pressedKey ) {
        case KEY_3:
          if( _subStep > 0 ) { // Only execute if the value was edited
            // Calculate value
            float Vin = measValues[M_Vsup] / settings[S_VsupFac];
            settings[S_VsupFac] = _editValue / Vin;
          }
          _step = (CalibrationScreenMode) ( (byte)_step + 1 );
          break;
        case KEY_0:
          switchScreen( mainScreen );
          break;
        default:
          // process rotary encoder
          if( rotMove != 0 ) _subStep = 1;
          _editValue += 0.1 * rotMove;
          _editValue = coerce_float( _editValue, 8, 16 );
          break;
      }
      break;

    case CSM_PRESSUREOFFSETS:
      switch( pressedKey ) {
        case KEY_3:
          settings[S_pOffset] += measValues[M_p];
          settings[S_pQoffset] += measValues[M_pQ];
          _step = (CalibrationScreenMode) ( (byte)_step + 1 );
          break;
        case KEY_0:
          switchScreen( mainScreen );
          break;
      }
      break;

    case CSM_MOTORPREPARE:
      switch( pressedKey ) {
        case KEY_3:
          _step = (CalibrationScreenMode) ( (byte)_step + 1 );
          break;
        case KEY_0:
          switchScreen( mainScreen );
          break;
      }
      break;

    case CSM_MOTORNORMAL:
      switch( _subStep ) {
        case 0: // Before measuring, park switch still active
          if( measValues[M_Park] == 0 ) {
            _subStep++;
          }
          break;
        case 1: // Before measuring, waiting for park switch
          if( measValues[M_Park] == 1 ) {
            // Park switch is now active, start measurement!
            _Iavg_normal = 0;
            _subStep++;
          }
        case 2: // Measuring and in park switch
          _Iavg_normal += measValues[M_Imot];
          _avg_counter ++;
          if( measValues[M_Park] == 0 ) {
            // Park switch not active anymore
            _subStep++;
          }
        case 3: // Measuring and waiting for park switch
          _Iavg_normal += measValues[M_Imot];
          _avg_counter ++;
          if( measValues[M_Park] == 1 ) {
            _Iavg_normal /= _avg_counter;
            _t_normal = measValues[M_tCycl];
            _subStep = 0;
            _step = (CalibrationScreenMode) ( (byte)_step + 1 );
          }
          break;
      }
      switch( pressedKey ) {
        case KEY_0:
          switchScreen( mainScreen );
          break;
      }
      break;

    case CSM_MOTORSLOWDOWN:
      switch( pressedKey ) {
        case KEY_0:
          _step = CSM_CANCELLED;
          break;
      }
      break;

    case CSM_COMPLETED:
      if( _subStep == 0 ) {
        float Vmot = VmotOverrule;

        // Calculate Ri and Kv
        settings[S_Ri] = (_t_slowdown * Vmot - _t_normal * Vmot) / (_Iavg_slowdown * _t_slowdown - _Iavg_normal * _t_normal);
        settings[S_Kv] = 60000 / (_t_normal * ( Vmot - _Iavg_normal * settings[S_Ri] ));
      }
      // fall through !
    case CSM_CANCELLED:
      if( _subStep == 0 ) {
        VmotOverrule = 0; // Stop the motor
        _subStep ++;
      }
      switch( pressedKey ) {
        case KEY_0:
        case KEY_3:
        case KEY_4:
        case KEY_ENTER:
          VmotOverrule = NAN; // Motor to normal operation
          switchScreen( mainScreen );
          break;
        case KEY_1:
        case KEY_2:
          menuScreen->switchMenu( &calibrationMenu );
          switchScreen( menuScreen );
          break;
      }
      break;
      
  }
}

void CalibrationScreen::onEnter()
{
  Serial.println( F("CalibrationScreen::onEnter()") );
  lcd.clear();
  _step = CSM_ASKCONTINUE;
  _prevStep = CSM_COMPLETED; // Force redraw
}

void CalibrationScreen::onLeave()
{
  Serial.println( F("CalibrationScreen::onLeave()") );
}

void CalibrationScreen::draw()
{
  char buf[21];
  int num_rows;
  long start_pos;
  enum : byte { None=0, Cancel=0x01, OK=0x02, OK_Cancel=0x03 } showButtons = None;

  switch( _step ) {
    case CSM_ASKCONTINUE:
      if( _prevStep != _step ) {
        lcd.clear();
        lcd.printxy( 0, 0, F("CALIBRATION") );
        lcd.printxy( 0, 2, F("Stop the") );
        lcd.printxy( 0, 3, F("machine now?") );
        showButtons = OK_Cancel;
      }
      break;
      
    case CSM_VOLTAGE:
      if( _prevStep != _step ) {
        lcd.clear();
        lcd.printxy( 0, 0, F("Measure") );
        lcd.printxy( 0, 1, F("supply voltage") );
        lcd.printxy( 0, 2, F("and set here:") );
        lcd.blink();
        showButtons = OK_Cancel;
      }
      format_float( buf, _editValue, 4, 1, true, true );
      lcd.printxy( 16, 2, buf );
      lcd.setCursor( 19, 2 ); // Force placing cursor
      break;
      
    case CSM_PRESSUREOFFSETS:
      if( _prevStep != _step ) {
        lcd.noBlink();
        lcd.clear();
        lcd.printxy( 0, 0, F("Disconnect") );
        lcd.printxy( 0, 1, F("pressure tubes") );
        lcd.printxy( 0, 2, F("and press OK.") );
        showButtons = OK_Cancel;
      }
      break;
      
    case CSM_MOTORPREPARE:
      if( _prevStep != _step ) {
        lcd.clear();
        lcd.printxy( 0, 0, F("Disconnect") );
        lcd.printxy( 0, 1, F("motor crank") );
        lcd.printxy( 0, 2, F("and press OK.") );
        lcd.printxy( 0, 3, F("Motor will turn!") );
        showButtons = OK_Cancel;
      }
      break;
      
    case CSM_MOTORNORMAL:
      if( _prevStep != _step ) {
        lcd.clear();
        lcd.printxy( 0, 0, F("Measuring") );
        lcd.printxy( 0, 2, F("...") );
        showButtons = Cancel;
      }
      break;
      
    case CSM_MOTORSLOWDOWN:
      if( _prevStep != _step ) {
        lcd.clear();
        lcd.printxy( 3, 0, F("Slow down") );
        lcd.printxy( 1, 1, F("\x7F the motor") );
        lcd.printxy( 1, 2, F("\x7F by hand.") );
        showButtons = Cancel;
      }
      vgraph.draw( measValues[M_Imot], -0.5, 3.5, 0, 3, 0 ); // Draw a graph from 0 to 2 A motor current
      
      if( measValues[M_Imot] < 1 ) {
        lcd.printxy( 3, 3, F("Use more force") );
      } else if( measValues[M_Imot] > 2 ) {
        lcd.printxy( 3, 3, F("Use less force") );
      } else {
        lcd.printxy( 3, 3, F("Measuring...") );
      }
      break;
      
    case CSM_COMPLETED:
    case CSM_CANCELLED:
      if( _prevStep != _step ) {
        lcd.clear();
        lcd.printxy( 0, 0, F("Calibration") );
        if( _step == CSM_COMPLETED ) {
          lcd.printxy( 0, 1, F("complete.") );
        } else {
          lcd.printxy( 0, 1, F("cancelled.") );
        }
        lcd.printxy( 0, 2, F("Reconnect crank and") );
        lcd.printxy( 0, 3, F("pressure tubes.") );
        showButtons = OK;
      }
      break;
      
  }
  if( showButtons & Cancel ) {
        lcd.printxy( 14, 0, F("Cancel") );
  }
  if( showButtons & OK ) {
        lcd.printxy( 18, 3, F("OK") );
  }
  _prevStep = _step;
}
