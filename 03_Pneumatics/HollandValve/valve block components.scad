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

use <common.scad>;

hosetail_id=10.2;
hosetail_od=13.2;
//hosetail_id=16.5;
//hosetail_od=19.5;

module pressure_controlled_valve_bottom_pos()
{
	translate( [-15,-15,0] ) rounded_rectangle( 30, 30, 0.5, 7 );
}

module pressure_controlled_valve_bottom_neg( d )
{
	d2 = min(d,8) + 0.5;
	hull() {
		translate( [-14,-14,0.5] ) rounded_rectangle( 28, 28, 0.01, 6 );
		translate( [0,0,0.5+0.02] ) sine_hole( d=28, depth=d2+0.01 );
	}
}

module pressure_controlled_valve_membrane_neg()
{
	// Requires no holes in the membrane
}

module pressure_controlled_valve_top_pos()
{
	h=34/2-0.75;
	w=34;
	translate( [0,0,0] ) hull() {
		translate( [-w/2+9,-w/2+h,h-0.01] ) rounded_rectangle( w-2*9, w-2*h, 0.01, 0.01 );
		translate( [-w/2,-w/2,0.01] ) rounded_rectangle( w, w, 0.01, 9 );
	}
}

module pressure_controlled_valve_top_neg()
{
	d = 2.5;
	translate( [0,0,-d] ) {

/*
		// 13 entry holes
		for( y=[-7.5:5:7+.5+0.02] ) {
			translate( [-11.9,y,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		for( y=[-10:5:+10+0.02] ) {
			translate( [-7.6,y,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		for( y=[-7.5:5:7+.5+0.02] ) {
			translate( [-3.3,y,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}

		// 13 exit holes
		for( y=[-7.5:5:7+.5+0.02] ) {
			translate( [+3.3,y,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		for( y=[-10:5:+10+0.02] ) {
			translate( [+7.6,y,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		for( y=[-7.5:5:7+.5+0.02] ) {
			translate( [+11.9,y,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
*/
/*
		// large opening
		translate( [-13.5,-14,-0.01] ) rounded_rectangle( 12, 28, d+0.5+0.02, 6 );
		translate( [+1.5,-14,-0.01] ) rounded_rectangle( 12, 28, d+0.5+0.02, 6 );
*/
		// ellipses
		hull() {
			translate( [-11.9,-9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [-11.9,-2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [-11.9,+2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [-11.9,+9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [-7.6,-12,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [-7.6,-2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [-7.6,+2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [-7.6,+12,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [-3.3,-9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [-3.3,-2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [-3.3,+2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [-3.3,+9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [+11.9,-9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [+11.9,-2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [+11.9,+2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [+11.9,+9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [+7.6,-12,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [+7.6,-2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [+7.6,+2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [+7.6,+12,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [+3.3,-9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [+3.3,-2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		hull() {
			translate( [+3.3,+2.0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			translate( [+3.3,+9.5,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
	}
	translate( [0,0,0.5] ) {
		hull() {
			translate( [-1.5-12,-14,-0.01] ) rounded_rectangle( 12, 28, 0.01, 6 );
			translate( [-1.5-12+6,-14+6,6] ) rounded_rectangle( 0.01, 16, 0.01, 0.01 );
		}
	}
	translate( [0,0,0.5] ) {
		hull() {
			translate( [+1.5,-14,-0.01] ) rounded_rectangle( 12, 28, 0.01, 6 );
			translate( [+1.5+6,-14+6,6] ) rounded_rectangle( 0.01, 16, 0.01, 0.01 );
		}
	}
}

module one_way_valve_bottom_pos()
{
	translate( [-15,-15,0] ) rounded_rectangle( 30, 30, 0.5, 7 );
}

module one_way_valve_bottom_neg( d )
{
	d2 = d+0.5+0.01;
	cos_pg = concat( [[0,0]], [ for( i=[0:1:12+0.01] ) [i+1, d2/2 - d2/2 * cos( 15 * i )] ], [[28,d2],[28,0]] );
	intersection() {
		translate( [-14+0.01,-14,0.5+0.01] ) rotate( [-90,0,0] ) linear_extrude( 28 ) polygon( cos_pg );	
		translate( [-14,-14,-d+0.01] ) rounded_rectangle( 28, 28, d2, 6 );
	}
}

module one_way_valve_membrane_neg()
{
	// flap
	translate( [-10, -12.25, -0.01] ) cube( [10+0.01,0.5,1] );
	translate( [-10, -12, -0.01] ) cylinder( d=2, h=1 );
	translate( [-10, +11.75, -0.01] ) cube( [10+0.01,0.5,1] );
	translate( [-10, +12, -0.01] ) cylinder( d=2, h=1 );
	
	translate( [0,0,-0.01] ) difference() {
		cylinder( d=24.5, h=1 );
		translate( [0,0,-0.01] ) cylinder( d=23.5, h=1+0.02 );
		translate( [-19,-19,-0.01] ) cube( [19,38,1+0.02] );
	}
}

module one_way_valve_top_pos()
{
	translate( [0,0,0] ) corridor_joint( w=19.5, h=17, f=0.75, b=2 );

/*
	translate( [0,0,0] )
		cylinder( d=19.5, h=7.25 );
	translate( [0,0,7.25] )
		cylinder( d1=19.5, d2=1.5, h=(19.5-1.5)/2 );
*/
	}

module one_way_valve_top_neg()
{
	d = 2.5;
	translate( [0,0,-d] ) {
/*
		// 15 entry holes
		for( a=[18:72:359] ) {
			rotate( [0,0,a] ) translate( [3.3,0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}
		for( a=[0:36:359] ) {
			rotate( [0,0,a] ) translate( [7.0,0,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
		}		
*/
		// 7 holes
		for( a=[0:60:359] ) {
			rotate( [0,0,a] ) hull() {
				translate( [5.5,   0,-0.01] ) cylinder( d=4, h=d+0.5+0.02 );
				translate( [6.6,-1.2,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
				translate( [6.2,   0,-0.01] ) cylinder( d=4, h=d+0.5+0.02 );
				translate( [6.6,+1.2,-0.01] ) cylinder( d=3, h=d+0.5+0.02 );
			}
		}
		translate( [0,0,-0.01] ) cylinder( d=4, h=d+0.5+0.02 );
	}
	translate( [0,0,0.5] )corridor_joint( w=16.5, h=16.5/2+6.75-0.5 );
/*
	translate( [0,0,0.5] )
		cylinder( d=16.5, h=6.75-0.5 );
	translate( [0,0,6.75-0.01] )
		cylinder( d1=16.5, d2=0, h=16.5/2 );
*/
}

module via_bottom_pos( r=9.75, e=0 )
{
	hull() {
		translate( [0,-e/2,0] ) cylinder( r=r-0.5, h=0.5 );
		translate( [0,+e/2,0] ) cylinder( r=r-0.5, h=0.5 );
	}
}

module via_bottom_neg( d, r=9.75, e=0 )
{
	hull() {
		translate( [0,-e/2,-d] ) cylinder( r=r-1.5, h=d+0.5+0.01 );
		translate( [0,+e/2,-d] ) cylinder( r=r-1.5, h=d+0.5+0.01 );
	}
}

module via_membrane_neg( r=9.75, e=0 )
{
	// hole
	hull() {
		translate( [0,-e/2,-0.01] ) cylinder( r=r-1.5, h=1 );
		translate( [0,+e/2,-0.01] ) cylinder( r=r-1.5, h=1 );
	}
}

module via_top_pos( r=9.75, e=0 )
{
	hull() {
		translate( [0,-e/2,0] )
			cylinder( r1=r+2, r2=r, h=2 );
		translate( [0,+e/2,0] )
			cylinder( r1=r+2, r2=r, h=2 );
	}
	hull() {
		translate( [0,-e/2,0] )
			cylinder( r=r, h=17-9.75 );
		translate( [0,+e/2,0] )
			cylinder( r=r, h=17-9.75 );
		cylinder( d=1.5, h=17-1.5/2 );
	}
}

module via_top_neg( r=9.75, e=0 )
{
	translate( [0,0,-2-0.01] ) hull() {
		translate( [0,-e/2,0] ) cylinder( r=r-1.5, h=2.5+0.02 );
		translate( [0,+e/2,0] ) cylinder( r=r-1.5, h=2.5+0.02 );
	}
	hull() {
		translate( [0,-e/2,0] )
			cylinder( r=r-1.5, h=16.5-9.75 );
		translate( [0,+e/2,0] )
			cylinder( r=r-1.5, h=16.5-9.75 );
		cylinder( d=0.01, h=15.75-1.5/2 );
	}
}

module corridor_pos( l=35 )
{
	/*
	rotate( [-90,-90,0] ) intersection() {
		union() {
			scale( [1.319,1,1] ) translate( [2,0,0] )  cylinder( d=19.5, h=l );
			linear_extrude( height=l ) polygon( [[0,-11.75],[11.75,0],[0,+11.75]] );
		}
		translate( [0,-20,-0.01]) cube( [20,40,l] );	
	}
	*/
	translate( [0,0,0] ) corridor( w=19.5, l=l, h=17, f=0.75, b=2 );
}

module corridor_neg( l=35, w=16.5, h=14.5 )
{
	translate( [0,-0.01,0.5] ) corridor( w=w, l=l, h=h );
}

module multiple_corridor_neg( l, w, h )
{
	translate( [-w-1,0,0] ) corridor( l=l, w=w, h=h );
	translate( [   0,0,0] ) corridor( l=l, w=w, h=h );
	translate( [+w+1,0,0] ) corridor( l=l, w=w, h=h );
}

module corridor_joint_pos()
{
	translate( [0,0,0] ) corridor_joint( w=19.5, h=17, f=0.75, b=2 );
}

module corridor_joint_neg()
{
	translate( [0,-0.01,0.5] ) corridor_joint( w=16.5, h=14.5 );
}

module hosetail_pos( h=30 )
{
	hosetail( d2=hosetail_od, d1=hosetail_id, h=h );
	cylinder( d=hosetail_od, h=0.5 ); 
	translate( [0,0,0] ) corridor_joint( w=19.5, h=17, f=0.75, b=2 );

}

module hosetail_neg( h=30 )
{
	translate( [0,-0.01,0.5-0.01] ) cylinder( d=hosetail_id, h=h );
	translate( [0,-0.01,0.5] ) corridor_joint( w=16.5, h=14.5 );
}

module bottom_hosetail_neg( d )
{
	translate( [0,0,-d] ) cylinder( d=16.5, h=d+2 );
}

module screw_hole_bottom_neg( d1=3, d2=7, h=9 )
{			
	cylinder( d=d1, h=h );
	cylinder( d1=d2, d1=3, h=(d2-d1)/2 );
}

intersection() {
	translate( [-100,0,-100] ) cube( [1000,1000,1000] );
	union() {
	
		translate( [0,0,0] ) difference() {
			pressure_controlled_valve_bottom_pos();
			pressure_controlled_valve_bottom_neg( 4 );
		}

		translate( [0,0,30] ) difference() {
			pressure_controlled_valve_top_pos();
			pressure_controlled_valve_top_neg();
		}

		translate( [40,0,0] ) difference() {
			one_way_valve_bottom_pos();
			one_way_valve_bottom_neg( 4 );
		}

		translate( [40,0,30] ) difference() {
			one_way_valve_top_pos();
			one_way_valve_top_neg();
		}
		
		translate( [80,0,0] ) difference() {
			via_bottom_pos( 6, 6 );
			via_bottom_neg( 4, 6, 6 );
		}
		translate( [80,0,30] ) difference() {
			via_top_pos( 6, 6 );
			via_top_neg( 6, 6 );
		}
		translate( [120,0,0] ) difference() {
			corridor_pos( 20 );
			corridor_neg( 20+0.02 );
		}
		translate( [160,0,0] ) difference() {
			corridor_joint_pos();
			corridor_joint_neg();
		}
		translate( [200,0,0] ) difference() {
			hosetail_pos();
			hosetail_neg();
		}
		translate( [200,0,30] ) difference() {
			bottom_hosetail_neg( 4 );
		}
	}
}
color( "darkblue", 0.25 ) intersection() {
	translate( [-100,-0.02,-100] ) cube( [1000,1000,1000] );
	union() {

		translate( [0,0,0] ) union() {
			 pressure_controlled_valve_bottom_neg( 4 );
		}
		translate( [0,0,20] ) union() {
			 pressure_controlled_valve_membrane_neg();
		}
		translate( [0,0,30] ) union() {
			pressure_controlled_valve_top_neg();
		}
		translate( [40,0,0] ) union() {
			one_way_valve_bottom_neg( 4 );
		}
		translate( [40,0,20] ) union() {
			one_way_valve_membrane_neg();
		}
		translate( [40,0,30] ) union() {
			one_way_valve_top_neg();
		}
		translate( [80,0,0] ) union() {
			via_bottom_neg( 4, 6, 6 );
		}
		translate( [80,0,20] ) union() {
			via_membrane_neg( 6, 6 );
		}
		translate( [80,0,30] ) union() {
			via_top_neg( 6, 6 );
		}
		translate( [120,0,0] ) union() {
			corridor_neg( 20 );
		}
		translate( [160,0,0] ) union() {
			corridor_joint_neg();
		}
		translate( [200,0,0] ) union() {
			hosetail_neg();
		}
		translate( [200,0,30] ) union() {
			bottom_hosetail_neg( 4 );
		}
	}
}

