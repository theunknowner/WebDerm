/*
 * matlab.h
 *
 *  Created on: Mar 4, 2015
 *      Author: jason
 */

#ifndef MATLAB_H_
#define MATLAB_H_

#include "../global.h"

class Matlab {
public:
	vector<double> diff(vector<double> vec);
	vector<int> diff(vector<int> vec);

	vector<int> indexOfNonMatchElem(vector<double> vec, double val, double offset=0);
	vector<int> indexOfNonMatchElem(vector<int> vec, int val, int offset=0);
	vector<int> indexOfMatchElem(vector<double> vec, double val, double offset=0);
	vector<int> indexOfMatchElem(vector<int> vec, int val, int offset=0);
};

#endif /* MATLAB_H_ */
