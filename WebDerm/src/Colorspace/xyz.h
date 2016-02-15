/*
 * xyz.h
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#ifndef XYZ_H_
#define XYZ_H_

#include "../global.h"

class Xyz {
public:
	vector<float> rgb2xyz(int r, int g, int b);
	vector<int> xyz2rgb(float x, float y, float z);
};

#endif /* XYZ_H_ */
