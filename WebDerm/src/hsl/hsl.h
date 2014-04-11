/*
 * hsl.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef HSL_H
#define HSL_H

#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "hsl_global.h"

class hsl
{
private:
	bool THRESH_IMPORTED = false;
public:
	bool isThreshImported();
	void setThreshImported(bool flag);
	bool importHslThresholds();
	bool importLsThreshold();
	double minRGB(double red, double green, double blue);
	double maxRGB(double red, double green, double blue);
	void rgb2hsl(double red, double green, double blue);
	void rgb2hsb(double red, double green, double blue);
	int getHue();
	double getSat();
	double getLum();
	double getBrite();
	double calcLum(double red, double green, double blue);
	double calcBrite(double red, double green, double blue);
	double calcRelLum(double red, double green, double blue);
	String getHslColor(double r,double g,double b);
	void release_memory();
	double calcHueAvg(deque<int> &vec);
};

#endif /* HSL_H_ */
