/*
 * testml.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#include "testml.h"

Mat TestML::getData() {
	return this->data;
}

Mat TestML::getLabels() {
	return this->labels;
}
void TestML::setLayerParams(Mat layers) {
	this->layers = layers;
}

//crops and fixes binary sample data
Mat TestML::prepareImage(Mat sample, Size size) {
	Functions fn;
	Mat img;
	Size _size = sample.size();
	if(size.width>0 && size.height>0)
		_size = size;
	img = fn.cropImage(sample);
	img = runResizeImage(img,_size);
	img = this->fixBinaryImage(img);
	img = fn.cropImage(img);
	img = runResizeImage(img,_size);
	img = this->fixBinaryImage(img);
	return img;
}

//converts single Mat into sample Mat form
Mat TestML::prepareMatSamples(vector<Mat> sampleVec) {
	int rows = sampleVec.at(0).rows;
	int cols = sampleVec.at(0).cols;
	if((rows*cols)==this->layers.at<int>(0,0)) {
		Mat sampleSet(sampleVec.size(),rows*cols,CV_32F);
		for(unsigned int i=0; i<sampleVec.size(); i++) {
			for(int j=0; j<rows; j++) {
				for(int k=0; k<cols; k++) {
					sampleSet.at<float>(i,k+j*cols) = sampleVec.at(i).at<uchar>(j,k);
				}
			}
		}
		return sampleSet;
	}
	else {
		cout << "prepareMatSamples: input size != rows*cols" << endl;
		return Mat();
	}
}

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

void TestML::importSamples(String folder, vector<Mat> &samples, Size size) {
	FileData fd;
	deque<String> files;
	String filename;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	for(unsigned int i=0; i<files.size(); i++) {
		String name = getFileName(files.at(i),"(");
		filename = folder+files.at(i);
		Mat img = imread(filename,0);
		if(!img.empty()) {
			//img = this->prepareImage(img,size);
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

void TestML::convertImagesToData(String folder, Mat outputLabels, Size size) {
	vector<Mat> samples;
	this->importSamples(folder,samples,size);
	if(samples.size()==outputLabels.rows) {
		Mat data(samples.size(),samples.at(0).total(),CV_32F);
		Mat labels(samples.size(),outputLabels.cols,CV_32F);
		int x=0;

		for(unsigned int i=0; i<samples.size(); i++) {
			Mat samp = samples.at(i);
			//samp = this->tempFixPrepareImg(samp);
			for(int j=0; j<samp.rows; j++) {
				for(int k=0; k<samp.cols; k++) {
					data.at<float>(i,x) = samp.at<uchar>(j,k);
					x++;
				}
			}
			x=0;
			for(unsigned int n=0; n<outputLabels.cols; n++) {
				labels.at<float>(i,n) = outputLabels.at<float>(i,n);
			}
		}
		this->data = data;
		this->labels = labels;
	}
	else {
		cout << "Sample size =/= Label size!" << endl;
		cout << "Sample Size: " << samples.size() << endl;
		cout << "Label Size: " << outputLabels.rows << endl;
		exit(1);
	}
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

Mat TestML::runANN(String param, vector<Mat> sampleVec) {
	CvANN_MLP ann;
	ann.load(param.c_str());
	Mat layers = ann.get_layer_sizes();
	this->setLayerParams(layers);
	Mat sample_set = this->prepareMatSamples(sampleVec);
	Mat results;
	ann.predict(sample_set,results);
	/*for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}*/
	return results;
}

Mat TestML::tempFixPrepareImg(Mat src) {
	Mat translatedImg = src.clone();
	for(int i=0; i<translatedImg.rows; i++) {
		for(int j=0; j<translatedImg.cols; j++) {
			//10,51,102,153,204,255
			int val = translatedImg.at<uchar>(i,j);
			if(val==255) translatedImg.at<uchar>(i,j) = 5; //white
			if(val==204) translatedImg.at<uchar>(i,j) = 4; //light
			if(val==153) translatedImg.at<uchar>(i,j) = 3; //low
			if(val==102) translatedImg.at<uchar>(i,j) = 2; //high
			if(val==51) translatedImg.at<uchar>(i,j) = 1; //dark
			//if(val==0) translatedImg.at<uchar>(i,j) = 0; //black
		}
	}
	return translatedImg;
}
