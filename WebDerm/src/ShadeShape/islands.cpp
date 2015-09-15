/*
 * islands.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "islands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/************ PRIVATE FUNCTIONS ******************/

//! determines shape of island using shape neural network
void Islands::determineIslandShape(Mat &islandImg) {
	TestML ml;
	String param = TestML::PARAM_PATH;
	vector<Mat> sampleVec;
	Mat sample = islandImg.clone();
	sample *= 255;
	sample = ml.prepareImage(sample,ml.getSize());
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
		if(max<0) labelNum = 4; // sets shape to default if all results are negative
		String shapeName = ml.getShapeName(labelNum);

		this->islShape = labelNum;
		this->islShapeName = shapeName;
	}
}

//get coordinates of non-zero pixels
void Islands::getIslandPoints(Mat &islandImg) {
	Mat nonZeroCoord;
	cv::findNonZero(islandImg,nonZeroCoord);
	// gets the center of mass and stores all the coords in a map
	int x=0, y=0, xCenter=0,yCenter=0;
	for(unsigned int i=0; i<nonZeroCoord.total(); i++) {
		x = nonZeroCoord.at<Point>(i).x;
		y = nonZeroCoord.at<Point>(i).y;
		String coords = toString(x)+","+toString(y);
		if(this->coordMap.find(coords)==this->coordMap.end()) {
			this->coordMap[coords] = Point(x,y);
		}
		xCenter += x;
		yCenter += y;
	}
	xCenter /= nonZeroCoord.total();
	yCenter /= nonZeroCoord.total();
	this->_centerOfMass = Point(xCenter,yCenter);

	// gets the start point of the island
	this->islPt = nonZeroCoord.at<Point>(0);


}

/*************** PUBLIC FUNCTIONS ******************/

Islands::Islands(){}

Islands::Islands(Mat islandImg) {
	this->islArea = countNonZero(islandImg);
	this->islShadeLevel = *max_element(islandImg.begin<uchar>(),islandImg.end<uchar>());
	this->islandImg = islandImg;
	this->determineIslandShape(islandImg);
	this->getIslandPoints(islandImg);
	this->_labelName = "";
	this->is_shape_shifted = false;
	this->prev_shape = -1;
}

//! returns area/number of pixels of island
int& Islands::area() {
	return this->islArea;
}

//! returns shade(luminance) of island
//! does not change original shade of island
int& Islands::shade() {
	return this->islShadeLevel;
}

//! returns Mat type image of island
Mat& Islands::image() {
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
Mat& Islands::nn_results() {
	return this->NN_Results;
}

Point Islands::startPt() {
	return this->islPt;
}

//! changes the original image shade/luminance value
void Islands::set_island_shade(int shade) {
	for(int i=0; i<this->islandImg.rows; i++) {
		for(int j=0; j<this->islandImg.cols; j++) {
			String coords = to_string(j) + "," + to_string(i);
			if(this->islandImg.at<uchar>(i,j)>0 && this->coordMap.find(coords)!=this->coordMap.end())
				this->islandImg.at<uchar>(i,j) = shade;
			else
				this->islandImg.at<uchar>(i,j) = 0;
		}
	}
	this->islShadeLevel = shade;
}

Point Islands::centerOfMass() {
	return this->_centerOfMass;
}

map<String,Point>& Islands::coordinates() {
	return this->coordMap;
}

bool Islands::containsCoordinate(String coords) {
	if(this->coordMap.find(coords)!=this->coordMap.end()) {
		return true;
	}
	return false;
}

bool Islands::containsCoordinate(Point pt) {
	String coords = std::to_string(pt.x) + "," + std::to_string(pt.y);
	if(this->coordMap.find(coords)!=this->coordMap.end()) {
		return true;
	}
	return false;
}

String& Islands::labelName() {
	return this->_labelName;
}

bool Islands::isEmpty() {
	return this->islandImg.empty();
}

bool& Islands::isShapeShifted() {
	return this->is_shape_shifted;
}

int& Islands::prevShape() {
	return this->prev_shape;
}
