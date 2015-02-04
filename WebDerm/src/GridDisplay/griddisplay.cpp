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
	int width = round((double)this->matSize.width/this->gridSize.width);
	int height = round((double)this->matSize.height/this->gridSize.height);
	Size boxSize(width,height);
	Point endPt;
	int row=0, col=0;
	while(row<this->img.rows) {
		while(col<this->img.cols) {
			endPt.x = min(col+boxSize.width-1,img.cols-1);
			endPt.y = min(row+boxSize.height-1,img.rows-1);
			rectangle(this->img,Point(col,row),endPt,Scalar(100));
			col = endPt.x+1;
		}
		col=0;
		row = endPt.y+1;
	}
}

void GridDisplay::displayGrid() {
	imgshow(this->img);
}
