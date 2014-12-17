/*
 * compareold.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: jason
 */

#include "entropy.h"

bool Entropy::importEntropyThresholdsOld() {
	String folderName = "Thresholds/";
	String filename = folderName+"entropy-thresholds-old.csv";
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

double Entropy::compareEntropyOld(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	deque<double> resultVec(vec1.size(),0);
	double ysv1[3];
	double ysv2[3];
	double valY=0, valS=0, valV=0;
	double avg=0, total=0;
	int colorsHit[vec1.size()]; //variable to mark colors not ignored

	for(unsigned int i=0; i<vec1.size(); i++) {
		this->resetThreshVals();
		if(colorNameVec.at(i)!="LowBrown" && colorNameVec.at(i)!="LowGreyBrown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
			}
			Y1 = ysv1[0];
			Y2 = ysv2[0];
			if(ysv1[1]>S_PERCEPTION || ysv2[1]>S_PERCEPTION)
				Y_PERCEPTION = 2.0;

			if(ysv1[0]>Y_PERCEPTION || ysv2[0]>Y_PERCEPTION) {
				//Total Population(Y) comparison
				valY = this->compareY(ysv1[0],ysv2[0],1);

				//Avg Density(S) comparison
				valS = this->compareS(ysv1[1],ysv2[1],1);

				//Variability(V) comparison
				valV = this->compareV(ysv1[2],ysv2[2],1);

				//Reassign Y && Y_THRESH if S & V are the same
				if(valS>=S_THRESH && valV>=V_THRESH && valY<Y_THRESH) {
					Y_THRESH *= 0.8;
					Y_DIST = 10;
					valY *= 1.3;
				}

				//Reassign V if Y & S are different
				if(valY<Y_THRESH && valS<S_THRESH) {
					double diff = valY/Y_THRESH;
					valV *= diff;
				}

				//Average value of YSV matching
				avg = (valY + valS + valV);
				avg /= 3.0;
				resultVec.at(i) = avg;


				//printf("%s : %f,%f,%f\n",colorNameVec.at(i).c_str(),valY,valS,valV);

				//Total of all the Y that are noticeable
				total += max(ysv1[0],ysv2[0]);
				colorsHit[i] = 1;
			}
		}
	}

	//calculating image similarity results
	double sum=0, results=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		ysv1[0] = vec1.at(i).at(0);
		ysv2[0] = vec2.at(i).at(0);
		if(colorsHit[i]==1) {
			sum = max(ysv1[0],ysv2[0])/total;
			sum *= resultVec.at(i);
			printf("%s : %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],resultVec.at(i));
			results += sum;
		}
	}
	//cout << "Mine: " << results << endl;
	return results;
}

double Entropy::compareEntropy2Old(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	int componentSize = 3;
	int colorComponents = vec1.size();
	double dist[colorComponents];
	fill_n(dist,colorComponents,-1);
	double sum=0, total=0, totalYSV[componentSize];
	double ysv1[componentSize], ysv2[componentSize];
	double ysvWeights[componentSize]; //weight of importance of YSV
	fill_n(ysvWeights,componentSize,1.0);
	int colorsHit[colorComponents];

	for(unsigned int i=0; i<vec1.size(); i++) {
		if(colorNameVec.at(i)!="LowBrown"&& colorNameVec.at(i)!="LowGreyBrown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
				if(ysv1[j]>0 || ysv2[j]>0) {
					totalYSV[j] = ysvWeights[j]*(pow(ysv1[j]-ysv2[j],2)/(pow(ysv1[j],2)+pow(ysv2[j],2)));
					sum += totalYSV[j];
				}
			}
			total += max(ysv1[0],ysv2[0]);
			if(ysv1[0]>0 || ysv2[0]>0) {
				dist[i] = sqrt(sum) / sqrt(3);
				colorsHit[i] = 1;
			}
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
				//printf("%d) %s: %f\n",i,colorNameVec.at(i).c_str(),sum);
				avgDist +=sum;
			}
			printf("%s: %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],dist[i]);
		}
	}
	avgDist = 1 - min(avgDist,1.0);
	return avgDist;
	//printf("Dr.Dube: %f\n",avgDist);
}

