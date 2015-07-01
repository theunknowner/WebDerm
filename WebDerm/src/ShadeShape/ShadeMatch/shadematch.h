/*
 * shadematch.h
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#ifndef SHADEMATCH_H_
#define SHADEMATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../shadeshape.h"
#include "../islands.h"

class ShadeShape;
class Islands;
class ShadeMatch {
private:
	const int SHIFT_RIGHT = 1;
	const int SHIFT_LEFT = -1;
public:
	bool shade_translation(vector<vector<vector<Islands> > > &islandVec, float totalArea);
};

#endif /* SHADEMATCH_H_ */
