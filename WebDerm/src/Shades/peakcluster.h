/*
 * peakcluster.h
 *
 *  Created on: Dec 15, 2015
 *      Author: jason
 */

#ifndef PEAKCLUSTER_H_
#define PEAKCLUSTER_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class PeakCluster {
private:
	vector<double> convert2Vec(Mat &src);
	void getMinMaxVal(vector<double> &data_vec);
public:
	int minVal;
	int maxVal;
	bool isOutliersRemoved = false;
	int getPeakClusters(Mat src);
	Mat removeShadeOutliers(Mat discreteImg, Mat origImg, float thresh);

};

#endif /* PEAKCLUSTER_H_ */
