/*
 * islands.h
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#ifndef ISLANDS_H_
#define ISLANDS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "subislands.h"

namespace Isl {
	void onMouseCheckSubIslands(int event, int x, int y, int flags, void* param);
}

class Islands {
private:

	vector<SubIslands> subIslandVec;
	String islName;
	int islArea;
	int islShadeLevel;
	int islShape;
	String islShapeName;
	Mat islandImg;
	Mat nn_prepared_img;
	Mat NN_Results;
	Mat NN_Results2;
	float NN_Score;
	float NN_Score_2;
	Point islPt;
	Point _centerOfMass;
	map<String,Point> coordMap;
	String _labelName;
	bool is_shape_shifted;
	int prev_shape;

	void determineIslandShape(Mat &islandImg);
	void getIslandPoints(Mat &islandImg);

	vector<Mat> extractSubIslands(Mat islandImg);
	void storeSubIslands(SubIslands& subIsland);
public:
	Islands();
	Islands(Mat islandImg);
	String name();
	int area();
	int& shade();
	Mat& image();
	Mat& nn_image();
	int& shape();
	String& shape_name();
	Mat& nn_results();
	Mat& nn_results2();
	float nn_score();
	float nn_score_2();
	Point startPt();
	void set_island_shade(int shade);
	Point centerOfMass();
	map<String,Point>& coordinates();
	bool containsCoordinate(String coords);
	bool containsCoordinate(Point pt);
	String& labelName();
	bool isEmpty();
	bool& isShapeShifted();
	int& prevShape();

	SubIslands& subIsland(int subIslNum);
	int numOfSubIslands();

	SubIslands getSubIslandWithPoint(Point pt);
	void showInteractiveSubIslands();

};

#endif /* ISLANDS_H_ */
