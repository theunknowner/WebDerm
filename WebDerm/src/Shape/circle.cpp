/*
 * circle.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#include "circle.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

Mat Circle::findCircles(Mat & img) {
	Mat dst,img2;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	GaussianBlur(img,img2,Size(3,3),0,0);
	Canny(img2,dst,100,200,3);
	findContours(dst,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point(0, 0));
	Mat output(img.size(),CV_8U);
	for(unsigned int i=0; i<contours.size(); i++) {
		drawContours(output,contours,i,Scalar(255));
	}
	return output;
}

//! gets contour of boundary of image
vector<vector<Point> > Circle::getContours(Mat img) {
	Mat dst,img2;
	vector<vector<Point> > contours;
	GaussianBlur(img,img2,Size(3,3),0,0);
	Canny(img2,dst,100,200,3);
	findContours(dst,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

	return contours;
}

//! returns point of center in float format
Point2f Circle::getContourCenter(vector<vector<Point> > contours) {
	vector<Moments> mu(contours.size());
	for(unsigned int i = 0; i < contours.size(); i++ ) {
		mu[i] = moments( contours[i], false );
	}
	vector<Point2f> mc( contours.size() );
	for(unsigned int i = 0; i < contours.size(); i++ ) {
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}
	Point2f center = mc.at(0);
	return center;
}

double Circle::findRadiusFromContours(vector<vector<Point> > contours) {
	double area = contourArea(contours.at(0));
	double radius = sqrt(area/M_PI);
	return radius;
}

double Circle::getContourArea(vector<vector<Point> > contours) {
	double area = contourArea(contours.at(0));
	return area;
}

double Circle::getContourPerimeter(vector<vector<Point> > contours) {
	double perimeter = arcLength(contours.at(0),true);
	return perimeter;
}

//! calculates roundness of contour
//! the max value = 1 -> perfect circle
double Circle::getRoundness(vector<vector<Point> > contours) {
	double area = this->getContourArea(contours);
	double perim = this->getContourPerimeter(contours);
	double roundness = (4.0*M_PI*area)/pow(perim,2);
	return roundness;
}
/*points are generated base on origin Point as (0,0)
 * So to display in an image, the points need to be offset to positive
 */
void Circle::generateCirclePoints(vector<Point> &points, double radius, double degree, int numberOfPoints) {
	double radToDeg = 3.14159/180.0;
	double angle=0;
	int x,y;
	for(int i=0; i<numberOfPoints; i++) {
		angle = i*degree;
		x = round(radius*cos(angle*radToDeg));
		y = round(radius*sin(angle*radToDeg));
		points.push_back(Point(x,y));
		//cout << x << "," << y << endl;
	}
}

/*points are generated base on origin Point as (0,0)
 * So to display in an image, the points need to be offset to positive
 */
void Circle::generateRandomPoints(vector<Point> &points, double radius, double spread, double degree, int numberOfPoints) {
	int newRadius = radius + spread;
	double radToDeg = 3.14159/180.0;
	double angle=0;
	int x,y;
	for(int i=0; i<numberOfPoints; i++) {
		angle = i*degree;
		if(i>0) {
			if(i%2==(rand()%2))
				newRadius -= (rand()%(int)radius+1);
			else
				newRadius += (rand()%(int)radius+1);
		}
		x = round(newRadius*cos(angle*radToDeg));
		y = round(newRadius*sin(angle*radToDeg));
		points.push_back(Point(x,y));
		//cout << x << "," << y << endl;
	}
}

/*points are generated base on origin Point as (0,0)
 * So to display in an image, the points need to be offset to positive
 */
void Circle::generateEllipsePoints(vector<Point> &points, double radius1, double radius2, double degree, int numberOfPoints) {
	double radToDeg = 3.14159/180.0;
	double angle=0;
	int x,y;
	double intervals = (radius1 - radius2)/(90.0/degree);
	for(int i=0; i<numberOfPoints; i++) {
		angle = i*degree;
		if((angle>0 && angle<=90) || (angle>180 && angle<=270)) {
			radius1 -= intervals;
		}
		if((angle>90 && angle<=180) || (angle>270 && angle<=360)) {
			radius1 += intervals;
		}
		x = round(radius1*cos(angle*radToDeg));
		y = round(radius1*sin(angle*radToDeg));
		points.push_back(Point(x,y));
		//cout << x << "," << y << endl;
	}
}
