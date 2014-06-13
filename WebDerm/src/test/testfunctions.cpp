/*
 * testfunctions.cpp
 *
 *  Created on: Feb 17, 2014
 *      Author: jason
 */

#include "testfunctions.h"

void drawOutlineOnMouseEvent(int evt, int x, int y, int flags, void* param)
{
	Mat* img = (Mat*) param;
	static deque<Point> vecPoints;
	if(evt == CV_EVENT_LBUTTONDOWN)
	{
		if(vecPoints.size()==0)
		{
			vecPoints.push_back(Point(x,y));
		}
		else
		{
			vecPoints.push_back(Point(x,y));
			rectangle(*img,Rect(vecPoints.at(0),vecPoints.at(1)),Scalar(0,0,255));
		}
	}
}

void drawOutlineOnMouse(Mat &img)
{
	Mat img2 = img.clone();
	namedWindow("image");
	cvSetMouseCallback("image", drawOutlineOnMouseEvent, &img2);
	while(waitKey(1)<0)
	{
		imshow("image", img2);
	}
	waitKey(0);
}

void Mat2Image(deque< deque<double> > &input, Mat &output) {
	output = output.zeros(input.size(),input.at(0).size(),CV_8U);
	for(unsigned int i=0; i<input.size(); i++) {
		for(unsigned int j=0; j<input.at(i).size(); j++) {
			output.at<uchar>(i,j) = 255-input.at(i).at(j);
		}
	}
}
