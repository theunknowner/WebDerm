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
			// helps connect islands that should be together
			shadeShape = sm.densityConnector(shadeShape,0.9999);
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
	String param = TestML::PARAM_PATH;
	vector<Mat> sampleVec;
	Mat sample = featureImg.clone();
	sample *= 255;
	sample = ml.prepareImage(sample,ml.getSize());
	sampleVec.push_back(sample);

	/*** NN2
	Mat results = ml.runANN(param,sampleVec);
	this->NN_Results = results;
	int labelNum = -1;
	auto maxIt = max_element(results.begin<float>(),results.end<float>());
	auto minIt = min_element(results.begin<float>(),results.end<float>());
	if(*maxIt==*minIt || *maxIt<=-0.5) labelNum = 4;
	else {
		labelNum = distance(results.begin<float>(),maxIt);
	}
	String shapeName = ml.getShapeName(labelNum);
	 ***/
	Mat results = ml.runANN2(sampleVec);
	this->NN_Results = results;
	float thresh = 0.0;
	bool flag = false;
	for(int i=0; i<results.cols; i++) {
		if(results.at<float>(0,i)>thresh) { //> threshold may change from testing
			int labelNum = i;
			this->featShape = labelNum;
			this->featShapeName = ml.getShapeName2(labelNum);
			this->NN_Score = results.at<float>(0,i);
			flag=true;
			break;
		}
	}
	if(flag==false) {
		this->NN_Score = *max_element(results.begin<float>(),results.end<float>());
		this->featShape = 5;
		this->featShapeName = "Default";
	}
}

//! gets the unique shade values of the islands and stores them in a vector
void Features::getShadesOfIslands() {
	int maxVal = *max_element(this->featureImg.begin<uchar>(),this->featureImg.end<uchar>());
	vector<int> shadeVec(maxVal+1,0);
	for(int i=0; i<this->numOfIslands(); i++) {
		try {
			shadeVec.at(this->island(i).shade())++;
		} catch (const std::out_of_range &oor) {
			printf("MaxVal: %d\n",maxVal);
			printf("ShadeVec.size(): %lu\n",shadeVec.size());
			printf("Island: %d, Shade: %d\n",i,this->island(i).shade());
			exit(1);
		}
	}
	for(unsigned int i=0; i<shadeVec.size(); i++) {
		if(shadeVec.at(i)>0) {
			this->shadeVec.push_back(i);
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
Islands& Features::island(int islNum) {
	return this->islandVec.at(islNum);
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

void Features::release() {
	this->islandVec.clear();
	this->islandVec.shrink_to_fit();
}
/*
//! gets the vector of islands that is shade(index)
vector<Islands> Features::islandsOfShade(int num) {
	return this->islVecShade.at(num);
}*/
