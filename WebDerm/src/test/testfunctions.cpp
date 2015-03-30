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
	vector<int> RGB;
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

double test_inverseGamma(int val) {
	double c = val/255.0;
	if ( c <= 0.04045 )
		return c/12.92;
	else
		return pow(((c+0.055)/(1.055)),2.4);
}

int test_Gamma(double val) {
	if(val<=0.0031308)
		val *= 12.92;
	else
		val = 1.055*pow(val,1.0/2.4)-0.055;
	return int(val*255+.5);
}

int test_gray(int r, int g, int b) {
	const double rY = 0.212655;
	const double gY = 0.715158;
	const double bY = 0.072187;
	return test_Gamma(rY*test_inverseGamma(r)+gY*test_inverseGamma(g)+bY*test_inverseGamma(b));
}

Mat input, dst, gray, edges;
int lowThreshold;
int const max_lowThreshold = 200;
int ratio = 1;
int kernel_size = 3;
String window_name = "Edge Map";
void CannyThreshold(int, void*) {
	/// Reduce noise with a kernel 3x3
	blur(gray, edges, Size(3,3) );
	/// Canny detector
	Canny(edges, edges, lowThreshold, lowThreshold*ratio, kernel_size );

	input.copyTo(dst,edges);
	imshow(window_name,edges);
}
void test_CannyEdge(Mat src) {
	input = src.clone();
	cvtColor(src,gray,CV_BGR2GRAY);
	namedWindow( window_name, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED );
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	CannyThreshold(0,0);
	waitKey(0);
}
