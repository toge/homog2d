// this file is part of homog2d
// used to build a figure that is included in manual
// see makefile target doc_fig

#include "fig_src.header"

int main()
{
	FRect r1( 50,50, 200,200 );
	FRect r2( 200,50, 300,200 );
	img::Image<cv::Mat> im( 400,300 );

	r1.draw( im, DrawParams().setColor(250,0,20) );
	r2.draw( im, DrawParams().setColor(20,0,250) );
	im.write( "frect2.png" );
}
