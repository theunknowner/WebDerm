/*
 * compare.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: jason
 */

#include "entropy.h"
#include "../functions.h"

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

double Entropy::fn_compareY(double y1, double y2, double weight) {
	double val;
	if(min(y1,y2)==0 && max(y1,y2)>0)
		return 0;
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
	if(min(s1,s2)==0 && max(s1,s2)>0)
		return 0;
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
	if(min(v1,v2)==0 && max(v1,v2)>0)
		return 0;
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
	double interval = 0.33; //this interval is the scale interval for shapeMetric
	unsigned int idx1=0, idx2=0;
	double val1=t1.at(0), val2=t2.at(0);

	double result=-1.0;
	if(val1==val2 && max(val1,val2)==0)
		result = -0.5; //matched randoms
	else if(val1==val2)
		result = 1.0; //matched circles
	else {
		double val = round((max(val1,val2)-min(val1,val2))/interval);
		if(min(val1,val2)!=0) {
			if(val<=1.0)
				result = 1.0; //matched circles 1 interval apart
			else
				result = 0.0; //matched circles 2 intervals apart
		}
	}
	return result;
}

double Entropy::compareYSV(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	deque<double> resultVec(vec1.size(),0);
	const int ysvSize = 3; //y,s,v
	const int tSize = 1; // t1,t2...tn
	double ysv1[ysvSize];
	double ysv2[ysvSize];
	deque<double> t1(tSize,0);
	deque<double> t2(tSize,0);
	double valY=0, valS=0, valV=0;
	double avg=0, total=0;
	vector<int> colorsHit(vec1.size(),0); //variable to mark colors not ignored
	for(unsigned int i=0; i<vec1.size(); i++) {
		this->resetThreshVals();
		String colorName = colorNameVec.at(i);
		if(colorName!="LowBrown" && colorName!="LowGreyBrown" && colorName!="HighBrown" && colorName!="HighGreyBrown") {
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
			if(ysv1[1]>S_PERCEPTION || ysv2[1]>S_PERCEPTION) {
				Y1 *= (ysv1[1] / S_PERCEPTION);
				Y2 *= (ysv2[1] / S_PERCEPTION);
				//Y_PERCEPTION /= (max(ysv1[1],ysv2[1])/S_PERCEPTION);
				//printf("%s: Y1: %f | Y2: %f\n",colorName.c_str(),Y1,Y2);
				Y_PERCEPTION = 2.0;
			}

			if(Y1>Y_PERCEPTION || Y2>Y_PERCEPTION) {
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
					//valY *= 1.3;
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
				colorsHit.at(i) = 1;
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
			results += sum;

			if(this->debugMode)
				printf("%s : %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],resultVec.at(i));
		}
	}
	//cout << "Mine: " << results << endl;
	if(this->debugMode)
		printf("Total: %f, NewTotal: %f\n",total,newTotal);
	return results;
}

double Entropy::compareT(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	const int ysvSize = 3; //y,s,v
	const int tSize = 1; // t1,t2...tn
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
	int componentSize = 3;
	int colorComponents = vec1.size();
	double dist[colorComponents];
	fill_n(dist,colorComponents,-1);
	double sum=0, total=0, totalYSV[componentSize];
	double ysv1[componentSize], ysv2[componentSize];
	Mat ysvWeights(componentSize,1,CV_32F,Scalar(1.0)); //weight of importance of YSV
	int colorsHit[colorComponents];
	fill_n(colorsHit,colorComponents,0);
	double maxYSV = 400.0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		String colorName = colorNameVec.at(i);
		if(colorName!="LowBrown" && colorName!="LowGreyBrown" && colorName!="HighBrown" && colorName!="HighGreyBrown") {
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
				if(ysv1[0]>0 || ysv2[0]>0) {
					if(j<componentSize) {
						double val = abs(ysv1[j]-ysv2[j]);
						if(j==0)
							totalYSV[j] = ysvWeights.at<float>(j,0)*(val/maxYSV);
						else
							totalYSV[j] = ysvWeights.at<float>(j,0)*val;
						//printf("(%f,%f,%f,%f,%f)",ysv1[j],ysv2[j],totalYSV[j],val,maxYSV);
						sum += totalYSV[j];
					}
				}
			}
			if(ysv1[0]>0 || ysv2[0]>0) {
				dist[i] = sum/3.0;
				colorsHit[i] = 1;
				printf("%s: (%f),(%f),(%f),%f,%f\n",colorName.c_str(),totalYSV[0],totalYSV[1],totalYSV[2],dist[i],sum);
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
				sum = dist[i] * this->colorWeights2.at(i);
				avgDist +=sum;
			}
			if(this->debugMode)
				printf("%s: %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],dist[i]);
		}
	}
	//avgDist = min(avgDist,1.0);
	//avgDist = 1.0 - avgDist;
	//cout << "Total:" << total << endl;
	return avgDist;
	//printf("Dr.Dube: %f\n",avgDist);
}

// test function for 2nd scheme of compareEntropy2
double Entropy::test_compareEntropy2a(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec) {
	int componentSize = 3;
	int colorComponents = vec1.size();
	double dist[colorComponents];
	fill_n(dist,colorComponents,-1);
	double sum=0, total=0, totalYSV[componentSize];
	double ysv1[componentSize], ysv2[componentSize];
	vector<float> ysvWeights = {1.0,1.5,1.0};
	int colorsHit[colorComponents];
	fill_n(colorsHit,colorComponents,0);
	double maxY = 0, yTotal1=0, yTotal2=0;
	// maxY = 196 - Y occupied by Brown
	for(unsigned int i=0; i<vec1.size(); i++) {
		String colorName = colorNameVec.at(i);
		if(colorName!="LowBrown" && colorName!="LowGreyBrown" && colorName!="HighBrown" && colorName!="HighGreyBrown") {
			double yVal1 = vec1.at(i).at(0);
			double yVal2 = vec2.at(i).at(0);
			yTotal1 += yVal1;
			yTotal2 += yVal2;
		}
	}
	maxY = max(yTotal1,yTotal2);
	for(unsigned int i=0; i<vec1.size(); i++) {
		String colorName = colorNameVec.at(i);
		if(colorName!="LowBrown" && colorName!="LowGreyBrown" && colorName!="HighBrown" && colorName!="HighGreyBrown") {
			/*
			if(yTotal1>yTotal2)
				vec2.at(i).at(0) *= (max(yTotal1,yTotal2)/min(yTotal1,yTotal2));
			else if(yTotal2>yTotal1)
				vec1.at(i).at(0) *= (max(yTotal1,yTotal2)/min(yTotal1,yTotal2));
			*/
			for(unsigned int j=0; j<vec1.at(i).size(); j++) {
				ysv1[j] = vec1.at(i).at(j);
				ysv2[j] = vec2.at(i).at(j);
				if(ysv1[0]>0 || ysv2[0]>0) {
					if(j<componentSize) {
						double val = abs(ysv1[j]-ysv2[j]);
						double maxVal = max(ysv1[j],ysv2[j]);
						totalYSV[j] = ysvWeights.at(j)*(val/maxVal);
						if(maxVal==0) totalYSV[j] = 0;
						sum += totalYSV[j];
					}
				}
			}
			//if(this->debugMode)
				//printf("%s: (%f,%f,%f),%f\n",colorName.c_str(),totalYSV[0],totalYSV[1],totalYSV[2],maxY);
			if(ysv1[0]>0 || ysv2[0]>0) {
				dist[i] = sum/3.0;
				colorsHit[i] = 1;
				if(this->debugMode)
					printf("%s: (%f),(%f),(%f),%f,%f\n",colorName.c_str(),totalYSV[0],totalYSV[1],totalYSV[2],dist[i],maxY);
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
				sum = dist[i] * this->colorWeights2.at(i) * (max(ysv1[0],ysv2[0])/maxY);
				avgDist +=sum;
			}
			if(this->debugMode)
				printf("%s: %f [%f][%f](%f)\n",colorNameVec.at(i).c_str(),sum,ysv1[0],ysv2[0],dist[i]);
		}
	}
	//printf("Dr.Dube: %f\n",avgDist);
	avgDist = 1.0 - min(avgDist,1.0);
	return avgDist;
}
