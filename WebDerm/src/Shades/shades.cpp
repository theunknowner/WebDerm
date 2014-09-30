/*
 * shades.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: jason
 */

#include "shades.h"

deque<String> g_Shades;
deque< deque<double> > g_ShadeThresh;

deque<String> g_Shades2;
deque< deque<double> > g_ShadeThresh2;

bool Shades::importThresholds() {
	String folderName = path+"Thresholds/";
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
			getSubstr(temp,',',vec);
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
			getSubstr(temp,',',vec);
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

void Shades::release_memory() {
	g_Shades.clear();
	g_ShadeThresh.clear();
	deque<String>().swap(g_Shades);
	deque< deque<double> >().swap(g_ShadeThresh);
}

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
