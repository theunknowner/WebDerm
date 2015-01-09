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
	const double circleThresh = 0.60;
	const double randomThresh = -0.40;
	Mat findShapes(Mat src);
	Mat dilation(Mat src, Size size,Point anchor=Point(-1,-1));
	Mat erosion(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat contrast1(Mat src);
	Mat contrast2(Mat src);
	Mat getStructElem(Size size,int shape);
	Mat elementaryDilation(Mat src, int flag=0);
	Mat prepareImage(Mat src);
	Mat grayscaleReconstruct(Mat src);
	vector<Mat> liquidExtraction(Mat src);

	Mat connectImage(Mat src, Size size, double dist);

	Mat filterNoise(Mat src, int radius);
	Mat detectHeat(Mat src, Size size);

	Mat kmeansClusterLC(Mat src);
	Mat kmeansClusterPt(Mat src);
};

#endif /* SHAPEMORPH_H_ */
