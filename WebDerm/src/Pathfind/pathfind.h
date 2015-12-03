/*
 * pathfind.h
 *
 *  Created on: Dec 3, 2015
 *      Author: jason
 */

#ifndef PATHFIND_H_
#define PATHFIND_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Pathfind {
private:
	int manhattanDist(Point pt1, Point pt2);
	vector<Point> getAdjacentPoints(Mat &src, Point pt);
public:
	Mat run(Mat src, Point start, Point end);
};

#endif /* PATHFIND_H_ */
