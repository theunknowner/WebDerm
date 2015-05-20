/*
 * halo_transform.cpp
 *
 *  Created on: May 20, 2015
 *      Author: jason
 */

#include "shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"

//increase LC for nearby
Mat ShapeMorph::haloTransform(Mat src, int iterations) {
	Mat tempResult(src.rows, src.cols, CV_32F, Scalar(0));
	Mat results(src.rows, src.cols, CV_8U, Scalar(0));
	Mat interPtsMap(src.rows,src.cols,CV_32S,Scalar(0));
	deque<deque<Point> > interPts(src.rows*src.cols,deque<Point>(1,Point(-1,-1)));
	deque<deque<double> > distVec(src.rows*src.cols,deque<double>(1,-1.0));
	const double alpha = 0.4;
	const double epsilon = 1.0;
	double absDiscernThresh = 1.0;
	const double cutoffThresh = 1.0;
	int row=0, col=0;
	double dist=0, cutoffDist=0;
	double maxVal=0;
	while(row<src.rows) {
		while(col<src.cols) {
			int index = col + row*src.cols;
			if(interPts.at(index).at(0)==Point(-1,-1))
				interPts.at(index).pop_front();
			if(distVec.at(index).at(0)==-1.0)
				distVec.at(index).pop_front();
			double dbl_Lc = sqrt((int)src.at<uchar>(row,col));
			if(dbl_Lc>absDiscernThresh) {
				tempResult.at<float>(row,col) = dbl_Lc;
				double cutoffVal = cutoffThresh + 1.0;
				while(cutoffVal>cutoffThresh) {
					cutoffDist++;
					cutoffVal = pow(alpha,cutoffDist) * pow(dbl_Lc,epsilon);
				}
				cutoffDist--;
				for(int i=(row-cutoffDist); i<=(row+cutoffDist); i++) {
					for(int j=(col-cutoffDist); j<=(col+cutoffDist); j++) {
						if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
							int index = j + i*src.cols;
							dist = floor(MyMath::eucDist(Point(j,i),Point(col,row)));
							if(dist<=cutoffDist) {
								interPts.at(index).push_back(Point(col,row));
								distVec.at(index).push_back(dist);
							}
						}
					}
				}
			}
			cutoffDist=0;
			col++;
		}
		col=0;
		row++;
	}
	int iter=1;
	for(int n=0; n<iter; n++) {
		for(int i=0; i<src.rows; i++) {
			for(int j=0; j<src.cols; j++) {
				int index = j + i*src.cols;
				for(unsigned int k=0; k<interPts.at(index).size(); k++) {
					if(interPts.at(index).size()>=2) {
						double dbl_Lc = sqrt((int)src.at<uchar>(interPts.at(index).at(k)));
						double val1 = pow(alpha,distVec.at(index).at(k)) * pow(dbl_Lc,epsilon);
						val1 *= (double)iterations; //same as iterating 2 times
						dbl_Lc *= (double)iterations;
						val1 = tempResult.at<float>(i,j) + val1;
						if(val1<0) val1=0;
						tempResult.at<float>(i,j) = val1;
						maxVal = max(val1,maxVal);
						interPtsMap.at<int>(interPts.at(index).at(k))++;
					}
				}
			}
		}
	}
	return tempResult;
}
