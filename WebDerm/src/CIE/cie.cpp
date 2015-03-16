/*
 * cie.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "cie.h"

//CIE Delta E 1976 using Euclidean distance
double Cie::deltaE76(vector<float> lch1, vector<float> lch2) {
	if(lch1.size()!=lch2.size())
		return -1;

	double sum=0;
	for(unsigned int i=0; i<lch1.size(); i++) {
		sum += pow(lch1.at(i)-lch2.at(i),2);
	}
	double deltaE = sqrt(sum);
	return deltaE;
}

double Cie::deltaE2000(vector<float> lch1, vector<float> lch2) {
	return -1;
}
