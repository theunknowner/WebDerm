/*
 * test.h
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "rgb/rgb.h"
#include "hsl/hsl.h"
#include "Hsv/hsv.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "Math/maths.h"
#include "FileData/filedata.h"
#include "Histogram/histogram.h"
#include "Shades/shades.h"

String testHysteresis(Mat &img, int row, int col, Size size);
String testMouseHysteresis(Mat &img, int row, int col, Size size, deque<Vec3b> &vec, deque<String> &colorVec);
void testColorIndex(Mat &img, int index);
String testColorAtLoc(Mat &img, Point pt, double &hue);

#endif /* TEST_H_ */
