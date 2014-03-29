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
	int size=3;
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
