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
