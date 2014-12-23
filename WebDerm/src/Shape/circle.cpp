/*
 * circle.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#include "circle.h"

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

vector<vector<Point> > Circle::findContourPoints(Mat &img) {
	Mat dst,img2;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	GaussianBlur(img,img2,Size(3,3),0,0);
	Canny(img2,dst,100,200,3);
	findContours(dst,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point(0, 0));

	return contours;
}

Point Circle::getContourCenter(vector<vector<Point> > contours) {
	double dist=0, maxDist=0;
	Point beginPt(contours.at(0).at(0).x,contours.at(0).at(0).y);
	printf("beginPt: %d,%d\n",beginPt.x,beginPt.y);
	Point pt, endPt;
	for(unsigned int i=0; i<contours.size(); i++) {
		for(unsigned int j=0; j<contours.at(i).size(); j++) {
			pt = Point(contours.at(i).at(j).x,contours.at(i).at(j).y);
			dist = sqrt(pow(beginPt.x-pt.x,2)+pow(beginPt.y-pt.y,2));
			if(dist>maxDist) {
				maxDist = dist;
				endPt = pt;
			}
		}
	}
	int centerX = (beginPt.x+endPt.x)/2;
	int centerY = (beginPt.y+endPt.y)/2;
	Point center = Point(centerX,centerY);
	return center;
}

double Circle::findRadiusFromContours(vector<vector<Point> > contours) {
	double dist=0, maxDist=0;
	Point beginPt(contours.at(0).at(0).x,contours.at(0).at(0).y);
	printf("beginPt: %d,%d\n",beginPt.x,beginPt.y);
	Point pt, endPt;
	for(unsigned int i=0; i<contours.size(); i++) {
		for(unsigned int j=0; j<contours.at(i).size(); j++) {
			pt = Point(contours.at(i).at(j).x,contours.at(i).at(j).y);
			dist = sqrt(pow(beginPt.x-pt.x,2)+pow(beginPt.y-pt.y,2));
			if(dist>maxDist) {
				maxDist = dist;
				endPt = pt;
			}
		}
	}
	printf("endPt: %d,%d\n",endPt.x,endPt.y);
	double radius = maxDist/2;
	return radius;
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
