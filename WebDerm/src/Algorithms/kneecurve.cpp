/*
 * kneecurve.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: jason
 */

#include "kneecurve.h"

//! finds the knee of the curve
//! pass by reference so the original gets sorted
int KneeCurve::kneeCurvePoint(deque<double> &vec) {
	sort(vec.begin(),vec.end());
	Point2f xy0 = Point2f(0,vec.at(0));
	Point2f xy1 = Point2f((vec.size()-1),vec.at(vec.size()-1));
	this->maxDist = 0;
	this->bestIdx=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		Point2f xy2 = Point2f(i,vec.at(i));
		double a = 1 + pow((xy1.y-xy0.y)/(xy1.x-xy0.x),2);
		double b = -2*xy0.x*pow((xy1.y-xy0.y)/(xy1.x-xy0.x),2);
		b += 2*xy0.y*((xy1.y-xy0.y)/(xy1.x-xy0.x));
		b -= ((xy0.y+xy2.y)*(xy1.y-xy0.y)/(xy1.x-xy0.x))-(xy0.x-xy2.x);
		double c = pow(xy0.y,2)+pow((xy1.y-xy0.y)/(xy1.x-xy0.x),2)*pow(xy0.x,2);
		c -= 2*xy0.y*((xy1.y-xy0.y)/(xy1.x-xy0.x))*xy0.x;
		c -= xy0.y*(xy0.y+xy2.y)+((xy0.y+xy2.y)*(xy1.y-xy0.y)/(xy1.x-xy0.x));
		c += (xy0.y*xy2.y)+(xy0.x*xy2.x);
		double x = (-b+sqrt(pow(b,2)-4*a*c))/(2*a);
		double y = xy0.y+((xy1.y-xy0.y)/(xy1.x-xy0.x))*(x-xy0.x);
		double dist = eucDist(xy2,Point(x,y));
		if(dist>this->maxDist) {
			this->maxDist = dist;
			this->bestIdx = i;
		}
	}
	return this->bestIdx;
}

double KneeCurve::getMaxDist() {
	return this->maxDist;
}
