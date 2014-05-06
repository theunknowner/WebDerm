/*
 * quicksort.cpp
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#include "quicksort.h"

int swapFn(deque<double> &input, int p, int r) {
	double pivot = input.at(r);

	while(p<r) {
		while(input.at(p)<pivot)
			p++;

		while(input.at(r)>pivot)
			r--;

		if(input.at(p)==input.at(r))
			p++;
		else if(p<r) {
			double tmp = input.at(p);
			input.at(p) = input.at(r);
			input.at(r) = tmp;
		}
	}
	return r;
}

void quicksort(deque<double> &input, int p, int r) {
	if(p<r) {
		int j = swapFn(input, p, r);
		quicksort(input,p,j-1);
		quicksort(input,j+1,r);
	}
}
