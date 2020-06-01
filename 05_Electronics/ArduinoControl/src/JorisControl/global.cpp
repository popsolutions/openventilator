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

char const *const measStrings[M_NUM_MEAS] = { "None", "PEEP", "pDrop", "pPl", "pPk", "RR", "E/I", "Vt", "VE", "p", "Q", "Vsup", "Vmot", "Imot", "Pmot" }; 
// TODO: get these strings in PROGMEM

float measValues[M_NUM_MEAS];
byte measPrecisions[M_NUM_MEAS] = { 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1 };

float settings[S_NUM_SETT];
// typedef enum : byte { S_NONE, S_PEEP, S_PEEPDeviation, S_pDropMax, S_pMax, S_pPl, S_pPlDeviation, S_RR, S_RRDeviation, S_EI, S_EIDeviation, S_Vt, S_VtDeviation, S_VE, S_VEDeviation, S_AssistEnabled, S_AssistThreshold, S_AssistMaxRR, S_VsupMin, S_ImotMax, S_NUM_SETT } Sett;
const float defaultSettings[S_NUM_SETT] PROGMEM = { 0, 10, 2, 4, 40, 25, 4, 15, 10, 2, 10, 300, 10, 4.5, 10, 0, 2, 30, 11, 3 };

const Alarm alarms[] PROGMEM = {
  {M_PEEP,  AT_AbsDeviation,  S_PEEP,         S_NONE },
  {M_pDrop, AT_UpperLimit,    S_pDropMax,     S_NONE },
  {M_pPl,   AT_AbsDeviation,  S_pPl,          S_pPlDeviation },
  {M_pPk,   AT_UpperLimit,    S_pMax,         S_NONE },
  {M_RR,    AT_PercDeviation, S_RR,           S_RRDeviation },
  {M_EI,    AT_PercDeviation, S_EI,           S_EIDeviation },
  {M_VE,    AT_PercDeviation, S_VE,           S_VEDeviation },
  {M_Vsup,  AT_LowerLimit,    S_VsupMin,      S_NONE },
  {M_Imot,  AT_UpperLimit,    S_ImotMax,      S_NONE },
  {M_NONE,  AT_NONE,          S_NONE,         S_NONE } }; // Always end with AT_NONE

// Settings
bool assistEnabled = false;

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
 
Sett findMeasSett( Meas meas )
{
  for( byte i=0; ; i++ ) {
    Alarm a;
    memcpy_P( &a, &(alarms[i]), sizeof( Alarm ) );
    if( a.type == AT_NONE ) {
      return S_NONE;
    }
    if( a.meas == meas ){
      return (Sett) a.sett;
    }
  }
  Serial.println( "NONE" );
  return S_NONE;
}

void setDefaultSettings()
{
  memcpy_P( settings, defaultSettings, sizeof(settings) );
}
