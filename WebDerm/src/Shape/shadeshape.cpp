/*
 * shadeshape.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#include "shadeshape.h"
#include "shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

vector<Mat> ShadeShape::extractShadeShape(Mat src) {
	ShapeMorph sm;
	vector<Mat> features = sm.liquidFeatureExtraction(src);
	vector<Mat> shadeShapeVec;
	for(unsigned int i=0; i<features.size(); i++) {
		vector<vector<Point> > ptsVec(256,vector<Point>(0,Point(-1,-1)));
		Mat island = features.at(i);
		for(int row=0; row<island.rows; row++) {
			for(int col=0; col<island.cols; col++) {
				int val = island.at<uchar>(row,col);
				if(val>0) {
					ptsVec.at(val).push_back(Point(col,row));
				}
			}
		}
		for(unsigned int j=0; j<ptsVec.size(); j++) {
			Mat shadeShape(src.size(),CV_8U,Scalar(0));
			if(ptsVec.at(j).size()>0) {
				for(unsigned int k=0; k<ptsVec.at(j).size(); k++) {
					shadeShape.at<uchar>(ptsVec.at(j).at(k)) = j;
				}
				vector<Mat> littleIslands = sm.liquidFeatureExtraction(shadeShape);
				for(unsigned int k=0; k<littleIslands.size(); k++) {
					if(countNonZero(littleIslands.at(k))>50)
						shadeShapeVec.push_back(littleIslands.at(k));
				}
			}
		}
	}
	return shadeShapeVec;
}

vector<Mat> ShadeShape::isolateConnectedFeatures(Mat src) {
	Size size(3,3);
	vector<Point> ptsVec;

	for(int row=0; row<src.rows; row++) {
		for(int col=0; col<src.cols; col++) {
			int count=0;
			int units=0;
			if(src.at<uchar>(row,col)>0) {
				for(int i=row-floor(size.height/2); i<row+floor(size.height/2); i++) {
					for(int j=col-floor(size.width/2); j<col+floor(size.width/2); j++) {
						if(i>=0 && i<src.rows && j>=0 && j<src.cols) {
							if(i!=row && j!=col) {
								units++;
								if(src.at<uchar>(i,j)>0)
									count++;
							}
						}
					}
				}
				if(count<=floor(units*0.75)) {
					ptsVec.push_back(Point(col,row));
				}
			}
		}
	}
	Mat results = src.clone();
	for(unsigned int i=0; i<ptsVec.size(); i++) {
		results.at<uchar>(ptsVec.at(i)) = 0;
	}
	imgshow(src);
	imgshow(results);
	ShapeMorph sm;
	vector<Mat> isolatedFeatures = sm.liquidFeatureExtraction(results);
	return isolatedFeatures;
}
