/*
 * features.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "features.h"
#include "../shapemorph.h";

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
