/*
 * pathfind.cpp
 *
 *  Created on: Dec 3, 2015
 *      Author: jason
 */

#include "pathfind.h"
#include "node.h"
#include "queue"

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

Mat Pathfind::run(Mat src, Point start, Point end, int NDIR, int steps) {
	int iDir[NDIR];
	int jDir[NDIR];
	const int four = 4;
	const int eight = 8;
	if(NDIR == four) {
		const int yDir[four] = {1, 0, -1, 0};
		const int xDir[four] = {0, 1, 0, -1};
		for(int i=0; i<NDIR; i++) {
			iDir[i] = yDir[i];
			jDir[i] = xDir[i];
		}
	}

	if(NDIR == eight) {
		const int yDir[eight] = {1, 1, 0, -1, -1, -1, 0, 1};
		const int xDir[eight] = {0, 1, 1, 1, 0, -1, -1, -1};
		for(int i=0; i<NDIR; i++) {
			iDir[i] = yDir[i];
			jDir[i] = xDir[i];
		}
	}

	int openNodes[src.rows][src.cols];
	int closeNodes[src.rows][src.cols];
	// map of directions (0: Right, 1: Up, 2: Left, 3: Down)
	int dirMap[src.rows][src.cols];

	priority_queue<Node> q[2];
	int qi = 0;
	Node *pNode1;
	Node *pNode2;
	int row=0, col=0;

	Mat results;
	cvtColor(src,results,CV_GRAY2BGR);
	results.at<Vec3b>(start) = Vec3b(0,0,255);
	results.at<Vec3b>(end) = Vec3b(255,0,0);
	this->pointVec.push_back(start);
	this->pathFound = false;

	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			openNodes[i][j] = 0;
			closeNodes[i][j] = 0;
		}
	}

	// create the start node and push into list of open nodes
	pNode1 = new Node(start, 0, 0);
	pNode1->calculateFValue(end);
	q[qi].push(*pNode1);

	while(!q[qi].empty()) {
		// get the current node w/ the lowest FValue
		// from the list of open nodes
		pNode1 = new Node( q[qi].top().getPos(), q[qi].top().getGValue(), q[qi].top().getFValue());
		row = pNode1->getPos().y;
		col = pNode1->getPos().x;

		if(pNode1->getGValue()==steps) {
			return results;
		}

		// remove the node from the open list
		q[qi].pop();
		openNodes[row][col] = 0;

		// mark it on the closed nodes list
		closeNodes[row][col] = 1;

		// stop searching when the goal state is reached
		if(row == end.y && col == end.x) {
			// generate the path from finish to start from dirMap
			while(!(row == start.y && col == start.x)) {
				int j = dirMap[row][col];
				row += iDir[j];
				col += jDir[j];

				if(row!=start.y && col!=start.x) {
					results.at<Vec3b>(row,col) = Vec3b(0,255,0);
					this->pointVec.push_back(Point(col,row));
				}
			}
			this->pointVec.push_back(end);

			// garbage collection
			delete pNode1;

			// empty the leftover nodes
			while(!q[qi].empty()) q[qi].pop();

			this->pathFound = true;
			return results;
		}

		// generate moves in all possible directions
		for(int i = 0; i < NDIR; i++) {
			int iNext = row + iDir[i];
			int jNext = col + jDir[i];

			// if not wall (obstacle) nor in the closed list
			if(!(iNext < 0 || iNext > src.rows - 1 || jNext < 0 || jNext > src.cols - 1 ||
					src.at<uchar>(iNext,jNext)==0 || closeNodes[iNext][jNext] == 1)) {

				// generate a child node
				pNode2 = new Node( Point(jNext, iNext), pNode1->getGValue(), pNode1->getFValue());
				pNode2->updateGValue(i);
				pNode2->calculateFValue(end);

				// if it is not in the open list then add into that
				if(openNodes[iNext][jNext] == 0) {
					openNodes[iNext][jNext] = pNode2->getFValue();
					q[qi].push(*pNode2);
					// mark its parent node direction
					dirMap[iNext][jNext] = (i + NDIR/2) % NDIR;
				}

				// already in the open list
				else if(openNodes[iNext][jNext] > pNode2->getFValue()) {
					// update the FValue info
					openNodes[iNext][jNext] = pNode2->getFValue();

					// update the parent direction info,  mark its parent node direction
					dirMap[iNext][jNext] = (i + NDIR/2) % NDIR;

					// replace the node by emptying one q to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while(!(q[qi].top().getPos().y == iNext && q[qi].top().getPos().x == jNext)) {
						q[1 - qi].push(q[qi].top());
						q[qi].pop();
					}

					// remove the wanted node
					q[qi].pop();

					// empty the larger size q to the smaller one
					if(q[qi].size() > q[1 - qi].size()) qi = 1 - qi;
					while(!q[qi].empty()) {
						q[1 - qi].push(q[qi].top());
						q[qi].pop();
					}
					qi = 1 - qi;

					// add the better node instead
					q[qi].push(*pNode2);
				}
				else delete pNode2;
			} // end if not obstacle or closed list
		} // end for i < NDIR
		delete pNode1;
	} // end while not empty
	return results;
}

vector<Point> Pathfind::getPathPoints() {
	return this->pointVec;
}

bool Pathfind::isPathFound() {
	return this->pathFound;
}
