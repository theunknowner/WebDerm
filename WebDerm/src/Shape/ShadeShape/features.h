/*
 * features.h
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#ifndef FEATURES_H_
#define FEATURES_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "islands.h"

class Islands;
class Features {
private:
	vector<Islands> islandVec;
	vector<int> shadeVec;
	vector<vector<Islands> > islVecShade;
	Mat featureImg;
	int featArea;
	int numOfIsls;
	int featShape;
	String featShapeName;
	Mat NN_Results;

	vector<Mat> extractIslands(Mat featureImg, int thresh);
	void storeIsland(Islands island);
	void determineFeatureShape(Mat featureImg);
	void getShadesOfIslands();
	void groupIslandsByShade();
public:
	Features(Mat featureImg);
	Islands island(int islNum);
	void extract(Mat featureImg);
	Mat image();
	int area();
	int numOfIslands();
	int shape();
	String shape_name();
	Mat nn_results();
	int numOfShades();
	int shade(int num);
	//vector<Islands> islandsOfShade(int num);
};

#endif /* FEATURES_H_ */
