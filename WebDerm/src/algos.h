/*
 * algos.h
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#ifndef ALGOS_H_
#define ALGOS_H_

#include "functions2.h"
#include "global.h"
#include "functions.h"

template<class T>
int swapFn(T* input, int p, int r) {
	T pivot = input[r];

	while(p<r) {
		while(input[p] < pivot)
			p++;

		while(input[r] > pivot)
			r--;

		if(input[p]==input[r])
			p++;

		else if(p<r) {
			T tmp = input[p];
			input[p] = input[r];
			input[r] = tmp;
		}
	}
	return r;
}

template<class T>
int swapFn(vector<T> &input, int p, int r) {
	T pivot = input.at(r);

	while(p<r) {
		while(input.at(p)<pivot)
			p++;

		while(input.at(r)>pivot)
			r--;

		if(input.at(p)==input.at(r))
			p++;
		else if(p<r) {
			T tmp = input.at(p);
			input.at(p) = input.at(r);
			input.at(r) = tmp;
		}
	}
	return r;
}

template<class T>
int swapFn(deque<T> &input, int p, int r) {
	T pivot = input.at(r);

	while(p<r) {
		while(input.at(p)<pivot)
			p++;

		while(input.at(r)>pivot)
			r--;

		if(input.at(p)==input.at(r))
			p++;
		else if(p<r) {
			T tmp = input.at(p);
			input.at(p) = input.at(r);
			input.at(r) = tmp;
		}
	}
	return r;
}

template<class T>
void quicksort(T* input, int p, int r) {
	if(p<r) {
		int j = swapFn(input, p, r);
		quicksort(input,p,j-1);
		quicksort(input,j+1,r);
	}
}

template<class T>
void quicksort(vector<T> &input, int p, int r) {
	if(p<r) {
		int j = swapFn(input, p, r);
		quicksort(input,p,j-1);
		quicksort(input,j+1,r);
	}
}

template<class T>
void quicksort(deque<T> &input, int p, int r) {
	if(p<r) {
		int j = swapFn(input, p, r);
		quicksort(input,p,j-1);
		quicksort(input,j+1,r);
	}
}

#endif /* ALGOS_H_ */
