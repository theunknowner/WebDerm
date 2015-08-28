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

class ShadeShape;
class Islands;
class ShapeMatch {
protected:
	static bool THRESH_IMPORTED;
	enum {SHIFT_NONE=0, SHIFT_LEFT, SHIFT_RIGHT};
	vector<String> SHIFT = {"SHIFT_NONE","SHIFT_START"};
	static vector<String> shapeNames;
	static vector<vector<String> > shiftingRules;
	static vector<vector<float> > shiftingPenalties;

public:
	ShapeMatch();
	bool importThresholds();
	bool shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int shiftType);
	bool shape_translation2(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int newShape);
	void showIslands(vector<vector<vector<Islands> > > &islandVec);
	void printIslandAreas(vector<vector<vector<Islands> > > &islandVec);
	int numOfShapes();
	String shapeName(int num);
	int getShapeIndex(String shape);
	void moveShape(vector<vector<vector<Islands> > > &islandVec,int shapeNum, int shadeNum, int islNum, int newShape);
	static void printRules();
	static void printPenalties();
	static float applyShiftPenalty(float score, int shapeNum, int shapeNum2);
	static float getShiftPenalty(int shapeNum, int shapeNum2);
};

#endif /* SHAPEMATCH_H_ */
