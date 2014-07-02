/*
 * maths.h
 *
 *  Created on: Feb 27, 2014
 *      Author: jason
 */

#ifndef MATHS_H_
#define MATHS_H_


#include "/home/jason/git/WebDerm/WebDerm/headers/stdafx.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/median.h"

double correlationDist(double * vec1, double * vec2);
double cosineDist(double * vec1, double * vec2);
double normalizeToRange(double value, int min, int max);
double myRound(double value);
double medianAbsDev(deque<double> &vec);
double meanAbsDev(deque<double> &vec);
double standardDevSort(deque<double> &vec);
double standardDevNoSort(deque<double> &vec);
int max(int argNum, ...);
double max(double numArr[], int length);

#endif /* MATHS_H_ */
