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
	vector<int> create(Mat img);
	float dotProduct(vector<int> statSignVec1, vector<int> statSignVec2);
	float proportion(vector<int> statSignVec1, vector<int> statSignVec2);
	void printCompare(vector<int> statSignVec1, vector<int> statSignVec2);
	void writeCompare(String name, vector<int> statSignVec1, vector<int> statSignVec2);
};

#endif /* STATSIGN_H_ */
