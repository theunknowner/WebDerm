/*
 * test.h
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "functions.h"
#include "rgb/rgb.h"
#include "skin/Skin.h"
#include "hysteresis/hysteresis.h"

String testHysteresis(Mat &img, int row, int col, Size size);
String testMouseHysteresis(Mat &img, int row, int col, Size size, vector<Vec3b> &vec, vector<String> &colorVec);
void testMouseColor(Mat img);
void testSatLum(Mat &img);
void testColorIndex(Mat &img, int index);
void testColor(int r, int g, int b);
void testThresh();

#endif /* TEST_H_ */
