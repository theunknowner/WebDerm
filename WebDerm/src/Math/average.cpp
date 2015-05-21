/*
 * average.cpp
 *
 *  Created on: May 21, 2015
 *      Author: jason
 */

#include "maths.h"

namespace MyMath {
double average(deque<double> &vec) {
	double result=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		result += vec.at(i);
	}
	result /= vec.size();
	return result;
}

double average(vector<double> &vec) {
	double result=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		result += vec.at(i);
	}
	result /= vec.size();
	return result;
}

double average(deque<int> &vec) {
	double result=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		result += vec.at(i);
	}
	result /= vec.size();
	return result;
}

double average(vector<int> &vec) {
	double result=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		result += vec.at(i);
	}
	result /= vec.size();
	return result;
}


}// end namespace
