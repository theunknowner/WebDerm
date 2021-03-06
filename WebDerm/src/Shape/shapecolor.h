/*
 * shapecolor.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef SHAPECOLOR_H_
#define SHAPECOLOR_H_

#include "../global.h"

class ShapeColor {
private:
	int debugLevel=0;
	int dbgCol, dbgRow;
	int test_row, test_col, test_localScanSize;
	bool enterFlag;
public:
	/* shapecolor.cpp */
	void setDebugLevel(int level=0);
	void setDebugColRow(int col, int row);
	Mat removeRunningLines(Mat input, Size size);
	Mat filterKneePt(Mat src, double thresh=0.0, double shift=1.0);

	/* applydiscreteshade.cpp */
	Mat applyDiscreteShade(Mat input);
	Mat applyDiscreteShade(Mat input, int minVal, int maxVal, int intervals);

	/* getshapeusingcolor.cpp */
	Mat getShapeUsingColor(Mat src, Mat noise, double shift=1.0);

	/* getshapeusinglumcontrast.cpp */
	Mat getShapeUsingLumContrast(Mat input, Mat noiseMap, float shift=1.0);


};

#endif /* SHAPECOLOR_H_ */
