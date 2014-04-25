/*
 * intensity.h
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#ifndef INTENSITY_H_
#define INTENSITY_H_

#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

class Intensity {
private:
	double max;
	double min;
	double range;
public:
	double calcIntensity(String color);
	void setMaxIntensityOfFeature(vector<double> &feature);
	void writeNormalizedIntensityMatrix(vector< vector<double> > vec, String name);
	String getShade(double inten);
	void writeIntensityFromMatrix(vector< vector<String> > &windowVec, String name);
	void writeMainColorMatrix(vector< vector<String> > &windowVec, String name);
};

#endif /* INTENSITY_H_ */
