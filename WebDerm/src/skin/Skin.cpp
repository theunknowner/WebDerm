/*
 * Skin.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "Skin.h"

/* Fills in holes in the image */
void imfill(Mat &img)
{
	vector<vector<Point> > contours; 	//holds all the contour points
	vector<Vec4i> hierarchy;
	findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	Mat drawing = Mat::zeros( img.size(), CV_8U );
	drawContours( drawing, contours, -1, Scalar(255), CV_FILLED, 8, hierarchy, 1, Point(0,0) );
	img.release();
	img = drawing.clone();
	drawing.release();
}

void getSkinUsingThresh(Mat &img, Mat &mask)
{
	mask = mask.zeros(img.rows,img.cols,CV_8U);
	int r,g,b;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			b = img.at<Vec3b>(row,col)[0];
			g = img.at<Vec3b>(row,col)[1];
			r = img.at<Vec3b>(row,col)[2];
			if(r>g && g>b)
			{
				if((b+g)<(2*r) && ((r-g) > 20 || (r-b) > 20) && r>95 && b>55 && b<200 && g>40 && g<200)
				{
					mask.at<uchar>(row,col) = 255;
				}
			}
		}
	}
	imfill(mask);
}

void getSkinUsingCorrelation(Mat &img, Mat &mask)
{
	mask = mask.zeros(img.rows,img.cols,CV_8U);
	double thresh = 0.038;
	double vec2[3] = {221,183,160}; //skin values for correlation
	int r,g,b;
	double dist=0;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			b = img.at<Vec3b>(row,col)[0];
			g = img.at<Vec3b>(row,col)[1];
			r = img.at<Vec3b>(row,col)[2];
			double vec1[3] = {(double)r,(double)g,(double)b};
			dist = correlationDist(vec1,vec2);
			if(dist<thresh)
			{
				mask.at<uchar>(row,col) = 255;
			}
		}
	}
	imfill(mask);
}

void getSkin(Mat &img, Mat &mask)
{

	double skinCount=(img.rows*img.cols)*0.2; //20% of 700x700
	getSkinUsingThresh(img,mask);
	if(countNonZero(mask)<skinCount)
	{
		mask.release();
		getSkinUsingCorrelation(img,mask);
	}
}
