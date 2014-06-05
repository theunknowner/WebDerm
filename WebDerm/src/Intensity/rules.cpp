/*
 * rules.cpp
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#include "rules.h"

String newShade(String pix, double indexChange, String shade, String shadePrev) {
	//rgb rgb;
	Color c;
	Intensity in;
	String color = c.getMainColor(pix);
	//double grayLevel = rgb.getGrayLevel(pix);
	//double colorLevel = rgb.getColorLevel(pix);
	double indexChangeThresh=2.25;

	if(indexChange>=indexChangeThresh) {
		int index = in.getShadeIndex(shade);
		index += (indexChange/indexChangeThresh);
		shade = in.getShade(index);
	}
/*
	if(colorLevel<=25)	return White;
	if(grayLevel>=85 && colorLevel<=30) return White;
	if(indexChange<=(-2) && (grayLevel>=91 || color=="Grey" || colorLevel<=25)) return White;
	if(indexChange>=2) return Dark;
	if(shadePrev==White && indexChange==0) {
		return shadePrev;
	}*/
	return shade;
}
