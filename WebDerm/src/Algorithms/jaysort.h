/*
 * jaysort.h
 *
 *  Created on: Nov 19, 2014
 *      Author: jason
 */

#ifndef JAYSORT_H_
#define JAYSORT_H_

#include "../global.h"

void jaysort(deque<double> &vec, deque<int> &origPos);
void jaysort(deque<int> &vec, deque<int> &origPos);
void jaysort(Mat &src, Mat &pos);
void jaysort(vector<double> &vec, vector<int> &origPos);
void jaysort(vector<int> &vec, vector<int> &origPos, int flag=0);
void jaysort(vector<float> &vec, vector<int> &origPos, int flag=0);
void jaysort(vector<vector<float> > &vec, vector<int> &origPos, int flag=0);


#endif /* JAYSORT_H_ */
