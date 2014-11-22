/*
 * scripts.h
 *
 *  Created on: Feb 21, 2014
 *      Author: jason
 */

#ifndef SCRIPTS_H_
#define SCRIPTS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"
#include "/home/jason/git/WebDerm/WebDerm/src/MD5/md5.h"

void colorThreshNamingScript();
void sortColorThreshold(String color1, String color2);
void outputFarRGBScript(Mat &img, String name);
String outputCorrelationRGB(int r, int g, int b, double &m);
String outputCorrelationRGBnorm(int r, int g, int b, double &m);
void spreadsheetCompareScript();
void outputPixLoc(Mat img,int index);
void showPushColorOnImage(Mat &img, int index);
void addNewColors(Mat &img, Point pt1, Point pt2,String color1, String color2);
void colorThreshRenamingScript();
void generateColorRegionTable(Mat &img, Point pt, Size size);
void generateColorRegionTable(Mat &img, Point pt, Point pt2);
double checkEucDist(int r,int g, int b);
void checkColorsFromList(Mat &img, Point pt1, Point pt2);
void dataDeduplicationGrayRGB(double threshold);
void push500Colors(double r, double g, double b);
void generateColorData(Mat &img, Point pt1, Point p2);
void outputHueTableColorImage(double hue);

#endif /* SCRIPTS_H_ */
