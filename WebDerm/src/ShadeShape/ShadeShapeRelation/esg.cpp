/*
 * esgVal.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#include "esg.h"

int Esg::shadeDiff(int shade1, int shade2) {
	return abs(shade1-shade2);
}

float Esg::calculate(float dist, int shadeDiff) {
	//float esg = pow(shadeDiff+1,4.0) / (ceil(dist/4.0) + 2.0);
	const float p00 = -1.52+07;
	const float p10 = -2.539e+07;
	const float p01 = 4.083e+06;
	const float p20 = 1.624e+05;
	const float p11 = 1.059e+05;
	const float p02 = -8752.0;

	float esg = p00 + (p10*dist) + (p01*shadeDiff) + (p20*pow(dist,2)) + (p11*dist*shadeDiff) + (p02*pow(shadeDiff,2));

	this->avgDist = dist;
	this->esgVal = esg;
	return esg;
}
