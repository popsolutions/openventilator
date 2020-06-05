/*
 * BufferedLiquidCrystal.cpp
 * Extends the LiquidCrystal class by buffering writing
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

#ifndef BUFFEREDLIQUIDCRYSTAL_H
#define BUFFEREDLIQUIDCRYSTAL_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class BufferedLiquidCrystal : public LiquidCrystal {
    public:
      BufferedLiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
      BufferedLiquidCrystal(uint8_t rs,             uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
      BufferedLiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
      BufferedLiquidCrystal(uint8_t rs,             uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

      ~BufferedLiquidCrystal();

      void begin( uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS );
      
      void clear();                    // Clear buffer and display

      void home();                     // Set cursor position to home position
      void setCursor( uint8_t x, uint8_t y ); // Set cursor position

      virtual size_t write( uint8_t ); // This function will skip writes if the byte is the same
                                       // If you use a visible cursor, always set cursor position after writing a screen
      void createChar( uint8_t ch, uint8_t charmap[] );
  
    private:
      uint8_t _num_cols, _num_rows;    // Size of the display
      uint8_t _cursor_x, _cursor_y;     // Intended cursor position
      uint8_t _mode;
      bool _pos_wrong;                 // Is true if we know the LCD cursor is at the wrong position and it needs repositioning
      uint8_t * _buffer;
};

#endif
