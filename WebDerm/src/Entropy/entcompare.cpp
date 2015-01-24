/*
 * compare.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: jason
 */

#include "entropy.h"

void Entropy::resetThreshVals() {
	Y_HIGH = 49.0;
	Y_LOW = 15.0;
	S_HIGH = 0.30;
	S_LOW = 0.10;
	V_HIGH = 0.10;
	V_LOW = 0.04;
	Y_THRESH = 0.70;
	S_THRESH = 0.70;
	V_THRESH = 0.70;
	Y_DIST = 7;
	S_DIST = 0.05;
	V_DIST = 0.03;
	Y_PERCEPTION = 5.0;
	S_PERCEPTION = 0.045;
	Y_LARGE_THRESH = 85.0;
	distPass = 1;
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

double Entropy::fn_compareY(double y1, double y2, double weight) {
	double val;
	if((y1>Y_HIGH && y2>Y_HIGH)) {
		val = min(y1,y2)/max(y1,y2);
		return val;
	}
	else if((y1<=Y_LOW && y2<=Y_LOW)) {
		val = max(y1,y2)-min(y1,y2);
		if(val<=Y_DIST) {
			if(y1>=Y_PERCEPTION && y2>=Y_PERCEPTION)
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

double Entropy::fn_compareS(double s1, double s2, double weight) {
	double val;
	if((s1>S_HIGH && s2>S_HIGH)) {
		val = min(s1,s2)/max(s1,s2);
		return val;
	}
	else if((s1<=S_LOW && s2<=S_LOW)) {
		val = max(s1,s2)-min(s1,s2);
		if(val<=S_DIST) {
			if(Y1>=Y_PERCEPTION && Y2>=Y_PERCEPTION)
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

double Entropy::fn_compareV(double v1, double v2, double weight) {
	double val=0;
	if((v1<=V_HIGH && v2<=V_HIGH)) {
		val = max(v1,v2)-min(v1,v2);
		if(val<=V_DIST) {
			if(Y1>=Y_PERCEPTION && Y2>=Y_PERCEPTION)
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

double Entropy::fn_compareT(deque<double> t1, deque<double> t2, double weight) {
	if(t1.size()!=t2.size()) {
		cout << "Entropy::compareT() t1.size != t2.size" << endl;
		exit(1);
	}
	/*
	if(t1.at(0)<0 && t1.at(1)>0 && t2.at(0)<0 && t2.at(1>0)) {
		if(t1.at(0)<t1.at(1) && t2.at(0)<t2.at(1))
			return -1;
	}
	double minVal=-1., maxVal=1.;
	double dist=0, val=0;
	for(unsigned int i=0; i<t1.size(); i++) {
		double val1 = (t1.at(i)-minVal)/(maxVal-minVal); //normalize t1 from -1:1 -> 0:1
		double val2 = (t2.at(i)-minVal)/(maxVal-minVal); //normalize t2 from -1:1 -> 0:1
		if(i==1) {
			val1 = 1. - val1;
			val2 = 1. - val2;
		}
		val = min(val1,val2)/max(val1,val2);
		dist += val;
	}
	dist /= t1.size();
	return dist;*/
	double max1=0, max2=0;
	unsigned int idx1=0, idx2=0;
	for(unsigned int i=0; i<t1.size(); i++) {
		if(t1.at(i)>max1) {
			max1 = t1.at(i);
			idx1 = i;
		}
		if(t2.at(i)>max2) {
			max2 = t1.at(i);
			idx2 = i;
		}
	}
	int t1Shape=0, t2Shape=0;
	t1Shape = idx1<(t1.size()-1) ? 0:1;
	t2Shape = idx2<(t2.size()-1) ? 0:1;
	if(t1Shape==t2Shape)
		return 1.0;
	else
		return 0.0;
}


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
			getSubstr(temp,',',vec);
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

double Entropy::compareYSV(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	deque<double> resultVec(vec1.size(),0);
	const int ysvSize = 3; //y,s,v
	const int tSize = 5; // t1,t2...tn
	double ysv1[ysvSize];
	double ysv2[ysvSize];
	deque<double> t1(tSize,0);
	deque<double> t2(tSize,0);
	double valY=0, valS=0, valV=0, valT=0;
	double avg=0, total=0;
	int colorsHit[vec1.size()]; //variable to mark colors not ignored
	for(unsigned int i=0; i<vec1.size(); i++) {
		this->resetThreshVals();
		if(colorNameVec.at(i)!="LowBrown" && colorNameVec.at(i)!="LowGreyBrown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				if(i==0 && j>=ysvSize) {
					t1.at(j-ysvSize) = vec1.at(i).at(j);
					t2.at(j-ysvSize) = vec2.at(i).at(j);
				}
				else {
					ysv1[j] = vec1.at(i).at(j);
					ysv2[j] = vec2.at(i).at(j);
				}
			}
			Y1 = ysv1[0];
			Y2 = ysv2[0];
			if(ysv1[1]>S_PERCEPTION || ysv2[1]>S_PERCEPTION)
				Y_PERCEPTION = 2.0;

			if(ysv1[0]>Y_PERCEPTION || ysv2[0]>Y_PERCEPTION) {
				//Total Population(Y) comparison
				valY = this->fn_compareY(ysv1[0],ysv2[0],1);

				//Avg Density(S) comparison
				valS = this->fn_compareS(ysv1[1],ysv2[1],1);

				//Variability(V) comparison
				valV = this->fn_compareV(ysv1[2],ysv2[2],1);

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
	//preparing the moving weights
	double weightTotal=0;
	int totalColorsHit=0;
	double normWeights[this->colorWeights.size()];
	double normSignifWeight[this->colorWeights.size()];
	for(unsigned int i=0; i<this->colorWeights.size(); i++) {
		if(colorsHit[i]==1) {

			weightTotal += this->colorWeights.at(i);
			totalColorsHit++;
		}
	}

	double colorPercent=0;
	deque<double> newColorWeights = this->colorWeights;
	double newWeightTotal=0; //reset for new adjusted weightTotal
	for(unsigned int i=0; i<this->colorWeights.size(); i++) {
		if(colorsHit[i]==1) {
			ysv1[0] = vec1.at(i).at(0);
			ysv2[0] = vec2.at(i).at(0);
			colorPercent = max(ysv1[0],ysv2[0])/total;
			double weightAdjustFactor = weightTotal*colorPercent;
			newColorWeights.at(i) = this->colorWeights.at(i)*weightAdjustFactor;
			newWeightTotal += newColorWeights.at(i);
		}
	}
	for(unsigned int i=0; i<this->colorWeights.size(); i++) {
		if(colorsHit[i]==1) {
			normWeights[i] = newColorWeights.at(i)/newWeightTotal;
			normSignifWeight[i] = normWeights[i] * totalColorsHit;
		}
	}
	double colorSignif[this->colorWeights.size()];
	double newTotal=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		ysv1[0] = vec1.at(i).at(0);
		ysv2[0] = vec2.at(i).at(0);
		if(colorsHit[i]==1) {
			colorSignif[i] = max(ysv1[0],ysv2[0]) * normSignifWeight[i];
			newTotal += colorSignif[i];
		}
	}

	double sum=0, results=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		if(colorsHit[i]==1) {
			ysv1[0] = vec1.at(i).at(0);
			ysv2[0] = vec2.at(i).at(0);
			sum = colorSignif[i]/newTotal;
			sum *= resultVec.at(i);
			//printf("%s : %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],resultVec.at(i));
			results += sum;
		}
	}
	//valT = this->fn_compareT(t1,t2,1);
	//printf("ShapeDist: %f, Before_YsvDist: %f\n",valT,results);
	//if(valT==0.0) {
	//	results = 0.0;
	//}
	//cout << "Mine: " << results << endl;
	return results;
}

double Entropy::compareT(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	const int ysvSize = 3; //y,s,v
	const int tSize = 5; // t1,t2...tn
	deque<double> t1(tSize,0);
	deque<double> t2(tSize,0);
	double valT=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		this->resetThreshVals();
		if(colorNameVec.at(i)!="LowBrown" && colorNameVec.at(i)!="LowGreyBrown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				if(i==0 && j>=ysvSize) {
					t1.at(j-ysvSize) = vec1.at(i).at(j);
					t2.at(j-ysvSize) = vec2.at(i).at(j);
				}
			}
		}
	}
	valT = this->fn_compareT(t1,t2,1);
	return valT;
}

double Entropy::compareEntropy2(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	const int ysvSize = 3; //y,s,v
	const int tSize = 2; //t1,t2
	int colorComponents = vec1.size();
	double distYSV[colorComponents];
	fill_n(distYSV,colorComponents,-1);
	double sum=0, total=0, totalYSV[ysvSize];
	double sumT=0, totalT=0, distT=0;
	double ysv1[ysvSize], ysv2[ysvSize];
	double t1[tSize], t2[tSize];
	double ysvWeights[ysvSize]; //weight of importance of YSV
	fill_n(ysvWeights,ysvSize,1.0);
	int colorsHit[colorComponents];

	for(unsigned int i=0; i<vec1.size(); i++) {
		if(colorNameVec.at(i)!="LowBrown"&& colorNameVec.at(i)!="LowGreyBrown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				if(i==0 && j>=ysvSize) {
					t1[j] = vec1.at(i).at(j);
					t2[j] = vec2.at(i).at(j);
					totalT = pow(t1[j]-t2[j],2)/(pow(t1[j],2)+pow(t2[j],2));
					sumT += totalT;
				}
				else {
					ysv1[j] = vec1.at(i).at(j);
					ysv2[j] = vec2.at(i).at(j);
					if(ysv1[j]>0 || ysv2[j]>0) {
						totalYSV[j] = ysvWeights[j]*(pow(ysv1[j]-ysv2[j],2)/(pow(ysv1[j],2)+pow(ysv2[j],2)));
						sum += totalYSV[j];
					}
				}
			}
			if(i==0) {
				distT = sqrt(sum) / sqrt(tSize);
			}
			total += max(ysv1[0],ysv2[0]);
			if(ysv1[0]>0 || ysv2[0]>0) {
				distYSV[i] = sqrt(sum) / sqrt(3);
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
				sum = (max(ysv1[0],ysv2[0])/total) * 1-distYSV[i] * this->colorWeights2.at(i);
				avgDist +=sum;
			}
			printf("%s: %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],distYSV[i]);
		}
	}
	//avgDist = 1 - min(avgDist,1.0);
	avgDist = (avgDist/2) + (distT/2);
	return avgDist;
	//printf("Dr.Dube: %f\n",avgDist);
}
