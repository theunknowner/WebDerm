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

double Esg::calculate(float dist, int shadeDiff) {
	//this->esgVal = pow(shadeDiff+1,4.0) / (ceil(dist/4.0) + 2.0);
	//const float p00 = 3.654;
	const float p10 = 0.01354;
	const float p01 = 0.08377;
	const float p20 = -0.0001186;
	const float p11 = -0.0007105;
	const float p02 = -0.0003207;
	const float p21 = -3.733e-07;
	const float p12 = 2.295e-06;
	const float p03 = 2.434e-07;
	vector<float> distVec;
	for(unsigned int i=0; i<this->esgVec.size(); i++) {
		float eucDist = MyMath::euclideanDist(dist,shadeDiff,this->esgVec.at(i).at(0),this->esgVec.at(i).at(1));
		distVec.push_back(eucDist);
	}
	int smallestIndex1 = Func::smallest(distVec,1);
	float x = dist;
	float y = shadeDiff;
	float x0 = this->esgVec.at(smallestIndex1).at(0);
	float y0 = this->esgVec.at(smallestIndex1).at(1);
	float z0 = this->esgVec.at(smallestIndex1).at(3);
	float del_zx = p10 + p11*y0 + 2*p20*x0 + 2*p21*x0*y0* + p12*y0*y0;
	float del_zy = p01 + p11*x0 + 2*p02*y0 + p21*x0*x0 + 2*p12*x0*y0 + 3*p03*y0*y0;
	float z = z0 + del_zx*(x-x0) + del_zy*(y-y0);
	//printf("X0,Y0,Z0: %f,%f,%f\n",x0,y0,z0);
	//printf("Esg: %e\n",this->esgVec.at(smallestIndex1).at(2));
	//printf("Z: %f\n",z);

	this->esgVal = pow(10,z);
	this->avgDist = dist;
	return this->esgVal;
}
