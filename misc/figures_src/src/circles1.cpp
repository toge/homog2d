// this file is part of homog2d
// used to build a figure that is included in manual
// see makefile target doc_fig

#include "fig_src.header"

int main()
{
	Circle c1( Point2d(120,100), 80 );
	Circle c2( Point2d(300,200), 45 );
	auto seg  = getSegment( c1, c2 );
	auto pseg = getTanSegs( c1, c2 );

	img::Image<cv::Mat>  im1( 450,300 );
	img::Image<SvgImage> im2( 450,300 );

	c1.draw( im1,  DrawParams().setColor(0,0,100) );
	c1.draw( im2,  DrawParams().setColor(0,0,100) );
	c2.draw( im1,  DrawParams().setColor(0,0,100) );
	c2.draw( im2,  DrawParams().setColor(0,0,100) );
	seg.draw( im1, DrawParams().setColor(250,0,0) );
	seg.draw( im2, DrawParams().setColor(250,0,0) );
//	auto lp1 = seg.getOrthonalLine( c1.center() );
	pseg.first.draw( im1,  DrawParams().setColor(0,200,100) );
	pseg.first.draw( im2,  DrawParams().setColor(0,200,100) );

	pseg.second.draw( im1, DrawParams().setColor(0,100,200) );
	pseg.second.draw( im2, DrawParams().setColor(0,100,200) );
	im1.write( "circles1.png" );
	im2.write( "circles1.svg" );
}
