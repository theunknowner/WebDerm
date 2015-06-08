/*
 * shadeshape.h
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#ifndef SHADESHAPE_H_
#define SHADESHAPE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class ShadeShape {
private:
	vector<String> shadeShapeProperty;
	bool isBridgeWeak(Mat &src, int x, int y);
	bool isOnTheEdge(Mat &src, int x, int y);
	bool isUnitBridged(Mat &src, int x, int y);
public:
	vector<Mat> extractShadeShape(Mat src);
	vector<Mat> isolateConnectedFeatures(Mat src);

	void setShadeShapeProperty(int shadeLevel, int islandNum, int shapeNum);
};

#endif /* SHADESHAPE_H_ */
