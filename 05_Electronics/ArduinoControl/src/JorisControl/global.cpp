/*
 * globals.cpp
 * implementation of global code
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

#include "globals.h"

MainScreen* mainScreen = new MainScreen;
MenuScreen* menuScreen = new MenuScreen;

Screen* activeScreen = NULL;

char const *const measStrings[M_NUM_MEAS] = { "None", "PEEP", "pDrop", "pPl", "pMax", "RR", "E/I", "Vt", "VE", "p", "Q", "Vsup", "Vmot", "Imot", "Pmot" }; 
float measValues[M_NUM_MEAS];
byte measPrecisions[M_NUM_MEAS] = { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 };
float * measLinkedSettings[M_NUM_MEAS] = { NULL, &PEEPSetpoint, &pDropAlarm, &pPlDeviationAlarm, &pMaxAlarm, &RRSetpoint, &EISetpoint, NULL, &VESetpoint, NULL, NULL, &VsupLowAlarm, NULL, NULL, NULL };

// Settings
bool assistEnabled = false;
float assistThreshold = 2.0;
float assistMaxRR = 20.0;
// Alarms
float pMaxAlarm = 40.0;
float pDropAlarm = 4.0;
float PEEPSetpoint = 10;
float PEEPDeviationAlarm = 2.0;
float pPlSetpoint = 25.0;
float pPlDeviationAlarm = 4.0;
float RRSetpoint = 15.0; // percentage
float RRDeviationAlarm = 5.0; // percentage
float EISetpoint = 2.0; // percentage
float EIDeviationAlarm = 10.0; // percentage
float VtSetpoint = 300; // ml
float VESetpoint = 4.5; // liter/min
float VsupLowAlarm = 11.0;
float VsupHighAlarm = 14.5;


void switchScreen( Screen* newScreen )
{
  Serial.println( F("switchScreen") );
  if( activeScreen != NULL && activeScreen != newScreen ) {
    activeScreen->onLeave();
  }
  activeScreen = newScreen;
  activeScreen->onEnter();
}

void strpad( char* buf, char chr, byte len )
{
  // Fill string to <len> characters, and add a termination char
  for( byte pos=strlen( buf ); pos < len; pos ++ ) buf[pos] = ' ';
  buf[len] = 0; // terminate string
}
