/*
 * esgVal.cpp
 *
 *  Created on: Nov 18, 2015
 *      Author: jason
 */

#include "esg.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"

vector<vector<float> > Esg::esgVec;
bool Esg::THRESH_IMPORTED = false;

Esg::Esg() {
	if(!this->THRESH_IMPORTED)
		this->THRESH_IMPORTED = this->importThresholds();
}

bool Esg::importThresholds() {
	if(!this->THRESH_IMPORTED) {
		fstream fs("Thresholds/esg_contrast_values.csv");
		if(fs.is_open()) {
			String temp;
			deque<String> vec;
			vector<float> contrastVec;
			while(getline(fs,temp)) {
				getSubstr(temp,',',vec);
				for(unsigned int i=0; i<vec.size(); i++) {
					contrastVec.push_back(atof(vec.at(i).c_str()));
				}
				this->esgVec.push_back(contrastVec);
				contrastVec.clear();
			}
			fs.close();
			return true;
		} else {
			cout << "Importing ESG Contrast Values Failed!" << endl;
			return false;
		}
	}
	return true;
}

int Esg::shadeDiff(int shade1, int shade2) {
	return abs(shade1-shade2);
}

float Esg::calculate(float dist, int shadeDiff) {
	//float esg = pow(shadeDiff+1,4.0) / (ceil(dist/4.0) + 2.0);
	vector<float> distVec;
	for(unsigned int i=0; i<this->esgVec.size(); i++) {
		float eucDist = MyMath::euclideanDist(dist,shadeDiff,this->esgVec.at(i).at(0),this->esgVec.at(i).at(1));
		distVec.push_back(eucDist);
	}
	int smallestIndex1 = Func::smallest(distVec,1);
	int smallestIndex2 = Func::smallest(distVec,2);
	float smallX0 = this->esgVec.at(smallestIndex1).at(0);
	float smallY0 = this->esgVec.at(smallestIndex1).at(1);
	float smallX1 = this->esgVec.at(smallestIndex2).at(0);
	float smallY1 = this->esgVec.at(smallestIndex2).at(1);
	int rank = 3;
	while(smallX0==smallX1 || smallY0==smallY1) {
		smallestIndex2 = Func::smallest(distVec,rank);
		smallX1 = this->esgVec.at(smallestIndex2).at(0);
		smallY1 = this->esgVec.at(smallestIndex2).at(1);
		rank++;
	}
	float x = dist;
	float y = shadeDiff;
	float z0 = this->esgVec.at(smallestIndex1).at(3);
	float x0 = smallX0;
	float y0 = smallY0;
	float x1 = this->esgVec.at(smallestIndex2).at(0);
	float y1 = this->esgVec.at(smallestIndex2).at(1);
	float z1 = this->esgVec.at(smallestIndex2).at(3);
	float z = z0 + ((z1-z0)/(x1-x0))*(x-x0) + ((z1-z0)/(y1-y0))*(y-y0);
	printf("X0,Y0,Z0: %f,%f,%f\n",smallX0,smallY0,z0);
	printf("X1,Y1,Z1: %f,%f,%f\n",smallX1,smallY1,z1);
	printf("Z: %f\n",z);
	this->esgVal = pow(10,z);
	this->avgDist = dist;
	return this->esgVal;
}
