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

class ADCReader
{
  public:
    ADCReader();
    void init( byte numChannels, int averaging );
    bool isSampleReady();  // Returns if there is a new sample ready
    long getSample();  // Returns this new sample and clears the flag for the readiness
    void addRawSample( byte channel, int a );
    int getAveraging() { return _averaging; };
    
  private:
    byte _numChannels;    // The number of channels we will scan
    int _averaging;           // We will average over how many raw samples
    int _growthCount;         // the count of raw samples currently in the growing sample
    long _growingSample;      // the growing sample as being taken
    long _storedSample;       // final sample data
    int _storedCounter;      // will be increased each time a new average sample is ready
    int _prevStoredCounter; // used on the reading side to check if a new sample has arrived
};

extern ADCReader ADCR;
