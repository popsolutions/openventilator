/*
 * CircularBuffer.h
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

class CircularBuffer {
    public:
      CircularBuffer();
      ~CircularBuffer();
      
      void init( int num_cols, int num_rows );
      // Clears the buffer and sets the number of columns and maximum size (rows)
 
      long appendRow( float values[] );
      // Adds a row of data to the buffer

      float getValue( int column, long pos );
      // Returns a single value from the storage
      
      int getRow( long pos, float * dest );
      // Copies a row from the storage
      // pos: position. Use negative value to indicate position relative to the head (latest added element).
      // dest: where to copy the values to
      // Returns the amount of items copied (0 if data for position is not available)
      
      int getColumn( int column, long pos, long len, float * dest );
      // Copies a column from the storage
      // pos: start position. Use negative value to indicate position relative to the head (next pos to add to).
      // len: max length to return. You may give a large number to just get all available.
      // dest: where to copy the values to
      // Returns the amount of items copied (0 if data for position is not available)

      int getNumCols()  { return _num_cols; }
      int getNumRows()  { return _num_rows; }
      long getTailPos() { return _tail; }
      long getHeadPos() { return _head; }
      
    private:
      int _num_cols, _num_rows;
      long _head; // number of next item to be written. 
      long _tail; // number of oldest item written.
      float * _buffer;
};
