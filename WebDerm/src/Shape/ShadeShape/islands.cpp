/*
 * islands.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#include "islands.h"

Islands::Islands(Mat islandImg) {
	this->islArea = countNonZero(islandImg);
	this->islShadeLevel = *max_element(islandImg.begin<uchar>(),islandImg.end<uchar>());
	this->islandImg = islandImg;
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
