/*
 * hsl_global.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef HSL_GLOBAL_H_
#define HSL_GLOBAL_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

extern double H, S, L;
extern deque<String> hslColors;
extern deque< deque<int> > hueThresh;
extern deque< deque<double> > satThresh;
extern deque< deque<double> > lumThresh;
extern deque<int> hueTableNum;

#endif /* HSL_GLOBAL_H_ */
