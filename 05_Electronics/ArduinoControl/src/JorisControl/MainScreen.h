/*
 * MainScreen.h
 * Class definition of the main screen.
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

#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "BufferedLiquidCrystal.h"
#include "Screen.h"

class MainScreen;
#include "globals.h"

typedef enum :byte { MSM_VALUES_AND_SETPOINTS, MSM_HALF_GRAPH_AND_VALUES, MSM_FULL_GRAPH, MSM_NUM_MODES } MainScreenMode;

class MainScreen : public Screen {
  public:
    MainScreen();
    void process();
    void draw();
    void onEnter();
    void onLeave();
    void setMode( MainScreenMode mode );
  private:
    MainScreenMode _mode;
    byte _graphCompression;
    Meas _measSel[4];
    char _editLine; // -1 for not editing
};

#endif
