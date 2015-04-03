/*
 * createtrainingdata.cpp
 *
 *  Created on: Apr 1, 2015
 *      Author: jason
 */

#include "createtrainingdata.h"

Mat CreateTrainingData::segmentImage(Mat input, Size size) {
	Mat segmentImg;
	cvtColor(input,segmentImg,CV_GRAY2BGR);
	for(int row=0; row<input.rows; row+=size.height) {
		for(int col=0; col<input.cols; col+=size.width) {
			rectangle(segmentImg,Point(col,row),Point(col+size.width-1,row+size.height-1),Scalar(0,0,255));
		}
	}
	return segmentImg;
}

Mat CreateTrainingData::maxDimensionCrop(Mat input) {
	Functions fn;
	Mat cropImg = fn.cropImage(input);
	int maxDim = max(cropImg.rows, cropImg.cols);
	Mat scaledImg = Mat::zeros(maxDim,maxDim,input.type());
	for(int i=0; i<cropImg.rows; i++) {
		for(int j=0; j<cropImg.cols; j++) {
			scaledImg.at<uchar>(i,j) = cropImg.at<uchar>(i,j);
		}
	}
	return scaledImg;
}

String globalName;
Mat globalInputImg;
Size globalSize;
void onMouseClickSelectSegment(int event, int x, int y, int flags, void* param) {
	Mat *img = (Mat*) param;
	static Point pt;
	if  ( event == EVENT_LBUTTONDOWN ){
		pt = Point(floor(x/globalSize.width)*globalSize.width,floor(y/globalSize.height)*globalSize.height);
		rectangle(*img,pt,Point(pt.x+globalSize.width-1,pt.y+globalSize.height-1),Scalar(0,255,0));
		Mat segment = globalInputImg(Rect(pt,Point(pt.x+globalSize.width,pt.y+globalSize.height)));
		String filename = globalName+"-Point("+toString(pt.x)+","+toString(pt.y)+").png";
		cout << "Writing " << filename << "..." << flush;
		imwrite(filename,segment);
		cout << "Done!" << endl;
	}
	if(event==EVENT_LBUTTONUP) {
		rectangle(*img,pt,Point(pt.x+globalSize.width-1,pt.y+globalSize.height-1),Scalar(0,0,255));
	}
	imshow(globalName,*img);
}

void CreateTrainingData::mouseSelectSegment(Mat input, Size size, String name) {
	Mat segmentImg = this->segmentImage(input,size);
	globalInputImg = input.clone();
	globalName = name;
	globalSize = size;
	namedWindow(name, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	cvSetMouseCallback(name.c_str(), onMouseClickSelectSegment, &segmentImg);
	imshow(name,segmentImg);
	waitKey(0);
}

void CreateTrainingData::makeFalsePairs(String directory, String file, int pairNum, vector<String> excludeList) {
	deque<String> files;
	FileData fd;
	String outputDir = "/home/jason/Desktop/Programs/Training Samples/Test/";
	bool flag = fd.getFilesFromDirectory(directory,files);
	if(flag) {
		Mat img1 = imread(file,0);
		for(unsigned int i=0; i<files.size(); i++) {
			bool flag = false;
			for(unsigned int j=0; j<excludeList.size(); j++) {
				size_t pos = files.at(i).find(excludeList.at(j));
				if(pos!=string::npos) {
					flag=true;
					break;
				}
			}
			if(!flag) {
				Mat img2 = imread(directory+files.at(i),0);
				String name1 = outputDir + "pair_" + toString(pairNum) + "a.png";
				String name2 = outputDir + "pair_" + toString(pairNum) + "b.png";
				imwrite(name1,img1);
				imwrite(name2,img2);
				pairNum++;
			}
		}
	}
}

void CreateTrainingData::makeTruePairs(String directory, String file, int pairNum, vector<String> includeList) {
	deque<String> files;
	FileData fd;
	String outputDir = "/home/jason/Desktop/Programs/Training Samples/Test/";

	bool flag = fd.getFilesFromDirectory(directory,files);
	if(flag) {
		Mat img1 = imread(file,0);
		for(unsigned int i=0; i<files.size(); i++) {
			bool flag = false;
			for(unsigned int j=0; j<includeList.size(); j++) {
				size_t pos = files.at(i).find(includeList.at(j));
				if(pos!=string::npos) {
					flag=true;
					break;
				}
			}
			if(flag) {
				Mat img2 = imread(directory+files.at(i),0);
				String name1 = outputDir + "pair_" + toString(pairNum) + "a.png";
				String name2 = outputDir + "pair_" + toString(pairNum) + "b.png";
				imwrite(name1,img1);
				imwrite(name2,img2);
				pairNum++;
			}
		}
	}
}

Mat CreateTrainingData::stitchData(Mat input1, Mat input2) {
	Mat result;
	hconcat(input1,input2,result);
	return result;
}

void CreateTrainingData::generateStitchPairs(String inDir, String outDir) {
	deque<String> files;
	FileData fd;
	fd.getFilesFromDirectory(inDir,files);
	sort(files.begin(),files.end());
	String start = "_";
	String end = ".";
	int count=1;
	Mat src1, src2;
	for(unsigned int i=0; i<files.size(); i+=2) {
		size_t pos1 = files.at(i).find(start)+1;
		size_t pos2 = files.at(i).find(end)-1;
		String num = files.at(i).substr(pos1,pos2-pos1);

		pos1 = files.at(i+1).find(start)+1;
		pos2 = files.at(i+1).find(end)-1;
		String num2 = files.at(i+1).substr(pos1,pos2-pos1);
		if(num==num2 && files.at(i).find("a")!=string::npos && files.at(i+1).find("b")!=string::npos) {
			try {
				src1 = imread(inDir+files.at(i),0);
				src2 = imread(inDir+files.at(i+1),0);
				Mat result = this->stitchData(src1,src2);
				imwrite(outDir+"sample_"+toString(count)+".png",result);
				count++;
			} catch (cv::Exception &e) {
				printf("%s\n",files.at(i).c_str());
				printf("Src1 type: %d\n",src1.type());
				printf("Src1 size: %dx%d\n",src1.rows, src1.cols);
				printf("%s\n",files.at(i+1).c_str());
				printf("Src2 type: %d\n",src2.type());
				printf("Src2 size: %dx%d\n",src2.rows, src2.cols);
				exit(1);
			}
		}
		else {
			printf("%s\n",files.at(i).c_str());
			printf("%s\n",files.at(i+1).c_str());
		}
	}
}
