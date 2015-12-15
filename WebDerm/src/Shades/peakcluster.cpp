/*
 * peakcluster.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: jason
 */

#include "peakcluster.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Cluster/cluster.h"

inline vector<double> PeakCluster::convert2Vec(Mat &src) {
	vector<double> data_vec;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double val = src.at<uchar>(i,j);
			if(val>0)
				data_vec.push_back(val);
		}
	}
	return data_vec;
}

inline void PeakCluster::getMinMaxVal(vector<double> &data_vec) {
	this->minVal = *min_element(data_vec.begin(),data_vec.end());
	this->maxVal = *max_element(data_vec.begin(),data_vec.end());
}

int PeakCluster::getPeakClusters(Mat src) {
	vector<double> data_vec = this->convert2Vec(src);
	this->getMinMaxVal(data_vec);

	std::unordered_map<int,int> map;
	for(unsigned int i=0; i<data_vec.size(); i++) {
		if(map.find(data_vec.at(i))==map.end())
			map[data_vec.at(i)] = 1;
	}
	float changeThresh = 1.13;
	int changeCountThresh = 3;
	int maxShades = 5;
	int error = 2;
	int maxClusters = min(8,(int)map.size());
	vector<double> densityVec;
	for(int i=0; i<maxClusters; i++) {
		Cluster clst2;
		clst2.kmeansCluster(data_vec,i+1);
		int totalPts=0;
		double totalDensity = 0.0;
		for(int j=0; j<clst2.getNumOfClusters(); j++) {
			totalPts += clst2.getSizeOfCluster(j);
		}
		for(int j=0; j<clst2.getNumOfClusters(); j++) {
			int numPts = clst2.getSizeOfCluster(j);
			double minVal = clst2.getMin(j);
			double maxVal = clst2.getMax(j);
			double density = numPts/(maxVal-minVal);
			if(std::isinf(density)) density = 1000.0;
			totalDensity += ((double)numPts/totalPts) * density;
			double center = clst2.getCenter(j);
			//printf("Clst: %d, Center: %.0f, Min: %.0f, Max: %.0f, Range: %.0f, Size: %d, Density: %f\n",j+1,center,minVal,maxVal,maxVal-minVal,numPts,density);

		}
		//cout << "-------------------------------------------" << endl;
		densityVec.push_back(totalDensity);
	}
	vector<double> changeVec;
	double change = -1.0;
	for(unsigned int i=0; i<densityVec.size(); i++) {
		if(i>0) {
			change = densityVec.at(i)/densityVec.at(i-1);
			changeVec.push_back(change);
			//printf("%d) Density: %f, Change: %f\n",i+1,densityVec.at(i),change);
		}
		else {
			//printf("%d) Density: %f, Change: %f\n",i+1,densityVec.at(i),change);
			changeVec.push_back(change);
		}
	}
	int changeCount = 0;
	int peakPos = 1;
	for(unsigned int i=0; i<changeVec.size(); i++) {
		double change = changeVec.at(i);
		if(change<=changeThresh && change>=0) changeCount++;
		else changeCount = 0;
		//printf("%d: %f, %d\n",i+1,change,changeCount);
		if(changeCount>=changeCountThresh || i==(maxClusters-1)) {
			peakPos = max((int)(i-changeCountThresh),0);
			peakPos++;
			break;
		}
	}
	if(peakPos==1) {
		vector<double>::iterator it = max_element(changeVec.begin(),changeVec.end());
		peakPos = (it - changeVec.begin())+1;
	}
	peakPos = min(peakPos+error,maxShades);
	return peakPos;
}

//! return new image after removing shade outliers base on discreteImg
Mat PeakCluster::removeShadeOutliers(Mat discreteImg, Mat origImg, float thresh) {
	std::unordered_map<int,int> shadeMap;
	std::unordered_map<int,vector<Point>> pointMap;
	Mat newImg = origImg.clone();
	for(int i=0; i<discreteImg.rows; i++) {
		for(int j=0; j<discreteImg.cols; j++) {
			int val = discreteImg.at<uchar>(i,j);
			if(val>0) {
				shadeMap[val]++;
				pointMap[val].push_back(Point(j,i));
			}
		}
	}
	int area = countNonZero(discreteImg);
	for(auto it=shadeMap.begin(); it!=shadeMap.end(); it++) {
		float relArea = it->second / (float)area;
		if(relArea<=thresh) {
			int val = it->first;
			for(unsigned int i=0; i<pointMap.at(val).size(); i++) {
				Point pt = pointMap.at(val).at(i);
				newImg.at<uchar>(pt) = 0;
			}
			this->isOutliersRemoved = true;
		}
	}
	if(this->isOutliersRemoved) {
		vector<double> data_vec = this->convert2Vec(newImg);
		this->getMinMaxVal(data_vec);
	}
	return newImg;
}
