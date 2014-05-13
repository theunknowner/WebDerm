/*
 * maths.cpp
 *
 *  Created on: Feb 27, 2014
 *      Author: jason
 */

#include "maths.h"

double correlationDist(double * vec1, double * vec2)
{
	int size=3;
	double total1=0, total2=0;
	double top=0, bottom1=0,bottom2=0;
	double result=0;
	for(int i=0; i<size; i++)
	{
		total1 -= vec1[i];
		total2 -= vec2[i];
	}
	for(int i=0; i<size; i++)
	{
		top = top + (vec1[i]+(total1/3))*(vec2[i]+(total2/3));
		bottom1 += pow(abs(vec1[i]+(total1/3)),2);
		bottom2 += pow(abs(vec2[i]+(total2/3)),2);
	}
	if(top!=0 && bottom1!=0 && bottom2!=0)
		result = 1 - (top/(sqrt(bottom1)*sqrt(bottom2)));
	else
		return -1;

	return result;
}

double cosineDist(double * vec1, double * vec2)
{
	int size=2;
	double top=0, bottom1=0,bottom2=0;
	double result=0;
	for(int i=0; i<size; i++)
	{
		top += vec1[i]*vec2[i];
		bottom1 += pow(abs(vec1[i]),2);
		bottom2 += pow(abs(vec2[i]),2);
	}
	result = 1 - (top/(sqrt(bottom1)*sqrt(bottom2)));
	return result;
}

double normalizeToRange(double value, int min, int max) {
	double result=0;
	int range = max-min;
	result = (value * range) + min;
	return result;
}

double myRound(double value) {
	double result=trunc(value);
	result=value-result;
	if(abs(result)>0.79999999) {
		result=round(value);
	}
	else result=trunc(value);
	return result;
}

double medianAbsDev(deque<double> &vec) {
	double median=0,val=0;
	deque<double> vec1;
	median = medianUnsorted(vec);
	for(unsigned int i=0; i<vec.size(); i++) {
		val = abs(vec.at(i)-median);
		vec1.push_back(val);
	}
	median = medianUnsorted(vec1);
	return median;
}

double meanAbsDev(deque<double> &vec) {
	double avg=0,val=0;
	deque<double> vec1;
	avg = mean(vec);
	for(unsigned int i=0; i<vec.size(); i++) {
		val = abs(vec.at(i)-avg);
		vec1.push_back(val);
	}
	avg = mean(vec1);
	return avg;
}

//standardDev with sorting
double standardDevSort(deque<double> &vec) {
	double avg=0,var=0;
	quicksort(vec,0,vec.size()-1);
	double min = vec.at(0);
	double max = vec.at(vec.size()-1);
	avg = (min+max)/2;
	//avg = mean(vec);
	for(unsigned int i=0; i<vec.size(); i++) {
		var += pow(vec.at(i)-avg,2);
	}
	var /= vec.size();
	var = sqrt(var);
	return var;
}

//requires sorting before hand
double standardDevNoSort(deque<double> &vec) {
	double avg=0,var=0;
	double min = vec.at(0);
	double max = vec.at(vec.size()-1);
	avg = (min+max)/2;
	//avg = mean(vec);
	for(unsigned int i=0; i<vec.size(); i++) {
		var += pow(vec.at(i)-avg,2);
	}
	var /= vec.size();
	var = sqrt(var);
	var = round(var);
	return var;
}
