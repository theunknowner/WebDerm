/*
 * testml.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#include "testml.h"

void TestML::importCsvData(String file, vector<vector<double> > &data, vector<vector<double> > &labels) {
	fstream fs(file.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		vector<double> pts;
		vector<double> lbl;
		size_t pos=0;
		int x,y;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i<400) {
					pts.push_back(atof(vec.at(i).c_str()));
				}
				else {
					lbl.push_back(atof(vec.at(i).c_str()));
				}
			}
			data.push_back(pts);
			labels.push_back(lbl);
			vec.clear();
			pts.clear();
			lbl.clear();
		}
		fs.close();
	}
	else {
		cout << "Import training data failed!" << endl;
	}
}

void TestML::vecToMat(vector<vector<double> > &data, vector<vector<double> > &labels, Mat &mData, Mat &mLabels) {
	for(unsigned int i=0; i<data.size(); i++) {
		for(unsigned int j=0; j<data.at(i).size(); j++) {
			mData.at<float>(i,j) = data.at(i).at(j);
		}
		for(unsigned int k=0; k<labels.at(0).size(); k++) {
			mLabels.at<float>(i,k) = labels.at(i).at(k);
		}
	}
}

void TestML::importSamples(String folder, vector<Mat> &samples) {
	FileData fd;
	Functions fn;
	deque<String> files;
	String filename;
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = getFileName(files.at(i),"(");
		filename = folder+name+"("+toString(i+1)+")"+".png";
		Mat img = imread(filename,0);
		if(img.data) {
			img = fn.cropImage(img);
			resize(img,img,Size(20,20),INTER_AREA);
			img = this->fixBinaryImage(img);
			img = fn.cropImage(img);
			resize(img,img,Size(20,20),INTER_AREA);
			img = this->fixBinaryImage(img);
			samples.push_back(img);
		}
	}
}

void TestML::writeData(String path, Mat &dataSet, Mat &labels) {
	FILE * fp;
	fp = fopen(path.c_str(),"w");
	for(int i=0; i<dataSet.rows; i++) {
		for(int j=0; j<dataSet.cols; j++) {
			fprintf(fp,"%.f,",dataSet.at<float>(i,j));
		}
		for(int k=0; k<labels.cols; k++) {
			fprintf(fp,"%.f,",labels.at<float>(i,k));
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void TestML::convertImagesToData(String folder, vector<double> outputLabels) {
	vector<Mat> samples;
	this->importSamples(folder,samples);
	Mat data(samples.size(),400,CV_32F);
	Mat labels(samples.size(),outputLabels.size(),CV_32F);
	int x=0;
	for(unsigned int i=0; i<samples.size(); i++) {
		Mat samp = samples.at(i);
		for(int j=0; j<samp.rows; j++) {
			for(int k=0; k<samp.cols; k++) {
				data.at<float>(i,x) = samp.at<uchar>(j,k);
				x++;
			}
		}
		x=0;
		for(unsigned int n=0; n<outputLabels.size(); n++) {
			labels.at<float>(i,n) = outputLabels.at(n);
		}
	}
	this->writeData(folder+"data_set.csv",data,labels);
}

void TestML::printData(vector<vector<Point> > &trainingData, vector<vector<double> > &labels) {
	for(unsigned int i=0; i<trainingData.size(); i++) {
		printf("%d:",i);
		for(unsigned int j=0; j<trainingData.at(i).size(); j++) {
			printf("(%d,%d)",trainingData.at(i).at(j).x,trainingData.at(i).at(j).y);
		}
		printf(" - %.f,%.f\n",labels.at(i).at(0),labels.at(i).at(1));
	}
}

Mat TestML::fixBinaryImage(Mat input) {
	Mat output = input.clone();
	for(int i=0; i<output.rows; i++) {
		for(int j=0; j<output.cols; j++) {
			if(output.at<uchar>(i,j)>40)
				output.at<uchar>(i,j) = 255;
			else
				output.at<uchar>(i,j) = 0;
		}
	}
	return output;
}
