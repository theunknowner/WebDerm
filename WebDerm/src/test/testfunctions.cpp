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

Mat test_normalizeColors(Mat src) {
	Hsl hsl;
	vector<double> HSL;
	int *RGB;
	Mat dst = src.clone();
	int r,g,b;
	double normR, normG, normB;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			r = src.at<Vec3b>(i,j)[2];
			g = src.at<Vec3b>(i,j)[1];
			b = src.at<Vec3b>(i,j)[0];
			normR = r/sqrt(pow(r,2)+pow(g,2)+pow(b,2));
			normG = g/sqrt(pow(r,2)+pow(g,2)+pow(b,2));
			normB = b/sqrt(pow(r,2)+pow(g,2)+pow(b,2));
			normR = (int)round(normR*normR);
			normG = (int)round(normG*normG);
			normB = (int)round(normB*normB);
			HSL = hsl.rgb2hsl(normR,normG,normB);
			//HSL[1] += 0.2;
			RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
			dst.at<Vec3b>(i,j)[2] = RGB[0];
			dst.at<Vec3b>(i,j)[1] = RGB[1];
			dst.at<Vec3b>(i,j)[0] = RGB[2];
		}
	}
	return dst;
}

Mat test_correctGamma( Mat& img, double gamma ) {
	double inverse_gamma = 1.0 / gamma;

	Mat lut_matrix(1, 256, CV_8UC1 );
	uchar * ptr = lut_matrix.ptr();
	for( int i = 0; i < 256; i++ )
		ptr[i] = (int)( pow( (double) i / 255.0, inverse_gamma ) * 255.0 );

	Mat result;
	LUT( img, lut_matrix, result );

	return result;
}
