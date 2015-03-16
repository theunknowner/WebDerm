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
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/xyz.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/cielab.h"

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
	Mat getShapeUsingColor2(Mat hMat, Mat sMat, Mat lMat, Mat noise);
	void setHslVals(deque<double> hueVals, deque<double> satVals, deque<double> lumVals);

	/** NOT CORRECT **/
	double epoh(double sat, double lum);
	Mat epohTheHue(Mat hMat, Mat sMat, Mat lMat);

	void maxLocalHslRanges(Mat hMat, Mat sMat, Mat lMat, double &hr, double &sr, double &lr);
};

#endif /* SHAPECOLOR_H_ */
