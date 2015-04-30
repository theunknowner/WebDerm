/*
 * cluster.h
 *
 *  Created on: Jan 15, 2015
 *      Author: jason
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Cluster {
private:
	int dataRange;
	int clusterCount;
	deque<int> dataVec; //holds all sample data
	deque<int> uniqueSamples; //count of each unique sample
	deque<int> sampleData; //unique samples being considered, usually when count>0
public:
	Mat centers;
	enum {UCHAR,INT,FLOAT,DOUBLE};

	Mat kmeansClusterGeneric(Mat src, double maxVal=0);
	void writeClusterData(Mat src, String name, int type);
	Mat colorClusters(Mat src, Mat centers, Mat labels, deque<Point> ptVec);
};

#endif /* CLUSTER_H_ */
