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
private:
	int islArea;
	int islShadeLevel;
	int islShape;
	String islShapeName;
	Mat islandImg;
	Mat NN_Results;

	void determineIslandShape(Mat islandImg);
public:
	Islands();
	Islands(Mat islandImg);
	int area();
	int shade();
	Mat image();
	int shape();
	String shape_name();
	Mat nn_results();
	void set_shape(int num);
};

#endif /* ISLANDS_H_ */
