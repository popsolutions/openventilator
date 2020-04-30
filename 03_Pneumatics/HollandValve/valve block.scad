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

//$fn = 20;
$fa = 5;
$fs = 0.25;

h = 12;
bh = 2;
text_h = 0.3;

size_x = 140;
size_y = 100;
top_part_p =  [  0,28,0];
top_part_size = [140,72,2];

pcv_p = [86,48,0];
pcv_size = [34,30];
pcv_rot = 45;
owv_p = [ [ 20, 48,0],
			[120, 48,0],
			[ 52, 80,0],
			[ 79, 80,0],
			[120, 80,0] ];
owv_rot = [-90,-90,180,-90,180];
owv_r = [8.5,8.5,8.5,2.25,8.5];
owv_e = [0,0,0,3.5,0];
owv_type = [0,0,0,1,0];

via_p = [ [ 96,80,0] ];

bottom_corridor_joint_p = [];

top_corridor_joint_p = [ [20,48,0] ];

top_hosetail_p = [		[ 20,40,0],
						[ 80,80,0],
						[120,80,0] ];
top_hosetail_h   = [	42,
						42,
						42 ];

bottom_hosetail_p = [	[ 20,80,0],
						[ 80,16,0],
						[120,16,0] ];
bottom_hosetail_h = [	45,
						45,
						45 ];

bottom_corridor_paths = [	[ [ 20,32-0.01,0], [ 20,40,0] ],
							[ [ 36-0.01,16,0], [ 85,16,0] ],
							[ [ 100,24,-5], [116,24,-5] ],
							[ [120,16,0], [120,48,0] ],
							[ [ 54,22,-2], [ 82,74,0] ],
//							[ [ 79,22,-5], [ 79,74,0] ],
							[ [ 20,80,0], [ 52,80,0] ],
							[ [ 96,80,0], [120,80,0] ],
							[ [98,80,0], [88,48,0] ] ];
bottom_corridor_widths =	[20, 20, 3, 20, 7, 20, 20, 3];
bottom_corridor_heights =	[h-1,h-1,h-5-1,h-1,h-2-1,h-1,h-1,5];

bottom_bent_corridor_centers =    [	[36,32,0] ];
bottom_bent_corridor_start_angles =	[ 180 ];
bottom_bent_corridor_bend_angles =	[ 90 ];
bottom_bent_corridor_widths =		[20];
bottom_bent_corridor_radii =		[16];
bottom_bent_corridor_heights =		[h-1];

top_corridor_paths = [	[ [ 20,40,0], [ 20,48,0] ],
						[ [ 20,48,0], [ 73,48,0] ],
						[ [ 99,48,0], [120,48,0] ],
						[ [ 52,80,0], [ 96,80,0] ] ];

// Screws are placed asymmetrycally, so the part cannot be assembled the wrong way round
screw_p = [ 	[   4, 32, 0],
				[  36, 32, 0],
				[  68, 32, 0],
				[ 104, 32, 0],
				[ 136, 32, 0],
				[   4, 64, 0],
				[  36, 64, 0],
				[  68, 64, 0],
				[ 104, 64, 0],
				[ 136, 64, 0],
				[  36, 96, 0],
				[  68, 96, 0],
				[  92, 96, 0],
				[ 136, 96, 0] ];
mount_p = [		[   4,  4, 0],
				[ 104,  4, 0],
				[   4, 96, 0],
				[ 104, 96, 0] ];

top_meas_port_p = [	[ 56,48,0],			// EXP pressure
					[ 64,48,0],			// EXP pressure
					[ 56,80,0] ];		// PUMP pressure

bottom_meas_port_p = [	[ 60,24,0],		// OVP pressure
						[100,24,0] ];	// PEEP pressure

top_text_p = [	[6,40,0],
				[6,80],
				[106,80],
				[134,80] ];
top_text_rot = [-90,-90,90,90];
top_text_text = [ "EXP", "INSP", "PUMP", "SUPPLY" ];

bottom_text_p =	[ [ 25,19],
				  [ 25,14],
				  [ 94,16],
				  [134,16] ];
bottom_text_rot = [0,0,90,90];
bottom_text_text = [ "OpenVentilator.io", "3Dp valves v1.3", "OVP", "PEEP" ];


module bottom_part()
{
	difference() {
		union() { // POSITIVE PART
			rounded_rectangle( size_x, size_y, bh+h );
			
			translate( [0,0,h+bh] ) {
				translate( pcv_p ) pressure_controlled_valve_bottom_pos( pcv_size[0], pcv_size[1] );
				for( i=[0:1:len(owv_p)-0.99] ) {
					p = owv_p[i];
					rot = owv_rot[i];
					r = owv_r[i];
					e = owv_e[i];
					type = owv_type[i];
					translate( p ) rotate( [0,0,rot] ) one_way_valve_bottom_pos( r=r, e=e, type=type ); 
				}
				for( p=via_p ) {
					translate( p ) via_bottom_pos( r=5, e=3.5 );
				}
				for( i=[0:1:len(bottom_hosetail_p)-0.99] ) {
					translate( bottom_hosetail_p[i] ) hosetail_pos( h=bottom_hosetail_h[i] );
				}
				for( p=bottom_meas_port_p ) {
					translate( p ) meas_port_pos( h=12 );
				}
				for( i=[0:1:len(bottom_text_p)-0.99] ) {
					translate( bottom_text_p[i] ) rotate( [0,0,bottom_text_rot[i]] ) color( "black" ) linear_extrude( height=text_h ) text( size=3, halign="center", valign="center", bottom_text_text[i] );
				}				
			}
		}
		union() { // NEGATIVE PART
			translate( [0,0,h+bh] ) {
				translate( pcv_p ) pressure_controlled_valve_bottom_neg( pcv_size[0], pcv_size[1], d=6 );
				for( i=[0:1:len(owv_p)-0.99] ) {
					p = owv_p[i];
					rot = owv_rot[i];
					r = owv_r[i];
					e = owv_e[i];
					type = owv_type[i];
					translate( p ) rotate( [0,0,rot] ) one_way_valve_bottom_neg( d=h, r=r, e=e, type=type ); 
				}
				for( p=via_p ) {
					translate( p ) via_bottom_neg( d=h, r=5, e=3.5 );
				}
				for( p=bottom_hosetail_p ) {
					translate( p ) hosetail_neg();
				}
				for( p=bottom_meas_port_p ) {
					translate( p ) meas_port_neg( h=12, depth=h );
				}
				for( p=bottom_corridor_joint_p ) {
					translate( p ) corridor_joint_neg( d=h );
				}
				for( i=[0:1:len(bottom_hosetail_p)-0.99] ) {
					translate( bottom_hosetail_p[i] ) hosetail_neg( h=bottom_hosetail_h[i] );
					translate( bottom_hosetail_p[i] ) bottom_hosetail_neg( d=h );
				}
				for( i=[0:1:len(bottom_corridor_paths)-0.99] ) {
					path = bottom_corridor_paths[i];
					w = bottom_corridor_widths[i];
					h2 = bottom_corridor_heights[i];
					l = 2D_length( path[0], path[1] );
					a = 2D_angle( path[0], path[1] );
					translate( path[0] ) rotate( [0,0,a ] ) multiple_corridor_neg( l, w, h2 );
				}
				// bent corridor
				for( i=[0:1:len(bottom_bent_corridor_centers)-0.99] ) {
					center = bottom_bent_corridor_centers[i];
					start_angle = bottom_bent_corridor_start_angles[i];
					bend_angle = bottom_bent_corridor_bend_angles[i];
					w = bottom_bent_corridor_widths[i];
					r = bottom_bent_corridor_radii[i];
					h2 = bottom_bent_corridor_heights[i];
					translate( center ) multiple_bent_corridor_neg( r, start_angle, bend_angle, w, h2 );
				}
				// MANUAL PARTS FROM HERE
			}
			// screw holes
			translate( [0,0,-0.01] ) {
				for( p=screw_p ) {
					translate( p ) cylinder( d=3.2, h=20 );
					translate( p ) cylinder( d1=7.2, d2=3.2, h=2 );
				}
			}
			// mount holes
			translate( [0,0,-0.01] ) {
				for( p=mount_p ) {
					translate( p ) cylinder( d=4.2, h=20 );
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
				translate( pcv_p ) pressure_controlled_valve_membrane_neg( pcv_size[0], pcv_size[1] );
				for( i=[0:1:len(owv_p)-0.99] ) {
					p = owv_p[i];
					rot = owv_rot[i];
					r = owv_r[i];
					e = owv_e[i];
					type = owv_type[i];
					translate( p ) rotate( [0,0,rot] ) one_way_valve_membrane_neg( r=r, e=e, type=type ); 
				}
				for( p=via_p ) {
					translate( p ) via_membrane_neg( r=5, e=3.5 );
				}
			}
			// screw holes
			translate( [0,0,-0.01] ) {
				for( p=screw_p ) {
					translate( p ) cylinder( d=3.6, h=20 );
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
				
				translate( pcv_p ) pressure_controlled_valve_top_pos( pcv_size[0], pcv_size[1] );
				
				for( i=[0:1:len(owv_p)-0.99] ) {
					p = owv_p[i];
					rot = owv_rot[i];
					r = owv_r[i];
					e = owv_e[i];
					type = owv_type[i];
					translate( p ) rotate( [0,0,rot] ) one_way_valve_top_pos( r=r, e=e, type=type ); 
				}
				for( p=via_p ) {
					translate( p ) via_top_pos( r=5, e=3.5 );
				}
				for( i=[0:1:len(top_hosetail_p)-0.99] ) {
					translate( top_hosetail_p[i] ) hosetail_pos( h=top_hosetail_h[i] );
				}
				for( p=top_meas_port_p ) {
					translate( p ) meas_port_pos( h=25 );
				}
				for( p=top_corridor_joint_p ) {
					translate( p ) corridor_joint_pos();
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
				translate( pcv_p ) pressure_controlled_valve_top_neg( pcv_size[0], pcv_size[1], pcv_rot );
				
				for( i=[0:1:len(owv_p)-0.99] ) {
					p = owv_p[i];
					rot = owv_rot[i];
					r = owv_r[i];
					e = owv_e[i];
					type = owv_type[i];
					translate( p ) rotate( [0,0,rot] ) one_way_valve_top_neg( r=r, e=e, type=type ); 
				}
				for( p=via_p ) {
					translate( p ) via_top_neg( r=5, e=3.5 );
				}
				for( i=[0:1:len(top_hosetail_p)-0.99] ) {
					translate( top_hosetail_p[i] ) hosetail_neg( h=top_hosetail_h[i] );
				}
				for( p=top_meas_port_p ) {
					translate( p ) meas_port_neg( h=25 );
				}
				for( p=top_corridor_joint_p ) {
					translate( p ) corridor_joint_neg();
				}
				for( path=top_corridor_paths ) {
					l = 2D_length( path[0], path[1] );
					a = 2D_angle( path[0], path[1] );
					translate( path[0] ) rotate( [0,0,a ] ) corridor_neg( l );
				}
				// MANUAL PARTS FROM HERE
				// cutout for INSP hosetail
				translate(  bottom_hosetail_p[0] ) translate( [0,0,-4] ) cylinder( d=20+1, h=5 );
				translate(  bottom_hosetail_p[0] ) translate( [0,0,-2-0.01] ) cylinder( d1=20+3+1, d2=20+1, h=1.5 );
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
					translate( p ) cylinder( d=3.2, h=20 );
				}
			}
			// mount holes
			translate( [0,0,-0.01] ) {
				for( p=mount_p ) {
					translate( p ) cylinder( d=4.2, h=20 );
				}
			}
		}
	}
}

/*
intersection() {
	union() {
		translate( [0,0,0] ) bottom_part();
		translate( [0,0,0+bh+h+1] ) top_part();
		translate( [0,0,0+bh+h+0.5] ) color( "grey" ) membrane();
	}
	//translate( [0,0,-1000+4] ) cube( [1000,1000,1000] );
}
*/

translate( [0,0,0] ) bottom_part();

//translate( [0,0,00] ) top_part();
