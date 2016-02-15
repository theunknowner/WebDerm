/*
 * Hsl.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef HSL_H
#define HSL_H

#include "../global.h"

class Hsl
{
private:

public:
	static deque<String> hslColors;
	static deque< deque<int> > hueThresh;
	static deque< deque<double> > satThresh;
	static deque< deque<double> > lumThresh;
	static bool THRESH_IMPORTED;

	Hsl();
	bool importHslThresholds();
	double minRGB(double red, double green, double blue);
	double maxRGB(double red, double green, double blue);
	vector<double> rgb2hsl(double red, double green, double blue);
	vector<int> hsl2rgb(double hue, double sat, double lum);
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
	int getIndex(double hue, double sat, double lum);
};

#endif /* HSL_H_ */
