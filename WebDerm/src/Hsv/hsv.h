/*
 * hsv.h
 *
 *  Created on: Nov 3, 2014
 *      Author: jason
 */

#ifndef HSV_H_
#define HSV_H_

#include "../global.h"

class Hsv {
public:
	double * rgb2hsv(double red, double green, double blue);
	int * hsv2rgb(double hue, double sat, double val);
};

#endif /* HSV_H_ */
