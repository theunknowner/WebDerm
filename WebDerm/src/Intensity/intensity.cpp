/*
 * intensity.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#include "intensity.h"

double Intensity::calcIntensity(String color) {
	rgb rgb;
	double colorLevel[mainColors.size()];
	double cHue[mainColors.size()];
	double colorIntensity[mainColors.size()];
	fill_n(colorLevel,mainColors.size(),0);
	fill_n(cHue,mainColors.size(),0);
	fill_n(colorIntensity,mainColors.size(),0);
	double totalLevel=0;
	double totalColorIntensity=0;
	vector<int> index;
	vector<double> vec,vec2;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		if(color.find(mainColors.at(i))!=string::npos) {
			colorLevel[i] = rgb.getColorLevel(color,mainColors.at(i));
			index.push_back(i);
			if(mainColors.at(i)!="Gray") {
				totalLevel+=colorLevel[i];
			}
			cHue[i] = colorFactors.at(i);
		}
	}
	for(unsigned int i=0; i<index.size(); i++) {
		colorIntensity[index.at(i)] = colorLevel[index.at(i)] * cHue[index.at(i)];
		if(mainColors.at(index.at(i))!="Gray") {
			colorIntensity[index.at(i)] *= (colorLevel[index.at(i)]/totalLevel);
		}
		totalColorIntensity += colorIntensity[index.at(i)];
	}
	return totalColorIntensity;
}

void Intensity::setMaxIntensityOfFeature(vector<double> &feature) {
	min = 0;
	max = 1;
	for(unsigned int i=0; i<feature.size(); i++) {
		if(feature.at(i)>max && feature.at(i)!=900) {
			max = feature.at(i);
		}
	}
	range = max - min;
	range /= max;
}

String Intensity::getShade(double inten) {
	String shade;
	min=0;
	max=722;
	range = max-min;
	range /=max;
	double thresh1 = range*0.20;
	double thresh2 = range*0.755;
	double intensity = inten/max;
	intensity = pow(intensity,2);
	if(intensity<=thresh1)
		shade = "Light";
	if(intensity>thresh1 && intensity<=thresh2)
		shade = "";
	if(intensity>thresh2)
		shade = "Dark";

	return shade;
}

void Intensity::writeNormalizedIntensityMatrix(vector< vector<double> > vec,String name) {
	double intensity=0;
	vector<double> iVec1;
	vector< vector<double> > iVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			intensity = vec.at(i).at(j)/max;
			intensity = pow(intensity,2);
			iVec1.push_back(intensity);
		}
		iVec2.push_back(iVec1);
		iVec1.clear();
	}
	String filename = name+"NormIntensity2x2";
	writeSeq2File(iVec2,filename);
	vector<double>().swap(iVec1);
	vector< vector<double> >().swap(iVec2);
}

void Intensity::writeIntensityFromMatrix(vector <vector<String> > &windowVec, String name) {
	String file;
	double colorIntensity=0;
	String color;
	vector<double> vec;
	vector< vector<double> > vec2;
	max=0;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-1); j++) {
			color = windowVec.at(i).at(j);
			colorIntensity = calcIntensity(color);
			if(colorIntensity>max && colorIntensity!=900) max = colorIntensity;
			vec.push_back(colorIntensity);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	file = name + "ColorIntense2x2";
	writeSeq2File(vec2,file);
	writeNormalizedIntensityMatrix(vec2,name);
	vector<double>().swap(vec);
	vector< vector<double> >().swap(vec2);
}

void Intensity::writeMainColorMatrix(vector< vector<String> > &windowVec, String name) {
	int flag=0;
	double colorIntensity=0;
	Point pt; //pointer to hold x,y of color window
	String color;
	vector<double> vec;
	vector< vector<double> > vec2;
	vector<Point> ptVec1; //pointerVec to hold all pointers
	vector <vector<Point> > ptVec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-2); j++) {
			color = windowVec.at(i).at(j);
				pt.x = j;
				pt.y = i;
				colorIntensity = calcIntensity(color);
				if(colorIntensity>max && colorIntensity!=900) max = colorIntensity;
				vec.push_back(colorIntensity);
				ptVec1.push_back(pt);
		}
		if(flag==0) {
			ptVec2.push_back(ptVec1);
			vec2.push_back(vec);
		}
		flag=0;
		ptVec1.clear();
		vec.clear();
	}
	Color c;
	int x=0,y=0;
	String pix;
	String shade;
	vector<String> strVec;
	vector< vector<String> > strVec2;
	for(unsigned int i=0; i<ptVec2.size(); i++) {
		for(unsigned int j=0; j<ptVec2.at(i).size(); j++) {
			x = ptVec2.at(i).at(j).x;
			y = ptVec2.at(i).at(j).y;
			pix = windowVec.at(y).at(x);
			pix = c.getMainColor(pix);
			shade = getShade(vec2.at(i).at(j));
			if(pix!="Black" && pix!="White")
				pix = shade + pix;
			pix += "("+toString(x+1) + ";" + toString(y+1)+")";
			strVec.push_back(pix);
		}
		strVec2.push_back(strVec);
		strVec.clear();
	}
	String filename = name +"MainColors";
	writeSeq2File(strVec2,filename);
	vector<double>().swap(vec);
	vector< vector<double> >().swap(vec2);
	vector<String>().swap(strVec);
	vector< vector<String> >().swap(strVec2);
	vector<Point>().swap(ptVec1);
	vector< vector<Point> >().swap(ptVec2);
}
