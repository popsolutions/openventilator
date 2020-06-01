/*
 * CircularBuffer.cpp
 * Defines a Circular Buffer of n x m data points
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
#include "CircularBuffer.h"

CircularBuffer::CircularBuffer()
:
  _buffer( NULL )
{}

CircularBuffer::~CircularBuffer() 
{
  if( _buffer ) {
    free( _buffer );
    _buffer = NULL;
  }
}

int CircularBuffer::init( int num_cols, int num_rows )
{
  if( _buffer ) {
    free( _buffer );
    _buffer = NULL;
  }
  _buffer = (float*) malloc( sizeof(float) * num_cols * num_rows );
  _num_cols = num_cols;
  _num_rows = num_rows;
  _head = 0;
  _tail = 0;
  return _buffer ? 0 : -1;
}

long CircularBuffer::appendRow( float values[] )
// Always use the correct number of columns!
{
  if( !_buffer ) return 0;

  int i = _num_cols * ( _head % _num_rows );
  for( int n = 0; n < _num_cols; n ++ )
  {
    _buffer[i++] = values[n];
  }
  _head ++;
  if( _head - _tail > _num_rows ) _tail ++;
  return _head;
}

float CircularBuffer::getValue( int column, long pos )
{
  if( !_buffer ) return NAN;

  if( pos < 0 ) {
    pos += _head;
  }
  if( pos < _tail || pos >= _head )
    return 0;

  int i = _num_cols * ( pos % _num_rows ) + column;
  return _buffer[i];
}

int CircularBuffer::getRow( long pos, float * dest )
{
  if( !_buffer ) return 0;
  if( !dest ) return 0;

  if( pos < 0 ) {
    pos += _head;
  }
  if( pos < _tail || pos >= _head )
    return 0;
  
  int i = _num_cols * ( pos % _num_rows );
  for( int n = 0; n < _num_cols; n ++ )
    dest[n] = _buffer[i];
  return _num_rows;
}

int CircularBuffer::getColumn( int column, long pos, int len, float * dest )
{
  if( !_buffer ) return 0;
  if( !dest ) return 0;

  if( pos < 0 ) {
    pos += _head;
  }
  if( pos < _tail || pos >= _head )
    return 0;

  if( len > _head - pos )
    len = _head - pos;

  int i = _num_cols * ( pos % _num_rows ) + column;
  for( int n = 0; n < len; n ++ ) {
    dest[n] = _buffer[i];
    i += _num_cols;
    if( i >= _num_cols * _num_rows )
      i -= _num_cols * _num_rows;
  }
  return len;
}
