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

void graphHue(Mat &img) {
	hsl hsl;
	int r,g,b;
	double hue=0;
	deque<double> hueVals;
	deque< deque<double> > hueVec;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			hsl.rgb2hsl(r,g,b);
			hue = (hsl.getHue()+180)%360;
			hue /= 360;
			hueVals.push_back(hue);
		}
		hueVec.push_back(hueVals);
		hueVals.clear();
	}
	writeSeq2File(hueVec,"huegraph");
}
