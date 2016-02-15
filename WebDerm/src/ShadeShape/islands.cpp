/*
 * islands.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "islands.h"
#include "../neuralnetworks/testml.h"
#include "../Shape/shapemorph.h"
#include "../functions.h"
#include "subislands.h"
#include "../Shape/shapes.h"

namespace Isl {
void onMouseCheckSubIslands(int event, int x, int y, int flags, void* param) {
	Islands &island = *((Islands*)param);
	Mat img = island.image().clone();
	SubIslands subIsland;
	static TestML ml;
	if  ( event == EVENT_LBUTTONDOWN ){
		subIsland = island.getSubIslandWithPoint(Point(x,y));
		if(!subIsland.isEmpty()) {
			char text[100];
			int lum = img.at<uchar>(y,x);
			int area = subIsland.area();
			float nnResult = *max_element(subIsland.nn_results().begin<float>(),subIsland.nn_results().end<float>());
			cvtColor(img,img,CV_GRAY2BGR);
			for(auto it=subIsland.coordinates().begin(); it!=subIsland.coordinates().end(); it++) {
				int x = it->second.x;
				int y = it->second.y;
				img.at<Vec3b>(y,x) = Vec3b(0,255,0);
			}
			String shade_shape = subIsland.shape_name();
			sprintf(text,"(%d,%d) | Lum: %d | Area: %d | ShadeShape: %s | NN: %f",x,y,lum,area,shade_shape.c_str(),nnResult);
			cv::displayStatusBar(island.name(),text);
			char textScore[100];
			Mat nnResults = subIsland.nn_results();
			sprintf(textScore,"[%.5f, %.5f, %.5f, %.5f, %.5f, %.5f]",nnResults.at<float>(0,0),nnResults.at<float>(0,1),nnResults.at<float>(0,2),nnResults.at<float>(0,3),nnResults.at<float>(0,4),nnResults.at<float>(0,5));
			namedWindow("SubIsland", CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
			imshow("SubIsland",subIsland.image());
			namedWindow("SubIsland_NN", CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
			cv::displayStatusBar("SubIsland_NN",textScore);
			imshow("SubIsland_NN",subIsland.nn_image());
		}
	}
	if(event == EVENT_LBUTTONUP) {
		img = island.image().clone();
	}
	imshow(island.name(),img);
}

}


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

	Mat results = ml.runANN2(sampleVec);
	this->NN_Results = results;
	auto maxIt = max_element(results.begin<float>(),results.end<float>());
	int labelNum = distance(results.begin<float>(),maxIt);
	float thresh = 0.0;
	if(*maxIt<thresh) labelNum = ml.getShapeIndex2("Default");
	String shapeName = ml.getShapeName2(labelNum);
	if(labelNum==0 || labelNum==1 || labelNum==3) {
		results = ml.runANN2b(sampleVec,labelNum);
		this->NN_Results2 = results;
		this->NN_Score_2 = results.at<float>(0,0);
		if(labelNum==0 || labelNum==1) {
			if(results.at<float>(0,0)>0.0) {
				shapeName = "Comp-" + shapeName;
			} else {
				shapeName = "Incomp-" + shapeName;
			}
		} else {
			if(results.at<float>(0,0)>0.0) {
				shapeName = "Fused-Donuts";
			}
		}

	}
	labelNum = ml.getShapeIndex(shapeName);
	this->NN_Score = *maxIt;
	this->islShape = labelNum;
	this->islShapeName = shapeName;
	this->nn_prepared_img = sample;
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
		String coords = ip::toString(x)+","+ ip::toString(y);
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

vector<Mat> Islands::extractSubIslands(Mat islandImg) {
	ShapeMorph sm;
	vector<Mat> littleIslands = sm.liquidFeatureExtractionInverse(islandImg);
	return littleIslands;

}


void Islands::storeSubIslands(SubIslands& subIsland) {
	this->subIslandVec.push_back(subIsland);
}

/*************** PUBLIC FUNCTIONS ******************/

Islands::Islands(){}

Islands::Islands(Mat islandImg) {
	this->islName = "Island";
	this->NN_Score_2 = -2.0;
	this->islArea = countNonZero(islandImg);
	this->islShadeLevel = *max_element(islandImg.begin<uchar>(),islandImg.end<uchar>());
	this->islandImg = islandImg;
	this->determineIslandShape(islandImg);
	this->getIslandPoints(islandImg);
	this->_labelName = "";
	this->is_shape_shifted = false;
	this->prev_shape = -1;

	if(this->islShapeName.find("Excavated")!=string::npos) {
		Functions fn;
		vector<Mat> littleSubIslands = this->extractSubIslands(islandImg.clone());
		for(unsigned int i=0; i<littleSubIslands.size(); i++) {
			SubIslands subIsland(littleSubIslands.at(i));
			Mat crop_img = fn.cropImage(subIsland.image());
			int frameArea = crop_img.total();
			if(frameArea<=50) {
			Shapes shapes;
			subIsland.shape_name() = "Unknown";
			subIsland.shape() = shapes.getShapeIndex(subIsland.shape_name());
		}
			this->storeSubIslands(subIsland);
		}
	}
}

String Islands::name() {
	return this->islName;
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
Mat& Islands::image() {
	return this->islandImg;
}

Mat& Islands::nn_image() {
	return this->nn_prepared_img;
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

Mat& Islands::nn_results2() {
	return this->NN_Results2;
}

float Islands::nn_score() {
	return this->NN_Score;
}

float Islands::nn_score_2() {
	return this->NN_Score_2;
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


SubIslands& Islands::subIsland(int subIslNum) {
	return this->subIslandVec.at(subIslNum);
}

int Islands::numOfSubIslands() {
	return this->subIslandVec.size();
}

SubIslands Islands::getSubIslandWithPoint(Point pt) {
	for(int i=0; i<this->numOfSubIslands(); i++) {
		SubIslands subIsland = this->subIsland(i);
		if(subIsland.containsCoordinate(pt)) {
			return this->subIsland(i);
		}
	}
	return SubIslands();
}

void Islands::showInteractiveSubIslands() {
	namedWindow(this->islName, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	cv::setMouseCallback(this->islName,Isl::onMouseCheckSubIslands, this);
	imshow(this->islName,this->islandImg);
	waitKey(0);
}
