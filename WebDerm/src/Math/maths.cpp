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

/** rounds 8 and above **/
double myRound(double value) {
	double result=trunc(value);
	result=value-result;
	if(abs(result)>0.7999999) {
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
int max(int argNum, ...) {
	int val, largest = 0;
	va_list vl;
	va_start(vl,argNum);
	largest = va_arg(vl,int);
	for(int i=1; i<argNum; i++) {
		val = va_arg(vl,int);
		largest=(largest>val)?largest:val;
	}
	va_end(vl);
	return largest;
}

double max(double numArr[], int length) {
	double largest=numArr[0];
	for(int i=0; i<length; i++) {
		if(largest<numArr[i])
			largest = numArr[i];
	}
	return largest;
}

double average(deque<double> &vec) {
	double result=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		result += vec.at(i);
	}
	result /= vec.size();
	return result;
}

/** yArr and xArr has to be same size **/
double forecast(double input, deque<double> &yArr, deque<double> &xArr) {
	double results=0;
	double avgY=average(yArr);
	double avgX=average(xArr);
	double a=0,b=0;
	double b_Top=0, b_Bottom=0;
	for(unsigned int i=0; i<yArr.size(); i++) {
		b_Top += (xArr.at(i)-avgX)*(yArr.at(i)-avgY);
		b_Bottom += pow((xArr.at(i)-avgX),2);
	}
	b = b_Top/b_Bottom;
	a = avgY - (b*avgX);
	results = a + (b*input);
	return results;
}

double max(deque<double> vec) {
	double largest=0;
	try {
		largest = vec.at(0);
		for(unsigned int i=0; i<vec.size(); i++) {
			if(largest<vec.at(i))
				largest = vec.at(i);
		}
	}
	catch(const std::out_of_range &oor) {
		printf("maths.cpp::max(deque<double> vec) out of range!\n");
		exit(0);
	}
	return largest;
}

double min(deque<double> vec) {
	double lowest=0;
	try {
		lowest = vec.at(0);
		for(unsigned int i=0; i<vec.size(); i++) {
			if(lowest<vec.at(i))
				lowest = vec.at(i);
		}
	}
	catch(const std::out_of_range &oor) {
		printf("maths.cpp::min(deque<double> vec) out of range!\n");
		exit(0);
	}
	return lowest;
}

double euclideanDist(double *vec1, double *vec2) {
	double result=0;
	double val[3] = {0};
	for(int i=0; i<3; i++)
	{
		val[i] = vec1[i]-vec2[i];
		val[i] *= val[i];
		result += val[i];
	}
	result = sqrt(result);
	return result;
}

double euclideanDist(int *vec1, int *vec2) {
	double result=0;
	int val[3] = {0};
	for(int i=0; i<3; i++)
	{
		val[i] = vec1[i]-vec2[i];
		val[i] *= val[i];
		result += val[i];
	}
	result = sqrt(result);
	return result;
}

double euclideanDist(deque<double> &vec1, deque<double> &vec2) {
	double result=0;
	double val[3] = {0};
	for(unsigned int i=0; i<3; i++)
	{
		val[i] = vec1.at(i)-vec2.at(i);
		val[i] *= val[i];
		result += val[i];
	}
	result = sqrt(result);
	return result;
}

double eucDist(Point pt1, Point pt2) {
	double result=0;
	double valX=0, valY=0;
	valX = pow(pt1.x-pt2.x,2);
	valY = pow(pt1.y-pt2.y,2);
	result = valX+valY;
	result = sqrt(result);
	return result;
}

//returns the least distance between the two degrees
double findDegreeDistance(double deg1, double deg2) {
	double diff = fabs(deg1-deg2);
	return min(360.0 - diff, diff);
}
