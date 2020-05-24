/*
 * RespirationAnalysis.cpp
 * Class implementation to analyse the pressure and flow, determining indicative values.
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
#include "RespirationAnalysis.h"

RespirationAnalysis::RespirationAnalysis()
{
	_kalmanGain = 1.0;
	_pThr = 3;
	_state = RS_MECH_INSPIRATION;
	_next_pMax = 0;
  _next_pMin = 1000;

}

void RespirationAnalysis::processData( float pressure, float flow )
{
	long t = millis();
	
	_pFilt = ( (1-_kalmanGain)*_pFilt + _kalmanGain*pressure );

	if( _state == RS_MECH_INSPIRATION ) {
		if( _pFilt > _next_pMax ) {
			_next_pMax = _pFilt;
			//tsMax = t;
		}
		if( _pFilt < _next_pMax - _pThr ) {
			_state = RS_MECH_EXPIRATION;
			_tsStartExp = t;
			_tInsp = _tsStartExp - _tsStartInsp;
			_pMax = _next_pMax;
      _next_pMax = 0;
		}
	}
	else if( _state == RS_MECH_EXPIRATION ) {
		if( _pFilt < _next_pMin ) {
			_next_pMin = _pFilt;
			//tsMin = t;
		}
		if( _pFilt > _next_pMin + _pThr ) {
			_state = RS_MECH_INSPIRATION;
			_tsStartInsp = t;
			_tExp = _tsStartInsp - _tsStartExp;
			_pMin = _next_pMin;
      _next_pMin = 1000;
    }
	}
}

float RespirationAnalysis::getPP()
{
	return _pMax; // to be refined
}

float RespirationAnalysis::getPEEP()
{
	return _pMin; // to be refined
}

float RespirationAnalysis::getEI()
{
	return (float)_tExp / _tInsp;
}

float RespirationAnalysis::getRR()
{
	return 60000.0 / (_tExp+_tInsp);
}
