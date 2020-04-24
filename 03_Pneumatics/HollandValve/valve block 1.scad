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
use <valve block components.scad>;

$fn = 90;

h = 12;
bh = 2;
text_h = 0.3;

size_x = 128;
size_y = 100;
top_part_p =  [  0,28,0];
top_part_size = [128,72,2];

pcv_p = [64,48,0];
owv_p = [	[ 20, 48,0],
			[108, 48,0],
			[ 52, 80,0],
			[108, 80,0] ];
owv_rot = [-90,-90,180,180];

via_p = [ [ 80,80,0] ];

bottom_corridor_joint_p = [];

top_corridor_joint_p = [ [0,0,0] ];

top_hosetail_p = [		[ 20,40,0],
						[ 68,80,0],
						[108,80,0] ];
top_hosetail_h   = [	42,
						42,
						42 ];

bottom_hosetail_p = [	[ 20,80,0],
						[ 68,16,0],
						[108,16,0] ];
bottom_hosetail_h = [	45,
						45,
						45 ];

bottom_corridor_paths = [	[ [ 20,32-0.01,0], [ 20,40,0] ],
							[ [ 36-0.01,16,0], [ 68,16,0] ],
							[ [108,48,0], [108,16,0] ],
							[ [ 20,80,0], [ 52,80,0] ],
							[ [ 80,80,0], [108,80,0] ] ];

bottom_bent_corridor_centers =    [	[36,32,0] ];
bottom_bent_corridor_start_angles =	[ 180 ];
bottom_bent_corridor_bend_angles =	[ 90 ];

bottom_thin_corridor_paths = [	[ [80,80-3.25,h-8], pcv_p ] ];

top_corridor_paths = [	[ [ 20,48,0], [ 63,48,0] ],
						[ [ 20,40,0], [ 20,48,0] ],
						[ [ 65,48,0], [108,48,0] ],
						[ [ 52,80,0], [ 80,80,0] ] ];

// Screws are placed asymmetrycally, so the part cannot be assembled the wrong way round
screw_p = [ 	[   4, 32, 0],
				[  42, 32, 0],
				[  86, 32, 0],
				[ 124, 32, 0],
				[   4, 64, 0],
				[  36, 64, 0],
				[  86, 64, 0],
				[ 124, 64, 0],
				[  36, 96, 0],
				[  76, 96, 0],
				[ 124, 96, 0] ];
mount_p = [		[   4,  4, 0],
				[  88,  4, 0],
				[   4,  96, 0],
				[  88, 96, 0] ];

top_text_p = [	[6,40,0],
				[6,80],
				[91,80],
				[122,80] ];
top_text_rot = [-90,-90,90,90];
top_text_text = [ "EXP", "INSP", "PUMP", "SUPPLY" ];

bottom_text_p =	[[25,19],
				[25,14],
				[82,16],
				[122,16] ];
bottom_text_rot = [0,0,90,90];
bottom_text_text = [ "OpenVentilator.io", "3Dp valves v1.1", "OVP", "PEEP" ];


module bottom_part()
{
	difference() {
		union() { // POSITIVE PART
			rounded_rectangle( size_x, size_y, bh+h );
			
			translate( [0,0,h+bh] ) {
				translate( pcv_p ) pressure_controlled_valve_bottom_pos();
				for( p=owv_p ) {
					translate( p ) one_way_valve_bottom_pos(); 
				}
				for( p=via_p ) {
					translate( p ) via_bottom_pos( r=6.5, e=6.5 );
				}
				for( p=bottom_corridor_joint_p ) {
					translate( p ) corridor_joint_pos();
				}
				for( i=[0:1:len(bottom_hosetail_p)-0.99] ) {
					translate( bottom_hosetail_p[i] ) hosetail_pos( h=bottom_hosetail_h[i] );
				}
				for( i=[0:1:len(bottom_text_p)-0.99] ) {
					translate( bottom_text_p[i] ) rotate( [0,0,bottom_text_rot[i]] ) color( "black" ) linear_extrude( height=text_h ) text( size=3, halign="center", valign="center", bottom_text_text[i] );
				}				
			}
		}
		union() { // NEGATIVE PART
			translate( [0,0,h+bh] ) {
				translate( pcv_p ) pressure_controlled_valve_bottom_neg( d=h );
				for( i=[0:1:len(owv_p)-0.99] ) {
					translate( owv_p[i] ) rotate( [0,0,owv_rot[i]] ) one_way_valve_bottom_neg( d=h ); 
				}
				for( p=via_p ) {
					translate( p ) via_bottom_neg( d=h, r=6.5, e=6.5 );
				}
				for( p=bottom_hosetail_p ) {
					translate( p ) hosetail_neg();
				}
				for( p=bottom_corridor_joint_p ) {
					translate( p ) corridor_joint_neg( d=h );
				}
				for( i=[0:1:len(bottom_hosetail_p)-0.99] ) {
					translate( bottom_hosetail_p[i] ) hosetail_neg( h=bottom_hosetail_h[i] );
					translate( bottom_hosetail_p[i] ) bottom_hosetail_neg( d=h );
				}
				for( path=bottom_corridor_paths ) {
					l = 2D_length( path[0], path[1] );
					a = 2D_angle( path[0], path[1] );
					translate( path[0] ) rotate( [0,0,a ] ) multiple_corridor_neg( l, 20, h );
				}
				// bent corridor
				for( i=[0:1:len(bottom_bent_corridor_centers)-0.99] ) {
					translate( bottom_bent_corridor_centers[i] ) multiple_bent_corridor_neg( 16, bottom_bent_corridor_start_angles[i], bottom_bent_corridor_bend_angles[i], 20, h );
				}
				// thin corridor to pressure controlled valve
				for( path=bottom_thin_corridor_paths ) {
					l = 2D_length( path[0], path[1] );
					a = 2D_angle( path[0], path[1] );
					translate( [0,0,-h] ) translate( path[0] ) rotate( [0,0,a ] ) corridor( w=2, l=l, h=3 );
				}
				// MANUAL PARTS FROM HERE
			}
			// screw holes
			translate( [0,0,-0.01] ) {
				for( p=screw_p ) {
					translate( p ) cylinder( d=3, h=20 );
					translate( p ) cylinder( d1=7, d2=3, h=2 );
				}
			}
			// mount holes
			translate( [0,0,-0.01] ) {
				for( p=mount_p ) {
					translate( p ) cylinder( d=4, h=20 );
				}
			}
		}
	}
}

module membrane()
{
	difference() {
		union() {
			translate( top_part_p ) cube( [top_part_size[0], top_part_size[1], 0.5 ] );
		}
		union() { // NEGATIVE PART
			translate( [0,0,0] ) {
				translate( pcv_p ) pressure_controlled_valve_membrane_neg();
				for( i=[0:1:len(owv_p)-0.99] ) {
					translate( owv_p[i] ) rotate( [0,0,owv_rot[i]] ) one_way_valve_membrane_neg(); 
				}
				for( p=via_p ) {
					translate( p ) via_membrane_neg( r=6.5, e=6.5 );
				}
			}
			// screw holes
			translate( [0,0,-0.01] ) {
				for( p=screw_p ) {
					translate( p ) cylinder( d=3.5, h=20 );
				}
			}
			// mount holes
			translate( [0,0,-0.01] ) {
				for( p=mount_p ) {
					translate( p ) cylinder( d=4.5, h=20 );
				}
			}
			// MANUAL PARTS FROM HERE
			// cutout for INSP hosetail
			translate(  bottom_hosetail_p[0] ) translate( [0,0,-4] ) cylinder( d=24, h=20 );
			translate( [bottom_hosetail_p[0][0] - 12, bottom_hosetail_p[0][1], -4 ] ) cube( [24,40,20] );
			translate( [bottom_hosetail_p[0][0] - 24, bottom_hosetail_p[0][1] - 12, -4 ] ) cube( [24,40,20] );
			// cutout for mount hole 3
			translate( mount_p[3] ) translate( [0,0,-4] ) cylinder( d=11, h=20 );
			translate( [mount_p[3][0] - 5.5, mount_p[3][1], -4 ] ) cube( [11,11,20] );
		}
	}
}

module top_part()
{
	difference() {
		
		union() { // POSITIVE PART

			// port 1 base and clamp down plate
			translate( top_part_p ) rounded_rectangle( top_part_size[0], top_part_size[1],  top_part_size[2] );
			translate( [0,0,2] ) {
				
				translate( pcv_p ) pressure_controlled_valve_top_pos();
				
				for( p=owv_p ) {
					translate( p ) one_way_valve_top_pos(); 
				}
				for( p=via_p ) {
					translate( p ) via_top_pos( r=6.5, e=6.5 );
				}
				for( p=top_corridor_joint_p ) {
					translate( p ) corridor_joint_neg();
				}
				for( i=[0:1:len(top_hosetail_p)-0.99] ) {
					translate( top_hosetail_p[i] ) hosetail_pos( h=top_hosetail_h[i] );
				}
				for( path=top_corridor_paths ) {
					l = 2D_length( path[0], path[1] );
					a = 2D_angle( path[0], path[1] );
					translate( path[0] ) rotate( [0,0,a ] ) corridor_pos( l );
				}
				for( i=[0:1:len(top_text_p)-0.99] ) {
					translate( top_text_p[i] ) rotate( [0,0,top_text_rot[i]] ) color( "black" ) linear_extrude( height=text_h ) text( size=3, halign="center", valign="center", top_text_text[i] );
				}
				// MANUAL PARTS FROM HERE
			}
		}
		union() { // NEGATIVE PART
			
			translate( [0,0,2] ) {
				translate( pcv_p ) pressure_controlled_valve_top_neg();
				
				for( p=owv_p ) {
					translate( p ) one_way_valve_top_neg(); 
				}
				for( p=via_p ) {
					translate( p ) via_top_neg( r=6.5, e=6.5 );
				}
				for( p=top_corridor_joint_p ) {
					translate( p ) corridor_joint_neg();
				}
				for( i=[0:1:len(top_hosetail_p)-0.99] ) {
					translate( top_hosetail_p[i] ) hosetail_neg( h=top_hosetail_h[i] );
				}
				for( path=top_corridor_paths ) {
					l = 2D_length( path[0], path[1] );
					a = 2D_angle( path[0], path[1] );
					translate( path[0] ) rotate( [0,0,a ] ) corridor_neg( l );
				}
				// MANUAL PARTS FROM HERE
				// cutout for INSP hosetail
				translate(  bottom_hosetail_p[0] ) translate( [0,0,-4] ) cylinder( d=19.5+4, h=5 );
				translate( mount_p[2] ) translate( [0,0,-4] ) cylinder( d=10, h=20 );
				translate( [mount_p[2][0] - 5, mount_p[2][1], -4 ] ) cube( [10,10,20] );
				translate( [mount_p[2][0] - 10, mount_p[2][1] - 5, -4 ] ) cube( [10,10,20] );
				// cutout for mount hole 3
				translate( mount_p[3] ) translate( [0,0,-4] ) cylinder( d=10, h=20 );
				translate( [mount_p[3][0] - 5, mount_p[3][1], -4 ] ) cube( [10,10,20] );
			}
			// screw holes
			translate( [0,0,-0.01] ) {
				for( p=screw_p ) {
					translate( p ) cylinder( d=3, h=20 );
				}
			}
			// mount holes
			translate( [0,0,-0.01] ) {
				for( p=mount_p ) {
					translate( p ) cylinder( d=4, h=20 );
				}
			}
		}
	}
}

intersection() {
	union() {
		translate( [0,0,0] ) bottom_part();
		//translate( [0,0,0+bh+h+1] ) top_part();
		//translate( [0,0,0+bh+h+0.5] ) color( "grey", 1 ) membrane();
	}
	translate( [0,0,0] ) cube( [1000,1000,1000] );
}

//translate( [0,0,0] ) bottom_part();

//translate( [0,0,00] ) top_part();
