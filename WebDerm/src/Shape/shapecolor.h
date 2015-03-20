/*
 * shapecolor.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef SHAPECOLOR_H_
#define SHAPECOLOR_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/CIE/cie.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/xyz.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/cielab.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/kneecurve.h"

class ShapeColor {
private:
	bool debugMode=false;
	int test_row, test_col, test_localScanSize;
	bool enterFlag;
public:
	void setDebugMode(bool mode);
	deque<double> hueVals, satVals, lumVals;
	Mat getShapeUsingColor(Mat src);
	Mat test_getShapeUsingColor(Mat src, int col=0, int row=0, int localScanSize=20, bool enterFlag=false);
	Mat getShapeUsingColor2(Mat src, Mat noise);
	void setHslVals(deque<double> hueVals, deque<double> satVals, deque<double> lumVals);

	/** NOT CORRECT **/
	double epoh(double sat, double lum);
	Mat epohTheHue(Mat hMat, Mat sMat, Mat lMat);

	void maxLocalRanges(Mat mat1, Mat mat2, Mat mat3, Mat hc, Mat noiseMap, double &maxRange);

	void img2HSL(Mat src, Mat &hMat, Mat &sMat, Mat &lMat);
	void img2XYZ(Mat src, Mat &xMat, Mat &yMat, Mat &zMat);
};

#endif /* SHAPECOLOR_H_ */
