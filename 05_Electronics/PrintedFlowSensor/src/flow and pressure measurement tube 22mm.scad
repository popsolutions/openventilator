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

//$fn = 60;
$fa = 2;
$fs = 0.25;

cd = 2; // channel diameter
grid_d = 20;
ml = grid_d - 8;  // measurement port length
td = 10; // tube distance

measp_x = 18;
measp_y = 5;
measp1_p = [measp_x,-measp_y,ml/2-2];
measp2_p = [measp_x,+measp_y,ml/2-2];
measp_dia = 3.5;

th = ml/2;
bh = -ml/2;

use <ISO6356-1.scad>;
module line_grid( th, d, x=20, y=30, z=0.5 )
// Creates a grid of lines
// th is line thickness.
// d is distance between lines (center to center).
// x, y and z are dimenions of created line grid.
{
	for( p=[0:d:x-th] ) {
		translate( [p,0,0] ) cube( [th,y,z] );
	}
}

module grid_ring_pos( d, gr_w, gr_d, gr_h )
{
	// grid
	intersection() {
		cylinder( d=d+0.01, h=2, center=true );
		union() {
			translate( [-d*0.75, -d*0.75, -gr_h] ) 
				line_grid( th=gr_w, d=gr_d+0.01, x=d*1.5, y=d*1.5, z=gr_h );
			rotate( [0,0,90] ) translate( [-d*0.75, -d*0.75, 0  ] ) 
				line_grid( th=gr_w, d=gr_d+0.01, x=d*1.5, y=d*1.5, z=gr_h );
		}
	}
}

module grid_ring_neg( d, gr_w, gr_d, gr_h )
{
	cylinder( d=d, h=2*gr_h-0.01, center=true );
}

module channel( p1, p2, d=2 )
// Creates a channel with sperical ends between indicated points, with diameter d
{
	l = sqrt( pow( p2[0] - p1[0], 2 ) + pow( p2[1] - p1[1], 2 ) + pow( p2[2] - p1[2], 2 ) );
	ax = atan2( sqrt( pow( p2[0] - p1[0], 2 ) + pow( p2[1] - p1[1], 2 ) ), p2[2] - p1[2] );
	az = 90 + atan2( p2[1] - p1[1], p2[0] - p1[0] );
	
	translate( p1 ) rotate( [ax,0,az] ) cylinder( d=d, h=l );
	translate( p1 ) sphere( d=d );
	translate( p2 ) sphere( d=d );
}

module meas_port_pos( d=3, h=20 )
{
	cylinder( d=d+3, h=h-8 );
	translate( [0,0,h-8-0.01] ) cylinder( d1=d+3, d2=3, h=d/4 );
	cylinder( d=d, h=h-2 );
	translate( [0,0,h-2-0.01] ) cylinder( d1=d, d2=d-1, h=2 );
}

module meas_port_neg( d=3, h=20, d_under=3, depth=0 )
{
	translate( [0,0,-depth+1-0.01] ) cylinder( d=d_under, h=h-10+depth+0.01 );
	translate( [0,0,h+1-10-0.01] ) cylinder( d1=d_under, d2=0.25, h=d_under/2+0.01 );
	translate( [0,0,0-0.01] ) cylinder( d=1, h=h+0.01 );
	translate( [0,0,h-0.5+0.01] ) cylinder( d1=0, d2=d-1.5, h=0.5+0.01 );
}

module main_part()
{
	difference() {
		union() { // POSITIVE
			
			// bottom normal thickness
			translate( [0,0,bh] ) cylinder( d=26, h=7+0.01 );

			// middle normal thickness
			hull() {
				translate( [0,0,-grid_d/2-2] ) cylinder( d=0.01, h=grid_d+4+0.01 );
				// Middle thicker part
				translate( [0,0,-1.5] ) cylinder( d=grid_d+2, h=3 );
			}
			// top meas port area
			hull() {
				block_h = max(-1.5,min(-1,bh+8));
				translate( [0,0,-8.25+block_h] ) cylinder( d=26, h=ml/2+8.25-block_h );
				translate( [measp_x,-measp_y,block_h] ) cylinder( d=6.5, h=ml/2-block_h );
				translate( [measp_x,+measp_y,block_h] ) cylinder( d=6.5, h=ml/2-block_h );
			}

			// measurement ports
			translate( measp1_p ) meas_port_pos( d=measp_dia, h=10 );
			translate( measp2_p ) meas_port_pos( d=measp_dia, h=10 );

			// bulge for channel on bottom side
			if( bh<-11 ) {
				channel( [9.5,0,bh+2.5], [measp_x,0,bh+11], 5 );
			}
			if( bh<-14 ) {
				channel( [measp_x,+0,bh+11], [measp_x,+0,bh+14], 5 );
			}
		}
		union() { // NEGATIVE
			
			// edge inside bottomm connector

			// below grid chamber
			if( bh<-6 ) translate( [0,0,bh+4] ) cylinder( d1=18, d2=20, h=2+0.01 );
			translate( [0,0,bh] ) cylinder( d=18, h=6+0.01 );
			// make sure (for female part at bottom) that the inside is open enough
			translate( [0,0,bh-10] ) cylinder( d=24, h=10+0.01 );
			

			// space where grid goes into
			difference() {
				translate( [0,0,0] ) grid_ring_neg( grid_d, 0.25, 0.85, 0.25 );
				// Note there is empty space inside the walls around the grid to make sure the grid bridges are printed properly
				// make hole for tube through grid
				translate( [measp_x,0,0] ) cylinder( d=4, h=2+0.02, center=true );
			}
			
			// Middle thicker part
			difference() {
				hull() {
					translate( [0,0,-grid_d/2] ) cylinder( d=0.01, h=grid_d+0.01 );
					translate( [0,0,0] ) cylinder( d=grid_d-2, h=2, center=true );
				}
				// channel through chamber
				translate( [measp_x,+0,bh+5] ) cylinder( d=4, h= 40 );
				translate( [measp_x,+0,-1.75+0.01] ) cylinder( d1=4, d2=6, h= 1.5 );
				translate( [measp_x,+0,+0.25] ) cylinder( d1=6, d2=4, h=1.5 );
			}

			// above grid chamber
			translate( [0,0,th-6] ) cylinder( d=18, h=16+0.01 );
			if( th>6 ) translate( [0,0,th-4-2] ) cylinder( d1=20, d2=18, h=2+0.01 );
			
			// inside of thin sensing channel 1 (high one)
			channel( [0,0,th-2.5], [13,0,th-2.5] );
			channel( [13,0,th-2.5], [measp_x,-measp_y,th-2.5] );

			// inside of thin sensing channel 2 (low one)
			channel( [0,0,bh+2.5], [9.5,0,bh+2.5] );
			channel( [9.5,0,bh+2.5], [measp_x,0,min(0,bh+11)] );
			channel( [measp_x,+0,min(0,bh+11)], [measp_x,0,max(0,th-12)] );
			channel( [measp_x,0,max(0,th-12)], [measp_x,+measp_y,th-2.5] );
			
			// measurement ports
			translate( measp1_p ) meas_port_neg( d=3, h=10, d_under=2, depth=1.5 );
			translate( measp2_p ) meas_port_neg( d=3, h=10, d_under=2, depth=1.5 );

		}
	}
	// top connector (machine side)
	translate( [0,0,th-0.01] ) 22mm_female();
	// bottom connector (patient side)
	translate( [0,0,bh+0.01] ) rotate( [180,0,0] ) 22mm_male();

	difference() {
		// grid
		translate( [0,0,0] ) grid_ring_pos( grid_d, 0.35, 0.7, 0.25 );
		// make hole for tube through grid
		translate( [measp_x,0,0] ) cylinder( d=4.1, h=2+0.02, center=true );
	}
}

intersection() {
	main_part();
	// cutout
	//translate( [-100,-8+$t*16,-100] ) cube( [200,200,200] );
	//translate( [-100,0,-100] ) cube( [200,200,200] );
}
