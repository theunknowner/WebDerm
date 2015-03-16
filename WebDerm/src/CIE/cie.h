/*
 * cie.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef CIE_H_
#define CIE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Cie {
public:
	double deltaE76(vector<float> vec1, vector<float> vec2);
	double deltaE2000(vector<float> lch1, vector<float> lch2);
};

#endif /* CIE_H_ */
