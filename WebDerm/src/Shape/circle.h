/*
 * circle.h
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "../global.h"

class Circle {
public:
	Mat findCircles(Mat &img);
	vector<vector<Point> > getContours(Mat img);
	Point2f getContourCenter(vector<vector<Point> > contours);
	double findRadiusFromContours(vector<vector<Point> > contours);
	double getContourArea(vector<vector<Point> > contours);
	double getContourPerimeter(vector<vector<Point> > contours);
	double getRoundness(vector<vector<Point> > contours);

	void generateCirclePoints(vector<Point> &points, double radius, double degree, int numberOfPoints);
	void generateRandomPoints(vector<Point> &points, double radius, double spread, double degree, int numberOfPoints);
	void generateEllipsePoints(vector<Point> &points, double radius1, double radius2, double degree, int numberOfPoints);
};

#endif /* CIRCLE_H_ */
