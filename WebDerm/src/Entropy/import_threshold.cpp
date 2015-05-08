/*
 * import_threshold.cpp
 *
 *  Created on: May 8, 2015
 *      Author: jason
 */

#include "entropy.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

bool Entropy::importEntropyThresholds() {
	String folderName = "Thresholds/";
	String filename = folderName+"entropy-thresholds.csv";
	fstream fsThresh(filename.c_str());
	if(fsThresh.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fsThresh,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==1) {
					this->colorWeights2.push_back(atof(vec.at(i).c_str()));
				}
				if(i==2) {
					this->colorWeights.push_back(atof(vec.at(i).c_str()));
				}
			}
			vec.clear();
		}
		fsThresh.close();
		deque<String>().swap(vec);
		return true;
	}
	else {
		cout << "Importing Entropy Thresholds Failed!" << endl;
		return false;
	}
}

void Entropy::releaseMemory() {
	this->colorWeights.clear();
	deque<double>().swap(this->colorWeights);
}
