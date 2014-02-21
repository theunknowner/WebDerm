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
public:
	bool importThresholds();
	bool importLsThreshold();
	double minRGB(double red, double green, double blue);
	double maxRGB(double red, double green, double blue);
	void rgb2hsl(double red, double green, double blue);
	int getHue();
	double getSat();
	double getLum();
	double calcLum(double red, double green, double blue);
	String getHslColor(double r,double g,double b);
	void release_memory();
};

#endif /* HSL_H_ */
