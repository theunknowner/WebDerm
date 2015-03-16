/*
 * cielab.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef CIELAB_H_
#define CIELAB_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class CieLab {
public:
	vector<float> xyz2lab(float x, float y, float z);
	vector<float> lab2xyz(float l, float a, float b);
};

#endif /* CIELAB_H_ */
