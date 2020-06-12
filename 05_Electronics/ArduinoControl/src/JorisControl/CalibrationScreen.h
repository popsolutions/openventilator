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

typedef enum : byte { CSM_ASK_CONTINUE, CSM_PARK_MOTOR, CSM_PRESSURE_OFFSETS, CSM_VOLTAGE, CSM_MOTOR_PREPARE, CSM_CURRENT_LOW_CONFIRM, CSM_CURRENT_LOW_IDLE, CSM_CURRENT_LOW_RUNNING, CSM_CURRENT_HIGH_CONFIRM, CSM_CURRENT_HIGH_IDLE, CSM_CURRENT_HIGH_RUNNING, CSM_MOTOR_NORMAL, CSM_MOTOR_SLOWDOWN1, CSM_MOTOR_SLOWDOWN2, CSM_COMPLETED, CSM_CANCELLED } CalibrationScreenStep;

class CalibrationScreen : public Screen {
  public:
    CalibrationScreen();
    void process();
    void draw();
    void onEnter();
    void onLeave();
  private:
    CalibrationScreenStep _step, _prevStep;
    byte _subStep;
    float _editValue;
    float _var1, _var2, _var3, _var4, _var5, _var6; // Use these variable spaces for multiple steps
    float& _Iavg_normal     = _var1;
    float& _Iavg_slowdown1  = _var2;
    float& _Iavg_slowdown2  = _var3;
    float& _t_normal        = _var4;
    float& _t_slowdown1     = _var5;
    float& _t_slowdown2     = _var6;
    float& _supplyCurrentIdle_lowV  = _var1;
    float& _motorCurrent_lowV       = _var2;
    float& _Iavg_lowV               = _var3;
    float& _supplyCurrentIdle_highV = _var1; // Reuse
    float& _motorCurrent_highV      = _var1; // Reuse
    float& _Iavg                    = _var4;
    unsigned int _avg_counter;
    
};

#endif
