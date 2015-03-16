/*
 * cielch.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "cielch.h"

vector<float> CieLch::lab2lch(float l, float a, float b) {
	float H = atan2(b,a);
	if ( H > 0 ) H = ( H / M_PI ) * 180.;
	else H = 360 - ( abs(H) / M_PI ) * 180.;

	float L = l;
	float C = sqrt(pow(a,2)+pow(b,2));

	vector<float> lchVec(3,0);
	lchVec.at(0) = L;
	lchVec.at(1) = C;
	lchVec.at(2) = H;

	return lchVec;
}
