/*
 * statsign.h
 *
 *  Created on: Oct 29, 2015
 *      Author: jason
 */

#ifndef STATSIGN_H_
#define STATSIGN_H_

#include "../../global.h"

//! Statistical signature class
class StatSign {
private:
	const int urnSize1 = 81;
	const int urnSize2 = 17;
	static int urnSize;
public:
	vector<float> create(Mat img, float relArea=1.0);
	float dotProduct(vector<float> statSignVec1, vector<float> statSignVec2);
	float proportion(vector<int> statSignVec1, vector<int> statSignVec2);
	float adjustValue(float value);
	static int getUrnSize();
	void print(vector<float> statSignVec);
	void printCompare(vector<float> statSignVec1, vector<float> statSignVec2);
	void writeCompare(String name, vector<float> statSignVec1, vector<float> statSignVec2);

};

#endif /* STATSIGN_H_ */
