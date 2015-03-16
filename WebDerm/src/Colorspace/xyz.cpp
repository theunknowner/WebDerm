/*
 * xyz.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "xyz.h"

vector<float> Xyz::rgb2xyz(int r, int g, int b) {
	float M[3][3] = {
			{0.4124564, 0.3575761, 0.1804375},
			{0.2126729, 0.7151522, 0.0721750},
			{0.0193339 , 0.1191920, 0.9503041}};
	Mat matrix(3,3,CV_32F, M);
	Mat rgbMat(3,1,CV_32F, Scalar(0));
	float R = r/255.0;
	float G = g/255.0;
	float B = b/255.0;
	rgbMat.at<float>(0,0) = R;
	rgbMat.at<float>(1,0) = G;
	rgbMat.at<float>(2,0) = B;
	for(int i=0; i<rgbMat.rows; i++) {
		if(rgbMat.at<float>(i,0)>0.04045)
			rgbMat.at<float>(i,0) = pow((rgbMat.at<float>(i,0)+0.055)/1.055,2.4);
		else
			rgbMat.at<float>(i,0) /= 12.92;

		rgbMat.at<float>(i,0) *= 100;
	}

	Mat xyzMat = matrix * rgbMat;

	vector<float> xyzVec;
	xyzMat.copyTo(xyzVec);

	return xyzVec;
}

vector<int> Xyz::xyz2rgb(float x, float y, float z) {
	float M[3][3] = {
			{3.2404542, -1.5371385, -0.4985314},
			{-0.9692660, 1.8760108, 0.0415560},
			{0.0556434, -0.2040259, 1.0572252}};
	Mat xyzMat(3,1,CV_32F,Scalar(0));
	Mat matrix(3,3,CV_32F,M);
	float X = x/100;
	float Y = y/100;
	float Z = z/100;
	xyzMat.at<float>(0,0) = X;
	xyzMat.at<float>(1,0) = Y;
	xyzMat.at<float>(2,0) = Z;
	Mat rgbMat = matrix * xyzMat;
	vector<int> rgbVec;
	for(int i=0; i<rgbMat.rows; i++) {
		if(rgbMat.at<float>(i,0)>0.0031308)
			rgbMat.at<float>(i,0) = 1.055 * pow(rgbMat.at<float>(i,0),(1/2.4)) - 0.055;
		else
			rgbMat.at<float>(i,0) *= 12.92;

		rgbMat.at<float>(i,0) = round(rgbMat.at<float>(i,0)*255);
		rgbVec.push_back((int)rgbMat.at<float>(i,0));
	}
	return rgbVec;
}
