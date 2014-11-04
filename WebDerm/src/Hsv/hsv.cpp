/*
 * hsv.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: jason
 */

#include "hsv.h"

double * Hsv::rgb2hsv(double red, double green, double blue) {
	Hsl hsl;
	double r = red/255;
	double g = green/255;
	double b = blue/255;
	double min = hsl.minRGB(r,g,b);
	double max = hsl.maxRGB(r,g,b);
	double delta = max-min;
	static double HSV[3] = {0};
	HSV[2] = max;
	if(delta==0) {
		HSV[0] = 0;
		HSV[1] = 0;
	}
	else {
		if(max==0)
			HSV[1] = 0;
		else
			HSV[1] = delta/max;

		if(max==r) {
			HSV[0] = ((g-b)/delta);
		}
		else if(max==g) {
			HSV[0] = ((b-r)/delta) + 2;
		}
		else {
			HSV[0] = ((r-g)/delta) + 4;
		}
		HSV[0] *= 60;
		if(HSV[0]<0) HSV[0]+=360;
		if(HSV[0]>=360) HSV[0]-=360;
	}
	HSV[0] = round(HSV[0]);
	return HSV;
}

int * Hsv::hsv2rgb(double hue, double sat, double val) {
	double RGB[3];
	static int results[3];
	int i=0;
	double v1,v2,v3;

	if(sat==0) {
		RGB[0] = round(val * 255);
		RGB[1] = round(val * 255);
		RGB[2] = round(val * 255);
	}
	else {
		if(hue>=360) hue-=360;
		hue /= 60;
		i = floor(hue);
		v1 = val * (1 - sat);
		v2 = val * (1 - sat * (hue - i));
		v3 = val * (1 - sat * (1 - (hue - i)));
		switch(i) {
		case 0:
			RGB[0] = val;
			RGB[1] = v3;
			RGB[2] = v1;
			break;
		case 1:
			RGB[0] = v2;
			RGB[1] = val;
			RGB[2] = v1;
			break;
		case 2:
			RGB[0] = v1;
			RGB[1] = val;
			RGB[2] = v3;
			break;
		case 3:
			RGB[0] = v1;
			RGB[1] = v2;
			RGB[2] = val;
			break;
		case 4:
			RGB[0] = v3;
			RGB[1] = v1;
			RGB[2] = val;
			break;
		default:
			RGB[0] = val;
			RGB[1] = v1;
			RGB[2] = v2;
			break;
		}
		RGB[0] *= 255;
		RGB[1] *= 255;
		RGB[2] *= 255;
	}
	results[0] = (int)RGB[0];
	results[1] = (int)RGB[1];
	results[2] = (int)RGB[2];
	return results;
}

