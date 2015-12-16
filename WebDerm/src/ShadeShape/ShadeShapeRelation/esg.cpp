/*
 * esgVal.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#include "esg.h"

int Esg::shadeDiff(int shade1, int shade2) {
	return abs(shade1-shade2) + 1;
}

float Esg::calculate(float dist, int shadeDiff) {
	float esg = pow(shadeDiff,13.0) / (ceil(dist/4.0) + 2.0);

	this->avgDist = dist;
	this->esgVal = esg;
	return esg;
}
