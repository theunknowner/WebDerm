/*
 * hsl_global.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef HSL_GLOBAL_H_
#define HSL_GLOBAL_H_

#include "/home/jason/git/WebDerm/WebDerm/src/global.h"

extern double H, S, L, B;
extern vector<String> hslColors;
extern vector< vector<int> > hueThresh;
extern vector< vector<double> > satThresh;
extern vector< vector<double> > lumThresh;
extern vector< vector<double> > satLevel;
extern vector< vector<double> > lumLevel;

#endif /* HSL_GLOBAL_H_ */
