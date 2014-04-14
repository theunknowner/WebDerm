/*
 * contrast.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#include "contrast.h"

double contrast::calcColorfulness(double contrastHue, double colorLevel) {
	double contrast=0;
	contrast = colorLevel * contrastHue;
	return contrast;
}

double contrast::calcContrast(double hue1, double hue2, String color1, String color2) {
	rgb rgb;
	double grayHue = -0.25;
	double colorLevel1=0, colorLevel2=0;
	double cHue1, cHue2;
	double colorfn1=0, colorfn2=0;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		cHue1 = hue1;
		cHue2 = hue2;
		if(color1.find(mainColors.at(i))!=string::npos) {
			colorLevel1 = rgb.getColorLevel(color1,mainColors.at(i));
			if(color1.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue1 = grayHue;
			}
			colorfn1 += calcColorfulness(cHue1, colorLevel1);
		}
		if(color2.find(mainColors.at(i))!=string::npos) {
			colorLevel2 = rgb.getColorLevel(color2,mainColors.at(i));
			if(color2.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue2 = grayHue;
			}
			colorfn2 += calcColorfulness(cHue2, colorLevel2);
		}
	}
	printf("Colorfn1: %f\n",colorfn1);
	printf("Colorfn2: %f\n",colorfn2);
	return colorfn2-colorfn1;
}

void contrast::calcContrastFromMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec,String name) {
	Color c;
	String file;
	int count1,count2;
	double contrast=0;
	String color1,color2;
	double hue1,hue2;
	vector<double> vec;
	vector< vector<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-1); j++) {
			color1 = windowVec.at(i).at(j);
			color2 = windowVec.at(i).at(j+1);
			hue1 = hueVec.at(i).at(j);
			hue2 = hueVec.at(i).at(j+1);
			count1 = c.countColors(color1);
			count2 = c.countColors(color2);
			if((count1+count2)==5) {
				if(count1==3)
					color1=windowVec.at(i).at(j-1);
				else
					color2=windowVec.at(i).at(j+2);
			}
			contrast = calcContrast(hue1, hue2, color1, color2);
			vec.push_back(contrast);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	file = name + "Contrast";
	writeSeq2File(vec2,file);
	vector<double>().swap(vec);
	vector< vector<double> >().swap(vec2);
}
