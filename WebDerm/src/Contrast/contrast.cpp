/*
 * contrast.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#include "contrast.h"

double contrast::calcContrastHue(int hue) {
	double h=0;
	h = ((hue+180)%360);
	h /= 360;
	return h;
}

double contrast::calcColorfulness(double contrastHue, double colorLevel) {
	double contrast=0;
	contrast = colorLevel * contrastHue;
	return contrast;
}

double contrast::calcContrast(int hue1, int hue2, String color1, String color2) {
	rgb rgb;
	double colorLevel1=0, colorLevel2=0;
	double cHue1=0, cHue2=0;
	double colorfn1=0, colorfn2=0;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		cHue1 = calcContrastHue(hue1);
		cHue2 = calcContrastHue(hue2);
		if(color1.find(mainColors.at(i))!=string::npos) {
			colorLevel1 = rgb.getColorLevel(color1,mainColors.at(i));
			if(color1.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue1 = -1;
			}
			colorfn1 += calcColorfulness(cHue1, colorLevel1);
		}
		if(color2.find(mainColors.at(i))!=string::npos) {
			colorLevel2 = rgb.getColorLevel(color2,mainColors.at(i));
			if(color2.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue2 = -1;
			}
			colorfn2 += calcColorfulness(cHue2, colorLevel2);
		}
	}
	return colorfn2-colorfn1;
}

void contrast::calcContrastFromMatrix(vector< vector<String> > &windowVec, vector< vector<int> > &hueVec) {
	double contrast=0;
	vector<double> vec;
	vector< vector<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-1); j++) {
			contrast = calcContrast(hueVec.at(i).at(j),hueVec.at(i).at(j+1),
					windowVec.at(i).at(j),windowVec.at(i).at(j+1));
			vec.push_back(contrast);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	writeSeq2File(vec2,"contrast");
	vector<double>().swap(vec);
	vector< vector<double> >().swap(vec2);
}
