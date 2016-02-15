/*
 * intensity.h
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#ifndef INTENSITY_H_
#define INTENSITY_H_

class FileData;

#include "../global.h"

class Intensity {
private:
	int global_flag;
	int shadeCount;
	double minIntensity;
	double maxIntensity;
	double range;
	double outlierCorrection;
	String status;
	String oldMinShade,oldMaxShade;
	String newMinShade,newMaxShade;
	int minIndex;
	int maxIndex;
	int oldShadeAmt;
	double oldInterval;
	double minOutlier;
	double maxOutlier;
	double *thresh;
	int *shadeIndex;
public:
	Intensity();
	~Intensity();
	double getMinIntensity();
	double getMaxIntensity();
	String getOldMinShade();
	String getOldMaxShade();
	String getNewMinShade();
	String getNewMaxShade();
	double calcIntensity(String pix);
	void setMinMaxShades();
	void setMinMax(deque< deque<double> > &input);
	String calcShadeExcludingOutliers(double inten);
	deque< deque<double> > calcNormalizedIntensityMatrix(deque< deque<double> > &vec);
	deque< deque<double> > calcIntensityMatrix(deque< deque<String> > &windowVec);
	deque< deque<double> > calcSmoothedIntensityMatrix(deque< deque<double> > &intensityVec);
	deque< deque<String> > calcMainColorMatrix(Mat img, deque< deque<String> > &windowVec,
									deque< deque<String> > &hslMat,String name, FileData &fd);

};

#endif /* INTENSITY_H_ */
