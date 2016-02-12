/*
 * subislands.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: jason
 */

#include "subislands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/************** PRIVATE FUNCTIONS ******************/
void SubIslands::determineIslandShape(Mat &subIslandImg) {
	TestML ml;
	String param = TestML::PARAM_PATH;
	vector<Mat> sampleVec;
	Mat sample = subIslandImg.clone();
	sample *= 255;
	sample = ml.prepareImage(sample,ml.getSize());
	sampleVec.push_back(sample);

	Mat results = ml.runANN2(sampleVec);
	Mat results2 = ml.runANN2b(sampleVec,3); //Fused-Donut NN3
	cv::hconcat(results,results2,results);
	this->NN_Results = results;
	auto maxIt = max_element(results.begin<float>(),results.end<float>());
	int labelNum = distance(results.begin<float>(),maxIt);
	float thresh = 0.0;
	if(*maxIt<thresh) labelNum = ml.getShapeIndex2("Default");
	String shapeName = ml.getShapeName2(labelNum);
	if(labelNum==0 || labelNum==1) {
		results = ml.runANN2b(sampleVec,labelNum);
		if(results.at<float>(0,0)>0.0) {
			shapeName = "Comp-" + shapeName;
		} else {
			shapeName = "Incomp-" + shapeName;
		}
	}
	labelNum = ml.getShapeIndex(shapeName);
	this->NN_Score = *maxIt;
	this->islShape = labelNum;
	this->islShapeName = shapeName;
	this->nn_prepared_img = sample;
}

//get coordinates of non-zero pixels
void SubIslands::getIslandPoints(Mat &islandImg) {
	Mat nonZeroCoord;
	cv::findNonZero(islandImg,nonZeroCoord);
	// gets the center of mass and stores all the coords in a map
	int x=0, y=0;
	for(unsigned int i=0; i<nonZeroCoord.total(); i++) {
		x = nonZeroCoord.at<Point>(i).x;
		y = nonZeroCoord.at<Point>(i).y;
		String coords = ip::toString(x)+","+ ip::toString(y);
		if(this->coordMap.find(coords)==this->coordMap.end()) {
			this->coordMap[coords] = Point(x,y);
		}
	}
}

/************** PUBLIC FUNCTIONS *******************/

SubIslands::SubIslands() {}

//! class is used to store the black discs subfeatures inside a feature
SubIslands::SubIslands(Mat subIslandImg) {
	this->islArea = countNonZero(subIslandImg);
	this->islandImg = subIslandImg;
	this->determineIslandShape(subIslandImg);
	this->getIslandPoints(subIslandImg);
}

int SubIslands::area() {
	return this->islArea;
}

Mat& SubIslands::image() {
	return this->islandImg;
}

Mat& SubIslands::nn_image() {
	return this->nn_prepared_img;
}

int& SubIslands::shape() {
	return this->islShape;
}

String& SubIslands::shape_name() {
	return this->islShapeName;
}

Mat& SubIslands::nn_results() {
	return this->NN_Results;
}

float SubIslands::nn_score() {
	return this->NN_Score;
}

map<String,Point>& SubIslands::coordinates() {
	return this->coordMap;
}

bool SubIslands::containsCoordinate(String coords) {
	if(this->coordMap.find(coords)!=this->coordMap.end()) {
		return true;
	}
	return false;
}
bool SubIslands::containsCoordinate(Point pt) {
	String coords = std::to_string(pt.x) + "," + std::to_string(pt.y);
	if(this->coordMap.find(coords)!=this->coordMap.end()) {
		return true;
	}
	return false;
}
bool SubIslands::isEmpty() {
	return this->islandImg.empty();
}
