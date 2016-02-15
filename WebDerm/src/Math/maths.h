/*
 * maths.h
 *
 *  Created on: Feb 27, 2014
 *      Author: jason
 */

#ifndef MATHS_H_
#define MATHS_H_

#include "../stdafx.h"

namespace MyMath {
	double correlationDist(double * vec1, double * vec2);
	double cosineDist(double * vec1, double * vec2);
	double normalizeToRange(double value, int min, int max);
	double myRound(double value, double thresh=.7999999);
	double medianAbsDev(deque<double> &vec);
	double meanAbsDev(deque<double> &vec);
	double standardDevSort(deque<double> &vec);
	double standardDevNoSort(deque<double> &vec);
	int max(int argNum, ...);
	double max(double numArr[], int length);
	double forecast(double input, deque<double> &yArr, deque<double> &xArr);
	double max(deque<double> vec);
	double min(deque<double> vec);
	double euclideanDist(deque<double> &vec1, deque<double> &vec2);
	double euclideanDist(double *vec1, double *vec2);
	double euclideanDist(int *vec1, int *vec2);
	double euclideanDist(float x0, float y0, float x1, float y1);
	float eucDist(Point pt1, Point pt2);
	double findDegreeDistance(double deg1, double deg2);

	/* average.cpp */
	double average(deque<double> &vec);
	double average(vector<double> &vec);
	double average(deque<int> &vec);
	double average(vector<int> &vec);
}

#endif /* MATHS_H_ */
