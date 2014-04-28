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

void Intensity::setMinMax(double intensity) {
	static int flag=0;
	if(flag==0) {
		min=900;
		max=0;
		flag=1;
	}
	if(intensity>max && intensity!=900)
		max = intensity;
	if(intensity<min)
		min = intensity;

	range = max-min;
	range /= range;
}

String Intensity::getShade(double inten) {
	String shade;
	shadeArr = {"Light","","Dark"};
	double thresh1 = range*0.25;
	double thresh2 = range*0.75;
	double intensity = inten/max;
	intensity = pow(intensity,power);
	if(intensity<=thresh1)
		shade = shadeArr[0];
	if(intensity>thresh1 && intensity<=thresh2)
		shade = shadeArr[1];
	if(intensity>thresh2)
		shade = shadeArr[2];

	return shade;
}

vector< vector<double> > Intensity::calcNormalizedIntensityMatrix(vector< vector<double> > vec) {
	double intensity=0;
	vector<double> iVec1;
	vector< vector<double> > iVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			intensity = (vec.at(i).at(j)-min)/range;
			intensity = pow(intensity,power);
			iVec1.push_back(intensity);
		}
		iVec2.push_back(iVec1);
		iVec1.clear();
	}
	return iVec2;
}

vector< vector<double> > Intensity::calcIntensityMatrix(vector <vector<String> > &windowVec) {
	double colorIntensity=0;
	String color;
	vector<double> vec;
	vector< vector<double> > vec2;
	max=0;
	min=900;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<windowVec.at(i).size(); j++) {
			color = windowVec.at(i).at(j);
			colorIntensity = calcIntensity(color);
			setMinMax(colorIntensity);
			vec.push_back(colorIntensity);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	return vec2;
}

vector< vector<String> > Intensity::calcMainColorMatrix(vector< vector<String> > &windowVec) {
	contrast con;
	Color c;
	String pix, shade;
	vector< vector<double> > intensityVec;
	vector< vector<double> > normIntensityVec;
	vector< vector<double> > contrastVec;
	vector< vector<double> > cumConVec;
	vector< vector<String> > colorVec;
	intensityVec = calcIntensityMatrix(windowVec);
	normIntensityVec = calcNormalizedIntensityMatrix(intensityVec);
	contrastVec = con.calcContrastFromMatrix(normIntensityVec);
	cumConVec = con.calcCumulativeContrast(contrastVec);
	for(unsigned int i=0; i<cumConVec.size(); i++) {
		for(unsigned int j=0; j<cumConVec.at(i).size(); j++) {
			pix = windowVec.at(i).at(j);
			pix = c.getMainColor(pix);
			shade = getShade(normIntensityVec.at(i).at(j));
			if(cumConVec.at(i).at(j)>0.1) {

			}
		}
	}
}

void Intensity::writeNormalizedIntensityMatrix(vector< vector<double> > &vec, String name) {
	vector< vector<double> > normVec;
	normVec = calcNormalizedIntensityMatrix(vec);
	String filename = name + "NormIntensity";
	writeSeq2File(normVec,filename);
	vector< vector<double> >().swap(normVec);
}

void Intensity::writeIntensityMatrix(vector< vector<String> > &windowVec, String name) {
	vector< vector<double> > intVec;
	intVec = calcIntensityMatrix(windowVec);
	String filename = name + "ColorIntensity";
	writeSeq2File(intVec,filename);
	vector< vector<double> >().swap(intVec);
}

void Intensity::writeMainColorMatrix(vector< vector<String> > &windowVec, String name) {
	vector< vector<String> > colorVec;
	colorVec = calcMainColorMatrix(windowVec);
	String filename = name + "MainColors";
	writeSeq2File(colorVec,filename);
	vector< vector<String> >().swap(colorVec);
}
