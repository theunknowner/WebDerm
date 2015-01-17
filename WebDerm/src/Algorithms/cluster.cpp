/*
 * cluster.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: jason
 */

#include "cluster.h"

/* returns mask using kmeans clustering,
 * set maxVal>0 for custom max value of samples
 */
Mat Cluster::kmeansClusterGeneric(Mat src, double maxVal) {
	//static unsigned int defaultSeed = theRNG().state;
	//theRNG().state = defaultSeed; //reset RNG seed for kmeans initial centroids
	//get max value of sample data
	if(maxVal==0) {
		for(int i=0; i<src.rows; i++) {
			for(int j=0; j<src.cols; j++) {
				if(src.at<float>(i,j)>maxVal) {
					maxVal = round(src.at<float>(i,j));
				}
			}
		}
	}
	deque<double> dataVec; //holds all sample data
	deque<int> uniqueSamples(maxVal+1.0,0); //count of each unique sample
	deque<int> sampleData; //unique samples being considered, usually when count>0
	deque<Point> ptVec; //holds (col,row) of each sample
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double val = src.at<float>(i,j);
			if(val>0) {
				//storing all sample data when conditions are met
				dataVec.push_back(val);
				//counting amount of each unique sample
				uniqueSamples.at(val)++;
				//storing point of each sample data
				ptVec.push_back(Point(j,i));
			}
		}
	}
	writeSeq2File(dataVec,"data");
	//getting unique samples that are being considered, usually when count>0
	for(unsigned int i=0; i<uniqueSamples.size(); i++) {
		if(uniqueSamples.at(i)>0) {
			sampleData.push_back(i);
		}
	}
	//converting deque -> Mat
	Mat samples(dataVec.size(), 1, CV_32F);
	for(unsigned int i=0; i<dataVec.size(); i++) {
		samples.at<float>(i,0) = dataVec.at(i);
	}

	int dataRange = sampleData.at(sampleData.size()-1) - sampleData.at(0);
	int clusterCount = round(sqrt(dataRange/2))<sampleData.size() ? round(sqrt(dataRange/2)) : sampleData.size();
	clusterCount = min(11,clusterCount);
	Mat labels;
	int attempts = 5;
	Mat centers;
	printf("Min Lum: %.f, Max Lum: %f, Range: %d\n",sampleData.at(0),sampleData.at(sampleData.size()-1),dataRange);
	cout << "Initial clusters: " << clusterCount << endl;
	cout << "Input size: " << dataVec.size() <<"/" << src.total() << endl;
	cout << "Unique Samples: " << maxVal << endl;
	cout << "Unique samples considered: " << sampleData.size() << endl;
	double compact = kmeans(samples,clusterCount,labels,TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	cout << "compactness: " << compact << endl;
	deque<int> centerCount(clusterCount,0);
	deque<deque<double> > ranges(clusterCount,deque<double>(2,0.0));
	for(unsigned int i=0; i<ranges.size(); i++) {
		for(unsigned int j=0; j<ranges.at(i).size(); j++) {
			if(j==0) {
				ranges.at(i).at(j) = uniqueSamples.size();
			}
			else if(j==1) {
				ranges.at(i).at(j) = 0;
			}
		}
	}
	Mat origPos;
	jaysort(centers,origPos);
	for(int i=0; i<labels.rows; i++) {
		int idx = labels.at<int>(i,0);
		for(int j=0; j<origPos.rows; j++) {
			if(idx==origPos.at<int>(j,0)) {
				idx = j;
				labels.at<int>(i,0) = idx;
				break;
			}
		}
		centerCount.at(idx)++;
		if(dataVec.at(i)>ranges.at(idx).at(1)) {
			ranges.at(idx).at(1) = dataVec.at(i);
		}
		if(dataVec.at(i)<ranges.at(idx).at(0)) {
			ranges.at(idx).at(0) = dataVec.at(i);
		}
	}
	for(int i=0; i<centers.rows; i++) {
		printf("%f - %d - Min: %d, Max: %d\n",centers.at<float>(i,0),centerCount.at(i),ranges.at(i).at(0),ranges.at(i).at(1));
	}
	Mat result = Mat::zeros(src.rows, src.cols, CV_8U);
	//Mat clusterImg = Mat::zeros(src.rows,src.cols,CV_8UC3);
	int idxThresh = ceil(clusterCount*0.5); //! threshold for cluster filtering
	idxThresh = 4;
	//cout << idxThresh << endl;
	for(unsigned int i=0; i<ptVec.size(); i++) {
		int idx = labels.at<int>(i,0);
		if(idx>=idxThresh) {
			result.at<uchar>(ptVec.at(i)) = 255;
		}
	}
	//this->centers = centers;
	//this->writeClusterData(centers,"centers",FLOAT);
	//result = this->colorClusters(src,centers,labels,ptVec);
	return result;
}

void Cluster::writeClusterData(Mat src, String name, int type) {
	String filename = name+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			if(type==UCHAR)
				fprintf(fp,"%d,",src.at<uchar>(i,j));
			if(type==INT)
				fprintf(fp,"%d,",src.at<int>(i,j));
			if(type==FLOAT)
				fprintf(fp,"%f,",src.at<float>(i,j));
			if(type==DOUBLE)
				fprintf(fp,"%f,",src.at<double>(i,j));

		}
		fprintf(fp,"\n");
	}
}

Mat Cluster::colorClusters(Mat src, Mat centers, Mat labels, deque<Point>  ptVec) {
	Mat results(src.rows,src.cols,CV_8UC3,Scalar(0,0,0));
	RNG rng(0);
	vector<Vec3b> colorVec;
	for(int i=0; i<centers.rows; i++) {
		Vec3b color(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
		colorVec.push_back(color);
		printf("[%d, %d, %d]\n",color[2],color[1],color[0]);
	}
	for(int i=0; i<ptVec.size(); i++) {
		int idx = labels.at<int>(i,0);
		results.at<Vec3b>(ptVec.at(i)) = colorVec.at(idx);
	}
	return results;
}
