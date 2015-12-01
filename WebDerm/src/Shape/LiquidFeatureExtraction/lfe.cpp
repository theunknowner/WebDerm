/*
 * lfe.cpp
 *
 *  Created on: Dec 1, 2015
 *      Author: jason
 */

#include "lfe.h"

void LiquidFeatureExtraction::localExtraction(Mat src, vector<Point> seed_vec) {
	this->seed_vec = seed_vec;
	this->seed_map_vec.resize(this->seed_vec.size(),map<Point,int>());

	int steps = 25;
	for(unsigned int i=0; i<this->seed_vec.size(); i++) {
		Point seed = this->seed_vec.at(i);
		//loop through local feature
	}
}
