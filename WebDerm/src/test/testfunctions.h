/*
 * testfunctions.h
 *
 *  Created on: Feb 17, 2014
 *      Author: jason
 */

#ifndef TESTFUNCTIONS_H_
#define TESTFUNCTIONS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

void drawOutlineOnMouse(Mat &img);
void Mat2Image(deque< deque<double> > &input, Mat &output);
Mat test_normalizeColors(Mat src);
double test_inverseGamma(int val);
int test_Gamma(double val);
int test_gray(int r, int g, int b);

void test_CannyEdge(Mat src);

#endif /* TESTFUNCTIONS_H_ */
