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
	vector<String> shapeNames = {"Strip","Default","Blotch","Incomp_Disc",
			"Comp_Disc","Incomp_Donut","Comp_Donut"};
public:
	bool shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum);
	void showIslands(vector<vector<vector<Islands> > > &islandVec);
	void printIslandAreas(vector<vector<vector<Islands> > > &islandVec);
	int numOfShapes();
	String shapeName(int num);
};

#endif /* SHAPEMATCH_H_ */
