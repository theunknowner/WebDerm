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
	static const int power = 1;
	static const int minRange = -1;
	static const int maxRange = 1;
public:
	double calcIntensity(String color);
	double sigmoidFn(double intensity);
	void setMinMax(vector< vector<double> > &input);
	String getShade(int index);
	int getShadeIndex(String shade);
	String calcShade(double inten);
	vector< vector<double> > calcNormalizedIntensityMatrix(vector< vector<double> > &vec);
	vector< vector<double> > calcIntensityMatrix(vector< vector<String> > &windowVec);
	vector< vector<double> > calcSmoothedIntensityMatrix(vector< vector<double> > &intensityVec);
	vector< vector<String> > calcMainColorMatrix(vector< vector<String> > &windowVec,String name);
	void writeNormalizedIntensityMatrix(vector< vector<String> > &windowVec, String name);
	void writeNormalizedIntensityMatrix(vector< vector<double> > &vec, String name);
	void writeIntensityMatrix(vector< vector<double> > &intensityVec, String name);
	void writeIntensityMatrix(vector< vector<String> > &windowVec, String name);
	void writeMainColorMatrix(vector< vector<String> > &windowVec, String name);
	void writeContrastMatrix(vector< vector<double> > &vec, String name);
	void writeCumConMatrix(vector< vector<double> > &vec, String name);
	void writeSmoothIntensityMatrix(vector< vector<double> > &vec, String name);
	vector< vector<double> > calcUniDimensionContrast(vector< vector<double> > &intensityVec);
};

#endif /* INTENSITY_H_ */
