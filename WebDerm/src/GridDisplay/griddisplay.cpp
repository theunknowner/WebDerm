/*
 * griddisplay.cpp
 *
 *  Created on: Feb 1, 2015
 *      Author: jason
 */

#include "griddisplay.h"

void GridDisplay::setGridVec(deque<deque<double> > gridVec) {
	this->setGridSize(Size(gridVec.at(0).size(),gridVec.size()));
	for(int i=0; i<this->gridSize.height; i++) {
		for(int j=0; j<this->gridSize.width; j++) {
			this->gridMat.at<float>(i,j) = gridVec.at(i).at(j);
		}
	}
}

//! sets the img as background for grid display
void GridDisplay::setImage(Mat img) {
	this->img = img.clone();
	this->setImageSize(this->img.size());
}

//! must set image size before grid size
void GridDisplay::setImageSize(Size size) {
	this->matSize = size;
}

//! must set image size before grid size
//! setImage() runs setImageSize()
void GridDisplay::setGridSize(Size size) {
	this->gridSize = size;
	this->gridMat = Mat::zeros(size,CV_32F);
}

void GridDisplay::drawGrid() {
	int stepX = round((double)this->matSize.width/this->gridSize.width);
	int stepY = round((double)this->matSize.height/this->gridSize.height);
	Point endPt;
	for(int i=0; i<this->img.rows; i+=stepY) {
		for(int j=0; j<this->img.cols; j+=stepX) {
			endPt.x = min(j+gridSize.width-1,img.cols-1);
			endPt.y = min(i+gridSize.height-1,img.rows-1);
			rectangle(this->img,Point(j,i),endPt,Scalar(100));

		}
	}
}

void GridDisplay::displayGrid() {
	imgshow(this->img);
}
