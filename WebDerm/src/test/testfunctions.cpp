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
	static vector<Point> vecPoints;
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
