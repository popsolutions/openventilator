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

module tube( d1=11, d2=13, h=60 )
{
	difference() {
		cylinder( d=d2, h=h );
		translate( [0,0,-0.01] ) cylinder( d=d1, h=h+0.02 );
	}
}

module hosetail( d1=16.5, d2=19.5, h=30, top_narrowing_th=1, top_narrowing_height=4, bottom_base_extension=4, bottom_base_height=2, bottom_inner_widening=0 )
{
	difference() {
		union() {
			cylinder( d=d2, h=h-top_narrowing_height );
			translate( [0,0,h-top_narrowing_height-0.01] ) cylinder( d2=d1+top_narrowing_th, d1=d2, h=top_narrowing_height+0.01 );
			cylinder( d2=d2, d1=d2+bottom_base_extension, h=bottom_base_height+0.01 );
		}
		translate( [0,0,-0.01] ) cylinder( d=d1, h=h+0.02 );
		translate( [0,0,-0.01] ) cylinder( d2=d1, d1=d1+bottom_inner_widening, h=bottom_inner_widening/2 );
	}
}

module sine_hole( d=20, depth=10)
// To be used as negative.
{
	max_i=12;
	p=concat( [[0,0]],[ for( i=[0:1:max_i+0.01] ) [d/2*i/max_i,depth*(-cos( i*90/max_i ))] ], [[d/2,0]] );

	rotate_extrude() polygon( p );
}

module sine_trench( w=20, l=40, depth=10)
// To be used as negative.
{
	max_i=24;
	p=concat( [[-w/2,0]],[ for( i=[0:1:max_i+0.01] ) [-w/2+w*i/max_i,depth*( +cos( 90+i*180/max_i ))] ], [[+w/2,0]] );

	rotate( [90,0,180] )
		linear_extrude( height=l ) polygon( p );
}

module corridor( w=10, l=40, h=10, f=0, b=0 )
// Makes a corridor with a 45 degrees roof.
// To be used as negative.
// f specifies flattening at the top, for if used as positive.
// b specifies base extension, for if used as positive.
{
	//rotate( [0,-90,-90] )
	rotate( [90,0,180] )
		linear_extrude( l+0.02 ) polygon( [[-w/2-b,0],[-w/2,b],[-w/2,h-w/2],[-f,h-f],[0,h-f],[+f,h-f],[w/2,h-w/2],[w/2,b],[w/2+b,0]] );
}

module corridor_joint( w=10, h=10, f=0, b=0 )
// Makes a corridor joint with a 45 degrees roof.
// To be used as negative.
// f specifies flattening at the top, for if used as positive.
// b specifies base extension, for if used as positive.
{
	rotate_extrude() polygon( [[0,0],[0,h-f],[f,h-f],[w/2,h-w/2],[w/2,b],[w/2+b,0]] );
}

module rounded_rectangle( x=20, y=40, h=10, r=4 )
{
	hull() {
		translate( [r, r, 0] ) cylinder( r=r, h=h );
		translate( [x-r, r, 0] ) cylinder( r=r, h=h );
		translate( [x-r, y-r, 0] ) cylinder( r=r, h=h );
		translate( [r, y-r, 0] ) cylinder( r=r, h=h );
	}
}

module cylinder_grid( x=40, y=40, h=10, d=4.5, x_incr=6 )
{
	y_incr=1/2*sqrt(3) * x_incr;
	for( y=[d/2:y_incr:y] ) {
		x_offset = (((y-d/2)/y_incr) % 2 ) * x_incr/2;
		for( x=[d/2+x_offset:x_incr:x] ) {
			translate( [x,y,0] ) cylinder( d=d, h=h );
		}
	}
}

function 2D_length( p0, p1 ) = sqrt( pow( p1[1] - p0[1], 2 ) + pow( p1[0] - p0[0], 2 ) );

function 2D_angle( p0, p1 ) =  -90+atan2( p1[1] - p0[1], p1[0] - p0[0] );

hosetail();

translate( [40,0,0] ) tube();

translate( [80,0,0] ) sine_hole();

translate( [120,0,0] ) sine_trench();

translate( [160,0,0] ) corridor();

translate( [200,0,0] ) corridor_joint();

translate( [240,0,0] ) rounded_rectangle();