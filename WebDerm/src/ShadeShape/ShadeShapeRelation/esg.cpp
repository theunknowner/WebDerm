/*
 * esgVal.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#include "esg.h"

float Esg::calculate(float dist, int shadeDiff) {
	float esg = pow(shadeDiff,4.0) / (ceil(dist/4.0) + 2.0);

	this->avgDist = dist;
	this->esgVal = esg;
	return esg;
}
