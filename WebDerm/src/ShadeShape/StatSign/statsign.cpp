/*
 * statsign.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: jason
 */

#include "statsign.h"

int StatSign::urnSize;

//! creates a statistical signature of a NN3 40x40 image
//! vector size is 81
//! pos[0] = total
//! pos[1] -> pos[40] = White
//! pos[41] -> pos[80] = Black
vector<float> StatSign::create(Mat img, float relArea) {
	vector<float> statSignVec(urnSize1,0);
	vector<float> statSignVec2(urnSize2,0); //> 80/5 = 16
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
	for(unsigned int i=1; i<statSignVec.size(); i++) {
		int mul = i>40 ? (i-40) : i;
//		mul = ceil(mul/5.0);
//		statSignVec.at(i) *= pow(2.5,mul) * relArea;
//		int urnNum = ceil(i/5.0);
//		statSignVec2.at(urnNum) += statSignVec.at(i);
		statSignVec.at(i) *=mul;
	}
	statSignVec.at(0) = std::accumulate(statSignVec.begin(),statSignVec.end(),0.0);
	statSignVec2.at(0) = std::accumulate(statSignVec2.begin(),statSignVec2.end(),0.0);
	StatSign::urnSize = statSignVec.size();
	return statSignVec;
}

//! scheme 1 for comparing statistical signature
float StatSign::dotProduct(vector<float> statSignVec1, vector<float> statSignVec2) {
	assert(statSignVec1.size()>0);
	assert(statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	if(statSignVec1.at(0)==0 || statSignVec2.at(0)==0) {
		return 0.0;
	}
	vector<float> statSignVecF1(statSignVec1.size(),0.0);
	vector<float> statSignVecF2(statSignVec2.size(),0.0);
	vector<float> statSignVecF11(statSignVec1.size(),0.0);
	vector<float> statSignVecF22(statSignVec2.size(),0.0);
	float sumOfMaxes = 0.0;
	//> sum of the max of the urns
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		sumOfMaxes += max(statSignVec1.at(i),statSignVec2.at(i));
	}
	//> relative proportions of the balls
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		statSignVecF1.at(i) = statSignVec1.at(i) / statSignVec1.at(0);
		statSignVecF2.at(i) = statSignVec2.at(i) / statSignVec2.at(0);
		statSignVecF11.at(i) = statSignVec1.at(i) / sumOfMaxes;
		statSignVecF22.at(i) = statSignVec2.at(i) / sumOfMaxes;
	}
	float numerSum = 0.0;
	float denomSumUP = 0.0;
	float denomSumDB = 0.0;
	for(unsigned int i=1; i<statSignVecF1.size(); i++) {
		numerSum += (statSignVecF11.at(i) * statSignVecF22.at(i));
		denomSumUP += pow(statSignVecF1.at(i),2);
		denomSumDB += pow(statSignVecF2.at(i),2);
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
	if(std::isnan(propWhite)) propWhite = 1.0;
	if(std::isnan(propBlack)) propBlack = 1.0;
	float result = propWhite * propBlack;
	return result;
}

//! adjust the score using inverse tan-hyperbolic function
float StatSign::adjustValue(float value) {
	float pMark = 0.65; //> percentage mark where values gets pushed up or down
	float result = value + (1.5*atanh(value - pMark)) * value;
	float upperLimit = 0.9;
	float lowerLimit = 0.0;
	result = min(result,upperLimit);
	result = max(result,lowerLimit);
	return result;
}

int StatSign::getUrnSize() {
	return StatSign::urnSize;
}

void StatSign::print(vector<float> statSignVec) {
	assert(statSignVec.size()>0);
	for(unsigned int i=1; i<statSignVec.size(); i++) {
		float porp = statSignVec.at(i) / statSignVec.at(0);
		try {
			printf("L%d: %0.f(%f)\n",i,statSignVec.at(i),porp);
		} catch(const std::out_of_range &oor) {
			printf("statSignVec.size(): %lu\n",statSignVec.size());
			printf("i: %d\n",i);
			exit(1);
		}
	}
	printf("Total: %0.f\n",statSignVec.at(0));
}

void StatSign::printCompare(vector<float> statSignVec1, vector<float> statSignVec2) {
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	//> sum of the max of the urns
	float sumOfMaxes = 0.0;
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		sumOfMaxes += max(statSignVec1.at(i),statSignVec2.at(i));
	}
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		float porp1 = statSignVec1.at(i) / statSignVec1.at(0);
		float porp2 = statSignVec2.at(i) / statSignVec2.at(0);
		float porp11 = statSignVec1.at(i) / sumOfMaxes;
		float porp22 = statSignVec2.at(i) / sumOfMaxes;
		try {
			int urn = i>40 ? i-40 : i;
			printf("L%d: %0.f(%f)(%f) | L%d: %0.f(%f)(%f)\n",urn,statSignVec1.at(i),porp1,porp11,urn,statSignVec2.at(i),porp2,porp22);
			fflush(stdout);
		} catch(const std::out_of_range &oor) {
			printf("statSignVec1.size(): %lu\n",statSignVec1.size());
			printf("statSignVec2.size(): %lu\n",statSignVec2.size());
			printf("i: %d\n",i);
			exit(1);
		}
	}
	printf("Total: %0.f | Total: %0.f\n",statSignVec1.at(0), statSignVec2.at(0));
	printf("SumOfMaxes: %0.f\n",sumOfMaxes);
}

void StatSign::writeCompare(String name, vector<float> statSignVec1, vector<float> statSignVec2) {
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	FILE * fp;
	fp = fopen(name.c_str(),"w");
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		float porp1 = (float)statSignVec1.at(i) / statSignVec1.at(0);
		float porp2 = (float)statSignVec2.at(i) / statSignVec2.at(0);
		fprintf(fp,"L%d,%0.f,%f,L%d,%0.f,%f\n",i,statSignVec1.at(i),porp1,i,statSignVec2.at(i),porp2);
	}
	fprintf(fp,"Total,%0.f,Total,%0.f\n",statSignVec1.at(0), statSignVec2.at(0));
}
