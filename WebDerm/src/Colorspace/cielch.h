/*
 * cielch.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef CIELCH_H_
#define CIELCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class CieLch {
public:
	vector<float> lab2lch(float l, float a, float b);
	vector<float> lch2lab(float l, float c, float h);
};

#endif /* CIELCH_H_ */
