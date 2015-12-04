/*
 * node.h
 *
 *  Created on: Dec 4, 2015
 *      Author: jason
 */

#ifndef NODE_H_
#define NODE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Node {
	Point currPos;
	int gVal;
	int fVal;
public:
	Node(const Point &pos, int g, int f) {
		this->currPos = pos;
		this->gVal = g;
		this->fVal = f;
	}

	Point getPos() const {
		return this->currPos;
	}

	int getGValue() const {
		return this->gVal;
	}

	int getFValue() const {
		return this->fVal;
	}

	void calculateFValue(Point &dest) {
		this->fVal = this->getHValue(dest) + this->gVal;
	}

	void updateGValue(const int &i) {
		//this->gVal += (NDIR == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
		this->gVal++;
	}

	const int& getHValue(Point &dest) const {
		int dist = abs(this->currPos.x-dest.x) + abs(this->currPos.y-dest.y);
		return (dist);
	}
	// Determine FValue (in the priority queue)
	friend bool operator<(const Node & a, const Node & b)
	{
	    return a.getFValue() > b.getFValue();
	}
};

#endif /* NODE_H_ */
