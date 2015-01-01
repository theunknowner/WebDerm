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
	Mat dilation(Mat src, Size size,Point anchor=Point(-1,-1));
	Mat erosion(Mat src, Size size, Point anchor=Point(-1,-1));
	void erosion2(Mat src, Mat &dst, Size size,Point anchor=Point(-1,-1));
	Mat uniqueLumPercentile(Mat src, double percentile);
	Mat extractShape(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat hysteresisDilation(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat hysteresisErosion(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat contrast(Mat src);
};

#endif /* SHAPEMORPH_H_ */
