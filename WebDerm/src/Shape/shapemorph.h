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
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"


class ShapeMorph {
private:

public:
	enum {RECT=0,CIRCLE=1};
	Mat findShapes(Mat src);
	Mat dilation(Mat src, Size size,Point anchor=Point(-1,-1));
	Mat erosion(Mat src, Size size, Point anchor=Point(-1,-1));
	void erosion2(Mat src, Mat &dst, Size size,Point anchor=Point(-1,-1));
	Mat uniqueLumPercentile(Mat src, double percentile);
	Mat extractShape(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat hysteresisDilation(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat hysteresisErosion(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat contrast1(Mat src);
	Mat contrast2(Mat src);
	Mat kmeansCluster(Mat src);
	Mat getStructElem(Size size,int shape);
	Mat elementaryDilation(Mat src, int flag=0);
	Mat prepareImage(Mat src);
	Mat connectImage(Mat src, int radius);
	Mat grayscaleReconstruct(Mat src);
	Mat liquidDilation(Mat src, Point seedPt=Point(-1,-1));
	Mat connectPixels(Mat src, Size size, double dist);
};

#endif /* SHAPEMORPH_H_ */
