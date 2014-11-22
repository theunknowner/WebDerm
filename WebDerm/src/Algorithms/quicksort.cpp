/*
 * quicksort.cpp
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#include "quicksort.h"

deque<double> numbersDouble;
deque<int> numbersInt;

inline void exchangeDouble(int i, int j) {
	double temp = numbersDouble.at(i);
	numbersDouble.at(i) = numbersDouble.at(j);
	numbersDouble.at(j) = temp;
}

inline void exchangeInt(int i, int j) {
	int temp = numbersInt.at(i);
	numbersInt.at(i) = numbersInt.at(j);
	numbersInt.at(j) = temp;
}

inline void sortDouble(int low, int high) {
	int i = low, j = high;
	// Get the pivot element from the middle of the list
	double pivot = numbersDouble.at(low + (high-low)/2);

	// Divide into two lists
	while (i <= j) {
		// If the current value from the left list is smaller then the pivot
		// element then get the next element from the left list
		while (numbersDouble.at(i) < pivot) {
			i++;
		}
		// If the current value from the right list is larger then the pivot
		// element then get the next element from the right list
		while (numbersDouble.at(j) > pivot) {
			j--;
		}

		// If we have found a values in the left list which is larger then
		// the pivot element and if we have found a value in the right list
		// which is smaller then the pivot element then we exchangeDouble the
		// values.
		// As we are done we can increase i and j
		if (i <= j) {
			exchangeDouble(i, j);
			i++;
			j--;
		}
	}
	// Recursion
	if (low < j)
		sortDouble(low, j);
	if (i < high)
		sortDouble(i, high);
}

inline void sortInt(int low, int high) {
	int i = low, j = high;
	// Get the pivot element from the middle of the list
	int pivot = numbersInt.at(low + (high-low)/2);

	// Divide into two lists
	while (i <= j) {
		// If the current value from the left list is smaller then the pivot
		// element then get the next element from the left list
		while (numbersInt.at(i) < pivot) {
			i++;
		}
		// If the current value from the right list is larger then the pivot
		// element then get the next element from the right list
		while (numbersInt.at(j) > pivot) {
			j--;
		}

		// If we have found a values in the left list which is larger then
		// the pivot element and if we have found a value in the right list
		// which is smaller then the pivot element then we exchangeDouble the
		// values.
		// As we are done we can increase i and j
		if (i <= j) {
			exchangeInt(i, j);
			i++;
			j--;
		}
	}
	// Recursion
	if (low < j)
		sortInt(low, j);
	if (i < high)
		sortInt(i, high);
}

void quicksort(deque<double> &values, int p, int r) {
	// check for empty or null array
	if (values.size()==0){
		return;
	}
	numbersDouble = values;
	sortDouble(p, r);

	values = numbersDouble;
}

//p = start pos, r = end pos
void quicksort(deque<int> &values, int p, int r) {
	// check for empty or null array
	if (values.size()==0){
		return;
	}
	numbersInt = values;
	sortInt(p, r);

	values = numbersInt;
}

void printResult() {
	for(unsigned int i=0; i<numbersDouble.size(); i++) {
		printf("%.0f\n",numbersDouble.at(i));
	}
}
