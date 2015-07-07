/*
 * stddev.h
 *
 *  Created on: May 6, 2014
 *      Author: jason
 */

#ifndef STDDEV_H_
#define STDDEV_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

namespace Algos {
double standardDev(deque<double> &input);
double standardDev(deque<int> &input);
double standardDev(vector<double> &input);
double standardDev(vector<float> &input);
double standardDev(Mat src);
}

#endif /* STDDEV_H_ */
