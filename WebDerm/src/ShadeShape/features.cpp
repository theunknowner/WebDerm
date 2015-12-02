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
#include "/home/jason/git/WebDerm/WebDerm/src/ImageData/imagedata.h"
#include "islands.h"
#include "shadeshape.h"

/************ PRIVATE FUNCTIONS ****************/

//! extracts islands from feature
vector<Mat> Features::extractIslands(Mat featureImg, float thresh) {
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
	//> prev size of image before resizing to 140x140
	Size prevSize = this->parentId.prevSize();
	//> magnification factor == (140 x 140) / (L x W)
	double m = featureImg.total() / (double)(prevSize.height * prevSize.width);
	for(unsigned int j=0; j<ptsVec.size(); j++) {
		Mat shadeShape(featureImg.size(),CV_8U,Scalar(0));
		if(ptsVec.at(j).size()>0) {
			for(unsigned int k=0; k<ptsVec.at(j).size(); k++) {
				shadeShape.at<uchar>(ptsVec.at(j).at(k)) = j;
			}
			// helps connect islands that should be together
			shadeShape = sm.densityConnector(shadeShape,.999999, m);
			vector<Mat> littleIslands = sm.liquidFeatureExtraction(shadeShape,0,0,0);
			for(unsigned int k=0; k<littleIslands.size(); k++) {
				float relArea = countNonZero(littleIslands.at(k)) / (float)this->parentId.area();
				if(relArea>thresh)
					islandVec.push_back(littleIslands.at(k));
			}
		}
	}
	return islandVec;
}

//! disconnect islands from feature
vector<Mat> Features::disconnectIslands(Mat featureImg) {
	ShapeMorph sm;
	vector<Mat> islandVec;
	Mat shadeShape = sm.densityDisconnector(featureImg,0.999999);
	vector<Mat> littleIslands = sm.liquidFeatureExtraction(shadeShape,0,0,0);
	for(unsigned int k=0; k<littleIslands.size(); k++) {
		if(countNonZero(littleIslands.at(k))>0)
			islandVec.push_back(littleIslands.at(k));
	}

	return islandVec;
}

//! store extracted island into vector
void Features::storeIsland(Islands& island) {
	this->islandVec.push_back(island);
}

//! appends an islandVec to the existing islandVec
void Features::appendIslands(vector<Islands> islandVec) {
	this->islandVec.insert(this->islandVec.end(),islandVec.begin(),islandVec.end());
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

	Mat results = ml.runANN2(sampleVec);
	this->NN_Results = results;
	auto maxIt = max_element(results.begin<float>(),results.end<float>());
	int labelNum = distance(results.begin<float>(),maxIt);
	float thresh = 0.0;
	if(*maxIt<thresh) labelNum = 5;
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
	this->featShape = labelNum;
	this->featShapeName = shapeName;
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

Features::Features() {}

Features::Features(Mat featureImg, ImageData &parentId) {
	this->parentId = parentId;
	this->featureImg = featureImg;
	this->featArea = countNonZero(featureImg);
	float thresh=0.001;
	vector<Mat> littleIslands = this->extractIslands(featureImg, thresh);
	Functions fn;
	for(unsigned int i=0; i<littleIslands.size(); i++) {
		Islands island(littleIslands.at(i));
		Mat crop_img = fn.cropImage(island.image());
		float frameArea = (float) crop_img.total() / island.image().total();
		if(frameArea>0.01 && island.shape_name().find("Excavated")!=string::npos) {
			bool containsRegularShape = false;
			vector<Islands> islandVec2;
			vector<Mat> littleIslands2 = this->disconnectIslands(island.image());
			for(unsigned int j=0; j<littleIslands2.size(); j++) {
				Islands island2(littleIslands2.at(j));
				islandVec2.push_back(island2);
				if(island2.shape_name().find("Fused-Donuts")!=string::npos || island2.shape_name().find("Comp-Donut")!=string::npos) {
					Mat crop_img2 = fn.cropImage(island2.image());
					float relArea = (float)island2.area()/island.area();
					float frameArea = (float) crop_img2.total() / crop_img.total();
					float bigFrameArea = (float) crop_img2.total() / island.image().total();
					int count = fn.countPositive(island2.nn_results());
					if(relArea>0.10 && frameArea>0.07 && bigFrameArea>0.05 && count==1) {
						containsRegularShape = true;
					}
				}
			}
			if(containsRegularShape) {
				this->appendIslands(islandVec2);
			} else {
				this->storeIsland(island);
			}
		} else {
			this->storeIsland(island);
		}
		//this->storeIsland(island);
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
