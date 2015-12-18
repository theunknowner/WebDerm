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
	vector<int> shadeVec;
	vector<String> _SHIFT = {"SHIFT_NONE","SHIFT_LEFT","SHIFT_RIGHT"};

	vector<vector<int> > featIslIdxStoreVec;
	bool featIslStored = false;

	static vector<float> shadeWeightsVec;

public:
	enum {SHIFT_NONE=0, SHIFT_LEFT=1, SHIFT_RIGHT=2};
	void setMaxShades(vector<int> shadeVec1, vector<int> shadeVec2);
	bool shade_translation1(ShadeShape &ss, int shiftType, int shiftAmt=1);
	vector<vector<int> >& getStoredFeatIslIdx();
	bool& isFeatIslStored();
	vector<String> SHIFT();
	float applyShiftPenalty(ShadeShape &ss, float score, int shiftAmt=1);
	vector<vector<vector<Islands> > > shiftShades(vector<vector<vector<Islands> > > &islandVec, int shiftType);
	float applyShadeWeights(int shadeLevel);
	bool shade_translation2(ShadeShape &ss, int shiftType, int shiftAmt=1);

};

#endif /* SHADEMATCH_H_ */
