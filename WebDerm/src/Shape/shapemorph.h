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
#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/cluster.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/kneecurve.h"
#include "/home/jason/git/WebDerm/WebDerm/src/GridDisplay/griddisplay.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Poly/poly.h"

class ShapeMorph {
private:
	bool debugMode=false;

public:
	void setDebugMode(bool mode);
	bool isDebugModeOn();
	enum {RECT=0,CIRCLE=1};
	const double circleThresh = 0.60;
	const double randomThresh = -0.40;
	Mat dilation(Mat src, Size size,Point anchor=Point(-1,-1));
	Mat erosion(Mat src, Size size, Point anchor=Point(-1,-1));
	Mat hysteresisGradient(Mat src);
	Mat getStructElem(Size size,int shape);
	Mat elementaryDilation(Mat origImg, Mat scaleImg);
	Mat prepareImage(Mat src);
	Mat custAnd(Mat origImg, Mat scaleImg, Mat map=Mat());
	Mat grayscaleReconstruct(Mat src, Mat scaleImg);
	deque<Mat> liquidFeatureExtraction(Mat src, double thresh=0);
	vector<Mat> filterFeatures(vector<Mat> featureVec);

	Mat connectImage(Mat src, Size size, double dist);

	Mat filterNoise(Mat src, int radius);
	Mat detectHeat(Mat src, Size size);

	Mat kmeansClusterLC(Mat src);
	Mat kmeansClusterPt(Mat src);

	Mat customFn(Mat src);
	Mat customFn2(Mat src);

	Mat custGrayscaleRecon(Mat src);
	Mat gsReconUsingRmin1(Mat src);
	Mat gsReconUsingRmin2(Mat src);

	vector<Mat> lumFilter1(Mat src);
	vector<Mat> lumFilter2(Mat src);
	Mat densityDetection(Mat src,double q);
	Mat origFilter(Mat src);
	Mat closeFilter(Mat src);
	void combineFilterFeatures(vector<Mat> &featureVec);
	vector<vector<Point> > findBoundary(Mat src);
	int countEdgeTouching(Mat src, int edgeSize);
	int countEdgeTouching(Mat src, int sideEdgeSize, int cornerEdgeSize);

	vector<Mat> runShapeMorphTest(deque<String> &nameVec, deque<int> &labels);
};

#endif /* SHAPEMORPH_H_ */
