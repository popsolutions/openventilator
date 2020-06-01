/*
 * Graphs.cpp
 * Class implementation to display graphs on the screen.
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
#include <LiquidCrystal.h>
#include "VerticalGraph.h"
#include "globals.h"

VerticalGraph::VerticalGraph() 
{}

void VerticalGraph::prepare()
{
  int c;
  byte shape[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };

  for( c = 0; c < 8; c++ )
  {
    shape[7-c] = B11111;
    lcd.createChar( c, shape );
  }
}

void VerticalGraph::draw( float value, float bottom_value, float top_value, int x, int y_bottom, int y_top )
{
  int lines = (y_bottom-y_top+1) * 8;
  float processed_value = round( lines * (value - bottom_value) / (top_value - bottom_value) );
  byte bargraphed_value = (processed_value<0) ? 0 : (processed_value>255) ? 255 : processed_value;
  for( int y = y_bottom; y >= y_top; y -- )
  {
    char c;
    if( bargraphed_value >= 8 ) {
      c = 7;
    } else if( bargraphed_value > 0 ) {
      c = bargraphed_value % 8 - 1;
    } else {
      c = 32; // space
    }
    if( bargraphed_value > 8 )
      bargraphed_value -= 8;
    else
      bargraphed_value = 0;
    lcd.setCursor( x, y );
    lcd.write( c );
  }  
}

void VerticalGraph::drawMultiple( byte count, float *value, float bottom_value, float top_value, int x_left, int y_bottom, int y_top )
// This routine prevents replacing the cursor every time
{
  int lines = (y_bottom-y_top+1) * 8;

  // Create 
  byte bargraphed_value[40]; // max 4 columns
  for( byte n = 0; n < count; n ++ )
  {
    float processed_value = round( lines * (value[n] - bottom_value) / (top_value - bottom_value) );
    bargraphed_value[n] = (processed_value<0) ? 0 : (processed_value>255) ? 255 : processed_value;
  }

  for( int y = y_bottom; y >= y_top; y -- )
  {
    lcd.setCursor( x_left, y );
    for( byte n = 0; n < count; n ++ )
    {
      byte b = bargraphed_value[n];
      char c;
      if( b >= 8 ) {
        c = 7;
      } else if( b > 0 ) {
        c = b % 8 - 1;
      } else {
        c = 32; // space
      }
      lcd.write( c );
      if( b > 8 )
        bargraphed_value[n] = b - 8;
      else
        bargraphed_value[n] = 0;
    }
  }
}
