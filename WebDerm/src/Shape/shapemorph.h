/*
 * shapemorph.h
 *
 *  Created on: Dec 29, 2014
 *      Author: jason
 */

#ifndef SHAPEMORPH_H_
#define SHAPEMORPH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class ShapeMorph {
private:

public:
	Mat findShapes(Mat src);
	void dilation(Mat src, Mat &dst, Size size,Point anchor=Point(-1,-1));
	void erosion(Mat src, Mat &dst, Size size, Point anchor=Point(-1,-1));
	void erosion2(Mat src, Mat &dst, Size size,Point anchor=Point(-1,-1));
	Mat uniqueLumPercentile(Mat src, double percentile);
};

#endif /* SHAPEMORPH_H_ */
