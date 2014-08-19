/*
 * shades.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: jason
 */

#include "shades.h"

deque<String> g_Shades;
deque< deque<double> > g_ShadeThresh;

bool Shades::importThresholds() {
	String folderName = path+"Thresholds/";
	String filename = folderName+"shade-thresholds.csv";
	fstream fsThresh(filename.c_str());
	if(fsThresh.is_open()) {
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
		fsThresh.close();
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

void Shades::release_memory() {
	deque<String>().swap(g_Shades);
	deque< deque<double> >().swap(g_ShadeThresh);
}