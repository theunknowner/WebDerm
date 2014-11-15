/*
 * compare.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: jason
 */

#include "entropy.h"

double Y_HIGH;
double Y_LOW;
double S_HIGH;
double S_LOW;
double V_HIGH;
double V_LOW;
double Y_THRESH;
double S_THRESH;
double V_THRESH;
double Y_DIST;
double S_DIST;
double V_DIST;
double Y_PERCEPTION;
double S_PERCEPTION;
double Y_LARGE_THRESH;
double threshPass, threshFail;
double distPass, distFail;

void resetThreshVals() {
	Y_HIGH = 49.0;
	Y_LOW = 15.0;
	S_HIGH = 0.30;
	S_LOW = 0.10;
	V_HIGH = 0.10;
	V_LOW = 0.04;
	Y_THRESH = 0.50;
	S_THRESH = 0.35;
	V_THRESH = 0.25;
	Y_DIST = 7;
	S_DIST = 0.05;
	V_DIST = 0.03;
	Y_PERCEPTION = 5.0;
	S_PERCEPTION = 0.045;
	Y_LARGE_THRESH = 85.0;
	threshPass = 1, threshFail = -1;
	distPass = 2, distFail = -2;
}

double compareY(double y1, double y2) {
	double val;
	double yThresh = Y_THRESH*1.3;
	if((y1>Y_HIGH && y2>Y_HIGH)) {
		val = (max(y1,y2)-min(y1,y2))/min(y1,y2);
		if(val<=yThresh)
			return threshPass;
		else
			return threshFail;
	}
	else if((y1<=Y_LOW && y2<=Y_LOW)) {
		val = max(y1,y2)-min(y1,y2);
		if(val<=Y_DIST)
			return distPass;
		else
			return distFail;
	}
	else {
		val = (max(y1,y2)-min(y1,y2))/min(y1,y2);
		if(val<=Y_THRESH)
			return threshPass;
		else
			return threshFail;
	}
}

double compareS(double s1, double s2) {
	double val;
	double sThresh = S_THRESH*1.3;
	if((s1>S_HIGH && s2>S_HIGH)) {
		val = (max(s1,s2)-min(s1,s2))/min(s1,s2);
		if(val<=sThresh)
			return threshPass;
		else
			return threshFail;
	}
	else if((s1<=S_LOW && s2<=S_LOW)) {
		val = max(s1,s2)-min(s1,s2);
		if(val<=S_DIST)
			return distPass;
		else
			return distFail;
	}
	else {
		val = (max(s1,s2)-min(s1,s2))/min(s1,s2);
		if(val<=S_THRESH)
			return threshPass;
		else
			return threshFail;
	}
}

double compareV(double v1, double v2) {
	double val=0;
	if((v1<=V_HIGH && v2<=V_HIGH)) {
		val = max(v1,v2)-min(v1,v2);
		if(val<=V_DIST)
			return distPass;
		else
			return distFail;
	}
	else {
		val = (max(v1,v2)-min(v1,v2))/min(v1,v2);
		if(val<=V_THRESH)
			return threshPass;
		else
			return threshFail;
	}
}

bool Entropy::loadEntropyFiles(String filepath, deque<deque<double> > &dataMat) {
	fstream fs(filepath.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		deque<double> thresh;
		getline(fs,temp);
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			//vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(0,vec.at(i).length());
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

deque<int> Entropy::compareEntropy(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<deque<double> > &matchVec) {
	matchVec.resize(vec1.size(),deque<double>(vec1.at(0).size(),0));
	deque<int> resultVec(vec1.size(),0);
	double ysv1[3];
	double ysv2[3];
	double valY=0, valS=0, valV=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		resetThreshVals();
		for(unsigned int j=0; j<vec1.at(i).size(); j++) {
			ysv1[j] = vec1.at(i).at(j);
			ysv2[j] = vec2.at(i).at(j);
		}
		if(ysv1[1]>S_PERCEPTION || ysv2[1]>S_PERCEPTION)
			Y_PERCEPTION = 2.0;

		if(ysv1[0]>Y_PERCEPTION || ysv2[0]>Y_PERCEPTION) {
			//Total Population(Y) comparison
			valY = compareY(ysv1[0],ysv2[0]);
			matchVec.at(i).at(0) = valY;

			//Avg Density(S) comparison
			valS = compareS(ysv1[1],ysv2[1]);
			matchVec.at(i).at(1) = valS;

			//Variability(V) comparison
			valV = compareV(ysv1[2],ysv2[2]);
			matchVec.at(i).at(2) = valV;

			//Recompute Y if S & V are same
			if(valS>0 && valV>0 && valY<0) {
				Y_THRESH = 0.65;
				Y_DIST = 10;
				valY = compareY(ysv1[0],ysv2[0]);
				matchVec.at(i).at(0) = valY;
			}

			if(valY>0 && valS>0 && valV>0)
				resultVec.at(i) = 1;
			else if(ysv1[0]>Y_LARGE_THRESH && ysv2[0]>Y_LARGE_THRESH)
				resultVec.at(i) = 1;
		}
	}
	return resultVec;
}

void Entropy::compareEntropy2(deque<deque<double> > vec1, deque<deque<double> > vec2) {
	double dist[vec1.size()];
	fill_n(dist,vec1.size(),-1);
	double sum=0, total=0, totalYSV[3];
	double ysv1[3], ysv2[3];
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec1.at(i).size(); j++) {
			ysv1[j] = vec1.at(i).at(j);
			ysv2[j] = vec2.at(i).at(j);
			if(ysv1[j]>0 || ysv2[j]>0) {
				totalYSV[j] = (pow(ysv1[j]-ysv2[j],2)/(pow(ysv1[j],2)+pow(ysv2[j],2)));
				sum += totalYSV[j];
				if(i==1)
					cout << totalYSV[j] << endl;
			}
		}
		total += max(ysv1[0],ysv2[0]);
		if(ysv1[0]>0 || ysv2[0]>0)
			dist[i] = sqrt(sum) * (1/sqrt(3));

		sum=0;
	}
	double avgDist=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		ysv1[0] = vec1.at(i).at(0);
		ysv2[0] = vec2.at(i).at(0);
		//cout << dist[i] << endl;
		if(ysv1[0]>0 || ysv2[0]>0) {
			sum = (max(ysv1[0],ysv2[0])/total) * dist[i];
			avgDist +=sum;
		}
	}
	cout << avgDist << endl;
}
