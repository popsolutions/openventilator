module rounded_60deg_segment( r1 = 20, r2 = 5, r3 = 5, cd = 5+2, res = 2 )
{
	// r1 = outside radius
	// r2 = inside radius
	// r3 = radius of corner curve
	// cd = center distance, distance of closest point to circle center
	// res = resolution of ourside circle

	sd = (cd-r2)/2;
	
	// calculate corner center
	// ref: https://mathworld.wolfram.com/Circle-LineIntersection.html
	x1 = 0 + sin(-60) * (sd + r3);
	y1 = 0 + cos(-60) * (sd + r3);
	x2 = sin(30) * r1 + sin(-60) * (sd + r3);
	y2 = cos(30) * r1 + cos(-60) * (sd + r3);
	r = r1 - r3;
	dx = x2 - x1;
	dy = y2 - y1;
	dr = sqrt( dx*dx + dy*dy );
	D = x1*y2 - x2*y1;

	x = (+D * dy + dx * sqrt( r*r * dr*dr - D*D )) / (dr*dr);
	y = (-D * dx + dy * sqrt( r*r * dr*dr - D*D )) / (dr*dr);

	// calculate start angle of outside circle segment
	sa = atan2( x, y );
	
	// calculate corner resolution
	cres = res * r1/r3;

	// calculate inside resolution
	ires = res * r1/r2;

	// inside curve
	pg1 = [ for( a=[120:ires:240] ) [r2*sin(a),cd+r2+r2*cos(a)] ];

	// corner 1
	pg2 = [ for( a=[240:cres:330] ) [-x+r3*sin(a),y+r3*cos(a)] ];

	// outside
	pg3 = [ for( a=[-sa:res:+sa] ) [r1*sin(a),r1*cos(a)] ];

	// corner 2
	pg4 = [ for( a=[ 30:cres:120] ) [+x+r3*sin(a),y+r3*cos(a)] ];

	pg = concat( pg1, pg2, pg3, pg4 );

	polygon( pg );
}


for( a=[0:60:359] ) {
	rotate( [0,0,a] ) 
		linear_extrude( height = 1 ) rounded_60deg_segment();
}

//rounded_60deg_segment();