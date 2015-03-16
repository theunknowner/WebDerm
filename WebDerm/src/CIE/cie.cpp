/*
 * cie.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "cie.h"

//CIE Delta E 1976 using Euclidean distance
double Cie::deltaE76(vector<float> vec1, vector<float> vec2) {
	if(vec1.size()!=vec2.size())
		return -1;

	double sum=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		sum += pow(vec1.at(i)-vec2.at(i),2);
	}
	double deltaE = sqrt(sum);
	return deltaE;
}

double Cie::deltaE2000(vector<float> lch1, vector<float> lch2) {
	return -1;
}
