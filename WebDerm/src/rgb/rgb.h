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
	String checkBlack(int r, int g, int b);
	String pushColor(int red, int green, int blue, double e);
	String pushColor(int red, int green, int blue);
	String pushColor(int red, int green, int blue, int &ind);
	String getModifier(int red,  int green, int blue);
	int calcGrayLevel(int red, int green, int blue);
	int calcColorLevel(int red, int green, int blue);
	int getGrayLevel(String color);
	void getColorLevel(String color, vector<int> &level);
	void getLevels(String color, int * levels);
	void showPixelColor(int r, int g, int b, Size size);
	void showPixelColorAtLoc(Mat img, int row, int col, Size size);

	void release_memory();
};


#endif /* RGB_H_ */
