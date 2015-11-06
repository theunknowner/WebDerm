/*
 * statsign.h
 *
 *  Created on: Oct 29, 2015
 *      Author: jason
 */

#ifndef STATSIGN_H_
#define STATSIGN_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

//! Statistical signature class
class StatSign {
public:
	vector<float> create(Mat img, float relArea=1.0);
	float dotProduct(vector<float> statSignVec1, vector<float> statSignVec2);
	float proportion(vector<int> statSignVec1, vector<int> statSignVec2);
	float adjustValue(float value);
	void printCompare(vector<float> statSignVec1, vector<float> statSignVec2);
	void writeCompare(String name, vector<float> statSignVec1, vector<float> statSignVec2);
};

#endif /* STATSIGN_H_ */
