/*
 * shapematch.h
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#ifndef SHAPEMATCH_H_
#define SHAPEMATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../ShadeShape/shadeshape.h"
#include "../ShadeShape/islands.h"

class ShadeShape;
class Islands;
class ShapeMatch {
private:
	vector<vector<vector<Islands> > > upIslandVec;
	vector<vector<vector<Islands> > > dbIslandVec;
	vector<vector<vector<Islands> > > groupIslandsByShade(ShadeShape &ss);
	vector<vector<vector<Islands> > > groupIslandsByShape(ShadeShape &ss);
	void sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec);
	vector<vector<vector<String> > > createLabels(vector<vector<vector<Islands> > > &islandVec);
	std::map<String,float> createPropAreaMap(vector<vector<vector<Islands> > > &islandVec, float totalArea);
	void fillPropAreaMapGaps(std::map<String,float> &upMap, std::map<String,float> &dbMap);
	float dotProduct(std::map<String,float> &upMap, std::map<String,float> &dbMap);
	void shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum);

	vector<String> shapeNames = {"Strip","Default","Blotch","Incomp_Disc",
			"Comp_Disc","Incomp_Donut","Comp_Donut"};
public:
	void test(ShadeShape &ss);
	float match(ShadeShape upSS, ShadeShape dbSS);
	void showIslands();
	void printIslandAreas();
};

#endif /* SHAPEMATCH_H_ */
