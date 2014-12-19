/*
 * circle.h
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class Circle {
public:
	Mat findCircles(Mat &img);
	vector<vector<Point> > findContourPoints(Mat &img);
	Point getContourCenter(vector<vector<Point> > contours);
	double findRadiusFromContours(vector<vector<Point> > contours);
	void importPoints(String file, vector<vector<Point> > &points);

	void generateCirclePoints(deque<Point> &points, double radius, double degree, int numberOfPoints);
	void generateRandomPoints(deque<Point> &points, double radius, double spread, double degree, int numberOfPoints);
	void generateEllipsePoints(deque<Point> &points, double radius1, double radius2, double degree, int numberOfPoints);
};

#endif /* CIRCLE_H_ */
