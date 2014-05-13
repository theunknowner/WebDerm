/*
 * mean.cpp
 *
 *  Created on: May 12, 2014
 *      Author: jason
 */

#include "mean.h"

double mean(deque<double> &vec) {
	double avg=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		avg += vec.at(i);
	}
	avg /= vec.size();
	return avg;
}
