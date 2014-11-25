/*
 * Hsl.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef HSL_H
#define HSL_H

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"

extern deque<String> hslColors;
extern deque< deque<int> > hueThresh;
extern deque< deque<double> > satThresh;
extern deque< deque<double> > lumThresh;

class Hsl
{
private:
	bool THRESH_IMPORTED;
public:
	bool isThreshImported();
	void setThreshImported(bool flag);
	bool importHslThresholds();
	double minRGB(double red, double green, double blue);
	double maxRGB(double red, double green, double blue);
	double *rgb2hsl(double red, double green, double blue);
	int *hsl2rgb(double hue, double sat, double lum);
	double getHue();
	double getSat();
	double getLum();
	double calcLum(double red, double green, double blue);
	String getHslColor(double hue,double sat,double lum);
	String getHslColor(double hue,double sat,double lum, int &ind, Point pt=Point(0,0));
	String getHslColor(int index);
	void release_memory();
	double calcHueAvg(deque<int> &vec);
	String printHS(double *HSL);
	int getHueDirection(double hue1, double hue2);
	int calcHueDifference(double hue1, double hue2);
};

#endif /* HSL_H_ */
