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

use <MPXV.scad>;

//$fn = 180;
$fa = 2;
$fs = 0.25;

tl = 110; // total length
cd = 2; // channel diameter

tube_od = 19.5;
tube_id = tube_od - 3;

s1_p = [-10,0,52];

s1_r = [180,90,0];

opening_h = 35;

port1_end_rel_p = [6.35,-2.16,+1.27+7.11 ]; // port for (usually positive) pressure
port2_end_rel_p = [6.35,+2.16,+1.27 ]; // port for reference pressure

// screw hole positions
cap_sh1_p = s1_p + [1-0.01,+5,24];
pcb_sh1_p = s1_p + [1-0.01,-2,15.5];

// cap holder pins
cap_pin1_p = s1_p + [-13.15,0,-0.01];

function rotate_v( a ) =
	  [[1,0,0],						[0,cos(a[0]),sin(a[0])],	[0,-sin(a[0]),cos(a[0])]]
	* [[cos(a[1]),0,-sin(a[1])],		[0,1,0],					[sin(a[1]),0,cos(a[1])]]
	* [[cos(a[2]),sin(a[2]),0],	[-sin(a[2]),cos(a[2]),0],	[0,0,1]] ;

layer_h = 0.25;
extrude_th = 0.35;

module line_grid( th, d, x=20, y=30, z=0.5 )
// Creates a grid of lines
// th is line thickness.
// d is distance between lines (center to center).
// x, y and z are dimenions of created line grid.
{
	for( p=[0:d:x-th] ) {
		translate( [p,0,0] ) cube( [th,y,z] );
	}
//	for( p=[0:d:y-th] ) {
//		translate( [0,p,0] ) cube( [x,th,z] );
//	}
}

module channel( p1, p2, d )
// Creates a channel with sperical ends between indicated points, with diameter d
{
	l = sqrt( pow( p2[0] - p1[0], 2 ) + pow( p2[1] - p1[1], 2 ) + pow( p2[2] - p1[2], 2 ) );
	ax = atan2( sqrt( pow( p2[0] - p1[0], 2 ) + pow( p2[1] - p1[1], 2 ) ), p2[2] - p1[2] );
	az = 90 + atan2( p2[1] - p1[1], p2[0] - p1[0] );
	
	translate( p1 ) rotate( [ax,0,az] ) cylinder( d=d, h=l );
	translate( p1 ) sphere( d=d );
	translate( p2 ) sphere( d=d );
}

module clip() 
{
	rotate( [90,0,90] ) linear_extrude( height=2 ) polygon( [[0,0],[1,0],[1,1],[0,2]] );
}

module main_part()
{
	difference() {
		union() { // POSITIVE
			translate( [0,0,0] ) cylinder( d1=tube_id+1, d2=tube_od, h=4+0.01, $fn=360 );
			translate( [0,0,4] ) cylinder( d=tube_od, h=tl-4-4+0.01, $fn=360 );
			translate( [0,0,tl-4] ) cylinder( d1=tube_od, d2=tube_id+1, h=4, $fn=360 );

			translate( [0,0,tl-30-0.01] ) cylinder( d1=20, d2=tube_od, h=(20-tube_od)/2, $fn=360 );
			translate( [0,0,30-(20-tube_od)/2+0.01] ) cylinder( d1=tube_od, d2=20, h=(20-tube_od)/2, $fn=360 );
			
			// box for channels and sensors
			difference () {
				hull() {
					translate( s1_p ) rotate( s1_r ) rotate( [0,90,0 ] ) translate( [-5,0,-3] ) cylinder( d=20, h=20, center=true );
					translate( [0,0,30] ) cylinder( d=20, h=tl-30-30 );
				}
				// open space where cap will come
				translate( [-10-10,0,s1_p[2]+20] ) rotate( [0,0,0] ) cube( [20,50,40], center=true );
			}
			// bottom pin to drop cap onto
			translate( cap_pin1_p ) cylinder( d=1.5, h=2 );
			
			// top pin to drop cap onto
			translate( s1_p ) hull() {
				translate( [-2,0,22.5] ) cylinder( d=1.5, h=1 );
				translate( [+1,0,19.5] ) cylinder( d=1.5, h=7 );
			}
			intersection() {
				translate( s1_p ) translate( [-2,0,23] ) cylinder( d=1.5, h=4 );
				translate( s1_p ) translate( [-1,0,23] ) rotate( [0,45,0] ) cube( 5.8, center=true );
			}
			
			// fixing 
			translate( s1_p ) hull() {
				translate( [-4.5,3,19] ) cylinder( d=2, h=1 );
				translate( [+1.5,3,13] ) cylinder( d=2, h=1 );
			}
			translate( s1_p ) hull() {
				translate( [-4.5,3,19] ) cylinder( d=2, h=1 );
				translate( [+1.5,3,25] ) cylinder( d=2, h=1 );
			}
		}
		union() { // NEGATIVE
			// inside of tube
			translate( [0,0,-0.01] ) cylinder( d=tube_id, h=tl+0.02, $fn=360 );

			// flow sensor space
			translate( s1_p ) rotate( s1_r ) sensor_space( second_port=true );
			
			// inside of thin sensing channel 1 (high one)
			channel( [-10,2,opening_h+8], [-8,2,opening_h+8], cd );
			channel( [-10,2,opening_h+8], s1_p + port1_end_rel_p * rotate_v( s1_r ), cd );

			// inside of thin sensing channel to outside
			channel( s1_p + port2_end_rel_p * rotate_v( s1_r ), s1_p + [-1,0,-16], cd );
			channel( s1_p + [-1,0,-16], s1_p + [-1,0,-25], cd );
			
			// clips
			translate( [-10-0.01,-10-0.01,s1_p[2]] ) clip();
			translate( [-10-0.01,+10+0.01,s1_p[2]] ) mirror( [0,1,0] ) clip();

			// cutout
			//translate( [-100,-105+$t*10,-100] ) cube( [200,100,200] );
		}
	}
}

module cap()
{
	difference() {
		union() { // POSITIVE
			intersection () {
				hull() {
					translate( s1_p ) rotate( s1_r ) rotate( [0,90,0 ] ) translate( [-5,0,-3] ) cylinder( d=20, h=20, center=true );
					translate( [0,0,30] ) cylinder( d=20, h=tl-30-30 );
				}
				translate( [-10-10,0,s1_p[2]+20] ) rotate( [0,0,0] ) cube( [20,50,40], center=true );
			}
			
			// clips
			translate( [-10-0.01,-10-0.01,s1_p[2]] ) clip();
			translate( [-10-0.01,+10+0.01,s1_p[2]] ) mirror( [0,1,0] ) clip();
		}
		union() { // NEGATIVE
			difference() {
				hull() {
					translate( s1_p ) rotate( s1_r ) rotate( [0,90,0 ] ) translate( [-5,0,-3] ) cylinder( d=18, h=19, center=true );
					translate( [0,0,30-1] ) cylinder( d=18, h=tl-30-30 );
				}
				// ears for cap pins
				translate( cap_pin1_p + [-2,0,0] ) rotate( [0,0,0 ] ) cylinder( d=7, h=1 );
		}
			// top cap pin hole
			translate( s1_p ) translate( [-2,0,23] ) cylinder( d=1.7, h=4 );
			
			// bottom cap pin hole
			translate( cap_pin1_p ) rotate( [0,0,0 ] ) cylinder( d=1.7, h=2 );

			// cable hole
		translate( s1_p ) translate( [-1,0,16] ) rotate( [0,-45,0] ) cylinder( d=5, h=10 );
		}
	}
}
main_part();
translate( [0.05,0,-s1_p[2]] ) cap();

// flow sensor
//color( "darkgrey" ) translate( s1_p ) rotate( s1_r ) sensor_space( second_port=true );
// cable
//color( "darkgrey" ) translate( s1_p ) translate( [-1,0,16] ) rotate( [0,-45,0] ) cylinder( d=4, h=30 );
