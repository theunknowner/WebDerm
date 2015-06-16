/*
 * shapematch.h
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#ifndef SHAPEMATCH_H_
#define SHAPEMATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class ShapeMatch {
private:
	vector<vector<float> > shapeMatchThresh;
public:
	bool importThresholds();
	void printThresholds();
};

#endif /* SHAPEMATCH_H_ */
