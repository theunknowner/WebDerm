/*
 * scripts.h
 *
 *  Created on: Feb 21, 2014
 *      Author: jason
 */

#ifndef SCRIPTS_H_
#define SCRIPTS_H_

#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

void colorThreshNamingScript();
void colorThreshScript();
void outputMeanScript();
void outputFarRGBScript(Mat &img, String name);
String outputCorrelationRGB(int r, int g, int b, double &m);
String outputCorrelationRGBnorm(int r, int g, int b, double &m);
void spreadsheetCompareScript();
void outputPixLoc(Mat img,int index);
void showPushColorOnImage(Mat &img, int index);
void addNewColors(Mat &img, Point pt1, Point pt2);

#endif /* SCRIPTS_H_ */
