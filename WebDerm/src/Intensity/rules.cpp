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
	double grayLumLevel = rgb.calcGrayLevel3(r,g,b);
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
	if(grayLumLevel>=90 && color!="Violet") {
		newPix = "Grey" + toString(colorLevel);
		if(colorLevel>=60) {
			newShade = "Dark";
		}
		flag=true;
	}

/** provisional rule #3 for Grayish Pink ONLY **/
	if(color=="Pink" || color=="BrownPink") {
		if(grayLumLevel>=39 && colorLevel>=45 && colorLevel<=50) {
			newShade = "Gray";
			flag = true;
		}
		if(grayLumLevel>=39 && colorLevel>=55 && colorLevel<=66) {
			newShade = "Gray";
			flag=true;
		}
		newPix="Pink";
	}

	shade = newShade;
	pix = newPix;
	return flag;
}

void checkGrayRatio(deque< deque<String> > &windowVec) {
	double grayLevel=0, colorLevel=0;
	double value=0;
	String pix;
	deque<double> vec1;
	deque< deque<double> > vec2;
	deque<double> grayVec1;
	deque< deque<double> > grayVec2;
	deque<double> colorVec1;
	deque< deque<double> > colorVec2;
	deque<String> strVec1;
	deque< deque<String> > strVec2;
	rgb rgb;
	Color c;
	//FILE * fp;
	//fp = fopen("lp4GrayRatio.csv","w");
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<windowVec.at(i).size(); j++) {
			pix = windowVec.at(i).at(j);
			grayLevel = rgb.getGrayLevel(pix);
			colorLevel = rgb.getColorLevel(pix);
			if(c.getMainColor(pix)=="Grey")
				grayLevel = 100;
			value = grayLevel/colorLevel;
			pix = pix + "(" + toString(value) + ")";
			strVec1.push_back(pix);
			//fprintf(fp,"%d;%d,%f,%f,%f\n",j,i,grayLevel,colorLevel,value);
			//vec1.push_back(value);
			//grayVec1.push_back(grayLevel);
			//colorVec1.push_back(colorLevel);
		}
		strVec2.push_back(strVec1);
		strVec1.clear();
		/*
		vec2.push_back(vec1);
		grayVec2.push_back(grayVec1);
		colorVec2.push_back(colorVec1);
		vec1.clear();
		grayVec1.clear();
		colorVec1.clear();*/
	}
	writeSeq2File(strVec2,"ves18GrayRatio");
	deque<String>().swap(strVec1);
	deque< deque<String> >().swap(strVec2);
	/*writeSeq2File(vec2,"lph4GrayRatio");
	writeSeq2File(grayVec2,"lph4GrayLevels");
	writeSeq2File(colorVec2,"lph4ColorLevels");
	deque<double>().swap(vec1);
	deque< deque<double> >().swap(vec2);
	deque<double>().swap(grayVec1);
	deque< deque<double> >().swap(grayVec2);
	deque<double>().swap(colorVec1);
	deque< deque<double> >().swap(colorVec2);*/
}
