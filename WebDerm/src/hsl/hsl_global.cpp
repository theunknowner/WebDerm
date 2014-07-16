/*
 * hsl_global.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hsl_global.h"

double H, S, L, B;
deque<String> hslColors;
deque< deque<int> > hueThresh;
deque< deque<double> > satThresh;
deque< deque<double> > lumThresh;
deque<int> hueTableNum;
