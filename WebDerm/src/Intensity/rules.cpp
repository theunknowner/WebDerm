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

	/** rule 3 **/
	if(color=="Brown" && colorLevel>=70 && grayLevel>=80)
		pix2 = "Grey" + toString(colorLevel);

	return pix2;
}

/** provisional rule #1 **/
int rule1(double &indexChange, String &shade, String &newShade) {
	Intensity in;
	bool flag=false;
	int ruleNum=1;
	double indexChangeThresh=2.25;
	if(abs(indexChange)>=indexChangeThresh) {
		int index = in.getShadeIndex(shade);
		index += (indexChange/indexChangeThresh);
		newShade = in.getShade(index);
		flag=true;
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** provisional rule #2 - Determining Gray **/
int rule2(String &pix, String &newPix, String &newShade) {
	bool flag=false;
	int ruleNum=2;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);
	double ratio = roundDecimal(grayLumLevel/colorLevel,2);
	if(grayLumLevel>=90 && color!="Violet" && ratio>=0.82) {
		newPix = "Grey" + toString(colorLevel);
		if(colorLevel>=60) {
			newShade = "Dark";
		}
		flag=true;
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** provisional rule #3 for Grayish Pink ONLY **/
int rule3(String &pix, String &newPix) {
	bool flag=false;
	int ruleNum=3;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);
	if(color=="Pink" || color=="BrownPink") {
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(grayLumLevel>=39 && colorLevel>=45 && colorLevel<=50 && ratio>=0.75 && ratio<=0.8) {
			newPix = "GreyPink" + toString(colorLevel);
			flag=true;
		}
		if(grayLumLevel>=39 && colorLevel>=55 && colorLevel<=70 && ratio>=0.75 && ratio<=0.8) {
			newPix = "GreyPink" + toString(colorLevel);
			flag=true;
		}
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** provisional rule #4 for DarkGrayPink ONLY **/
int rule4(String &pix, String &newPix, String &newShade) {
	bool flag=false;
	int ruleNum=4;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(newPix);
	if(newShade=="Dark" &&  color=="GreyPink"){
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(ratio>=0.75) {
			newPix="Grey" + toString(colorLevel);
			flag=true;
		}
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** Rule #5 - Determining Dark Grey **/
int rule5(String &pix, String &newPix, String &newShade) {
	int ruleNum=5;
	bool flag=false;
	rgb rgb;
	Color c;
	String color = c.getMainColor(pix);
	double grayLevel = rgb.getGrayLevel1(pix);
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	double ratio = roundDecimal(grayLumLevel/colorLevel,2);
	if((color=="Pink" || color=="BrownPink"))
		if(newShade=="Dark")
			if(ratio>=1.17) {
				newShade="Dark";
				newPix = "Grey";
				flag=true;
			}

	if(color=="Brown" && grayLevel>=70)
		if(newShade=="Dark")
			if(ratio>=1.2) {
				newShade = "Dark";
				newPix = "Grey";
				flag=true;
			}

	if(color=="Grey")
		if(newShade=="High" || newShade=="Dark")
			if(ratio>=1.15) {
				newShade= "Dark";
				newPix = "Grey";
				flag=true;
			}
	if(flag==true) return ruleNum;

	return 0;
}

bool specialRules(String &pix, double &indexChange, String &shade, String &shadePrev, double &darkness, deque<int> &ruleNo) {
	bool flag=false;
	String newShade = shade;
	String newPix = pix;

	ruleNo.push_back(rule1(indexChange, shade, newShade));
	ruleNo.push_back(rule5(pix,newPix,newShade));

	for(unsigned int i=0; i<ruleNo.size(); i++) {
		if(ruleNo.at(i)!=0)
			flag=true;
	}

	//rule2(pix, newPix, newShade, flag, ruleNo);
	//rule3(pix,newPix, flag, ruleNo);
	//rule4(pix, newPix, newShade, flag, ruleNo);

	shade = newShade;
	pix = newPix;
	return flag;
}
