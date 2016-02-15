/*
 * cluster.h
 *
 *  Created on: Jan 15, 2015
 *      Author: jason
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include "../global.h"

class Cluster {
private:
	double dataRange;
	int clusterCount;
	deque<int> dataVec; //holds all sample data
	deque<int> uniqueSamples; //count of each unique sample
	deque<int> sampleData; //unique samples being considered, usually when count>0

	Mat samples;
	deque<int> centerCount;
	deque<deque<double> > ranges;
	Mat labels, centers;
public:
	enum {UCHAR,INT,FLOAT,DOUBLE};

	Mat kmeansClusterGeneric(Mat src, double maxVal=0);
	void writeClusterData(Mat src, String name, int type);
	Mat colorClusters(Mat src, Mat centers, Mat labels, deque<Point> ptVec);

	Mat kmeansCluster(vector<double> data_vec, int clusters=0);
	Mat kmeansCluster(vector<int> data_vec, int clusters=0);
	Mat kmeansCluster(Mat src, int clusters=0, int minVal=0, int maxVal=225);
	int getNumOfClusters();
	int getSizeOfCluster(int clusterNum);
	double getMin(int clusterNum);
	double getMax(int clusterNum);
	double getCenter(int clusterNum);
	void printInfo();

	/** whiten.cpp **/
	vector<double> whiten(vector<double> &data_vec);
};

#endif /* CLUSTER_H_ */
