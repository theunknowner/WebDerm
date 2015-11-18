/*
 * esgVal.h
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#ifndef ESG_H_
#define ESG_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Esg {
public:
	float avgDist;
	float esgVal;

	float calculate(vector<float> distVec, int shadeDiff);
};

#endif /* ESG_H_ */
