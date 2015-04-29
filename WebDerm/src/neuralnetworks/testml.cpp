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
	sample = this->convertToBinary(sample);
	img = fn.cropImage(sample);
	img = runResizeImage(img,_size);
	img = this->convertToBinary(img,0,1,0,255);
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
					Mat samp = sampleVec.at(i);
					if(samp.type()==CV_8U)
						sampleSet.at<float>(i,k+j*cols) = samp.at<uchar>(j,k);
					if(samp.type()==CV_32S)
						sampleSet.at<float>(i,k+j*cols) = samp.at<int>(j,k);
					if(samp.type()==CV_32F)
						sampleSet.at<float>(i,k+j*cols) = samp.at<float>(j,k);
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
		filename = folder+files.at(i);
		Mat img = imread(filename,0);
		if(!img.empty()) {
			if(size!=Size(0,0))
				img = this->prepareImage(img,size);
			samples.push_back(img);
		}
	}
}

void TestML::importLabels(String path,vector<Mat> &labels) {
	fstream fs(path);
	if(fs.is_open()) {
		String file;
		fstream fs2;
		while(getline(fs,file)) {
			fs2.open(file);
			if(fs2.is_open()) {
				String temp;
				deque<String> vec;
				vector<float> fVec;
				while(getline(fs2,temp)) {
					getSubstr(temp,',',vec);
					//starts at 1 to skip filename in first column
					for(unsigned int i=1; i<vec.size(); i++) {
						fVec.push_back(atof(vec.at(i).c_str()));
					}
					Mat mLabels(1,fVec.size(),CV_32F,Scalar(0));
					for(unsigned int j=0; j<fVec.size(); j++) {
						mLabels.at<float>(0,j) = fVec.at(j);
					}
					labels.push_back(mLabels.clone());
					mLabels.release();
					vec.clear();
					fVec.clear();
				}
				fs2.close();
			}
			else {
				printf("Cannot open %s\n",file.c_str());
				exit(1);
			}
		}
		fs.close();
	}
	else {
		printf("Cannot open %s\n",path.c_str());
		exit(1);
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
					if(samp.type()==CV_8U)
						data.at<float>(i,x) = samp.at<uchar>(j,k);
					else if(samp.type()==CV_32S)
						data.at<float>(i,x) = samp.at<int>(j,k);
					if(samp.type()==CV_32F)
						data.at<float>(i,x) = samp.at<float>(j,k);
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

//!converts min & max to 0's & 1's
Mat TestML::convertToBinary(Mat input, int min, int max, int newMin, int newMax) {
	Mat result = input.clone();
	for(int i=0; i<input.rows; i++) {
		for(int j=0; j<input.cols; j++) {
			int val = input.at<uchar>(i,j);
			if(val==min) result.at<uchar>(i,j) = newMin;
			else if(val==max) result.at<uchar>(i,j) = newMax;
		}
	}
	return result;
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
	//Mat translatedImg = src.clone();
	Mat translatedImg(src.size(),CV_32S,Scalar(0));
	for(int i=0; i<translatedImg.rows; i++) {
		for(int j=0; j<translatedImg.cols; j++) {
			//10,51,102,153,204,255
			int val = src.at<uchar>(i,j);
			if(val==255) translatedImg.at<int>(i,j) = 1; //white
			//if(val==204) translatedImg.at<uchar>(i,j) = 4; //light
			//if(val==153) translatedImg.at<uchar>(i,j) = 3; //low
			//if(val==102) translatedImg.at<uchar>(i,j) = 2; //high
			//if(val==51) translatedImg.at<uchar>(i,j) = 1; //dark
			if(val==0) translatedImg.at<int>(i,j) = -1; //black
		}
	}
	return translatedImg;
}

//reads csv file containing path to files/folders
void TestML::importTrainingData(String samplePath, String labelsPath, Size size) {
	fstream fs(samplePath);
	if(fs.is_open()) {
		String folder;
		vector<Mat> samples;
		vector<Mat> labels;
		while(getline(fs,folder)) {
			this->importSamples(folder,samples,size);
		}
		this->importLabels(labelsPath,labels);
		if(samples.size()==labels.size()) {
			Mat mData(samples.size(),samples.at(0).total(),CV_32F,Scalar(0));
			Mat mLabels(labels.size(),labels.at(0).cols,CV_32F,Scalar(0));
			int x=0;
			for(unsigned int i=0; i<samples.size(); i++) {
				Mat samp = samples.at(i);
				Mat lbl = labels.at(i);
				//samp = this->tempFixPrepareImg(samp);
				for(int j=0; j<samp.rows; j++) {
					for(int k=0; k<samp.cols; k++) {
						if(samp.type()==CV_8U)
							mData.at<float>(i,x) = samp.at<uchar>(j,k);
						else if(samp.type()==CV_32S)
							mData.at<float>(i,x) = samp.at<int>(j,k);
						if(samp.type()==CV_32F)
							mData.at<float>(i,x) = samp.at<float>(j,k);
						x++;
					}
				}
				x=0;
				for(int n=0; n<lbl.cols; n++) {
					mLabels.at<float>(i,n) = lbl.at<float>(0,n);
				}
			}
			this->data = mData;
			this->labels = mLabels;

		}
		else {
			printf("TestML::importTrainingData() error!\n");
			printf("Sample size != Label size\n");
			printf("Sample size: %lu\n",samples.size());
			printf("Label size: %lu\n",labels.size());
			fs.close();
			exit(1);
		}
		fs.close();
	}
}
