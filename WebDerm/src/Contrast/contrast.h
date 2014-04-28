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
	double max;
	double min;
	double range;
public:
	double calcColorfulness(double hue, String color);
	double calcContrast(double hue1, double hue2, String color1, String color2);
	void calcContrastFromMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec, String name);
	vector< vector<double> > calcContrastFromMatrix(vector< vector<double> > &intensityVec);
	double calcIntensity(double hue, String color);
	void colorfulnessMatrix1x1(Mat &img, String name);
	void calcColorfulnessMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec, String name);
	void colorfulMatrix(Mat img, Size size, String name);
	double getContrastAngle(double hue1, double hue2, String color1, String color2);
	double calcContrastAngle(double hue1, double hue2, double colorfn1, double colorfn2);
	void shadeOfFeature(vector<double> &feature);
	String getShade(double feature);
	void writeMainColorMatrix(vector< vector<String> > &windowVec, String name);

	vector< vector<double> > calcCumulativeContrast(vector< vector<double> > &vec);
	void writeCumulativeContrast(vector< vector<double> > &vec, String name);
};

#endif /* CONTRAST_H_ */
