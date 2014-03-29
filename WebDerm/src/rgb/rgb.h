/*
 * rgb.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef RGB_H_INCLUDED
#define RGB_H_INCLUDED
#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "rgb_global.h"

class rgb
{
public:
	bool importThresholds();
	double normEucDist(double red, double green, double blue, vector<double> &vec);
	double absEucDist(double red, double green, double blue, vector<double> &vec);
	double absEucDist(double red, double green, double blue, vector<int> &vec);
	void outputRGBVals(FILE * fp, int red, int green, int blue, Point coord, double dist, String color, int ind);
	bool checkAbsDist(double dist, double thresh);
	String checkBlack(int r, int g, int b);
	String pushColor(int red, int green, int blue);
	String pushColor(int red, int green, int blue,double &dist, int &ind);
	String pushColor(int red, int green, int blue, int &ind);
	double pushColor2(int red, int green, int blue, int &ind);
	double calcGrayLevel(int red, int green, int blue);
	double calcColorLevel(double red, double green, double blue);
	double getGrayLevel(String color);
	double getColorLevel(String color, String mainColor);
	void getLevels(String color, int * levels);
	double calcIntensity(double red, double green, double blue);
	void showPixelColor(int r, int g, int b, Size size);
	void showPixelColorAtLoc(Mat img, int row, int col, Size size);
	void release_memory();
	String calcColor(int red, int green, int blue);
};


#endif /* RGB_H_ */
