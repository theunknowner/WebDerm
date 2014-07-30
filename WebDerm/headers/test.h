/*
 * test.h
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/skin/Skin.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Intensity/intensity.h"

String testHysteresis(Mat &img, int row, int col, Size size);
String testMouseHysteresis(Mat &img, int row, int col, Size size, deque<Vec3b> &vec, deque<String> &colorVec);
void testColorIndex(Mat &img, int index);
String testColorAtLoc(Mat &img, Point pt, double &hue);

#endif /* TEST_H_ */
