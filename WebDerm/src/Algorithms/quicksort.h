/*
 * quicksort.h
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include "../global.h"

void quicksort(deque<double> &values, int p, int r);
void quicksort(deque<int> &values, int p,int r);
void quicksort(Mat &values, int p, int r);

#endif /* QUICKSORT_H_ */
