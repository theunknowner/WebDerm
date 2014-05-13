/*
 * stddev.cpp
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#include "stddev.h"

double standardDev(deque<double> &input) {
	double total=0;
	double avg=0;
	double var=0;
	double result=0;
	for(unsigned int i=0; i<input.size(); i++) {
		total += input.at(i);
	}
	avg = total/input.size();
	for(unsigned int i=0; i<input.size(); i++) {
		var += pow(input.at(i)-avg,2);
	}
	var /= input.size();
	result = sqrt(var);
	result = round(result);
	return result;
}
