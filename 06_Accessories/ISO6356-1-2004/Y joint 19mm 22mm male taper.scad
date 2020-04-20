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

use <projected text.scad>

$fn = 200;

tl = 85;

difference() {
	union() {
		translate( [0,0,tl-0.25] ) cylinder( d2=21.99-0.5, d1=21.99, h=0.25 );
		translate( [0,0,tl-0.25-21] ) cylinder( d2=21.99, d1=22.52, h=21.00 );
		translate( [0,0,tl-21-5] ) cylinder( d=26, h=5+0.01 );
		translate( [-20,0,0] ) cylinder( d1=18.5, d2=19.5, h=4+0.01 );
		translate( [-20,0,4] ) cylinder( d=19.5, h=30-4 );
		translate( [+20,0,0] ) cylinder( d1=18.5, d2=19.5, h=4+0.01 );
		translate( [+20,0,4] ) cylinder( d=19.5, h=30-4 );

		translate( [0,0,tl-21-2.5] ) round_text( "22 mm", 3, 13.2 );
		
		// Y external
		hull() {
			translate( [0,0,tl-21-5-0.01] ) cylinder( d=26, h=0.01 );
			translate( [-20,0,30-0.01] ) cylinder( d=19.5, h=1+0.01 );
		}
		hull() {
			translate( [0,0,tl-21-5-0.01] ) cylinder( d=26, h=0.01 );
			translate( [+20,0,30-0.01] ) cylinder( d=19.5, h=1+0.01 );
		}
	}
	translate( [0,0,tl-21-3-0.01] ) cylinder( d=18, h=40+0.02 );
	translate( [-20,0,-0.01] ) cylinder( d=16.5, h=30+0.5+0.02 );
	translate( [+20,0,-0.01] ) cylinder( d=16.5, h=30+0.5+0.02 );
	
	// Y internal
	hull() {
		translate( [0,0,tl-21-3] ) cylinder( d=18, h=0.01 );
		translate( [-20,0,30+0.5] ) cylinder( d=16.5, h=0.01 );
	}
	hull() {
		translate( [0,0,tl-21-3] ) cylinder( d=18, h=0.01 );
		translate( [+20,0,30+0.5] ) cylinder( d=16.5, h=0.01 );
	}

	//translate( [-100,-100,-100] ) cube( [200,100,200] );
}
