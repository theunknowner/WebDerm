/*
 * Rgb.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef RGB_H_INCLUDED
#define RGB_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Rgb
{
public:
	static deque<String> mainColors;
	static deque<String> allColors;
	static bool THRESH_IMPORTED;

	Rgb();
	int getMainColorIndex(String color);
	int getColorIndex(String color);
	bool importThresholds();
	double normEucDist(double red, double green, double blue, deque<double> &vec);
	double absEucDist(double red, double green, double blue, deque<double> &vec);
	double absEucDist(double red, double green, double blue, deque<int> &vec);
	void outputRGBVals(FILE * fp, int red, int green, int blue, Point coord, double dist, String color, int ind);
	bool checkAbsDist(double dist, double thresh);
	String checkBlack(int r, int g, int b);
	double calcGrayLevel(int red, int green, int blue);
	double calcColorLevel(double red, double green, double blue);
	double calcGrayLumLevel(double red, double green, double blue);
	double getGrayLevel1(String color);
	double getGrayLevel2(String color);
	double getColorLevel(String color, String mainColor);
	double getColorLevel(String pix);
	void showPixelColor(int r, int g, int b, Size size);
	void showPixelColorAtLoc(Mat img, int row, int col, Size size);
	void release_memory();
	String calcColor2(int red, int green, int blue);
	String calcColor(int red, int green, int blue, double &dist, int &ind);
	String calcColor(int red, int green, int blue);
	double calcPerceivedBrightness(double red, double green, double blue);
};


#endif /* RGB_H_ */
