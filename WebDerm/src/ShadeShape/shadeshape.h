/*
 * shadeshape.h
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#ifndef SHADESHAPE_H_
#define SHADESHAPE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "features.h"
#include "islands.h"

class Features;
class Islands;

class ShadeShape {
private:
	vector<Features> featureVec;
	vector<int> shadeVec;
	int numOfFeats;
	int ssArea;
	Mat img;

	bool isBridgeWeak(Mat &src, int x, int y);
	bool isOnTheEdge(Mat &src, int x, int y);
	bool isUnitBridged(Mat &src, int x, int y);
	vector<Mat> extractFeatures(Mat src);
	void storeFeature(Features feature);
	void getShadesOfFeatures(Mat src);
	void removeDuplicatePointsFromIslands();

public:
	void extract(Mat src);
	Features& feature(int featNum);
	int numOfFeatures();
	int shade(int num);
	int numOfShades();
	int getIndexOfShade(int shade);
	int area();
	Mat& image();
	vector<int> get_shades();
	void release();
	Islands getIslandWithPoint(Point pt);

	vector<Mat> isolateConnectedFeatures(Mat src);
};

#endif /* SHADESHAPE_H_ */
