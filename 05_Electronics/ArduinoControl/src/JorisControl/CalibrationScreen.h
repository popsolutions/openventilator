/*
 * CalibrationScreen.h
 * Class definition of the calibration screen.
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

#ifndef CALIBRATIONSCREEN_H
#define CALIBRATIONSCREEN_H

#include "BufferedLiquidCrystal.h"
#include "Screen.h"

class CalibrationScreen;
#include "globals.h"

typedef enum : byte { CSM_ASKCONTINUE, CSM_VOLTAGE, CSM_PRESSUREOFFSETS, CSM_MOTORPREPARE, CSM_MOTORNORMAL, CSM_MOTORSLOWDOWN, CSM_COMPLETED, CSM_CANCELLED } CalibrationScreenMode;

class CalibrationScreen : public Screen {
  public:
    CalibrationScreen();
    void process();
    void draw();
    void onEnter();
    void onLeave();
  private:
    CalibrationScreenMode _step, _prevStep;
    byte _subStep;
    float _editValue;
    float _Iavg_normal, _Iavg_slowdown;
    unsigned int _avg_counter, _t_normal, _t_slowdown;
    
};

#endif
