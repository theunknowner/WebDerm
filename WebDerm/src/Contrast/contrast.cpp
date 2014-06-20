/*
 * contrast.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#include "contrast.h"

double contrast::calcContrastAngle(double hue1, double hue2, double colorfn1, double colorfn2) {
	const double c = 65.0;
	double fColor1 = c * hue1;
	double fColor2 = c * hue2;
	double angle=0;
	double top=0, bottom=0;
	double root = 1/2.5;
	top = abs((fColor1*colorfn2) - (colorfn1*fColor2));
	bottom = sqrt(pow(fColor1,2)+pow(colorfn1,2)) * sqrt(pow(fColor2,2)+pow(colorfn2,2));
	angle = top/bottom;
	angle = pow(angle,root);
	return angle;
}

deque< deque<double> > contrast::calcContrastFromMatrix(deque< deque<double> > &intensityVec) {
	Intensity in;
	double contrast=0, intensity=0;
	double minIntensity = in.getMinIntensity();
	double maxIntensity = in.getMaxIntensity();
	double rangeIntensity = maxIntensity-minIntensity;
	double flag=0;
	deque<double> vec1;
	deque< deque<double> > vec2;
	for(unsigned int i=0; i<intensityVec.size(); i++) {
		for(unsigned int j=0; j<intensityVec.at(i).size(); j++) {
			intensity = intensityVec.at(i).at(j);
			intensity = (intensity*rangeIntensity)+minIntensity;
			if(intensity<900 && flag!=0) {//no black color
				contrast = intensityVec.at(i).at(j) - intensityVec.at(i).at(j-1);
			}
			if(intensity<900 && flag==0) {
				contrast=0;
				flag=1;
			}
			if(intensity>=900 && flag!=0) { //no black color in between other colors
				contrast=0;
				flag=0;
			}
			vec1.push_back(contrast);
		}
		flag=0;
		contrast=0;
		vec2.push_back(vec1);
		vec1.clear();
	}
	return vec2;
}

deque< deque<double> > contrast::calcCumulativeContrast(deque< deque<double> > &vec) {
	double contrast=0;
	deque<double>	contrastVec1;
	deque< deque<double> > contrastVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			contrast += vec.at(i).at(j);
			contrastVec1.push_back(contrast);
		}
		contrastVec2.push_back(contrastVec1);
		contrastVec1.clear();
		contrast=0;
	}
	return contrastVec2;
}

void contrast::writeCumulativeContrast(deque< deque<double> > &vec, String name) {
	deque< deque<double> > conVec;
	conVec = calcCumulativeContrast(vec);
	String filename = name + "CumulContrast";
	writeSeq2File(conVec,filename);
	deque< deque<double> >().swap(conVec);
}
