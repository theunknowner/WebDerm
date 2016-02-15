/*
 * subislands.h
 *
 *  Created on: Nov 13, 2015
 *      Author: jason
 */

#ifndef SUBISLANDS_H_
#define SUBISLANDS_H_

#include "../global.h"

//! class is used to store the black discs subfeatures inside a feature
class SubIslands {
private:
	int islArea;
	int islShape;
	String islShapeName;
	Mat islandImg;
	Mat nn_prepared_img;
	Mat NN_Results;
	float NN_Score;
	map<String,Point> coordMap;

	void determineIslandShape(Mat &subIslandImg);
	void getIslandPoints(Mat &islandImg);
public:
	SubIslands();
	SubIslands(Mat subIslandImg);
	int area();
	Mat& image();
	Mat& nn_image();
	int& shape();
	String& shape_name();
	Mat& nn_results();
	float nn_score();
	map<String,Point>& coordinates();
	bool containsCoordinate(String coords);
	bool containsCoordinate(Point pt);
	bool isEmpty();
};

#endif /* SUBISLANDS_H_ */
