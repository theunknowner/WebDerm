/*
 * islands.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "islands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"

/************ PRIVATE FUNCTIONS ******************/

//! determines shape of island using shape neural network
void Islands::determineIslandShape(Mat &islandImg) {
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	vector<Mat> sampleVec;
	Mat sample = islandImg.clone();
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

		this->islShape = labelNum;
		this->islShapeName = shapeName;
	}
}

void Islands::getIslandStartPt(Mat &islandImg) {
	Mat nonZeroPts;
	cv::findNonZero(islandImg,nonZeroPts);
	this->islPt = nonZeroPts.at<Point>(0);
}

/*************** PUBLIC FUNCTIONS ******************/

Islands::Islands(){}

Islands::Islands(Mat islandImg) {
	this->islArea = countNonZero(islandImg);
	this->islShadeLevel = *max_element(islandImg.begin<uchar>(),islandImg.end<uchar>());
	this->islandImg = islandImg;
	this->getIslandStartPt(islandImg);
	this->determineIslandShape(islandImg);
}

//! returns area/number of pixels of island
int Islands::area() {
	return this->islArea;
}

//! returns shade(luminance) of island
//! does not change original shade of island
int& Islands::shade() {
	return this->islShadeLevel;
}

//! returns Mat type image of island
Mat Islands::image() {
	return this->islandImg;
}

//! returns index of shape of the island
int& Islands::shape() {
	return this->islShape;
}

//! returns name of shape of the island
String& Islands::shape_name() {
	return this->islShapeName;
}

//! outputs shape neural network results
Mat Islands::nn_results() {
	return this->NN_Results;
}

Point Islands::coordinate() {
	return this->islPt;
}

//! changes the original image shade/luminance value
void Islands::set_island_shade(int shade) {
	for(int i=0; i<this->islandImg.rows; i++) {
		for(int j=0; j<this->islandImg.cols; j++) {
			if(this->islandImg.at<uchar>(i,j)>0)
				this->islandImg.at<uchar>(i,j) = shade;
		}
	}
	this->islShadeLevel = shade;
}
