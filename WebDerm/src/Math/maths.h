/*
 * maths.h
 *
 *  Created on: Feb 27, 2014
 *      Author: jason
 */

#ifndef MATHS_H_
#define MATHS_H_


#include "/home/jason/git/WebDerm/WebDerm/src/stdafx.h"

double correlationDist(double * vec1, double * vec2);
double cosineDist(double * vec1, double * vec2);
double normalizeToRange(double value, int min, int max);
double myRound(double value);

#endif /* MATHS_H_ */
