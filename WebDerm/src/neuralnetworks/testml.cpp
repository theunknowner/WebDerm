/*
 * testml.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#include "testml.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"

bool TestML::THRESH_IMPORTED = false;
String TestML::PARAM_PATH = "Thresholds/param.xml";
vector<String> TestML::shapeNames;
Size TestML::img_size;

TestML::TestML() {
	assert(fs::exists(PARAM_PATH)==true);
	if(!TestML::THRESH_IMPORTED)
		TestML::THRESH_IMPORTED = this->importThresholds();
}

bool TestML::importThresholds() {
	fstream fs("Thresholds/shape_names.csv");
	fstream fs2("Thresholds/ml_nn_size.csv");
	if(fs.is_open() && fs2.is_open()) {
		String temp;
		vector<String> vec;
		while(getline(fs,temp)) {
			TestML::shapeNames.push_back(temp);
		}
		fs.close();
		while(getline(fs2,temp)) {
			getSubstr(temp,',',vec);
			TestML::img_size.width = atoi(vec.at(0).c_str());
			TestML::img_size.height = atoi(vec.at(1).c_str());
		}
		fs2.close();
		return true;
	} else {
		printf("TestML::importThreshold() failed, shape_names.csv does not exist!\n");
	}
	return false;
}

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
	Size _size = sample.size();
	if(size.width>0 && size.height>0)
		_size = size;
	//sample = this->convertToBinary(sample,0,255,0,1);
	Mat crop_img = fn.cropImage(sample);

	// get multiplier base on the biggest side
	int maxSize = max(crop_img.cols,crop_img.rows);
	float multiplier = (float)size.height / maxSize;

	// assign new size using multiplier
	int newRows = min((int)ceil(crop_img.rows * multiplier),size.height);
	int newCols = min((int)ceil(crop_img.cols * multiplier),size.width);
	_size = Size(newCols,newRows);
	Mat img;
	try {
		img = runResizeImage(crop_img,_size);
	} catch(cv::Exception &e) {
		printf("TestML::prepareImage(), runResizeImage() error!\n");
		cout << "Crop_img size: " << crop_img.size() << endl;
		cout << "Size: " << _size << endl;
		printf("Max Size: %d\n",maxSize);
		printf("Multiplier: %f\n",multiplier);
		exit(1);
	}
	img *= 255;

	//centers the feature
	Mat newImg = Mat::zeros(size,img.type());
	Point centerSize(floor(size.width)/2,floor(size.height/2));
	Point center(floor(img.cols/2),floor(img.rows/2));
	Point startPt(centerSize.x-center.x,centerSize.y-center.y);
	try {
		img.copyTo(newImg(Rect(startPt.x,startPt.y,img.cols,img.rows)));
	} catch(cv::Exception &e) {
		cout << "Orig Img Size: " << crop_img.size() << endl;
		printf("Max Size: %d\n",maxSize);
		printf("Multiplier: %f\n",multiplier);
		cout << "New Size: " << _size << endl;
		cout << "Img Size: " << img.size() << endl;
		cout << "New Img Size: " << newImg.size() << endl;
		cout << "CenterSize: " << centerSize << endl;
		cout << "Center: " << center << endl;
		cout << "StartPt: " << startPt << endl;
		exit(1);
	}
	return newImg;
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
		printf("TestML::prepareMatSamples() error!\n");
		cout << "input size != rows*cols" << endl;
		return Mat();
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

//! reads csv file containing path to files/folders
//! used for importing training data to train ANN
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

String TestML::getShapeName(int num) {
	return TestML::shapeNames[num];
}

int TestML::numOfShapes() {
	return TestML::shapeNames.size();
}

Size TestML::getSize() {
	return TestML::img_size;
}
