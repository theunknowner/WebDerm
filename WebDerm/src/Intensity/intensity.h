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
	double getMinIntensity();
	double getMaxIntensity();
	double calcIntensity(String color);
	double sigmoidFn(double intensity);
	void setMinMax(deque< deque<double> > &input);
	String getShade(int index);
	int getShadeIndex(String shade);
	String calcShade(double inten);
	deque< deque<double> > calcNormalizedIntensityMatrix(deque< deque<double> > &vec);
	deque< deque<double> > calcIntensityMatrix(deque< deque<String> > &windowVec);
	deque< deque<double> > calcSmoothedIntensityMatrix(deque< deque<double> > &intensityVec);
	deque< deque<String> > calcMainColorMatrix(deque< deque<String> > &windowVec,String name);
	void writeNormalizedIntensityMatrix(deque< deque<String> > &windowVec, String name);
	void writeNormalizedIntensityMatrix(deque< deque<double> > &vec, String name);
	void writeIntensityMatrix(deque< deque<double> > &intensityVec, String name);
	void writeIntensityMatrix(deque< deque<String> > &windowVec, String name);
	void writeMainColorMatrix(deque< deque<String> > &windowVec, String name);
	void writeContrastMatrix(deque< deque<double> > &vec, String name);
	void writeCumConMatrix(deque< deque<double> > &vec, String name);
	void writeSmoothIntensityMatrix(deque< deque<double> > &vec, String name);
	deque< deque<double> > calcUniDimensionContrast(deque< deque<double> > &intensityVec);
};

#endif /* INTENSITY_H_ */
