/*
 * features.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "features.h"
#include "../shapemorph.h";
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/************ PRIVATE FUNCTIONS ****************/

vector<Mat> Features::extractIslands(Mat featureImg, int thresh) {
	ShapeMorph sm;
	vector<Mat> islandVec;
	vector<vector<Point> > ptsVec(256,vector<Point>(0,Point(-1,-1)));
	for(int row=0; row<featureImg.rows; row++) {
		for(int col=0; col<featureImg.cols; col++) {
			int val = featureImg.at<uchar>(row,col);
			if(val>0) {
				ptsVec.at(val).push_back(Point(col,row));
			}
		}
	}
	for(unsigned int j=0; j<ptsVec.size(); j++) {
		Mat shadeShape(featureImg.size(),CV_8U,Scalar(0));
		if(ptsVec.at(j).size()>0) {
			for(unsigned int k=0; k<ptsVec.at(j).size(); k++) {
				shadeShape.at<uchar>(ptsVec.at(j).at(k)) = j;
			}
			vector<Mat> littleIslands = sm.liquidFeatureExtraction(shadeShape);
			for(unsigned int k=0; k<littleIslands.size(); k++) {
				if(countNonZero(littleIslands.at(k))>thresh)
					islandVec.push_back(littleIslands.at(k));
			}
		}
	}
	return islandVec;
}

void Features::storeIsland(Islands island) {
	this->islandVec.push_back(island);
}

void Features::determineFeatureShape(Mat featureImg) {
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	vector<Mat> sampleVec;
	Mat sample = featureImg.clone();
	sample *= 255;
	sample = ml.prepareImage(sample,Size(20,20));
	sampleVec.push_back(sample);
	Mat results = ml.runANN(param,sampleVec);
	this->NN_Results = results;
	for(int i=0; i<results.rows; i++) {
		float max = -2.0;
		int labelNum = -1;
		for(int j=0; j<results.cols; j++) {
			if(results.at<float>(i,j)>max) {
				max = results.at<float>(i,j);
				labelNum = j;
			}
		}
		if(max<0) labelNum = 4;
		String shapeName = ml.getShapeName(labelNum);
		this->featShape = labelNum;
		this->featShapeName = shapeName;
	}
}

/************ PUBLIC FUNCTIONS *******************/

Features::Features(Mat featureImg) {
	int thresh=10;
	this->featureImg = featureImg;
	this->featArea = countNonZero(featureImg);
	vector<Mat> littleIslands = this->extractIslands(featureImg, thresh);
	for(unsigned int i=0; i<littleIslands.size(); i++) {
		Islands island(littleIslands.at(i));
		this->storeIsland(island);
	}
	this->numOfIsls = this->islandVec.size();
	this->determineFeatureShape(featureImg);
}

Islands Features::island(int islNum) {
	return this->islandVec.at(islNum);
}

void Features::extract(Mat featureImg) {
	int thresh=10;
	this->featureImg = featureImg;
	this->featArea = countNonZero(featureImg);
	vector<Mat> littleIslands = this->extractIslands(featureImg, thresh);
	for(unsigned int i=0; i<littleIslands.size(); i++) {
		Islands island(littleIslands.at(i));
		this->storeIsland(island);
	}
	this->numOfIsls = this->islandVec.size();
}

Mat Features::image() {
	return this->featureImg;
}

int Features::area() {
	return this->featArea;
}

int Features::numOfIslands() {
	return this->numOfIsls;
}

int Features::shape() {
	return this->featShape;
}

String Features::shape_name() {
	return this->featShapeName;
}

Mat Features::nn_results() {
	return this->NN_Results;
}

