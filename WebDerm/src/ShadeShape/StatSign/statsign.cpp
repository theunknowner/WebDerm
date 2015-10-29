/*
 * statsign.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: jason
 */

#include "statsign.h"

//! creates a statistical signature of a NN3 40x40 image
//! vector size is 81
//! pos[0] = total
//! pos[1] -> pos[40] = White
//! pos[41] -> pos[80] = Black
vector<int> StatSign::create(Mat img) {
	vector<int> statSignVec(81,0.0);
	for(int y=0; y<img.rows; y++) {
		int countConsecWhite = 0;
		int countConsecBlack = 0;
		int entered=0;
		for(int x=0; x<img.cols; x++) {
			if(img.at<uchar>(y,x)>0 && entered==-1) {
				entered=1;
				countConsecWhite++;
				statSignVec.at((countConsecBlack)+40)++;
				countConsecBlack=0;
			} else if(img.at<uchar>(y,x)>0) {
				entered=1;
				countConsecWhite++;
			} else if(img.at<uchar>(y,x)==0 && entered==1) {
				entered=-1;
				countConsecBlack++;
				statSignVec.at(countConsecWhite)++;
				countConsecWhite=0;
			} else if(img.at<uchar>(y,x)==0 && entered!=0) {
				entered=-1;
				countConsecBlack++;
			}
		}
	}
	this->adjustValues(statSignVec);
	statSignVec.at(0) = std::accumulate(statSignVec.begin(),statSignVec.end(),0);
	return statSignVec;
}

void StatSign::adjustValues(vector<int>& statSignVec) {
	for(unsigned int i=1; i<statSignVec.size(); i++) {
		int mul = i>40 ? (i-40) : i;
		statSignVec.at(i) *= mul;
	}
}

//! scheme 1 for comparing statistical signature
float StatSign::dotProduct(vector<int> statSignVec1, vector<int> statSignVec2) {
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	if(*max_element(statSignVec1.begin(),statSignVec1.end())==0 || *max_element(statSignVec2.begin(),statSignVec2.end())==0) {
		return -1.0;
	}
	float numerSum = 0.0;
	float denomSumUP = 0.0;
	float denomSumDB = 0.0;
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		numerSum += (statSignVec1.at(i) * statSignVec2.at(i));
		denomSumUP += pow(statSignVec1.at(i),2);
		denomSumDB += pow(statSignVec2.at(i),2);
	}
	float denomSum = sqrt(denomSumUP) * sqrt(denomSumDB);
	float results = numerSum / denomSum;
	return results;
}

//! scheme 2 for comparing statistical signature
float StatSign::proportion(vector<int> statSignVec1, vector<int> statSignVec2) {
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	float totalWhite1 = 0.0, totalWhite2=0., totalBlack1=0., totalBlack2=0.;
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		if(i>40) {
			totalBlack1 += statSignVec1.at(i);
			totalBlack2 += statSignVec2.at(i);
		} else {
			totalWhite1 += statSignVec1.at(i);
			totalWhite2 += statSignVec2.at(i);
		}
	}
	float propWhite = min(totalWhite1,totalWhite2) / max(totalWhite1,totalWhite2);
	float propBlack = min(totalBlack1,totalBlack2) / max(totalBlack1,totalBlack2);
	if(std::isnan(propWhite)) propWhite = 0.0;
	if(std::isnan(propBlack)) propBlack = 0.0;
	float result = propWhite * propBlack;
	return result;
}

void StatSign::printCompare(vector<int> statSignVec1, vector<int> statSignVec2) {
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		float porp1 = (float)statSignVec1.at(i) / statSignVec1.at(0);
		float porp2 = (float)statSignVec2.at(i) / statSignVec2.at(0);
		try {
			printf("L%d: %d(%f) | L%d: %d(%f)\n",i,statSignVec1.at(i),porp1,i,statSignVec2.at(i),porp2);
		} catch(const std::out_of_range &oor) {
			printf("statSignVec1.size(): %lu\n",statSignVec1.size());
			printf("statSignVec2.size(): %lu\n",statSignVec2.size());
			printf("i: %d\n",i);
			exit(1);
		}
	}
	printf("Total: %d | Total: %d\n",statSignVec1.at(0), statSignVec2.at(0));
}

void StatSign::writeCompare(String name, vector<int> statSignVec1, vector<int> statSignVec2) {
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	FILE * fp;
	fp = fopen(name.c_str(),"w");
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		float porp1 = (float)statSignVec1.at(i) / statSignVec1.at(0);
		float porp2 = (float)statSignVec2.at(i) / statSignVec2.at(0);
		fprintf(fp,"L%d,%d,%f,L%d,%d,%f\n",i,statSignVec1.at(i),porp1,i,statSignVec2.at(i),porp2);
	}
	fprintf(fp,"Total,%d,Total,%d\n",statSignVec1.at(0), statSignVec2.at(0));
}
