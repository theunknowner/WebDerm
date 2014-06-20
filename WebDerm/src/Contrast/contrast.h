/*
 * contrast.h
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#ifndef CONTRAST_H_
#define CONTRAST_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

class contrast {
public:
	double max;
	double min;
	double range;
public:
	deque< deque<double> > calcContrastFromMatrix(deque< deque<double> > &intensityVec);
	double calcContrastAngle(double hue1, double hue2, double colorfn1, double colorfn2);
	deque< deque<double> > calcCumulativeContrast(deque< deque<double> > &vec);
	void writeCumulativeContrast(deque< deque<double> > &vec, String name);
};

#endif /* CONTRAST_H_ */
