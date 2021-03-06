/*
f * features.h
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#ifndef FEATURES_H_
#define FEATURES_H_

#include "../global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ImageData/imagedata.h"
#include "islands.h"

class Islands;
class ImageData;

class Features {
private:
	ImageData parentId;

	vector<Islands> islandVec;
	vector<int> shadeVec;
	Mat featureImg;
	int featArea;
	int numOfIsls;
	int featShape;
	String featShapeName;
	Mat NN_Results;
	float NN_Score;

	vector<Mat> extractIslands(Mat featureImg, float thresh);
	vector<Mat> disconnectIslands(Mat featureImg);

	void storeIsland(Islands& island);
	void appendIslands(vector<Islands> islandVec);
	void determineFeatureShape(Mat featureImg);
	void getShadesOfIslands();
public:
	Features();
	Features(Mat featureImg, ImageData &parentId, bool disconnectIslands=true);
	Islands& island(int islNum);
	Mat image();
	int area();
	int numOfIslands();
	int shape();
	String shape_name();
	Mat nn_results();
	float nn_score();
	int numOfShades();
	int shade(int num);
	void release();
	//vector<Islands> islandsOfShade(int num);
};

#endif /* FEATURES_H_ */
