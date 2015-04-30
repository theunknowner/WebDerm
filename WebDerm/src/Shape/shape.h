/*
 * shape.h
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Shape {
public:
	void importPoints(String file, vector<vector<Point> > &points);
	void pointsToImage(Mat &img, vector<Point> points, int connect);
};

#endif /* SHAPE_H_ */
