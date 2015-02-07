/*
 * kneecurve.h
 *
 *  Created on: Jan 23, 2015
 *      Author: jason
 */

#ifndef KNEECURVE_H_
#define KNEECURVE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class KneeCurve {
	double maxDist;
	int bestIdx;
public:
	int kneeCurvePoint(deque<double> &vec);
	int kneeCurvePoint(vector<double> & vec);
	double getMaxDist();
	void removeOutliers(deque<double> &vec, double percent);
	void removeOutliers(vector<double> &vec, double percent);

	void loadVectorFile(String path, deque<double> &vec);
};

#endif /* KNEECURVE_H_ */
