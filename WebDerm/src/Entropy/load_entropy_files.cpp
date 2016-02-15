/*
 * load_entropy_files.cpp
 *
 *  Created on: May 8, 2015
 *      Author: jason
 */

#include "entropy.h"
#include "../functions.h"

bool Entropy::loadEntropyFiles(String filepath, deque<deque<double> > &dataMat, deque<String> &colorNameVec) {
	fstream fs(filepath.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		deque<double> thresh;
		dataMat.clear();
		colorNameVec.clear();
		getline(fs,temp);
		while(getline(fs,temp)) {
			ip::getSubstr(temp,',',vec);
			if(vec.at(vec.size()-1)=="")
				vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==0)
					colorNameVec.push_back(vec.at(i));
				if(i>0)
					thresh.push_back(atof(vec.at(i).c_str()));
			}
			dataMat.push_back(thresh);
			thresh.clear();
			vec.clear();
		}
		deque<String>().swap(vec);
		deque<double>().swap(thresh);
		fs.close();
		return true;
	}
	else
		printf("Failed to load Entropy file!\n");
	return false;
}


