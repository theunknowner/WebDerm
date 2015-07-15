/*
 * islands.h
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#ifndef ISLANDS_H_
#define ISLANDS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Islands {
public:
	int islArea;
	int islShadeLevel;
	int islShape;
	String islShapeName;
	Mat islandImg;
	Mat NN_Results;
	Point islPt;

	void determineIslandShape(Mat &islandImg);
	void getIslandStartPt(Mat &islandImg);
public:
	Islands();
	Islands(Mat islandImg);
	int area();
	int& shade();
	Mat image();
	int& shape();
	String& shape_name();
	Mat nn_results();
	Point coordinate();
	void set_island_shade(int shade);
};

#endif /* ISLANDS_H_ */
