/*
 * BufferedLiquidCrystal.cpp
 * Extends the LiquidCrystal class by preventing writing if  writing
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
#include "BufferedLiquidCrystal.h"

BufferedLiquidCrystal::BufferedLiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
:
  LiquidCrystal(rs, rw, enable, d0, d1, d2, d3, d4, d5, d6, d7),
  _buffer(NULL)
{}

BufferedLiquidCrystal::BufferedLiquidCrystal(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
:
  LiquidCrystal(rs, 255, enable, d0, d1, d2, d3, d4, d5, d6, d7),
  _buffer(NULL)
{}

BufferedLiquidCrystal::BufferedLiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
:
  LiquidCrystal(rs, rw, enable, d0, d1, d2, d3),
  _buffer(NULL)
{}

BufferedLiquidCrystal::BufferedLiquidCrystal(uint8_t rs,  uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
:
  LiquidCrystal(rs, 255, enable, d0, d1, d2, d3),
  _buffer(NULL)
{}

BufferedLiquidCrystal::~BufferedLiquidCrystal() 
{
  if( _buffer ) {
    free( _buffer );
    _buffer = NULL;
  }
}

void BufferedLiquidCrystal::begin( uint8_t cols, uint8_t rows, uint8_t charsize )
{ 
  if( _buffer ) {
    free( _buffer );
    _buffer = NULL;
  }
  _num_cols = cols;
  _num_rows = rows;
  _buffer = (uint8_t*) malloc( cols * rows );
  LiquidCrystal::begin( cols, rows, charsize );
  clear();
}

void BufferedLiquidCrystal::clear()
{
  LiquidCrystal::clear();
  memset( _buffer, ' ', _num_cols * _num_rows );
}

void BufferedLiquidCrystal::home()
{
  LiquidCrystal::home();
  _cursor_x = 0;
  _cursor_y = 0;
  _pos_wrong = false;
}

void BufferedLiquidCrystal::setCursor( uint8_t x, uint8_t y )
{
  LiquidCrystal::setCursor( x, y );
  _cursor_x = x;
  _cursor_y = y;
  _pos_wrong = false;
}

void BufferedLiquidCrystal::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  LiquidCrystal::command(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    LiquidCrystal::write(charmap[i]);
  }
}

size_t BufferedLiquidCrystal::write( uint8_t ch )
{
  if( _buffer[_cursor_y*_num_cols+_cursor_x] != ch ) {
    if( _pos_wrong ) {
      LiquidCrystal::setCursor( _cursor_x, _cursor_y );
    }
    _pos_wrong = false;
    LiquidCrystal::write( ch );
  }
  else {
    // We skip a write action
    _pos_wrong = true;
  }
  _buffer[_cursor_y*_num_cols+_cursor_x] = ch;
  // Update the intended cursor position
  _cursor_x ++;
  if( _cursor_x >= _num_cols ) {
    _cursor_x %= _num_cols;
    _cursor_y ++;
    _cursor_y %= _num_rows;
  }
}
