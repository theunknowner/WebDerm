/*
 * testfunctions.h
 *
 *  Created on: Feb 17, 2014
 *      Author: jason
 */

#ifndef TESTFUNCTIONS_H_
#define TESTFUNCTIONS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"

void drawOutlineOnMouse(Mat &img);
void Mat2Image(deque< deque<double> > &input, Mat &output);
Mat test_normalizeColors(Mat src);
Mat test_correctGamma(Mat &img, double gamma);

#endif /* TESTFUNCTIONS_H_ */
