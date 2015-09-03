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
protected:
	int islArea;
	int islShadeLevel;
	int islShape;
	String islShapeName;
	Mat islandImg;
	Mat NN_Results;
	Point islPt;
	Point _centerOfMass;
	map<String,Point> coordMap;
	String _labelName;
	bool is_shape_shifted;

	void determineIslandShape(Mat &islandImg);
	void getIslandPoints(Mat &islandImg);
public:
	Islands();
	Islands(Mat islandImg);
	int area();
	int& shade();
	Mat& image();
	int& shape();
	String& shape_name();
	Mat& nn_results();
	Point startPt();
	void set_island_shade(int shade);
	Point centerOfMass();
	map<String,Point>& coordinates();
	bool containsCoordinate(String coords);
	bool containsCoordinate(Point pt);
	String& labelName();
	bool isEmpty();
	bool& isShapeShifted();
};

#endif /* ISLANDS_H_ */
