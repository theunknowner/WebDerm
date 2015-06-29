/*
 * shadeshape.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#include "shadeshape.h"
#include "../shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"

/******************** PRIVATE FUNCTIONS **********************/
bool ShadeShape::isOnTheEdge(Mat &src, int x, int y) {
	Size size(3,3);
	int beginCol = x - floor(size.width/2);
	int beginRow = y - floor(size.height/2);
	int endCol = x + floor(size.width/2);
	int endRow = y + floor(size.height/2);
	if(beginCol<0) beginCol = x;
	if(beginRow<0) beginRow = y;
	if(endCol>=src.cols) endCol = x;
	if(endRow>=src.rows) endRow = y;
	Mat rec = src(Rect(beginCol,beginRow,size.width,size.height));
	int leftEdge = countNonZero(rec.col(0));
	int rightEdge = countNonZero(rec.col(rec.cols-1));
	int topEdge = countNonZero(rec.row(0));
	int bottomEdge = countNonZero(rec.row(rec.rows-1));
	if(leftEdge==0) return true;
	if(rightEdge==0) return true;
	if(topEdge==0) return true;
	if(bottomEdge==0) return true;
	return false;
}

bool ShadeShape::isUnitBridged(Mat &src, int x, int y) {
	Size size(3,3);
	Point ptArr[4][2] = {
			{Point(0,0), Point(2,2)},
			{Point(1,0), Point(1,2)},
			{Point(2,0),Point(0,2)},
			{Point(0,1),Point(2,1)}
	};

	int beginCol = x - 1;
	int beginRow = y - 1;
	int endCol = x + 1;
	int endRow = y + 1;
	Mat rec(size,CV_8U,Scalar(0));
	if(beginCol<0) {
		for(int n=0; n<size.height; n++) {
			rec.at<uchar>(n,0) = 1;
		}
		beginCol = x;
	}
	if(endCol>=src.cols) {
		for(int n=0; n<size.height; n++) {
			rec.at<uchar>(n,2) = 1;
		}
		endCol = x;
	}
	if(beginRow<0) {
		for(int m=0; m<size.width; m++) {
			rec.at<uchar>(0,m) = 1;
		}
		beginRow = y;
	}
	if(endRow>=src.rows) {
		for(int m=0; m<size.width; m++) {
			rec.at<uchar>(2,m) = 1;
		}
		endRow = y;
	}
	for(int i=beginRow; i<=endRow; i++) {
		for(int j=beginCol; j<=endCol; j++) {
			rec.at<uchar>(i-beginRow,j-beginCol) = src.at<uchar>(i,j);
		}
	}
	for(int i=0; i<4; i++) {
		int val1 = rec.at<uchar>(ptArr[i][0]);
		int val2 = rec.at<uchar>(ptArr[i][1]);
		if(val1>0 && val2>0) return true;
	}

	return false;
}

//! should only be used if isUnitBridged() returns true
bool ShadeShape::isBridgeWeak(Mat &src, int x, int y) {
	Size size(3,3);
	int beginCol = x - floor(size.width/2);
	int beginRow = y - floor(size.height/2);
	int endCol = x + floor(size.width/2);
	int endRow = y + floor(size.height/2);
	if(beginCol<0) beginCol = x;
	if(beginRow<0) beginRow = y;
	if(endCol>=src.cols) endCol = x;
	if(endRow>=src.rows) endRow = y;
	double thresh = 2.55;
	Mat kernel(size,CV_32F,Scalar(0));
	for(int i=0; i<kernel.rows; i++) {
		for(int j=0; j<kernel.cols; j++) {
			float dist = MyMath::eucDist(Point(j,i),Point(1,1));
			if(dist!=0) {
				float val = 1.0/(dist*2.0);
				kernel.at<float>(i,j) = roundDecimal(val,2);
			}
		}
	}
	float total = 0, sum=0;
	for(int i=beginRow; i<=endRow; i++) {
		for(int j=beginCol; j<endCol; j++) {
			total += kernel.at<float>(i-beginRow,j-beginCol);
			if(src.at<uchar>(i,j)>0) sum+= kernel.at<float>(i-beginRow,j-beginCol);
		}
	}
	if(sum<=0.75*total)
		return true;

	return false;
}

vector<Mat> ShadeShape::extractFeatures(Mat src) {
	ShapeMorph sm;
	vector<Mat> featureVec = sm.liquidFeatureExtraction(src,0,0,0);
	return featureVec;
}

void ShadeShape::storeFeature(Features feature) {
	this->featureVec.push_back(feature);
}

//! gets the unique shade values of the islands and stores them in a vector
void ShadeShape::getShadesOfFeatures(Mat src) {
	int maxVal = *max_element(src.begin<uchar>(),src.end<uchar>());
	vector<int> shadeVec(maxVal+1,0);
	for(int i=0; i<this->numOfFeatures(); i++) {
		for(int j=0; j<this->feature(i).numOfIslands(); j++) {
			shadeVec.at(this->feature(i).island(j).shade())++;
		}
	}
	for(unsigned int i=0; i<shadeVec.size(); i++) {
		if(shadeVec.at(i)>0) {
			this->shadeVec.push_back(i);
		}
	}
}

/******************** PUBLIC FUNCTIONS *********************/

//! extracts the features from the image
void ShadeShape::extract(Mat src) {
	vector<Mat> featureVec = this->extractFeatures(src);
	for(unsigned int i=0; i<featureVec.size(); i++)  {
		Features feature(featureVec.at(i));
		this->storeFeature(feature);
	}
	this->numOfFeats = this->featureVec.size();
	this->getShadesOfFeatures(src);
	this->ssArea = countNonZero(src);
}

//! extracts the feature image and returns them in a vector
vector<Mat> ShadeShape::extractShadeShape(Mat src) {
	ShapeMorph sm;
	vector<Mat> features = sm.liquidFeatureExtraction(src);
	vector<Mat> shadeShapeVec;
	for(unsigned int i=0; i<features.size(); i++) {
		vector<vector<Point> > ptsVec(256,vector<Point>(0,Point(-1,-1)));
		Mat island = features.at(i);
		for(int row=0; row<island.rows; row++) {
			for(int col=0; col<island.cols; col++) {
				int val = island.at<uchar>(row,col);
				if(val>0) {
					ptsVec.at(val).push_back(Point(col,row));
				}
			}
		}
		for(unsigned int j=0; j<ptsVec.size(); j++) {
			Mat shadeShape(src.size(),CV_8U,Scalar(0));
			if(ptsVec.at(j).size()>0) {
				for(unsigned int k=0; k<ptsVec.at(j).size(); k++) {
					shadeShape.at<uchar>(ptsVec.at(j).at(k)) = j;
				}
				vector<Mat> littleIslands = sm.liquidFeatureExtraction(shadeShape);
				for(unsigned int k=0; k<littleIslands.size(); k++) {
					if(countNonZero(littleIslands.at(k))>10)
						shadeShapeVec.push_back(littleIslands.at(k));
				}
			}
		}
	}
	return shadeShapeVec;
}

//! returns feature of [index]
Features ShadeShape::feature(int featNum) {
	return this->featureVec.at(featNum);
}

//! returns number of features in image
int ShadeShape::numOfFeatures() {
	return this->numOfFeats;
}

//! returns shade value of [index]
int ShadeShape::shade(int num) {
	return this->shadeVec.at(num);
}

//! returns numbers of shades
int ShadeShape::numOfShades() {
	return this->shadeVec.size();
}

int ShadeShape::area() {
	return this->ssArea;
}

vector<Mat> ShadeShape::isolateConnectedFeatures(Mat src) {
	Size size(3,3);
	vector<Point> ptsVec;

	for(int row=0; row<src.rows; row++) {
		for(int col=0; col<src.cols; col++) {
			if(src.at<uchar>(row,col)>0) {
				bool edged = this->isOnTheEdge(src,col,row);
				bool bridged = this->isUnitBridged(src,col,row);
				if(!edged && bridged) {
					bool weak = this->isBridgeWeak(src,col,row);
					if(weak)
						ptsVec.push_back(Point(col,row));
				}
			}
		}
	}
	Mat results = src.clone();
	for(unsigned int i=0; i<ptsVec.size(); i++) {
		results.at<uchar>(ptsVec.at(i)) = 0;
	}
	imgshow(src);
	imgshow(results);
	ShapeMorph sm;
	vector<Mat> isolatedFeatures = sm.liquidFeatureExtraction(results);
	return isolatedFeatures;
}
