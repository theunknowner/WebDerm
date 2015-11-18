/*
 * esgVal.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#include "esg.h"

float Esg::calculate(vector<float> distVec, int shadeDiff) {
	float avgDist = std::accumulate(distVec.begin(),distVec.end(),0.0);
	avgDist /= distVec.size();
	float esg = shadeDiff / (avgDist + 1.0);

	this->avgDist = avgDist;
	this->esgVal = esg;
	return esg;
}
