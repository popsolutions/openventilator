/*
 * globals.h
 * Definition of things that can be used anywhere.
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


#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// All measurements are stored in an array with all elements accessible by name
typedef enum : byte { M_NONE, M_PEEP, M_pDrop, M_pPl, M_pPk, M_RR, M_EI, M_Vt, M_VE, M_p, M_Q, M_Vsup, M_Vmot, M_Imot, M_Pmot, M_Park, M_NUM_MEAS } Meas;
// All settings are stored in an array with all elements accessible by name. This way they can easily be stored to and restored from EEPROM.
typedef enum : byte { S_NONE, S_VmotTEMP, S_PEEP, S_PEEPDeviation, S_pDropMax, S_pMax, S_pPl, S_pPlDeviation, S_RR, S_RRDeviation, S_EI, S_EIDeviation, S_Vt, S_VtDeviation, S_VE, S_VEDeviation, S_AssistEnabled, S_AssistThreshold, S_AssistMaxRR, S_VsupMin, S_ImotMax, S_KvMot, S_RiMot, S_NUM_SETT } Sett;

// Alarm information describes the relation between the measurements and settings
typedef enum : byte { AT_NONE, AT_LowerLimit, AT_UpperLimit, AT_AbsDeviation, AT_PercDeviation } AlarmType;
typedef struct {
  Meas meas;
  AlarmType type;
  Sett sett; // This is the value that is settable in the main screen
  Sett deviationSett; // This is the deviation that is settable in the menu
} Alarm;

extern const Alarm alarms[];

extern char const *const measStrings[M_NUM_MEAS];
extern float measValues[M_NUM_MEAS];
extern byte measPrecisions[M_NUM_MEAS];  // Precision of the values for formatting
//extern float * measLinkedSettings[M_NUM_MEAS];

extern float settings[S_NUM_SETT];

#include "RotaryEncoder.h"
#include "BufferedLiquidCrystal.h"
#include "VerticalGraph.h"
#include "RespirationAnalysis.h"
#include "CircularBuffer.h"
#include "Screen.h"
#include "MainScreen.h"
#include "MenuScreen.h"
#include "Menu.h"
#include "KeyScanner.h"

extern BufferedLiquidCrystal lcd;
extern RotaryEncoder rotEnc; 
extern VerticalGraph vgraph;
extern RespirationAnalysis an;
extern CircularBuffer circBuf;
extern KeyScanner keySc;

extern MainScreen* mainScreen;
extern MenuScreen* menuScreen;
extern Screen* activeScreen;

extern bool assistEnabled;

extern void switchScreen( Screen* newScreen );
extern void switchMenu( Menu* newMenu );
extern void strpad( char* buf, char chr, byte len );

extern Sett findMeasSett( Meas );

extern void setDefaultSettings();

#define COERCE( a, b, c ) ( (a)<(b)?(b):(a)>(c)?(c):(a) )

#endif
