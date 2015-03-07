/*
 * jaysort.h
 *
 *  Created on: Nov 19, 2014
 *      Author: jason
 */

#ifndef JAYSORT_H_
#define JAYSORT_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

void jaysort(deque<double> &vec, deque<int> &origPos);
void jaysort(deque<int> &vec, deque<int> &origPos);
void jaysort(Mat &src, Mat &pos);
void jaysort(deque<deque<double> > &vec, deque<int> &origPos, deque<int> sortKey);


#endif /* JAYSORT_H_ */
