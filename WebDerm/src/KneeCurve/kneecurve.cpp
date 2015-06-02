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
	Mat vecFromFirst(vec.size(),2,CV_32F,Scalar(0));
	Mat firstPt(1,2,CV_32F,Scalar(0));
	Mat lastPt(1,2,CV_32F,Scalar(0));
	Mat firstPtPow,lastPtPow;
	firstPt.at<float>(0,0) = 0;
	firstPt.at<float>(0,1) = vec.at(0);
	lastPt.at<float>(0,0) = (vec.size()-1) - firstPt.at<float>(0,0);
	lastPt.at<float>(0,1) = vec.at(vec.size()-1) - firstPt.at<float>(0,1);
	for(unsigned int i=0; i<vec.size(); i++) {
		vecFromFirst.at<float>(i,0) = i - firstPt.at<float>(0,0);
		vecFromFirst.at<float>(i,1) = vec.at(i) - firstPt.at<float>(0,1);
	}
	pow(lastPt,2,lastPtPow);
	double sqrtSum = sqrt(sum(lastPtPow).val[0]);
	Mat lastPtNorm = lastPt / sqrtSum;
	Mat lastPtNormRep(0,0,CV_32F,Scalar(0));
	while(lastPtNormRep.rows!=vec.size()) {
		lastPtNormRep.push_back(lastPtNorm.row(0));
	}
	Mat scalarProduct;
	multiply(vecFromFirst,lastPtNormRep,scalarProduct);
	Mat vecFromFirstParallel = scalarProduct.col(1) * lastPtNorm;
	Mat vecToLine = vecFromFirst - vecFromFirstParallel;
	Mat vecToLinePow;
	pow(vecToLine,2,vecToLinePow);
	Mat sumMat(vecToLine.rows,1,CV_32F,Scalar(0));
	for(int i=0; i<vecToLinePow.rows; i++) {
		for(int j=0; j<vecToLinePow.cols; j++) {
			sumMat.at<float>(i,0) += vecToLinePow.at<float>(i,j);
		}
	}
	Mat distMat;
	sqrt(sumMat,distMat);
	this->maxDist = 0;
	this->bestIdx = 0;
	for(int i=0; i<distMat.rows; i++) {
		double dist = distMat.at<float>(i,0);
		if(dist>this->maxDist) {
			this->maxDist = dist;
			this->bestIdx = i;
		}
	}
	return this->bestIdx;
}

//! pass by reference so the original gets sorted
int KneeCurve::kneeCurvePoint(vector<double> &vec) {
	sort(vec.begin(),vec.end());
	Mat vecFromFirst(vec.size(),2,CV_32F,Scalar(0)); //basically vec with X coords
	Mat firstPt(1,2,CV_32F,Scalar(0));
	Mat lastPt(1,2,CV_32F,Scalar(0));
	Mat firstPtPow,lastPtPow;
	firstPt.at<float>(0,0) = 0;
	firstPt.at<float>(0,1) = vec.at(0);
	lastPt.at<float>(0,0) = (vec.size()-1) - firstPt.at<float>(0,0);
	lastPt.at<float>(0,1) = vec.at(vec.size()-1) - firstPt.at<float>(0,1);
	cout << "First Pt" << firstPt << endl;
	cout << "Last Pt" << lastPt << endl;

	pow(lastPt,2.0,lastPtPow);
	double sqrtSum = sqrt(sum(lastPtPow).val[0]);
	Mat lastPtNorm = lastPt / sqrtSum;
	Mat lastPtNormRep(0,0,CV_32F,Scalar(0));
	while(lastPtNormRep.rows!=vec.size()) {
		lastPtNormRep.push_back(lastPtNorm.row(0));
	}
	for(unsigned int i=0; i<vec.size(); i++) {
		vecFromFirst.at<float>(i,0) = i - firstPt.at<float>(0,0);
		vecFromFirst.at<float>(i,1) = vec.at(i) - firstPt.at<float>(0,1);
	}
	Mat scalarProduct;
	multiply(vecFromFirst,lastPtNormRep,scalarProduct);
	Mat vecFromFirstParallel = scalarProduct.col(1) * lastPtNorm;
	Mat vecToLine = vecFromFirst - vecFromFirstParallel;
	Mat vecToLinePow;
	pow(vecToLine,2,vecToLinePow);
	Mat sumMat(vecToLine.rows,1,CV_32F,Scalar(0));
	for(int i=0; i<vecToLinePow.rows; i++) {
		for(int j=0; j<vecToLinePow.cols; j++) {
			sumMat.at<float>(i,0) += vecToLinePow.at<float>(i,j);

		}
	}
	Mat distMat;
	sqrt(sumMat,distMat);
	this->maxDist = 0;
	this->bestIdx = 0;
	for(int i=0; i<distMat.rows; i++) {
		double dist = distMat.at<float>(i,0);
		if(dist>this->maxDist) {
			this->maxDist = dist;
			this->bestIdx = i;
		}
	}
	return this->bestIdx;
}

//! pass by reference so the original gets sorted
int KneeCurve::kneeCurvePoint(vector<int> &vec) {
	sort(vec.begin(),vec.end());
	Mat vecFromFirst(vec.size(),2,CV_32F,Scalar(0));
	Mat firstPt(1,2,CV_32F,Scalar(0));
	Mat lastPt(1,2,CV_32F,Scalar(0));
	Mat firstPtPow,lastPtPow;
	firstPt.at<float>(0,0) = 0;
	firstPt.at<float>(0,1) = vec.at(0);
	lastPt.at<float>(0,0) = (vec.size()-1) - firstPt.at<float>(0,0);
	lastPt.at<float>(0,1) = vec.at(vec.size()-1) - firstPt.at<float>(0,1);
	for(unsigned int i=0; i<vec.size(); i++) {
		vecFromFirst.at<float>(i,0) = i - firstPt.at<float>(0,0);
		vecFromFirst.at<float>(i,1) = vec.at(i) - firstPt.at<float>(0,1);
	}
	pow(lastPt,2,lastPtPow);
	double sqrtSum = sqrt(sum(lastPtPow).val[0]);
	Mat lastPtNorm = lastPt / sqrtSum;
	Mat lastPtNormRep(0,0,CV_32F,Scalar(0));
	while(lastPtNormRep.rows!=vec.size()) {
		lastPtNormRep.push_back(lastPtNorm.row(0));
	}
	Mat scalarProduct;
	multiply(vecFromFirst,lastPtNormRep,scalarProduct);
	Mat vecFromFirstParallel = scalarProduct.col(1) * lastPtNorm;
	Mat vecToLine = vecFromFirst - vecFromFirstParallel;
	Mat vecToLinePow;
	pow(vecToLine,2,vecToLinePow);
	Mat sumMat(vecToLine.rows,1,CV_32F,Scalar(0));
	for(int i=0; i<vecToLinePow.rows; i++) {
		for(int j=0; j<vecToLinePow.cols; j++) {
			sumMat.at<float>(i,0) += vecToLinePow.at<float>(i,j);
		}
	}
	Mat distMat;
	sqrt(sumMat,distMat);
	this->maxDist = 0;
	this->bestIdx = 0;
	for(int i=0; i<distMat.rows; i++) {
		double dist = distMat.at<float>(i,0);
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

void KneeCurve::removeOutliers(deque<double> &vec, double percent) {
	sort(vec.begin(),vec.end());
	int amt = round(vec.size() * percent);
	for(int i=0; i<amt; i++) {
		vec.pop_back();
		vec.pop_front();
	}
}

void KneeCurve::removeOutliers(vector<double> &vec, double percent) {
	sort(vec.begin(),vec.end());
	int amt = round(vec.size() * percent);
	for(int i=0; i<amt; i++) {
		vec.pop_back();
		vec.erase(vec.begin());
	}
}

void KneeCurve::removeOutliers(deque<int> &vec, double percent) {
	sort(vec.begin(),vec.end());
	int amt = round(vec.size() * percent);
	for(int i=0; i<amt; i++) {
		vec.pop_back();
		vec.pop_front();
	}
}

void KneeCurve::loadVectorFile(String path, deque<double> &vec) {
	fstream fs(path.c_str());
	if(fs.is_open()) {
		String temp;
		while(getline(fs,temp)) {
			vec.push_back(atof(temp.c_str()));
		}
		fs.flush();
		fs.close();
	}
}

//src = input; thresh=perceptible thresh; shift=kneePt shift percentage
Mat KneeCurve::filterKneePt(Mat src, double thresh, double shift) {
	vector<double> yVec1;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double lum = src.at<uchar>(i,j);
			if(lum>thresh)
				yVec1.push_back(lum);
		}
	}
	int bestIdx = this->kneeCurvePoint(yVec1);
	bestIdx *= shift;
	double threshFilter = yVec1.at(bestIdx);
	Mat dst =src.clone();
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double lum = src.at<uchar>(i,j);
			if(lum<threshFilter)
				dst.at<uchar>(i,j) = 0;
		}
	}
	return dst;
}
