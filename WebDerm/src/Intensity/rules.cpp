/*
 * rules.cpp
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#include "rules.h"

String specialRules(String pix, int r, int g, int b) {
	Color c;
	rgb rgb;
	String pix2 = pix;
	String color = c.getMainColor(pix);
	double grayLevel = rgb.calcGrayLevel(r,g,b);
	//double grayLumLevel = rgb.calcGrayLevel3(r,g,b);
	double colorLevel = rgb.calcColorLevel2(r,g,b);

	/** rule 1 **/
	if(color!="Violet" && grayLevel>=90)
		pix2 = "Grey" + toString(colorLevel);

	/** rule 2 **/
	if(color=="Pink" && grayLevel>=80 && colorLevel>=60)
		pix2 = "Grey" + toString(colorLevel);

	return pix2;
}

bool specialRules(String &pix, double &indexChange, String &shade, String &shadePrev, double &darkness) {
	bool flag=false;
	rgb rgb;
	Color c;
	Intensity in;
	String newShade = shade;
	String newPix = pix;
	double grayLumLevel = rgb.getGrayLevel(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);

/** provisional rule #1 **/
	double indexChangeThresh=2.25;
	if(abs(indexChange)>=indexChangeThresh) {
		int index = in.getShadeIndex(shade);
		index += (indexChange/indexChangeThresh);
		newShade = in.getShade(index);
		flag=true;
	}

/** provisional rule #2 - Determining Gray **/
	double ratio = roundDecimal(grayLumLevel/colorLevel,2);
	if(grayLumLevel>=90 && color!="Violet" && ratio>=0.82) {
		newPix = "Grey" + toString(colorLevel);
		if(colorLevel>=60) {
			newShade = "Dark";
		}
		flag=true;
	}

/** provisional rule #3 for Grayish Pink ONLY **/
	if(color=="Pink" || color=="BrownPink") {
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(grayLumLevel>=39 && colorLevel>=45 && colorLevel<=50 && ratio>=0.75) {
			newPix = "GreyPink" + toString(colorLevel);
			flag = true;
		}
		if(grayLumLevel>=39 && colorLevel>=55 && colorLevel<=70 && ratio>=0.75) {
			newPix = "GreyPink" + toString(colorLevel);
			flag=true;
		}
	}

/** provisional rule #4 for DarkGrayPink ONLY **/
	color = c.getMainColor(newPix);
	if(newShade=="Dark" &&  color=="GreyPink"){
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(ratio>=0.75)
			newPix="Grey" + toString(colorLevel);
	}

	shade = newShade;
	pix = newPix;
	return flag;
}
