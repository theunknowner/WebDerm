/*
 * shapemorph.h
 *
 *  Created on: Dec 29, 2014
 *      Author: jason
 */

#ifndef SHAPEMORPH_H_
#define SHAPEMORPH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class ImageData;

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
	Mat getStructElem(Size size,int shape);
	Mat elementaryDilation(Mat origImg, Mat scaleImg);
	Mat prepareImage(Mat src);
	Mat custAnd(Mat origImg, Mat scaleImg, Mat map=Mat());
	Mat grayscaleReconstruct(Mat src, Mat scaleImg);
	vector<Mat> liquidFeatureExtraction(Mat src, double lcThresh=0, int sort=0, int numOfPtsThresh=10);
	vector<ImageData> liquidFeatureExtraction(ImageData &id, double lcThresh=0, int sort=0, int numOfPtsThresh=10);
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

	vector<Mat> lumFilter1(Mat src, int featuresToHold=1);
	vector<Mat> lumFilter2(Mat src, int featuresToHold=1);
	Mat densityConnector(Mat src,double q, double coeff=1.0);
	Mat origFilter(Mat src, double shift=1.0);
	Mat closeFilter(Mat src, Size elementSize, double shift=1.0);
	vector<vector<Point> > findBoundary(Mat src);
	int countEdgeTouching(Mat src, int sideEdgeSize, int cornerEdgeSize);
	Mat removeNoiseOnBoundary(Mat src);

	/* halo_transform.cpp */
	Mat haloTransform(Mat src, int iterations=1);
};

#endif /* SHAPEMORPH_H_ */
