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
#include <EEPROM.h>

MainScreen* mainScreen = new MainScreen;
MenuScreen* menuScreen = new MenuScreen;
CalibrationScreen* calibrationScreen = new CalibrationScreen;

Screen* activeScreen = NULL;

const char M_PEEP_s[]  PROGMEM = "PEEP";
const char M_pDrop_s[] PROGMEM = "pDrop";
const char M_pPl_s[]   PROGMEM = "pPl";
const char M_pPk_s[]   PROGMEM = "pPk";
const char M_RR_s[]    PROGMEM = "RR";
const char M_EI_s[]    PROGMEM = "EI";
const char M_Vt_s[]    PROGMEM = "Vt";
const char M_VE_s[]    PROGMEM = "VE";
const char M_p_s[]     PROGMEM = "p";
const char M_pQ_s[]    PROGMEM = "pQ";
const char M_Q_s[]     PROGMEM = "Q";
const char M_Vsup_s[]  PROGMEM = "Vsup";
const char M_Vmot_s[]  PROGMEM = "Vmot";
const char M_Imot_s[]  PROGMEM = "Imot";
const char M_Pmot_s[]  PROGMEM = "Pmot";
const char M_Park_s[]  PROGMEM = "Park";
const char M_tCycl_s[] PROGMEM = "tCycl";
const char M_Pos_s[]   PROGMEM = "Pos";
//                                typedef enum : byte { M_NONE, M_PEEP,   M_pDrop,   M_pPl,   M_pPk,   M_RR,   M_EI,   M_Vt,   M_VE,   M_p,   M_pQ,   M_Q,   M_Vsup,   M_Vmot,   M_Imot,   M_Pmot,   M_Park,   M_tCycl,   M_Pos, M_NUM_MEAS } Meas;
char const *const measStrings_P[M_NUM_MEAS] PROGMEM = { NULL,   M_PEEP_s, M_pDrop_s, M_pPl_s, M_pPk_s, M_RR_s, M_EI_s, M_Vt_s, M_VE_s, M_p_s, M_pQ_s, M_Q_s, M_Vsup_s, M_Vmot_s, M_Imot_s, M_Pmot_s, M_Park_s, M_tCycl_s, M_Pos_s };
const byte measPrecisions_P[M_NUM_MEAS] PROGMEM =     { 0,      1,        1,         1,       1,       1,      1,      0,      1,      1,     2,      2,     1,        1,        2,        1,        0,        1,         0 };
float measValues[M_NUM_MEAS];

//                              typedef enum : byte { S_NONE, S_PEEP, S_PEEPDeviation, S_pDropMax, S_pMax, S_pPl, S_pPlDeviation, S_RR, S_RRDeviation, S_EI, S_EIDeviation, S_Vt, S_VtDeviation, S_VE, S_VEDeviation, S_AssistEnabled, S_AssistThreshold, S_AssistMaxRR, S_VsupMin, S_ImotMax, S_VsupFactor, S_ImotShuntConductance, S_ImotOffset, S_pOffset, S_pQoffset, S_Kv, S_Ri0, S_RiIdep, S_NUM_SETT } Sett;
const float defaultSettings_P[S_NUM_SETT] PROGMEM = { 0,      10,     2,               4,          40,     25,    4,              15,   10,            2,    10,            300,  10,            4.5,  10,            0,               2,                 30,            11,        4.0,       4.0,          20,                     0.2,          0,         0,          NAN,  NAN,   0         };
const FloatProps settingsProps_P[S_NUM_SETT] PROGMEM = 
{ // byte precision, float lowLimit, float highLimit, float stepSize
  { 0, 0,  0, 0 }, // S_NONE
  { 1, 2, 30, 1 }, // S_PEEP
  { 1, 0, 10, 0.5 }, // S_PEEPDeviation
  { 1, 1, 10, 0.5 }, // S_pDropMax
  { 1, 0,100, 1 }, // S_pMax
  { 1, 0, 40, 1 }, // S_pPl
  { 1, 0, 10, 1 }, // S_pPlDeviation
  { 1, 6, 30, 0.5 }, // S_RR
  { 1, 0, 20, 1 }, // S_RRDeviation %
  { 1, 1,  6, 0.1 }, // S_EI
  { 0, 0, 20, 1 }, // S_EIDeviation %
  { 0, 0,1000,5 }, // S_Vt
  { 0, 1, 20, 1 }, // S_VtDeviation %
  { 1, 0, 20, 0.1 }, // S_VE
  { 1, 0, 20, 1 }, // S_VEDeviation %
  { 0, 0,  0, 0 }, // S_AssistEnabled // boolean, these numbers are not used
  { 1, 1, 10, 0.1 }, // S_AssistThreshold
  { 1, 6, 30, 0.1 }, // S_AssistMaxRR
  { 1, 8, 12, 0.1 }, // S_VsupMin
  { 2, 0, 10, 0.01 }, // S_ImotMax
  { 2, 0, 10, 0.01 }, // S_VsupFactor
  { 1, 0, 40, 0.1 }, // S_ImotShuntConductance
  { 2, 0, 1, 0.01 }, // S_ImotOffset
  { 2,-5, 5, 0.01 }, // S_pOffset
  { 2,-5, 5, 0.01 }, // S_pQoffset
  { 1, 1, 20, 0.1 }, // S_Kv
  { 2, 0, 4, 0.01 },  // S_Ri0
  { 2, -1, 0, 0.01 }  // S_RiIdep
 };
float settings[S_NUM_SETT];

const Alarm alarms_P[] PROGMEM = {
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

bool& assistEnabled = (bool&) settings[S_AssistEnabled]; // This bool is stored in float space; so cannot be read as float

float VmotOverrule = NAN; // Overrule voltage for motor, NAN if in operational mode (not stored in EEPROM)
float motorSpeedSetpoint; // Currently set motor speed in RPM/min (not stored in EEPROM)


void strpad( char* buf, char chr, byte len )
// Always supply a buffer of at least size len + 1
{
  // Fill string to <len> characters, and add a termination char
  for( byte pos=strlen( buf ); pos < len; pos ++ ) buf[pos] = chr;
  buf[len] = 0; // terminate string
}


char* format_float( char* buf, float value, byte len, byte prec, bool right_align )
// Always supply a buffer of at least size len + 1
// In contrast to dtostrf, this function is safe for length
{
  buf[0] = 0;
  byte sign_len, int_len, frac_len;
  byte pos;
  
  if( value == INFINITY ) {
    if( right_align ) strpad( buf, ' ', len-4 );
    strncpy_P( right_align?(buf+len-4):buf, PSTR("+INF"), len );
    buf[len] = 0;
    return buf;
  } 
  if( value == -INFINITY ) {
    if( right_align ) strpad( buf, ' ', len-4 );
    strncpy_P( right_align?(buf+len-4):buf, PSTR("-INF"), len );
    buf[len] = 0;
    return buf;
  }
  if( isnan( value ) ) {
    if( right_align ) strpad( buf, ' ', len-3 );
    strncpy_P( right_align?(buf+len-3):buf, PSTR("NAN"), len );
    buf[len] = 0;
    return buf;
  }

  { // Rounding
    long rounding = 2;
    for( byte i = 0; i < prec; ++i )
      rounding *= 10;     
    if( value >= 0 ) value += 1.0 / rounding;
    else             value -= 1.0 / rounding;
  }
  // Find out length of int part
  for( int_len = 1; int_len < 20; int_len ++, value *= 0.1 ) // Value is divided by 10 every step
    if( value < 10 && value > -10 ) break; // found correct size, only 1 digit before decimal point now

  frac_len = ( prec == 0 ) ? 0 : prec + 1;
  sign_len = ( value < 0 ); // Minus symbol

  // Does the number fit?
  if( len < sign_len + int_len + frac_len ) {
    strpad( buf, '#', len );
    return buf;
  }
  
  // Determine position of first char
  pos = 0;
  if( right_align ) { 
    byte first_pos = len - sign_len - int_len - frac_len;
    for( ; pos < first_pos; pos++ ) buf[pos] = ' ';
  }

  // Place minus symbol
  if( value < 0 ) {
    buf[pos++] = '-';
    value = -value;
  }

  // Print the digits
  for( byte n=1; n < int_len + frac_len; n++ ) {
    byte digit = (byte)value;
    buf[pos++] = '0' + digit;
    if( n == int_len && pos < len ) {
      buf[pos++] = '.';
    }
    value = ( value - digit ) * 10;
  }
  buf[pos] = 0;
  strpad( buf, ' ', len );
  
  return buf;
}

void switchScreen( Screen* newScreen )
{
  if( activeScreen != NULL ) {
    activeScreen->onLeave();
  }
  activeScreen = newScreen;
  activeScreen->onEnter();
}

Sett findMeasSett( Meas meas )
{
  for( byte i=0; ; i++ ) {
    Alarm a;
    memcpy_P( &a, &(alarms_P[i]), sizeof( Alarm ) );
    if( a.type == AT_NONE ) {
      return S_NONE;
    }
    if( a.meas == meas ){
      return (Sett) a.sett;
    }
  }
  return S_NONE;
}

FloatProps getSettingsProps( Sett sett )
{
  FloatProps fd;
  memcpy_P( &fd, &(settingsProps_P[sett]), sizeof( FloatProps ) );
  return fd;
}

void setDefaultSettings()
{
  memcpy_P( settings, defaultSettings_P, sizeof(settings) );
}

void loadSettingsFromEEPROM()
{
  byte checksum = sizeof( settings) & 0xFF; // This way if size changes this also results in mismatch
  byte* ptr = (byte*) &settings;
  for( int n=0; n<sizeof(settings); n++ ) {
    byte val = EEPROM.read( n+1 );
    *ptr = val;
    ptr ++;
    checksum += val;
  }
  
  if( checksum != EEPROM.read( 0 )) {
    Serial.println( F("EEPROM checksum mismatch, setting default settings") );
    setDefaultSettings();
  }
  else {
    Serial.println( F("Loaded settings from EEPROM") );
  }
}

void saveSettingsIntoEEPROM()
{
  byte checksum = sizeof( settings) & 0xFF; // This way if size changes this also results in mismatch
  byte* ptr = (byte*) &settings;
  for( int n=0; n<sizeof(settings); n++ ) {
    EEPROM.update( n+1, *ptr );
    ptr ++;
    checksum += *((byte*) &settings + n );
  }
  EEPROM.update( 0, checksum );
  Serial.println( F("Saved settings into EEPROM") );
}
