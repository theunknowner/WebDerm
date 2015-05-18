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
