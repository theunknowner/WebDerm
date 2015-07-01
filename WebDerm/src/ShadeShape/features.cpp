/*
 * features.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "features.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/************ PRIVATE FUNCTIONS ****************/

//! extracts islands from feature
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
			vector<Mat> littleIslands = sm.liquidFeatureExtraction(shadeShape,0,0,0);
			for(unsigned int k=0; k<littleIslands.size(); k++) {
				if(countNonZero(littleIslands.at(k))>thresh)
					islandVec.push_back(littleIslands.at(k));
			}
		}
	}
	return islandVec;
}

//! store extracted island into vector
void Features::storeIsland(Islands island) {
	this->islandVec.push_back(island);
}

//! determines the shape of feature using shape neural network
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

//! gets the unique shade values of the islands and stores them in a vector
void Features::getShadesOfIslands() {
	int maxVal = *max_element(this->featureImg.begin<uchar>(),this->featureImg.end<uchar>());
	vector<int> shadeVec(maxVal+1,0);
	for(int i=0; i<this->numOfIslands(); i++) {
		shadeVec.at(this->island(i).shade())++;
	}
	for(unsigned int i=0; i<shadeVec.size(); i++) {
		if(shadeVec.at(i)>0) {
			this->shadeVec.push_back(i);
		}
	}
}

//! groups islands of same shade to together in a vector
void Features::groupIslandsByShade() {
	this->islVecShade.resize(this->numOfShades(),vector<Islands>(0,Islands()));
	for(int i=0; i<this->numOfShades(); i++) {
		int shadeVal = this->shade(i);
		for(int j=0; j<this->numOfIslands(); j++) {
			int shadeVal2 = this->island(j).shade();
			if(shadeVal2==shadeVal)
				this->islVecShade.at(i).push_back(this->island(j));
		}
	}
}

/************ PUBLIC FUNCTIONS *******************/

Features::Features(Mat featureImg) {
	int thresh=0;
	this->featureImg = featureImg;
	this->featArea = countNonZero(featureImg);
	vector<Mat> littleIslands = this->extractIslands(featureImg, thresh);
	for(unsigned int i=0; i<littleIslands.size(); i++) {
		Islands island(littleIslands.at(i));
		this->storeIsland(island);
	}
	this->numOfIsls = this->islandVec.size();
	this->determineFeatureShape(featureImg);
	this->getShadesOfIslands();
	//this->groupIslandsByShade();
}

//! returns the island of island(index)
Islands Features::island(int islNum) {
	return this->islandVec.at(islNum);
}

//! extracts the islands from the feature
//! islands with area less than <thresh> is ignored
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

//!returns Mat type image of feature
Mat Features::image() {
	return this->featureImg;
}

//! returns area/number of pixels of feature
int Features::area() {
	return this->featArea;
}

//! returns number of islands the feature contains
int Features::numOfIslands() {
	return this->numOfIsls;
}

//! returns index of feature shape
int Features::shape() {
	return this->featShape;
}

//! returns name of the shape of feature
String Features::shape_name() {
	return this->featShapeName;
}

//! outputs shape neural network results
Mat Features::nn_results() {
	return this->NN_Results;
}

//! returns number of unique shades the feature contains
int Features::numOfShades() {
	return this->shadeVec.size();
}

//! gets the value of shade(index)
int Features::shade(int num) {
	return this->shadeVec.at(num);
}

/*
//! gets the vector of islands that is shade(index)
vector<Islands> Features::islandsOfShade(int num) {
	return this->islVecShade.at(num);
}*/