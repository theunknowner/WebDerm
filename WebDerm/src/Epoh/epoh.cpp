/*
 * epoh.cpp
 *
 *  Created on: Jul 9, 2015
 *      Author: jason
 */

#include "epoh.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

//! Expressive Power of Hue
double epoh(double &sat, double &lum) {
	double result;
	if(lum<=50) {
		result = (sat/100.) * (lum/50.);
	}
	else {
		result = (sat/100.) * (100-lum)/50;
	}
	return result;
}

//using Minkowski distance
Mat epohTheHue(Mat &hMat, Mat &sMat, Mat &lMat) {
	Mat hc(hMat.size(),CV_32F, Scalar(0));
	for(int i=0; i<hMat.rows; i++) {
		for(int j=0; j<hMat.cols; j++) {
			double hue = hMat.at<float>(i,j);
			double sat = sMat.at<float>(i,j);
			double lum = lMat.at<float>(i,j);
			hue *= epoh(sat,lum);
			hc.at<float>(i,j) = ip::roundDecimal(hue,2);
		}
	}
	return hc;
}
