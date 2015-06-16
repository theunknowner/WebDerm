/*
 * applydiscreteshade.cpp
 *
 *  Created on: May 18, 2015
 *      Author: jason
 */

#include "shapecolor.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"

//input is grayscale image
Mat ShapeColor::applyDiscreteShade(Mat input) {
	Mat dst(input.size(),CV_8U,Scalar(0));
	Shades sh;
	vector<String> discreteShade = {"Dark2","Dark3","High","Low","Light","White"};
	vector<int> discreteVals = {0,51,102,153,204,255};
	for(int row=0; row<input.rows; row++) {
		for(int col=0; col<input.cols; col++) {
			int val = input.at<uchar>(row,col);
			if(val>0) {
				String shade = sh.calcShade2(val);
				for(unsigned int i=0; i<discreteShade.size(); i++) {
					if(shade==discreteShade.at(i)) {
						dst.at<uchar>(row,col) = discreteVals.at(i);
						break;
					}
				}
			}
		}
	}
	return dst;
}

Mat ShapeColor::applyDiscreteShade(Mat input, int minVal, int maxVal, int intervals) {
	double sw = (double)(maxVal - minVal) / intervals;
	Mat dst(input.size(),CV_8U,Scalar(0));
	vector<vector<double> > ranges(intervals,vector<double>(3,0.0));
	for(int i=0; i<intervals; i++) {
		double dMinVal = minVal + (sw*i);
		ranges.at(i).at(0) = dMinVal;
		ranges.at(i).at(1) = dMinVal + sw;
		ranges.at(i).at(2) = (ranges.at(i).at(0)+ranges.at(i).at(1)) / 2;
	}
	for(int row=0; row<input.rows; row++) {
		for(int col=0; col<input.cols; col++) {
			int val = input.at<uchar>(row,col);
			if(val>0) {
				for(unsigned int i=0; i<ranges.size(); i++) {
					if(val>ranges.at(i).at(0) && val<=ranges.at(i).at(1)) {
						dst.at<uchar>(row,col) = (int)ranges.at(i).at(2);
					}
					else if(val==minVal) {
						dst.at<uchar>(row,col) = (int)ranges.at(0).at(2);
					}
				}
			}
		}
	}
	return dst;
}
