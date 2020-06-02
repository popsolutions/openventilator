/*
 * ADCReader.h
 * Class definition to continuously read from the ADC and average (or actally sum) the samples, creating one sample with more resolution.
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

#ifndef ADCREADER_H
#define ADCREADER_H

#define ADCR_MAX_CH 4

class ADCReader
{
  public:
    ADCReader( byte numChannels, uint8_t ADCpins[] ); // Create reader object and use indicated pins for each channel
    void init( int averaging );       // Sets up the ADC and sets averaging
    bool areSamplesReady();            // Returns if there are new summed samples ready
    long getSample( byte ch );        // Reads a summed sample for the given channel number
    void signalSamplesRead();         // Clears the flag for the readiness
    int getAveraging() { return _averaging; }; // Returns the set averaging
    void conversionComplete();        // Internal routine to add a raw sample to the sum and set the mux to the setting for the next sample
    
  private:
    int  _numChannels;
    uint8_t _pin[ADCR_MAX_CH];        // For each channel the pins to use 
    int  _averaging;                  // We will average over how many raw samples per channel
    byte _selCh;                      // Channel for which we will get the raw sample next
    int  _growthCount;                // Counts the amount of samples added for the current result
    long _growingResult[ADCR_MAX_CH]; // The growing result as being taken
    long _storedResult[ADCR_MAX_CH];  // Final summed sample data
    byte  _storedCounter;             // Will be increased each time a new summed sample is ready
    byte  _readCounter;               // Used on the reading side to check if a new summed sample has arrived
};

#endif
