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

/** provisional rule #1 **/
void rule1(double &indexChange, String &shade, String &newShade, bool &flag, deque<int> &ruleNo) {
	Intensity in;
	int ruleNum=1;
	double indexChangeThresh=2.25;
	if(abs(indexChange)>=indexChangeThresh) {
		int index = in.getShadeIndex(shade);
		index += (indexChange/indexChangeThresh);
		newShade = in.getShade(index);
		flag=true;
		ruleNo.push_back(ruleNum);
	}
}

/** provisional rule #2 - Determining Gray **/
void rule2(String &pix, String &newPix, String &newShade, bool &flag, deque<int> &ruleNo) {
	int ruleNum=2;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);
	double ratio = roundDecimal(grayLumLevel/colorLevel,2);
	if(grayLumLevel>=90 && color!="Violet" && ratio>=0.82) {
		newPix = "Grey" + toString(colorLevel);
		if(colorLevel>=60) {
			newShade = "Dark";
		}
		flag=true;
		ruleNo.push_back(ruleNum);
	}
}

/** provisional rule #3 for Grayish Pink ONLY **/
void rule3(String &pix, String &newPix, bool &flag, deque<int> &ruleNo) {
	bool ruleFlag=false;
	int ruleNum=3;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);
	if(color=="Pink" || color=="BrownPink") {
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(grayLumLevel>=39 && colorLevel>=45 && colorLevel<=50 && ratio>=0.75 && ratio<=0.8) {
			newPix = "GreyPink" + toString(colorLevel);
			ruleFlag=true;
		}
		if(grayLumLevel>=39 && colorLevel>=55 && colorLevel<=70 && ratio>=0.75 && ratio<=0.8) {
			newPix = "GreyPink" + toString(colorLevel);
			ruleFlag=true;
		}
	}
	if(ruleFlag==true) {
		flag=true;
		ruleNo.push_back(ruleNum);
	}
}

/** provisional rule #4 for DarkGrayPink ONLY **/
void rule4(String &pix, String &newPix, String &newShade, bool &flag, deque<int> &ruleNo) {
	int ruleNum=4;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(newPix);
	if(newShade=="Dark" &&  color=="GreyPink"){
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(ratio>=0.75) {
			newPix="Grey" + toString(colorLevel);
			flag=true;
			ruleNo.push_back(ruleNum);
		}
	}
}

bool specialRules(String &pix, double &indexChange, String &shade, String &shadePrev, double &darkness, deque<int> &ruleNo) {
	bool flag=false;
	String newShade = shade;
	String newPix = pix;

	rule1(indexChange, shade, newShade,flag, ruleNo);
	//rule2(pix, newPix, newShade, flag, ruleNo);
	//rule3(pix,newPix, flag, ruleNo);
	//rule4(pix, newPix, newShade, flag, ruleNo);

	shade = newShade;
	pix = newPix;
	return flag;
}
