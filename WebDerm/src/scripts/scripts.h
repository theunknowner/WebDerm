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

void colorThreshNamingScript();
void colorThreshScript();
void outputMeanScript();
void outputFarRGB(Mat &img);
String outputCorrelationRGB(int r, int g, int b);
void spreadsheetCompareScript();

#endif /* SCRIPTS_H_ */
