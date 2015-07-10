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
	enum {SHIFT_NONE=0, SHIFT_LEFT, SHIFT_RIGHT};
	vector<String> SHIFT = {"SHIFT_NONE","SHIFT_LEFT","SHIFT_RIGHT"};
	vector<String> shapeNames = {"Strip","Excavated","Default","Blotch","Incomp_Disc",
			"Comp_Disc","Incomp_Donut","Comp_Donut"};
public:
	bool shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int shiftType);
	void showIslands(vector<vector<vector<Islands> > > &islandVec);
	void printIslandAreas(vector<vector<vector<Islands> > > &islandVec);
	int numOfShapes();
	String shapeName(int num);
	void moveShape(vector<vector<vector<Islands> > > &islandVec,int shapeNum, int shadeNum, int islNum, int newShape);
};

#endif /* SHAPEMATCH_H_ */
