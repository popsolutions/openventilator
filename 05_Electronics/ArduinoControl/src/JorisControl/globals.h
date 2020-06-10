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

typedef struct {
  byte precision;
  float lowLimit;
  float highLimit;
  float stepSize;
} FloatProps;

// Default PSTR does not allow using it at object static initialisation, replace it
#undef PSTR
#define PSTR(s) ([]{ static const char c[] PROGMEM = (s); return &c[0]; }())
// Allow other data in program memory
#define PDATA(t, ...) ([]{ static const t d PROGMEM = __VA_ARGS__; return &d; }())

// All measurements are stored in an array with all elements accessible by name
typedef enum : byte { M_NONE, M_PEEP, M_pDrop, M_pPl, M_pPk, M_RR, M_EI, M_Vt, M_VE, M_p, M_pQ, M_Q, M_Vsup, M_Vmot, M_Imot, M_Pmot, M_Park, M_tCycl, M_Pos, M_NUM_MEAS } Meas;
// All settings are stored in an array with all elements accessible by name. This way they can easily be stored to and restored from EEPROM.
typedef enum : byte { S_NONE, S_PEEP, S_PEEPDeviation, S_pDropMax, S_pMax, S_pPl, S_pPlDeviation, S_RR, S_RRDeviation, S_EI, S_EIDeviation, S_Vt, S_VtDeviation, S_VE, S_VEDeviation, S_AssistEnabled, S_AssistThreshold, S_AssistMaxRR, S_VsupMin, S_ImotMax, S_VsupFactor, S_ImotShuntConductance, S_pOffset, S_pQoffset, S_Kv, S_Ri, S_NUM_SETT } Sett;

// Alarm information describes the relation between the measurements and settings
typedef enum : byte { AT_NONE, AT_LowerLimit, AT_UpperLimit, AT_AbsDeviation, AT_PercDeviation } AlarmType;
typedef struct {
  Meas meas;
  AlarmType type;
  Sett sett; // This is the value that is settable in the main screen
  Sett deviationSett; // This is the deviation that is settable in the menu
} Alarm;

extern char const *const measStrings_P[M_NUM_MEAS];
extern const byte measPrecisions_P[M_NUM_MEAS];  // Precision of the values for formatting
extern float measValues[M_NUM_MEAS];

extern const float defaultSettings_P[S_NUM_SETT]; // Settings used on factory reset, or if EEPROM data invalid
extern const FloatProps settingsProps_P[S_NUM_SETT]; // Properties for the settings
extern float settings[S_NUM_SETT];

extern const Alarm alarms_P[];

extern bool& assistEnabled;

extern float VmotOverrule; // Overrule voltage for motor, NAN if in operational mode (not in EEPROM)
extern float motorSpeedSetpoint; // Currently set motor speed in RPM/min (not in EEPROM)

#include "RotaryEncoder.h"
#include "BufferedLiquidCrystal.h"
#include "VerticalGraph.h"
#include "RespirationAnalysis.h"
#include "CircularBuffer.h"
#include "Screen.h"
#include "MainScreen.h"
#include "MenuScreen.h"
#include "CalibrationScreen.h"
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
extern CalibrationScreen* calibrationScreen;
extern Screen* activeScreen;

inline float coerce_float( float in, float low, float high )
{
  if( in < low ) return low;
  if( in > high ) return high;
  return in;
}

inline int coerce_int( int in, int low, int high )
{
  if( in < low ) return low;
  if( in > high ) return high;
  return in;
}

extern void strpad( char* buf, char chr, byte len );

extern char* format_float( char* buf, float value, byte len, byte precision, bool allow_shift, bool right_align );
// Always supply a buffer of at least size len + 1

extern void switchScreen( Screen* newScreen );

extern Sett findMeasSett( Meas );
FloatProps getSettingsProps( Sett sett );
extern bool isAlarmActiveForMeas( Meas );
extern void setDefaultSettings();

#endif
