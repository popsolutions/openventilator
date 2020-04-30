/*
Copyright (c) 2020, Joris Robijn
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

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

od1 = 13.2; // Normally take hose inner diameter + 0.5 mm (depending on stretchability of hose)
id1 = od1-3;
od2 = 13.2;
id2 = od2-3;
od3 = 13.2;
id3 = od3-3;

pos2 = -20;
pos3 = +20;

$fn = 200;

tl = 85;

difference() {
	union() { // POSITIVE
		translate( [0,0,tl-4] ) cylinder( d2=od1-1, d1=od1, h=4 );
		translate( [0,0,tl-30] ) cylinder( d2=od1, d1=od1, h=30-4 );
		translate( [pos2,0,0] ) cylinder( d1=od2-1, d2=od2, h=4+0.01 );
		translate( [pos2,0,4] ) cylinder( d=od2, h=30-4 );
		translate( [pos3,0,0] ) cylinder( d1=od3-1, d2=od3, h=4+0.01 );
		translate( [pos3,0,4] ) cylinder( d=od3, h=30-4 );

		// Y external
		hull() {
			translate( [0,0,tl-30-0.4*od1-0.01] ) cylinder( d=od1, h=0.4*od1+0.01 );
			translate( [pos2,0,30-0.01] ) cylinder( d=od2, h=0.4*od2+0.01 );
		}
		hull() {
			translate( [0,0,tl-30-0.4*od1-0.01] ) cylinder( d=od1, h=0.4*od1+0.01 );
			translate( [pos3,0,30-0.01] ) cylinder( d=od3, h=0.4*od3+0.01 );
		}
	}
	union() { // NEGATIVE
		translate( [0,0,tl-30-0.5-0.01] ) cylinder( d=id1, h=30+0.5+0.02 );
		translate( [pos2,0,-0.01] ) cylinder( d=id2, h=30+0.5+0.02 );
		translate( [pos3,0,-0.01] ) cylinder( d=id3, h=30+0.5+0.02 );
		
		// Y internal
		hull() {
			translate( [0,0,tl-30-0.5-0.4*id1] ) cylinder( d=id1, h=0.4*id1+0.01 );
			translate( [pos2,0,30+0.5] ) cylinder( d=id2, h=0.4*id2+0.01 );
		}
		hull() {
			translate( [0,0,tl-30-0.5-0.4*id1] ) cylinder( d=id1, h=0.4*id1+0.01 );
			translate( [pos3,0,30+0.5] ) cylinder( d=id3, h=0.4*id3+0.01 );
		}
		// make coutout
		translate( [-100,-100,-100] ) cube( [200,100,200] );
	}
}
