/*
 * shapemorph.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: jason
 */

#include "shapemorph.h"

Mat prepareImage(Mat src) {
	Mat dst(src.rows,src.cols,src.type());
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			dst.at<uchar>(i,j) = 255 - src.at<uchar>(i,j);
		}
	}
	return dst;
}

Mat ShapeMorph::findShapes(Mat src) {
	Size size(3,3);
	Mat dst, dImg, eImg;
	Mat _src = prepareImage(src);
	Point anchor(0,size.height-1);
	//Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	//morphologyEx(_src,dst,MORPH_GRADIENT,element,anchor);
	this->erosion(_src,eImg,size,Point(0,size.height-1));
	this->dilation(_src,dImg,size,Point(0,size.height-1));
	//this->erosion2(dImg,eImg,size,Point(size.width-1,0));
	dst = dImg - eImg;
	dst = this->uniqueLumPercentile(dst,0.30);
	return dst;
}

void ShapeMorph::dilation(Mat src, Mat &dst, Size size, Point anchor) {
/*
	Mat dst = Mat::zeros(src.rows,src.cols, CV_8U);
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMax(window);
			dst.at<uchar>(row+size.height-1,col) = lum;
			col++;
		}
		col=0;
		row++;
	}*/
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	dilate(src,dst,element,anchor);
}

void ShapeMorph::erosion(Mat src, Mat &dst, Size size, Point anchor) {
	/*
	Mat results = Mat::zeros(src.rows,src.cols, src.type());
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMin(window);
			results.at<uchar>(row+size.height-1,col) = lum;
			col++;
		}
		col=0;
		row++;
	}
	dst = results;*/
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	erode(src,dst,element,anchor);
}

void ShapeMorph::erosion2(Mat src, Mat &dst, Size size, Point anchor) {
	/*
	Mat results = Mat::zeros(src.rows,src.cols, src.type());
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMin(window);
			results.at<uchar>(row+size.height-1,col) = lum;
			col++;
		}
		col=0;
		row++;
	}
	dst = results;*/
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	erode(src,dst,element,anchor);
}

Mat ShapeMorph::uniqueLumPercentile(Mat src, double percentile) {
	Mat dst = Mat::zeros(src.rows,src.cols,src.type());
	int lum=0;
	deque<int> lumsFlag(256,0);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			lum = src.at<uchar>(i,j);
			if(lum>0)
				lumsFlag.at(lum) = 1;
		}
	}
	deque<int> lums;
	for(unsigned int i=0; i<lumsFlag.size(); i++) {
		if(lumsFlag.at(i)!=0) {
			lums.push_back(i);
		}
	}
	int n = percentile * lums.size();
	int thresh = lums.at(n);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			lum = src.at<uchar>(i,j);
			if(lum>thresh)
				dst.at<uchar>(i,j) = lum;
		}
	}
	return dst;
}
