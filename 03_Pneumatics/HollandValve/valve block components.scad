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
use <rounded_60deg_segment.scad>;

// hosetail settings
//hosetail_od=19.5;
hosetail_od=16.0;
//hosetail_od=13.5;
hosetail_id=hosetail_od-3;

// top corridor settings
top_corr_ow = 20;	// outside width
top_corr_oh = 17;	// outside height
top_corr_f  = 0.75;	// flattening of top
top_corr_iw = top_corr_ow - 3;	// inside width
top_corr_ih = top_corr_oh - 2;	// inside height
top_corr_osh = top_corr_oh - top_corr_ow/2; // outside shoulder height
top_corr_ish = top_corr_ih - top_corr_iw/2; // inside shoulder height
top_corr_b = 1;

echo( top_corr_ih );

module pressure_controlled_valve_bottom_pos( x=30, y=30 )
{
	translate( [-x/2,-y/2,0] ) rounded_rectangle( x, y, 0.5, 7 );
}

module pressure_controlled_valve_bottom_neg( x=30, y=30, d )
{
	d2 = min(d,6) + 0.5;
	hull() {
		translate( [-x/2+1,-y/2+1,0.5] ) rounded_rectangle( x-2, y-2, 0.01, 6 );
		translate( [-(x-y)/2,0,0.5+0.02] ) sine_hole( d=min(x,y)-2, depth=d2+0.01 );
		translate( [(x-y)/2,0,0.5+0.02] ) sine_hole( d=min(x,y)-2, depth=d2+0.01 );
	}
	
}

module pressure_controlled_valve_membrane_neg( x, y )
{
	// Requires no holes in the membrane
}

module pressure_controlled_valve_top_pos( x=30, y=30 )
{
	h=17-0.75;
	translate( [0,0,0] ) hull() {
		translate( [-x/2+8,-1.5/2,h-0.01] ) rounded_rectangle( x-16, 1.5, 0.01, 0.01 );
		translate( [-x/2-1,-y/2-1,-0.01] ) rounded_rectangle( x+2, y+2, 0.01, 8 );
	}
	// Add corridor
	translate( [-x/2-1,0,0] ) rotate( [0,0,-90] ) corridor( w=top_corr_ow, l=x+2, h=top_corr_oh, f=top_corr_f, b=top_corr_b );
}

module pressure_controlled_valve_top_neg( x=30, y=30, rot=0 )
{
	d = 3;
	diag_size=sqrt( x*x + y*y );

	translate( [0,0,-d+0.5+0.01] ) {
		intersection() {
			union() {
				// entry holes
				rotate( [0,0,-90+rot] ) translate( [-diag_size/2,1,0] ) cylinder_grid( x=diag_size, y=diag_size/2, h=d+0.5+0.02, d=4.5, x_incr=6 );

				// exit holes
				rotate( [0,0,90+rot] ) translate( [-diag_size/2,1,0] ) cylinder_grid( x=diag_size, y=diag_size/2, h=d+0.5+0.02, d=4.5, x_incr=6 );
			}
			translate( [-x/2+1,-y/2+1,0] ) rounded_rectangle( x-2, y-2, d, 6 );
		}
	}
	translate( [0,0,0.5] ) {
		difference() {
			// roof shape inside
			union() {
				hull() {
					translate( [-x/2+6,0,top_corr_ih-0.5] ) rounded_rectangle( x-20, 0.01, 0.01, 0.01 );
					translate( [-x/2+1,-y/2+1,0] ) rounded_rectangle( x-2, y-2, 0.01, 6 );
				}
				translate( [-x/2-1,0,0] ) rotate( [0,0,-90] ) corridor( w=top_corr_iw, l=x, h=top_corr_ih-0.5 );
			}
			// wall
			rotate( [0,0,-90+rot] ) translate( [-diag_size/2,-1,0] ) cube( [diag_size,2,top_corr_ih-0.5+0.01] );
		}
	}
}

module one_way_valve_bottom_pos( r=8.5, e=0, type=0 )
{
	x = 2*r+2*e+11;
	y = 2*r+11;
	translate( [-x/2-1,-y/2-1,0] ) rounded_rectangle( x+2, y+2, 0.5, 7 );
}

module one_way_valve_bottom_neg( d, r=8.5, e=0, type=0 )
{
	d2 = d+0.5+0.01;
	x = 2*r+2*e+11;
	y = 2*r+11;
	cos_pg = concat( [[0,0]], [ for( i=[0:0.05:1+0.0001] ) [i*x/2, d2/2 - d2/2 * cos( 180*i )] ], [[x,d2],[x,0]] );
	intersection() {
		translate( [-x/2+0.01,-y/2,0.5+0.01] ) rotate( [-90,0,0] ) linear_extrude( y ) polygon( cos_pg );	
		translate( [-x/2,-y/2,-d+0.01] ) rounded_rectangle( x, y, d2, 6 );
	}
}

module one_way_valve_membrane_neg( r=8.5, e=0, type=0 )
{
	x = 2*r+2*e+11;
	y = 2*r+11;
	
	// flap
	translate( [-r-e, -r-3.75-0.25, -0.01] ) cube( [+r+2*e+0.01,0.5,1] );
	translate( [-r-e, -r-3.75     , -0.01] ) cylinder( d=2, h=1 );
	translate( [-r-e, +r+3.75-0.25, -0.01] ) cube( [+r+2*e+0.01,0.5,1] );
	translate( [-r-e, +r+3.75, -0.01] ) cylinder( d=2, h=1 );
	
	translate( [e,0,-0.01] ) difference() {
		cylinder( r=r+4.0, h=1 );
		translate( [0,0,-0.01] ) cylinder( r=r+3.5, h=1+0.02 );
		translate( [-25,-25,-0.01] ) cube( [25,50,1+0.02] ); // cut off left half
	}
}

module one_way_valve_top_pos( r=8.5, e=0, type=0 )
{
	hull() {
		translate( [-e,0,0] )
			cylinder( r1=r+1.5+top_corr_b, r2=r+1.5, h=top_corr_b );
		translate( [+e,0,0] )
			cylinder( r1=r+1.5+top_corr_b, r2=r+1.5, h=top_corr_b );
	}
	hull() {
		translate( [-e,0,0] )
			cylinder( r=r+1.5, h=top_corr_osh );
		translate( [+e,0,0] )
			cylinder( r=r+1.5, h=top_corr_osh );
		cylinder( d=2*top_corr_f, h=top_corr_oh-top_corr_f );
	}
}

module one_way_valve_top_neg( r=8.5, e=0, type=0 )
{
	d = 2.5;
	if( type == 0 ) {
		translate( [0,0,-d] ) {
			// 7 holes
			//intersection() {
			//	union() { 
					for( a=[0:60:359] ) {
						rotate( [0,0,a] ) 
							linear_extrude( height = d+0.5+0.02 ) rounded_60deg_segment( 8.5, 2.25, 1, 2.25+1.5, res=2 );
/*						hull() {
							translate( [6.0,   0,-0.01] ) cylinder( d=4.5, h=d+0.5+0.02 );
							translate( [7.2,-2.1,-0.01] ) cylinder( d=2, h=d+0.5+0.02 );
							translate( [8.2,   0,-0.01] ) cylinder( d=0.5, h=d+0.5+0.02 );
							translate( [7.2,+2.1,-0.01] ) cylinder( d=2, h=d+0.5+0.02 );
						}*/
					}
					translate( [0,0,-0.01] ) cylinder( d=4.5, h=d+0.5+0.02 );
				//}
				// bounding cylinder
				//translate( [0,0,-0.01] ) cylinder( d=17, h=d+0.5+0.02 );
			//}
		}
	}
	else if( type == 1 ) {
		translate( [0,0,-d] ) {
			// 2 holes
			translate( [-e,0,-0.01] ) cylinder( r=r, h=d+0.5+0.02 );
			translate( [+e,0,-0.01] ) cylinder( r=r, h=d+0.5+0.02 );
		}
	}
	hull() {
		translate( [-e,0,0.5] )
			cylinder( r=r, h=top_corr_ish-0.5 );
		translate( [+e,0,0.5] )
			cylinder( r=r, h=top_corr_ish-0.5 );
		translate( [0,0,0.5] )
			cylinder( d=0.01, h=top_corr_ih-0.5 );
	}
}

module via_bottom_pos( r=8.5, e=0 )
{
	hull() {
		translate( [0,-e,0] ) cylinder( r=r+1, h=0.5 );
		translate( [0,+e,0] ) cylinder( r=r+1, h=0.5 );
	}
}

module via_bottom_neg( d, r=8.5, e=0 )
{
	hull() {
		translate( [0,-e,-d] ) cylinder( r=r, h=d+0.5+0.01 );
		translate( [0,+e,-d] ) cylinder( r=r, h=d+0.5+0.01 );
	}
}

module via_membrane_neg( r=8.5, e=0 )
{
	// hole
	hull() {
		translate( [0,-e,-0.01] ) cylinder( r=r, h=1 );
		translate( [0,+e,-0.01] ) cylinder( r=r, h=1 );
	}
}

module via_top_pos( r=8.5, e=0 )
{
	hull() {
		translate( [0,-e,0] )
			cylinder( r1=r+1.5+top_corr_b, r2=r+1.5, h=top_corr_b );
		translate( [0,+e,0] )
			cylinder( r1=r+1.5+top_corr_b, r2=r+1.5, h=top_corr_b );
	}
	hull() {
		translate( [0,-e,0] )
			cylinder( r=r+1.5, h=top_corr_osh );
		translate( [0,+e,0] )
			cylinder( r=r+1.5, h=top_corr_osh );
		cylinder( d=2*top_corr_f, h=top_corr_oh-top_corr_f );
	}
}

module via_top_neg( r=8.5, e=0 )
{
	translate( [0,0,-2-0.01] ) hull() {
		translate( [0,-e,0] ) cylinder( r=r, h=2.5+0.02 );
		translate( [0,+e,0] ) cylinder( r=r, h=2.5+0.02 );
	}
	hull() {
		translate( [0,-e,0] )
			cylinder( r=r, h=top_corr_ish );
		translate( [0,+e,0] )
			cylinder( r=r, h=top_corr_ish );
		cylinder( d=0.01, h=top_corr_ih );
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
	translate( [0,0,0] ) corridor( w=top_corr_ow, l=l, h=top_corr_oh, f=top_corr_f, b=top_corr_b );
}

module corridor_neg( l=35, w=top_corr_iw, h=top_corr_ih )
{
	translate( [0,-0.01,0.5] ) corridor( w=w, l=l, h=h-0.5 );
}

module multiple_corridor_neg( l, w, h )
{
	// Calculate channel width
	num_channels = ceil( w / h );
	pw = (h+3)/7; // pillar width
	cw = (w-(num_channels-1)*pw) / num_channels; // channel width

	// Create polygon with the shape of the channel
	pg = [[-cw/2+0.5,0],[-cw/2,0.5],[-cw/2,h-cw/2],[0,h],[+cw/2,h-cw/2],[+cw/2,0.5],[+cw/2-0.5,0]];
	
	translate( [0,0,-h-1] ) for( i=[0:1:num_channels-1+0.01] ) {
		rotate( [90,0,180] ) translate( [-w/2+cw/2+i*(cw+pw),0,0] ) linear_extrude( height=l+0.02 ) polygon( pg );
	}
}

module multiple_bent_corridor_neg( r, start_a, bend_a, w, h )
{
	// Calculate channel width
	num_channels = ceil( w / h );
	pw = (h+3)/7; // pillar width
	cw = (w-(num_channels-1)*pw) / num_channels; // channel width

	// Create polygon with the shape of the channel
	pg = [[-cw/2+0.5,0],[-cw/2,0.5],[-cw/2,h-cw/2],[0,h],[+cw/2,h-cw/2],[+cw/2,0.5],[+cw/2-0.5,0]];

	translate( [0,0,-h-1] ) rotate( [0,0,start_a] ) {
		rotate_extrude( angle=bend_a ) {
			for( i=[0:1:num_channels-1+0.01] ) {
				translate( [r-w/2+cw/2+i*(cw+pw),0,0] ) polygon( pg );
			}
		}
	}
}

module corridor_joint_pos()
{
	translate( [0,0,0] ) corridor_joint( w=top_corr_ow, h=top_corr_oh, f=top_corr_f, b=top_corr_b );
}

module corridor_joint_neg()
{
	translate( [0,-0.01,0.5] ) corridor_joint( w=top_corr_iw, h=top_corr_ih-0.5 );
}

module hosetail_pos( h=30 )
{
	hosetail( d2=hosetail_od, d1=hosetail_id, h=h, bottom_base_extension=2*top_corr_b, bottom_base_height=top_corr_b );
	cylinder( d=hosetail_od, h=0.5 ); 
	translate( [0,0,0] ) corridor_joint( w=top_corr_ow, h=top_corr_oh, f=top_corr_f, b=top_corr_b );

}

module hosetail_neg( h=30 )
{
	translate( [0,-0.01,0.5-0.01] ) cylinder( d=hosetail_id, h=h );
	translate( [0,-0.01,0.5] ) corridor_joint( w=top_corr_iw, h=top_corr_ih-0.5 );
}

module bottom_hosetail_neg( d )
{
	translate( [0,0,-d] ) cylinder( d=16.5, h=d+2 );
}

module meas_port_pos( d=3, h=20 )
{
	cylinder( d=d+3, h=h-8 );
	translate( [0,0,h-8-0.01] ) cylinder( d1=d+3, d2=3, h=d/4 );
	cylinder( d=d, h=h-2 );
	translate( [0,0,h-2-0.01] ) cylinder( d1=d, d2=d-1, h=2 );
}

module meas_port_neg( d=3, h=20, depth=-0.5-0.01 )
{
	translate( [0,0,-depth-0.01] ) cylinder( d=d, h=h-10+depth+0.01 );
	translate( [0,0,h-10-0.01] ) cylinder( d1=d, d2=0, h=d/2+0.01 );
	translate( [0,0,h-0.5] ) cylinder( d1=0, d2=d-1, h=0.2*d+0.01 );
}

module screw_hole_bottom_neg( d1=3, d2=7, h=9 )
{			
	cylinder( d=d1, h=h );
	cylinder( d1=d2, d2=3, h=(d2-d1)/2 );
}

intersection() {
	translate( [-100,0,-100] ) cube( [1000,1000,1000] );
	union() {
	
		translate( [0,0,0] ) difference() {
			pressure_controlled_valve_bottom_pos( 50, 30 );
			pressure_controlled_valve_bottom_neg( 50, 30, 4 );
		}
		translate( [0,0,30] ) difference() {
			pressure_controlled_valve_top_pos( 50, 30 );
			pressure_controlled_valve_top_neg( 50, 30, rot=60 );
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
			one_way_valve_bottom_pos( 2.5, 3.5, type=1 );
			one_way_valve_bottom_neg( 4, 2.5, 3.5, type=1 );
		}
		translate( [80,0,30] ) difference() {
			one_way_valve_top_pos( 2.5, 3.5, type=1 );
			one_way_valve_top_neg( 2.5, 3.5, type=1 );
		}
		translate( [120,0,0] ) difference() {
			via_bottom_pos( 6, 3 );
			via_bottom_neg( 4, 6, 3 );
		}
		translate( [120,0,30] ) difference() {
			via_top_pos( 6, 3 );
			via_top_neg( 6, 3 );
		}
		translate( [160,0,0] ) difference() {
			corridor_pos( 20 );
			corridor_neg( 20+0.02 );
		}
		translate( [200,0,0] ) difference() {
			corridor_joint_pos();
			corridor_joint_neg();
		}
		translate( [240,0,0] ) difference() {
			hosetail_pos();
			hosetail_neg();
		}
		translate( [240,0,30] ) difference() {
			bottom_hosetail_neg( 4 );
		}
		translate( [280,0,0] ) {
			multiple_corridor_neg( 40, 20, 12 );
		}
		translate( [280,0,0] ) {
			multiple_bent_corridor_neg( -20, 180, 30, 20, 12 );
		}
		translate( [360,0,00] )  difference() {
			meas_port_pos( d=3, h=20 );
			meas_port_neg( d=3, h=20 );
		}
	}
}

color( "darkblue", 0.25 ) intersection() {
	translate( [-100,-0.02,-100] ) cube( [1000,1000,1000] );
	union() {

		translate( [0,00,0] ) union() {
			 pressure_controlled_valve_bottom_neg( 50, 30, 4 );
		}
		translate( [0,0,20] ) union() {
			 pressure_controlled_valve_membrane_neg( 50, 30 );
		}
		translate( [0,40,30] ) union() {
			pressure_controlled_valve_top_neg( 50, 30, rot=60 );
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
			one_way_valve_bottom_neg( 4, 2.5, 3.5, type=1 );
		}
		translate( [80,0,20] ) union() {
			one_way_valve_membrane_neg( 2.5, 3.5, type=1 );
		}
		translate( [80,0,30] ) union() {
			one_way_valve_top_neg( 2.5, 3.5, type=1 );
		}
		translate( [120,0,0] ) union() {
			via_bottom_neg( 4, 6, 3 );
		}
		translate( [120,0,20] ) union() {
			via_membrane_neg( 6, 3 );
		}
		translate( [120,0,30] ) union() {
			via_top_neg( 6, 3 );
		}
		translate( [160,0,0] ) union() {
			corridor_neg( 20 );
		}
		translate( [200,0,0] ) union() {
			corridor_joint_neg();
		}
		translate( [240,0,0] ) union() {
			hosetail_neg();
		}
		translate( [240,0,30] ) union() {
			bottom_hosetail_neg( 4 );
		}
		translate( [320,0,30] ) union() {
			screw_hole_bottom_neg( 4 );
		}
		translate( [360,0,30] ) union() {
			meas_port_neg( d=3, h=20 );
		}
	}
}

