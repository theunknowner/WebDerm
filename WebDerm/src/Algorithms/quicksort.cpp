/*
 * quicksort.cpp
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#include "quicksort.h"

deque<double> numbers;

inline void exchange(int i, int j) {
	double temp = numbers.at(i);
	numbers.at(i) = numbers.at(j);
	numbers.at(j) = temp;
}

inline void sort(int low, int high) {
	int i = low, j = high;
	// Get the pivot element from the middle of the list
	double pivot = numbers.at(low + (high-low)/2);

	// Divide into two lists
	while (i <= j) {
		// If the current value from the left list is smaller then the pivot
		// element then get the next element from the left list
		while (numbers.at(i) < pivot) {
			i++;
		}
		// If the current value from the right list is larger then the pivot
		// element then get the next element from the right list
		while (numbers.at(j) > pivot) {
			j--;
		}

		// If we have found a values in the left list which is larger then
		// the pivot element and if we have found a value in the right list
		// which is smaller then the pivot element then we exchange the
		// values.
		// As we are done we can increase i and j
		if (i <= j) {
			exchange(i, j);
			i++;
			j--;
		}
	}
	// Recursion
	if (low < j)
		sort(low, j);
	if (i < high)
		sort(i, high);
}

void quicksort(deque<double> &values, int p, int r) {
	// check for empty or null array
	if (values.size()==0){
		return;
	}
	numbers = values;
	sort(p, r);

	values = numbers;
}

void printResult() {
	for(unsigned int i=0; i<numbers.size(); i++) {
		printf("%.0f\n",numbers.at(i));
	}
}
