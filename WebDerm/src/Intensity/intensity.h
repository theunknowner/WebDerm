/*
 * intensity.h
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#ifndef INTENSITY_H_
#define INTENSITY_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/table.h"

#include "rules.h"

class FileData;
class Intensity {
public:
	double getMinIntensity();
	double getMaxIntensity();
	String getOldMinShade();
	String getOldMaxShade();
	String getNewMinShade();
	String getNewMaxShade();
	int getShadeCount();
	void release_memory();
	bool importThresholds();
	double calcIntensity(String pix);
	double sigmoidFn(double intensity);
	void setMinMax(deque< deque<double> > &input);
	String getShade(int index);
	int getShadeIndex(String shade);
	String calcShade(double inten, bool eof);
	deque< deque<double> > calcNormalizedIntensityMatrix(deque< deque<double> > &vec);
	deque< deque<double> > calcIntensityMatrix(deque< deque<String> > &windowVec);
	deque< deque<double> > calcSmoothedIntensityMatrix(deque< deque<double> > &intensityVec);
	deque< deque<String> > calcMainColorMatrix(Mat &img, deque< deque<String> > &windowVec,
									deque< deque<String> > &hslMat,String name, FileData &fd);
	void writeMainColorMatrix(Mat &img, deque< deque<String> > &windowVec,
							deque< deque<String> > &hslMat,String name, FileData &fd);
	void writeMainColorMatrix(FileData &fd);
};

#endif /* INTENSITY_H_ */
