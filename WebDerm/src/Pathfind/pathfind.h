/*
 * pathfind.h
 *
 *  Created on: Dec 3, 2015
 *      Author: jason
 */

#ifndef PATHFIND_H_
#define PATHFIND_H_

#include "../global.h"

class Pathfind {
private:
	vector<Point> pointVec;
	bool pathFound;

	int manhattanDist(Point pt1, Point pt2);
	vector<Point> getAdjacentPoints(Mat &src, Point pt);
public:
	//ndir = 4/8, 8 includes diagonals
	Mat run(Mat src, Point start, Point end, int NDIR=4, int steps=-1);
	vector<Point> getPathPoints();
	bool isPathFound();
};

#endif /* PATHFIND_H_ */
