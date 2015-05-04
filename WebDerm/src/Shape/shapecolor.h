/*
 * shapecolor.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef SHAPECOLOR_H_
#define SHAPECOLOR_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class ShapeColor {
private:
	int debugLevel=0;
	int dbgCol, dbgRow;
	int test_row, test_col, test_localScanSize;
	bool enterFlag;
public:
	void setDebugLevel(int level=0);
	void setDebugColRow(int col, int row);
	Mat getShapeUsingColor(Mat src);
	Mat test_getShapeUsingColor(Mat src, int col=0, int row=0, int localScanSize=20, bool enterFlag=false);
	Mat getShapeUsingColor2(Mat src, Mat noise, double shift=1.0);

	/** NOT CORRECT **/
	double epoh(double sat, double lum);
	Mat epohTheHue(Mat hMat, Mat sMat, Mat lMat);

	void maxLocalRanges(Mat mat1, Mat mat2, Mat mat3, Mat hc, Mat noiseMap, double &maxRange, double shift=1.0);

	Mat removeRunningLines(Mat input, Size size);

	Mat filterKneePt(Mat src, double thresh=0.0, double shift=1.0);

	Mat applyDiscreteShade(Mat input);

	Mat getShapeUsingLumContrast(Mat input, Mat noiseMap, float shift=1.0);

};

#endif /* SHAPECOLOR_H_ */
