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
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"

class Intensity {
private:
	static const int power = 3;
	double max;
	double min;
	double range;
	String shadeArr[];
public:
	double calcIntensity(String color);
	void setMinMax(double intensity);
	String getShade(double inten);
	vector< vector<double> > calcNormalizedIntensityMatrix(vector< vector<double> > vec);
	vector< vector<double> > calcIntensityMatrix(vector< vector<String> > &windowVec);
	vector< vector<String> > calcMainColorMatrix(vector< vector<String> > &windowVec);
	void writeNormalizedIntensityMatrix(vector< vector<double> > &vec, String name);
	void writeIntensityMatrix(vector< vector<String> > &windowVec, String name);
	void writeMainColorMatrix(vector< vector<String> > &windowVec, String name);
};

#endif /* INTENSITY_H_ */