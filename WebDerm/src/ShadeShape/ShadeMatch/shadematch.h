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
protected:
	int maxNumOfShades;
	enum {SHIFT_NONE=0, SHIFT_LEFT, SHIFT_RIGHT};
	vector<String> SHIFT = {"SHIFT_NONE","SHIFT_LEFT","SHIFT_RIGHT"};

public:
	bool shade_translation(vector<vector<vector<Islands> > > &islandVec, float thresh, int shiftType, int shiftAmt=1);
	bool shade_translation(vector<vector<vector<Islands> > > &islandVec, int shiftType, int shittAmt=1);

};

#endif /* SHADEMATCH_H_ */
