/*
 * pathfind.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: jason
 */

#include "pathfind.h"

//! city-block distance
int Pathfind::manhattanDist(Point pt1, Point pt2) {
	int dist = abs(pt1.x-pt2.x) + abs(pt1.y-pt2.y);
	return dist;
}

vector<Point> Pathfind::getAdjacentPoints(Mat &src, Point pt) {
	vector<Point> adjacentPoints;
	Point top(pt.x,pt.y-1);
	Point right(pt.x+1,pt.y);
	Point down(pt.x,pt.y+1);
	Point left(pt.x-1,pt.y);
	if(top.y>=0 && src.at<uchar>(top)>0)
		adjacentPoints.push_back(top);
	if(right.x<src.cols && src.at<uchar>(right)>0)
		adjacentPoints.push_back(right);
	if(down.y<src.rows && src.at<uchar>(down)>0)
		adjacentPoints.push_back(down);
	if(left.x>=0 && src.at<uchar>(left)>0)
		adjacentPoints.push_back(left);

	return adjacentPoints;
}

Mat Pathfind::run(Mat src, Point start, Point end) {
	deque<Point> openList;
	deque<Point> closedList;
	deque<int> fValList;
	deque<int> hValList;
	deque<int> gValList;
	openList.push_back(start);
	int gVal = this->manhattanDist(start,openList.front());
	int hVal = this->manhattanDist(openList.front(),end);
	int fVal = gVal + hVal;
	gValList.push_back(gVal);
	hValList.push_back(hVal);
	fValList.push_back(fVal);
	while(!openList.empty()) {
		vector<Point> adjPoints = this->getAdjacentPoints(src,openList.front());
		for(unsigned int i=0; i<adjPoints.size(); i++) {
			gVal = this->manhattanDist(start,adjPoints.at(i));
			hVal = this->manhattanDist(adjPoints.at(i),end);
			fVal = gVal + hVal;
		}
		closedList.push_back(openList.front());
		openList.pop_front();
	}
}
