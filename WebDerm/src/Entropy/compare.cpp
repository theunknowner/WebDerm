/*
 * compare.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: jason
 */

#include "entropy.h"

double Y_HIGH;	//upper Y range boundary
double Y_LOW;	//lower Y range boudnary
double S_HIGH;
double S_LOW;
double V_HIGH;
double V_LOW;
double * Y_THRESH;	//% threshold Y must meet for special conditions
double * S_THRESH;
double * V_THRESH;
double * Y_DIST;	//distance threshold Y must meet for special conditions
double * S_DIST;
double * V_DIST;
double * Y_PERCEPTION;	//threshold in which the eyes starts noticing color
double * S_PERCEPTION;
double Y_LARGE_THRESH;
double distPass;
deque<double> colorWeights; //holds the weights for color impact
double Y1, Y2;

void resetThreshVals(int size) {
	Y_HIGH = 49.0;
	Y_LOW = 15.0;
	S_HIGH = 0.30;
	S_LOW = 0.10;
	V_HIGH = 0.10;
	V_LOW = 0.04;
	Y_THRESH = new double[size];
	S_THRESH = new double[size];
	V_THRESH = new double[size];
	Y_DIST = new double[size];
	S_DIST = new double[size];
	V_DIST = new double[size];
	Y_PERCEPTION = new double[size];
	S_PERCEPTION = new double[size];
	Y_LARGE_THRESH = 85.0;
	distPass = 1;
	for(int i=0; i<size; i++) {
		Y_THRESH[i] = 0.70;
		S_THRESH[i] = 0.70;
		V_THRESH[i] = 0.70;
		Y_DIST[i] = 7.0;
		S_DIST[i] = 0.05;
		V_DIST[i] = 0.03;
		Y_PERCEPTION[i] = 5.0;
		S_PERCEPTION[i] = 0.045;
	}
}

void deletePointers() {
	delete[] Y_THRESH;
	delete[] S_THRESH;
	delete[] V_THRESH;
	delete[] Y_DIST;
	delete[] S_DIST;
	delete[] V_DIST;
	delete[] Y_PERCEPTION;
	delete[] S_PERCEPTION;
}

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
				if(i==1)
					colorWeights.push_back(atof(vec.at(i).c_str()));
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
	colorWeights.clear();
	deque<double>().swap(colorWeights);
}

double compareY(double y1, double y2, int index, int componentSize) {
	double val;
	if((y1>Y_HIGH && y2>Y_HIGH)) {
		val = min(y1,y2)/max(y1,y2);
		return val;
	}
	else if((y1<=Y_LOW && y2<=Y_LOW)) {
		val = max(y1,y2)-min(y1,y2);
		if(val<=Y_DIST[index/componentSize]) {
			if(y1>=Y_PERCEPTION[index/componentSize] && y2>=Y_PERCEPTION[index/componentSize])
				return distPass;
			else
				return min(y1,y2)/max(y1,y2);
		}
		else
			return min(y1,y2)/max(y1,y2);
	}
	else {
		val = min(y1,y2)/max(y1,y2);
		return val;
	}
}

double compareS(double s1, double s2, int index, int componentSize) {
	double val;
	if((s1>S_HIGH && s2>S_HIGH)) {
		val = min(s1,s2)/max(s1,s2);
		return val;
	}
	else if((s1<=S_LOW && s2<=S_LOW)) {
		val = max(s1,s2)-min(s1,s2);
		if(val<=S_DIST[index/componentSize]) {
			if(Y1>=Y_PERCEPTION[index/componentSize] && Y2>=Y_PERCEPTION[index/componentSize])
				return distPass;
			else
				return min(s1,s2)/max(s1,s2);
		}
		else
			return min(s1,s2)/max(s1,s2);
	}
	else {
		val = min(s1,s2)/max(s1,s2);
		return val;
	}
}

double compareV(double v1, double v2, int index, int componentSize) {
	double val=0;
	if((v1<=V_HIGH && v2<=V_HIGH)) {
		val = max(v1,v2)-min(v1,v2);
		if(val<=V_DIST[index/componentSize]) {
			if(Y1>=Y_PERCEPTION[index/componentSize] && Y2>=Y_PERCEPTION[index/componentSize])
				return distPass;
			else
				return min(v1,v2)/max(v1,v2);
		}
		else
			return min(v1,v2)/max(v1,v2);
	}
	else {
		val = min(v1,v2)/max(v1,v2);
		return val;
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
			if(vec.at(vec.size()-1)=="")
				vec.pop_back(); //removes the weird empty space at the end of deque
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

double Entropy::compareEntropy(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<deque<double> > &matchVec) {
	int componentSize = 3; //3 components -> 1)Y 2)S 3)V
	int shadeComponents = vec1.at(0).size()/componentSize;
	int colorComponents = vec1.size();
	matchVec.resize(vec1.size(),deque<double>(vec1.at(0).size(),0));
	deque<deque<double> > resultVec(colorComponents,deque<double>(shadeComponents,0));
	double ysv1[vec1.at(0).size()];
	double ysv2[vec2.at(0).size()];
	double valYSV[vec1.at(0).size()];
	double avg=0;
	double total=0;
	int colorsHit[colorComponents][shadeComponents]; //variable to mark colors not ignored

	for(unsigned int i=0; i<vec1.size(); i++) {
		resetThreshVals(vec1.at(i).size()/componentSize); //divide componentSize for YSV components
		if(allColors.at(i)!="Brown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
			}
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				int indexY = j;
				int indexS = j+1;
				int indexV = j+2;
				if(j%componentSize==0) {
					Y1 = ysv1[j];
					Y2 = ysv2[j];
				}
				if(j%componentSize==0) {
					if(ysv1[indexS]>S_PERCEPTION[indexS/componentSize] || ysv2[indexS]>S_PERCEPTION[indexS/componentSize])
						Y_PERCEPTION[indexS/componentSize] = 2.0;

					if(ysv1[j]>Y_PERCEPTION[j/componentSize] || ysv2[j]>Y_PERCEPTION[j/componentSize]) {
						//Total Population(Y) comparison
						valYSV[indexY] = compareY(ysv1[indexY],ysv2[indexY],indexY,componentSize);
						matchVec.at(i).at(indexY) = valYSV[indexY];

						//Avg Density(S) comparison
						valYSV[indexS] = compareS(ysv1[indexS],ysv2[indexS],indexS,componentSize);
						matchVec.at(i).at(indexS) = valYSV[indexS];

						//Variability(V) comparison
						valYSV[indexV] = compareV(ysv1[indexV],ysv2[indexV],indexV,componentSize);
						matchVec.at(i).at(indexV) = valYSV[indexV];

						//Reassign Y && Y_THRESH if S & V are the same
						if(valYSV[indexY]<Y_THRESH[indexY/componentSize] && valYSV[indexS]>=S_THRESH[indexS/componentSize] && valYSV[indexV]>=V_THRESH[indexV/componentSize]) {
							Y_THRESH[indexY/componentSize] *= 0.8;
							Y_DIST[indexY/componentSize] = 10;
							valYSV[indexY]*=1.3;
						}

						//Reassign V if Y & S are different
						if(valYSV[indexY]<Y_THRESH[indexY/componentSize] && valYSV[indexS]<S_THRESH[indexS/componentSize]) {
							double diff = valYSV[indexY]/Y_THRESH[indexY/componentSize];
							valYSV[indexV] *= diff;
						}

						//Average value of YSV matching
						avg = valYSV[indexY] + valYSV[indexS] + valYSV[indexV];
						avg /= (double)componentSize;
						resultVec.at(i).at(indexY/componentSize) += avg;

						//Total of all the Y that are noticeable
						total += max(ysv1[indexY],ysv2[indexY]);
						colorsHit[i][indexY/componentSize] = 1;
					}
				}
			}
		}
		deletePointers();
	}

	//calculating image similarity results
	double sum;
	double results;
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec1.at(i).size(); j++) {
			if(j%componentSize==0) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
				if(colorsHit[i][j/componentSize]==1) {
					sum = max(ysv1[j],ysv2[j])/total;
					resultVec.at(i).at(j/componentSize) *= sum;
					results += resultVec.at(i).at(j/componentSize);
				}
			}
		}
	}
	//cout << "Mine: " << results << endl;
	return results;
}

double Entropy::compareEntropy2(deque<deque<double> > vec1, deque<deque<double> > vec2) {
	int componentSize = 3; //3 components -> 1)Y 2)S 3)V
	int shadeComponents = vec1.at(0).size()/componentSize;
	int colorComponents = vec1.size();
	double dist[colorComponents];
	fill_n(dist,colorComponents,-1);
	double sum=0, total=0, totalYSV;
	double ysv1[vec1.at(0).size()];
	double ysv2[vec2.at(0).size()];
	double ysvWeights[3];
	fill_n(ysvWeights,3,1.0);
	int colorsHit[colorComponents];
	for(unsigned int i=0; i<vec1.size(); i++) {
		if(allColors.at(i)!="Brown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
				if(ysv1[j]>0 || ysv2[j]>0) {
					totalYSV = ysvWeights[j]*(pow(ysv1[j]-ysv2[j],2)/(pow(ysv1[j],2)+pow(ysv2[j],2)));
					sum += totalYSV;
				}
			}
			total += max(ysv1[0],ysv2[0]);
			if(ysv1[0]>0 || ysv2[0]>0)
				dist[i] = sqrt(sum) / sqrt(3);
			colorsHit[i] = 1;
		}
		sum=0;
	}
	double avgDist=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		if(colorsHit[i]==1) {
			ysv1[0] = vec1.at(i).at(0);
			ysv2[0] = vec2.at(i).at(0);
			if(ysv1[0]>0 || ysv2[0]>0) {
				sum = (max(ysv1[0],ysv2[0])/total) * dist[i];
				//printf("%s: %f\n",allColors.at(i).c_str(),dist[i]);
				avgDist +=sum;
			}
		}
	}
	avgDist = 1 - min(avgDist,1.0);
	return avgDist;
	//printf("Dr.Dube: %f\n",avgDist);
}
