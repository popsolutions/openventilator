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
#include "half.h"

half f2h( float in )
{
  if( isnan( in )) return 0x7C01;
  uint16_t fraction = (( *((uint32_t*) &in ) & 0x007FFFFF) + 0x00001000 ) >> 13; // take correct part and add a bit for rounding
  int16_t  exponent = (( *((uint32_t*) &in ) & 0x7F800000 ) >> 23 ) - (127-HALF_EXPONENT_BIAS); // exponent bias is 112 lower
  uint16_t sign     =  ( *((uint32_t*) &in ) & 0x80000000 ) >> 16;
  if( exponent < -9 ) {
    // value too small, return zero
    fraction = 0;
    exponent = 0;
    // sign will stay as is
  } else if( exponent < 1) {
    // subnormal number
    fraction = ((( 0x400 | fraction ) >> -exponent) + 1) >> 1; // correct rounding
    exponent = 0;
    // sign will stay as is
  } else if( exponent > 30) {
    // value too large, return infinity
    fraction = 0;
    exponent = 0x1F;
    // sign will stay as is
  }
  return sign | (exponent<<10) | fraction;
}

float h2f( half in )
{
  if( in == 0x7C00 ) return +INFINITY;
  if( in == 0xFC00 ) return -INFINITY;
  if( (in & 0x7C00) == 0x7C00 ) return NAN;
  uint16_t fraction =  ( *((uint16_t*) &in ) & 0x03FF );
  uint16_t exponent = (( *((uint16_t*) &in ) & 0x7C00 ) >> 10 ); // exponent bias is 112 higher
  uint16_t sign =      ( *((uint16_t*) &in ) & 0x8000 );
  if( exponent == 0 ) {
    return (sign?-1:+1) * ((float)(fraction)/((long)1<<(10+HALF_EXPONENT_BIAS-1)));
  } else {
    uint32_t f = ((uint32_t)sign)<<16 | ((uint32_t)(exponent+(127-HALF_EXPONENT_BIAS))<<23) | (uint32_t)fraction<<13;
    return *((float*) &f );
  }
}
