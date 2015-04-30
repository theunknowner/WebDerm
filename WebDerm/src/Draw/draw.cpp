/*
 * draw.cpp
 *
 *  Created on: Apr 2, 2015
 *      Author: jason
 */

#include "draw.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

Scalar Draw::randomColor(RNG &rng) {
	int icolor = (unsigned) rng;
	return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

void Draw::drawRandomLines(Mat &image, int numOfLines, int thickness) {
	RNG rng(getTickCount());
	Point pt1, pt2;
	Size size(image.cols,image.rows);

	int type = image.type();
	for( int i = 0; i < numOfLines; i++ ) {
		while(pt1==pt2) {
			pt1.x = rng.uniform( 0, size.width );
			pt1.y = rng.uniform( 0, size.height );
			pt2.x = rng.uniform( 0, size.width );
			pt2.y = rng.uniform( 0, size.height );
		}

		if(type==CV_8UC3)
			line( image, pt1, pt2, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)),thickness);
		if(type==CV_8U)
			line( image, pt1, pt2, Scalar(255),thickness);
	}
}

void Draw::drawRandomRectangles( Mat &image, int numOfRect, int thickness) {
	RNG rng(getTickCount());
	Point pt1(-1,-1), pt2(-1,-1);
	Size size(image.cols,image.rows);

	int type = image.type();
	for( int i = 0; i < numOfRect; i++ )
	{
		while(pt1==pt2) {
			pt1.x = rng.uniform( 0, size.width );
			pt1.y = rng.uniform( 0, size.height );
			pt2.x = rng.uniform( 0, size.width );
			pt2.y = rng.uniform( 0, size.height );
		}

		if(type==CV_8UC3)
			rectangle( image, pt1, pt2, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), thickness);
		if(type==CV_8U)
			rectangle( image, pt1, pt2, Scalar(255), thickness);
	}
}

void Draw::drawRandomEllipses( Mat &image, int numOfEllipse, int thickness ) {
	RNG rng(getTickCount());
	Size size(image.cols,image.rows);

	int type=image.type();
	for ( int i = 0; i < numOfEllipse; i++ ){
		Point center;
		center.x = rng.uniform(0, size.width);
		center.y = rng.uniform(0, size.height);

		Size axes;
		axes.width = rng.uniform(0, size.width/3);
		axes.height = rng.uniform(0, size.height/3);

		double angle = rng.uniform(0, 360);

		if(type==CV_8UC3)
			ellipse( image, center, axes, angle, 0, 360, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), thickness);
		if(type==CV_8U)
			ellipse( image, center, axes, angle, 0, 360, Scalar(255), thickness);
	}
}

void Draw::drawRandomPolylines(Mat &image, int numOfLines, int thickness) {
	RNG rng(getTickCount());
	Size size(image.cols,image.rows);

	int type = image.type();
	for( int i = 0; i< numOfLines; i++ ) {
		Point pt[2][3];
		pt[0][0].x = rng.uniform( 0, size.width );
		pt[0][0].y = rng.uniform(0, size.height);
		pt[0][1].x = rng.uniform( 0, size.width );
		pt[0][1].y = rng.uniform(0, size.height);
		pt[0][2].x = rng.uniform( 0, size.width );
		pt[0][2].y = rng.uniform(0, size.height);
		pt[1][0].x = rng.uniform( 0, size.width );
		pt[1][0].y = rng.uniform(0, size.height);
		pt[1][1].x = rng.uniform( 0, size.width );
		pt[1][1].y = rng.uniform(0, size.height);
		pt[1][2].x = rng.uniform( 0, size.width );
		pt[1][2].y = rng.uniform(0, size.height);

		const Point* ppt[2] = {pt[0], pt[1]};
		int npt[] = {3, 3};

		if(type==CV_8UC3)
			polylines(image, ppt, npt, 2, true, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), thickness);
		if(type==CV_8U)
			polylines(image, ppt, npt, 2, true, Scalar(255), thickness);
	}
}

void Draw::drawRandomFilledPolygons( Mat &image, int numOfPolys) {
	RNG rng(getTickCount());
	Size size(image.cols,image.rows);

	int type = image.type();
	for ( int i = 0; i < numOfPolys; i++ ) {
		Point pt[2][3];
		pt[0][0].x = rng.uniform( 0, size.width );
		pt[0][0].y = rng.uniform(0, size.height);
		pt[0][1].x = rng.uniform( 0, size.width );
		pt[0][1].y = rng.uniform(0, size.height);
		pt[0][2].x = rng.uniform( 0, size.width );
		pt[0][2].y = rng.uniform(0, size.height);
		pt[1][0].x = rng.uniform( 0, size.width );
		pt[1][0].y = rng.uniform(0, size.height);
		pt[1][1].x = rng.uniform( 0, size.width );
		pt[1][1].y = rng.uniform(0, size.height);
		pt[1][2].x = rng.uniform( 0, size.width );
		pt[1][2].y = rng.uniform(0, size.height);

		const Point* ppt[2] = {pt[0], pt[1]};
		int npt[] = {3, 3};

		if(type==CV_8UC3)
			fillPoly( image, ppt, npt, 2, Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)) );
		if(type==CV_8U)
			fillPoly( image, ppt, npt, 2, Scalar(255) );
	}
}

void Draw::drawRandomCircles( Mat &image, int numOfCircles, int thickness) {
	RNG rng(getTickCount());
	Size size(image.cols,image.rows);

	int type = image.type();
	int radius = min(size.width, size.height)/2;
	for (int i = 0; i < numOfCircles; i++) {
		Point center;
		center.x = rng.uniform(0, size.width);
		center.y = rng.uniform(0, size.height);

		if(type==CV_8UC3)
			circle( image, center, rng.uniform(0, radius), Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)), thickness);
		if(type==CV_8U)
			circle( image, center, rng.uniform(0, radius), Scalar(255), thickness);
	}
}
