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



translate( [0,0,0] ) cube( [10, 2, 5] );

translate( [0,52,0] ) cube( [10, 2, 5] );

translate( [0,0,5] ) line_grid( 0.35, 0.7, 5, 2+50+2, 0.25 );
translate( [5,1,5] ) line_grid( 0.35, 0.7, 5, 1+50+1, 0.25 );