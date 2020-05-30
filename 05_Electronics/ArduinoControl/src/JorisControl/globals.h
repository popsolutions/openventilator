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
#include <LiquidCrystal.h>

typedef enum : byte { M_NONE, M_PEEP, M_pDrop, M_pPl, M_pMax, M_RR, M_EI, M_Vt, M_VE, M_p, M_Q, M_Vsup, M_Vmot, M_Imot, M_Pmot, M_NUM_MEAS } Meas;
extern char const *const measStrings[M_NUM_MEAS];
extern float measValues[M_NUM_MEAS];
extern byte measPrecisions[M_NUM_MEAS];  // Precision of the values for formatting
extern float * measLinkedSettings[M_NUM_MEAS];

#include "RotaryEncoder.h"
#include "VerticalGraph.h"
#include "RespirationAnalysis.h"
#include "CircularBuffer.h"
#include "Screen.h"
#include "MainScreen.h"
#include "MenuScreen.h"
#include "Menu.h"
#include "KeyScanner.h"

extern LiquidCrystal lcd;
extern RotaryEncoder rotEnc; 
extern VerticalGraph vgraph;
extern RespirationAnalysis an;
extern CircularBuffer circBuf;
extern KeyScanner keySc;

extern MainScreen* mainScreen;
extern MenuScreen* menuScreen;
extern Screen* activeScreen;


// Observations
//extern float pressure;
//extern float flow;
//extern float Vsupply;
//extern float Vmotor;
//extern float Imotor;
//extern float Pmotor;

// Settings
extern bool assistEnabled;
extern float assistThreshold;
extern float assistMaxRR;

extern float PEEPSetpoint;
extern float RRSetpoint; // percentage
extern float EISetpoint; // percentage
extern float pPlSetpoint;
extern float VESetpoint; // liter/min

// Alarms
extern float pMaxAlarm;
extern float pDropAlarm;
extern float PEEPDeviationAlarm;
extern float pPlDeviationAlarm;
extern float RRDeviationAlarm; // percentage
extern float EIDeviationAlarm; // percentage
extern float VsupLowAlarm;
extern float VsupHighAlarm;

extern void switchScreen( Screen* newScreen );
extern void switchMenu( Menu* newMenu );
extern void strpad( char* buf, char chr, byte len );

#endif
