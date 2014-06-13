/*
 * testfunctions.h
 *
 *  Created on: Feb 17, 2014
 *      Author: jason
 */

#ifndef TESTFUNCTIONS_H_
#define TESTFUNCTIONS_H_

#include "/home/jason/git/WebDerm/WebDerm/src/global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"

void drawOutlineOnMouse(Mat &img);
void Mat2Image(deque< deque<double> > &input, Mat &output);

#endif /* TESTFUNCTIONS_H_ */
