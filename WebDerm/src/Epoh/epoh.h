/*
 * epoh.h
 *
 *  Created on: Jul 9, 2015
 *      Author: jason
 */

#ifndef EPOH_H_
#define EPOH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

double epoh(double &sat, double &lum);
Mat epohTheHue(Mat &hMat, Mat &sMat, Mat &lMat);

#endif /* EPOH_H_ */
