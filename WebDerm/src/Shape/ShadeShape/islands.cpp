/*
 * islands.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "islands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"

/************ PRIVATE FUNCTIONS ******************/

void Islands::determineIslandShape(Mat islandImg) {
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


/*************** PUBLIC FUNCTIONS ******************/

Islands::Islands(){}

Islands::Islands(Mat islandImg) {
	this->islArea = countNonZero(islandImg);
	this->islShadeLevel = *max_element(islandImg.begin<uchar>(),islandImg.end<uchar>());
	this->islandImg = islandImg;
	this->determineIslandShape(islandImg);
}

int Islands::area() {
	return this->islArea;
}

int Islands::shade() {
	return this->islShadeLevel;
}

Mat Islands::image() {
	return this->islandImg;
}

int Islands::shape() {
	return this->islShape;
}

String Islands::shape_name() {
	return this->islShapeName;
}

Mat Islands::nn_results() {
	return this->NN_Results;
}
