/*
 * draw.h
 *
 *  Created on: Apr 2, 2015
 *      Author: jason
 */

#ifndef DRAW_H_
#define DRAW_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Draw {
public:
	Scalar randomColor( RNG& rng );
	void drawRandomLines(Mat &image, int numOfLines=1, int thickness=1);
	void drawRandomRectangles( Mat &image, int numOfRect=1, int thickness=1);
	void drawRandomEllipses( Mat &image, int numOfEllipse=1, int thickness=1 );
	void drawRandomPolylines( Mat &image, int numOfLines=1, int thickness=1 );
	void drawRandomFilledPolygons( Mat &image, int numOfPolys=1);
	void drawRandomCircles( Mat &image, int numOfCircles=1, int thickness=1);
};

#endif /* DRAW_H_ */
