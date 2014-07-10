/*
 * hsl.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef HSL_H
#define HSL_H

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "hsl_global.h"

class hsl
{
private:
	bool THRESH_IMPORTED;
public:
	bool isThreshImported();
	void setThreshImported(bool flag);
	bool importHslThresholds();
	bool importLsThreshold();
	double minRGB(double red, double green, double blue);
	double maxRGB(double red, double green, double blue);
	double *rgb2hsl(double red, double green, double blue);
	double *hsl2rgb(double hue, double sat, double lum);
	double getHue();
	double getSat();
	double getLum();
	double getBrite();
	double calcLum(double red, double green, double blue);
	double calcBrite(double red, double green, double blue);
	double calcRelLum(double red, double green, double blue);
	String getHslColor(double hue,double sat,double lum, int &ind);
	void release_memory();
	double calcHueAvg(deque<int> &vec);
};

#endif /* HSL_H_ */
