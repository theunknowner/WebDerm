/*
 * testml.h
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#ifndef TESTML_H_
#define TESTML_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/stdafx.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"

class TestML {
public:
	int layers;
	int inputSize;
	int outputSize;
	int hiddenNodes;
	void setLayerParams(int inputSize, int hiddenNodes, int outputSize);
	Mat prepareImage(Mat sample);
	Mat prepareMatSamples(vector<Mat> sampleVec);
	void importCsvData(String file, vector<vector<double> > &data, vector<vector<double> > &labels);
	void vecToMat(vector<vector<double> > &data, vector<vector<double> > &labels, Mat &mData, Mat &mLabels);
	void importSamples(String folder, vector<Mat> &samples);
	void writeData(String path, Mat &dataSet, Mat &labels);
	void convertImagesToData(String folder,vector<double> outputLabels);
	void printData(vector<vector<Point> > &trainingData, vector<vector<double> > &labels);
	Mat fixBinaryImage(Mat input);
};

#endif /* TESTML_H_ */
