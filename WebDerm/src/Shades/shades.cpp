/*
 * shades.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: jason
 */

#include "shades.h"
#include "../hsl/hsl.h"
#include "../functions.h"
#include "../rgb/rgb.h"

deque<String> Shades::g_Shades;
deque< deque<double> > Shades::g_ShadeThresh;

//! g_Shades2 is for combined shades
deque<String> Shades::g_Shades2;
deque< deque<double> > Shades::g_ShadeThresh2;
bool Shades::THRESH_IMPORTED = false;

Shades::Shades() {
	if(!this->THRESH_IMPORTED)
		this->THRESH_IMPORTED = this->importThresholds();
}

bool Shades::importThresholds() {
	if(!this->THRESH_IMPORTED) {
		String folderName = "Thresholds/";
		String filename = folderName+"shade-thresholds.csv";
		String filename2 = folderName+"shade-thresholds2.csv";
		fstream fsThresh(filename.c_str());
		fstream fsThresh2(filename2.c_str());
		if(fsThresh.is_open() && fsThresh2.is_open()) {
			String temp;
			deque<String> vec;
			deque<double> thresh;
			getline(fsThresh,temp);
			while(getline(fsThresh,temp)) {
				ip::getSubstr(temp,',',vec);
				for(unsigned int i=0; i<vec.size(); i++) {
					if(i==0) {
						g_Shades.push_back(vec.at(i));
					}
					if(i>=1 && i<=2)
						thresh.push_back(atof(vec.at(i).c_str()));
				}
				g_ShadeThresh.push_back(thresh);
				thresh.clear(); vec.clear();
			}
			getline(fsThresh2,temp);
			while(getline(fsThresh2,temp)) {
				ip::getSubstr(temp,',',vec);
				for(unsigned int i=0; i<vec.size(); i++) {
					if(i==0) {
						g_Shades2.push_back(vec.at(i));
					}
					if(i>=1 && i<=2)
						thresh.push_back(atof(vec.at(i).c_str()));
				}
				g_ShadeThresh2.push_back(thresh);
				thresh.clear(); vec.clear();
			}
			fsThresh.close();
			fsThresh2.close();
			deque<double>().swap(thresh);
			return true;
		}
		else {
			cout << "Importing Shade Thresholds Failed!" << endl;
			return false;
		}
	}
	return true;
}

int Shades::getShadeCount() {
	return g_Shades.size();
}

/** extracts number at the end of shade only **/
int Shades::extractShadeLevel(String shade) {
	String substr;
	substr = shade.substr(shade.length()-1,1);
	return atoi(substr.c_str());
}

String Shades::getShade(int index) {
	int shadeCount = g_Shades.size();
	int ind=index;
	if(ind<0) ind=0;
	if(ind>(shadeCount-1)) ind=(shadeCount-1);
	return g_Shades.at(ind);
}

int Shades::getShadeIndex(String shade) {
	unsigned int index=0;
	int shadeCount = g_Shades.size();
	for(int i=0; i<shadeCount; i++) {
		if(shade==getShade(i)) {
			index=i;
			break;
		}
	}
	return index;
}

String Shades::extractShade(String pix) {
	int shadeCount = getShadeCount();
	//int shadeCount = sh.getShadeCount();
	String shade = "";
	if(pix=="Zero") return pix;
	if(pix.find("White")!=string::npos) return "White";
	if(pix.find("Black")!=string::npos) return getShade(0);
	//if(pix.find("Gray")!=string::npos) return "Gray";
	for(int i=0; i<shadeCount; i++) {
		shade = getShade(i);
		if(pix.find(shade)!=string::npos)
			break;
	}
	return shade;
}

String Shades::calcShade(double intensity) {
	for(unsigned int i=0; i<g_ShadeThresh.size(); i++) {
		if(intensity<g_ShadeThresh.at(i).at(1) && intensity>=g_ShadeThresh.at(i).at(0))
			return g_Shades.at(i);
	}
	return "NONE";
}

String Shades::calcShade2(double intensity) {
	for(unsigned int i=0; i<g_ShadeThresh2.size(); i++) {
		if(intensity<g_ShadeThresh2.at(i).at(1) && intensity>=g_ShadeThresh2.at(i).at(0))
			return g_Shades2.at(i);
	}
	return "NONE";
}

void Shades::release_memory() {
	g_Shades.clear();
	g_ShadeThresh.clear();
	g_Shades2.clear();
	g_ShadeThresh2.clear();
	g_Shades.shrink_to_fit();
	g_ShadeThresh.shrink_to_fit();
	g_Shades2.shrink_to_fit();
	g_ShadeThresh2.shrink_to_fit();
}

//compares shade1 with shade2 and returns difference
//if diff is <0 -> shade1 is darker, >0 -> shade1 is lighter
int Shades::shadeDifference(String shade1, String shade2) {
	int shadeIndex1 = this->getShadeIndex(shade1);
	int shadeIndex2 = this->getShadeIndex(shade2);
	int diff = shadeIndex1 - shadeIndex2;
	return diff;
}

//return index for g_Shades2
int Shades::getShadeIndex2(String shade) {
	unsigned int index=0;
	int shadeCount = g_Shades2.size();
	for(int i=0; i<shadeCount; i++) {
		if(shade==getShade2(i)) {
			index=i;
			break;
		}
	}
	return index;
}

String Shades::getShade2(int index) {
	int shadeCount = g_Shades2.size();
	int ind=index;
	if(ind<0) ind=0;
	if(ind>(shadeCount-1)) ind=(shadeCount-1);
	return g_Shades2.at(ind);
}

//custom function for combing shades that might look the same
String Shades::combineShades(String shade) {
	if(shade.find("Dark2")!=string::npos || shade.find("Dark1")!=string::npos)
		return "Dark2";
	if(shade.find("Dark3")!=string::npos) return "Dark3";
	if(shade.find("High")!=string::npos) return "High";
	if(shade.find("Low")!=string::npos) return "Low";
	if(shade.find("Light")!=string::npos) return "Low";
	return shade;
}
