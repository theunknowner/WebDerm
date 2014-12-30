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
	//this->erosion(_src,eImg,size,Point(0,size.height-1));
	//this->dilation(_src,dImg,size,Point(0,size.height-1));
	//this->erosion2(dImg,eImg,size,Point(size.width-1,0));
	//dst = dImg - eImg;
	//Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	//morphologyEx(dst,dst,MORPH_CLOSE,element,anchor);
	this->hysteresisDilation(_src,dImg,size,anchor);
	this->hysteresisErosion(_src,eImg,size,anchor);
	dst = dImg - eImg;
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	morphologyEx(dst,dst,MORPH_CLOSE,element,anchor);
	this->uniqueLumPercentile(dst,0.40);
	//imshow("img2",dst);
	imwrite("shapemorph.png",dst);
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
	deque<int> dataVec;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			lum = src.at<uchar>(i,j);
			dataVec.push_back(lum);
			if(lum>0)
				lumsFlag.at(lum)++;
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

Mat ShapeMorph::extractShape(Mat src, Size size, Point anchor) {
	double dist=5.0;
	deque<deque<Point> > shapeVec;
	deque<Point> ptVec;
	Mat ptMap = Mat::zeros(src.rows,src.cols,CV_8U);
	Mat window;
	int right=0,down=0,left=0,up=0;
	if(anchor==Point(-1,-1)) anchor = Point(floor(size.width/2),floor(size.height/2));
	anchor = Point(0,1);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			if(ptMap.at<uchar>(i,j)==0) {
				int x = i;
				int y = j;
				window = src(Rect(x,y,size.width,size.height));
				if(countNonZero(window.row(anchor.y))>=size.width) {
					right=1;
					while(right==1) {
						//if()
						for(int k=x; k<(x+size.width); k++) {
							if(ptMap.at<uchar>(y+anchor.y,k)==0)
								ptVec.push_back(Point(k,y+anchor.y));
						}
						if(countNonZero(window.col(size.width-1))<=round(size.height/2)) {
							down=1;
						}
						if(down==1) {
							//do something
						}
						x++;
						window = src(Rect(x,y,size.width,size.height));
					}
				}
			}
		}
	}
}

void ShapeMorph::hysteresisDilation(Mat src, Mat &dst, Size size, Point anchor) {
	double h=0.5, scale=1.5;
	Mat window = Mat::zeros(src.rows, src.cols, src.type());
	//Mat results = Mat::zeros(src.rows,src.cols, src.type());
	Mat results = src.clone();
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	if(anchor==Point(-1,-1)) anchor = Point(floor(size.width/2),floor(size.height/2));
	int lum0=-1,lum45=-1,lum90=-1;
	Point pt0, pt45, pt90;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMax(window);
			pt0 = Point(col+anchor.x-1,row+anchor.y);
			pt45 = Point(col+anchor.x-1,row+anchor.y-1);
			pt90 = Point(col+anchor.x,row+anchor.y-1);
			if(pt0.y>=0 && pt0.x>=0) {
				lum0 = results.at<uchar>(pt0.y,pt0.x);
			}
			if(pt45.y>=0 && pt45.x>=0) {
				lum45 = results.at<uchar>(pt45.y,pt45.x);
			}
			if(pt90.y>=0 && pt90.x>=0) {
				lum90 = results.at<uchar>(pt90.y,pt90.x);
			}
			if(lum0>=0 && lum45>=0 && lum90>=0) {
				double avg = ((lum0+lum45+lum90)/3.0);
				double newLum = h*lum*scale+(1.0-h)*avg;
				lum = round(newLum);
				if(lum>255) lum=255;
				if(lum<0) lum=0;
			}
			results.at<uchar>(row+anchor.y,col) = lum;
			col++;
			lum0=-1; lum45=-1; lum90=-1;
		}
		col=0;
		row++;
	}
	dst = results;
}

void ShapeMorph::hysteresisErosion(Mat src, Mat &dst, Size size, Point anchor) {
	double h=0.5, scale=1.5;
	Mat window = Mat::zeros(src.rows, src.cols, src.type());
	Mat results = src.clone();
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	if(anchor==Point(-1,-1)) anchor = Point(floor(size.width/2),floor(size.height/2));
	int lum0=-1,lum45=-1,lum90=-1;
	Point pt0, pt45, pt90;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMin(window);
			pt0 = Point(col+anchor.x-1,row+anchor.y);
			pt45 = Point(col+anchor.x-1,row+anchor.y-1);
			pt90 = Point(col+anchor.x,row+anchor.y-1);
			if(pt0.y>=0 && pt0.x>=0) {
				lum0 = results.at<uchar>(pt0.y,pt0.x);
			}
			if(pt45.y>=0 && pt45.x>=0) {
				lum45 = results.at<uchar>(pt45.y,pt45.x);
			}
			if(pt90.y>=0 && pt90.x>=0) {
				lum90 = results.at<uchar>(pt90.y,pt90.x);
			}
			if(lum0>=0 && lum45>=0 && lum90>=0) {
				double avg = ((lum0+lum45+lum90)/3.0);
				double newLum = h*lum*scale+(1.0-h)*avg;
				lum = round(newLum);
				if(lum>255) lum=255;
				if(lum<0) lum=0;
			}
			results.at<uchar>(row+anchor.y,col) = lum;
			col++;
			lum0=-1; lum45=-1; lum90=-1;
		}
		col=0;
		row++;
	}
	dst = results;
}
