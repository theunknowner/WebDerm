/*
 * shapematch.h
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#ifndef SHAPEMATCH_H_
#define SHAPEMATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../shadeshape.h"
#include "../islands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/shapes.h"

class ShadeShape;
class Islands;
class ShapeMatch : public Shapes {
protected:
	static bool THRESH_IMPORTED;
	enum {SHIFT_NONE=0, SHIFT_LEFT, SHIFT_RIGHT};
	vector<String> _SHIFT = {"SHIFT_NONE","SHIFT_START"};

	static vector<float> shapeWeightsVec;

public:
	static vector<vector<String> > shiftingRules;
	static vector<vector<float> > shiftingPenalties;
	ShapeMatch();
	bool importThresholds();
	vector<vector<int> > getIslandVecIdxByArea(vector<vector<vector<Islands> > > &islandVec);
	bool shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int newShape,int rank=0);
	bool shape_translation2(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int newShape, int shiftAmt=1);
	void showIslands(vector<vector<vector<Islands> > > &islandVec);
	void printIslandAreas(vector<vector<vector<Islands> > > &islandVec);

	void moveShape(vector<vector<vector<Islands> > > &islandVec,int shapeNum, int shadeNum, int islNum, int newShape);
	int numOfShapes();
	vector<String> SHIFT();
	float applyShiftPenalty(float score, int shapeNum, int shapeNum2);
	float getShiftPenalty(int shapeNum, int shapeNum2);
	float applyShapeWeight(int shapeNum);
	static void printRules();
	static void printPenalties();
	static void printWeights();
};

#endif /* SHAPEMATCH_H_ */
