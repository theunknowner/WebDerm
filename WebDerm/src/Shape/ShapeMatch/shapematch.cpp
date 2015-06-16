/*
 * shapematch.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#include "shapematch.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

bool ShapeMatch::importThresholds() {
	ifstream ifs("/home/jason/git/Thresholds/Thresholds/shape-match-thresholds.csv");
	if(ifs.is_open()) {
		String temp;
		deque<String> vec;
		vector<float> thresh;
		getline(ifs,temp); // removes frst row headers
		while(getline(ifs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i>0) {
					thresh.push_back(atof(vec.at(i).c_str()));
				}
			}
			this->shapeMatchThresh.push_back(thresh);
			thresh.clear();
			vec.clear();
		}
	}
}

void ShapeMatch::printThresholds() {
	for(unsigned int i=0; i<this->shapeMatchThresh.size(); i++) {
		for(int j=0; j<this->shapeMatchThresh.at(i).size(); j++) {
			printf("%f, ",shapeMatchThresh.at(i).at(j));
		}
		printf("\n");
	}
}
