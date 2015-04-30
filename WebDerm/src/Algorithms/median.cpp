/*
 * median.cpp
 *
 *  Created on: May 12, 2014
 *      Author: jason
 */

#include "median.h"
#include "quicksort.h"

double medianSorted(deque<double> &vec) {
	unsigned int size = vec.size();
	double median=0;
	if(size%2==0) {
		median = vec.at(size/2) + vec.at((size/2)+1);
		median /= 2;
	}
	else median = vec.at((size/2)+1);

	return median;
}

double medianUnsorted(deque<double> &vec) {
	quicksort(vec,0,vec.size()-1);
	return medianSorted(vec);
}
