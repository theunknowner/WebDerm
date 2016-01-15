/*
 * esgVal.h
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#ifndef ESG_H_
#define ESG_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Esg {
private:
	// esgVec[i][0]=dist, [i][1] = deltaLum, [i][2]=eyeContrast, [i][3]=log10(eyeContrast)
	static vector<vector<float> > esgVec;
	static bool THRESH_IMPORTED;

public:
	float avgDist;
	double esgVal;
	float esgTestVal;

	Esg();
	bool importThresholds();
	int shadeDiff(int shade1, int shade2);
	double calculate(float dist, int shadeDiff, int dgb=0);

};

#endif /* ESG_H_ */
