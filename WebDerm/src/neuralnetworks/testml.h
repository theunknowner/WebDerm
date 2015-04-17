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
private:
	Mat data;
	Mat labels;
public:
	Mat getData();
	Mat getLabels();
	Mat layers;
	void setLayerParams(Mat layers);
	Mat prepareImage(Mat sample, Size size=Size(0,0));
	Mat prepareMatSamples(vector<Mat> sampleVec);
	void importCsvData(String file, vector<vector<double> > &data, vector<vector<double> > &labels);
	void vecToMat(vector<vector<double> > &data, vector<vector<double> > &labels, Mat &mData, Mat &mLabels);
	void importSamples(String folder, vector<Mat> &samples, Size size=Size(0,0));
	void writeData(String path, Mat &dataSet, Mat &labels);
	void convertImagesToData(String folder,Mat outputLabels, Size size=Size(0,0));
	void printData(vector<vector<Point> > &trainingData, vector<vector<double> > &labels);
	Mat fixBinaryImage(Mat input);

	Mat runANN(String param, vector<Mat> sampleVec);

	Mat tempFixPrepareImg(Mat src);
};

#endif /* TESTML_H_ */
