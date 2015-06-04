/*
 * shadeshape.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#include "shadeshape.h"
#include "shapemorph.h"

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

