/*
 * rgb.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef RGB_H_INCLUDED
#define RGB_H_INCLUDED
#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "rgb_global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Intensity/rules.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

class rgb
{
private:
	bool THRESH_IMPORTED;
public:
	bool isThreshImported();
	void setThreshImported(bool flag);
	int getIndex(String color);
	bool importColorThresholds();
	bool importThresholds();
	double normEucDist(double red, double green, double blue, deque<double> &vec);
	double absEucDist(double red, double green, double blue, deque<double> &vec);
	double absEucDist(double red, double green, double blue, deque<int> &vec);
	void outputRGBVals(FILE * fp, int red, int green, int blue, Point coord, double dist, String color, int ind);
	bool checkAbsDist(double dist, double thresh);
	String checkBlack(int r, int g, int b);
	String pushColor(int red, int green, int blue);
	String pushColor(int red, int green, int blue,double &dist, int &ind);
	String pushColor(int red, int green, int blue, int &ind, double &dist);
	double calcGrayLevel(int red, int green, int blue);
	double calcGrayLevel2(int red, int green, int blue);
	double calcColorLevel(double red, double green, double blue);
	double calcColorLevel2(double red, double green, double blue);
	double getGrayLevel1(String color);
	double getGrayLevel2(String color);
	double getColorLevel(String color, String mainColor);
	double getColorLevel(String pix);
	void showPixelColor(int r, int g, int b, Size size);
	void showPixelColorAtLoc(Mat img, int row, int col, Size size);
	void release_memory();
	String calcColor2(int red, int green, int blue);
	String calcColor(int red, int green, int blue, double &dist, int &ind);
	double calcRelLum(double red, double green, double blue);
	double calcPerceivedBrightness(double red, double green, double blue);
	double calcGrayLumLevel(double red, double green, double blue);
	bool importGrayLUT();
};


#endif /* RGB_H_ */
