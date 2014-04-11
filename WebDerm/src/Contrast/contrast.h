/*
 * contrast.h
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#ifndef CONTRAST_H_
#define CONTRAST_H_

#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

class contrast {
public:
	double calcColorfulness(double contrastHue, double colorLevel);
	double calcContrast(double hue1, double hue2, String color1, String color2);
	void calcContrastFromMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec, String name);
};

#endif /* CONTRAST_H_ */
