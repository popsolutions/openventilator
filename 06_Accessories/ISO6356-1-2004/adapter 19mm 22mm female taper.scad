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

/*
Dimensions of 22 mm conical male connector according to ISO 6356-1:2004
Should engage at least 15 mm, at that point diameter should be 22.37 mm
Should when engaged have 6 mm clearance left
Should have taper ratio of 1:40
Length of cone should be:
- 21 mm to shoulder of 26 mm diameter; OR
- 19.5 mm to recess of a 20 mm diameter, 6 mm long, with sharp edges.
That means at the cone front, the diameter is 21.99 mm
And at the cone end, the diameter is 22.52 mm. Total cone legth 21.00 mm.

Dimensions of 22 mm conical female connector according to ISO 6356-1:2004
Inside measurement at flange is 22.37 mm
There is at least 21 mm taper length
Should have taper ratio of 1:40
That means that if we make the taper 22.00 mm deep, it has a diameter of 21.82 there.
*/

use <projected text.scad>

$fn = 360;

tl = 60;

difference() {
	union() {
		translate( [0,0,0] ) cylinder( d1=18.5, d2=19.5, h=4+0.01 );
		translate( [0,0,4] ) cylinder( d=19.5, h=30-4+0.01 );
		translate( [0,0,30] ) cylinder( d1=19.5, d2=26, h=3+0.01 );
		translate( [0,0,30+3] ) cylinder( d=26, h=tl-30-3-0.25+0.01 );
		translate( [0,0,tl-0.25] ) cylinder( d2=26-0.5, d1=26, h=0.25 );
		translate( [0,0,tl-5] ) round_text( "22 mm", 3, 13.2 );
	}
	translate( [0,0,-0.01] ) cylinder( d=16.5, h=40+0.02 );
	translate( [0,0,30+1] ) cylinder( d1=16.5, d2=22, h=tl-30-1-22 );
	translate( [0,0,tl-22-0.01] ) cylinder( d1=21.82, d2=22.37, h=22 );
	translate( [0,0,tl-0.25] ) cylinder( d1=22.37, d2=22.37+0.5, h=0.25+0.01 );
	
	//translate( [-100,-100,-100] ) cube( [200,100,200] );
}
