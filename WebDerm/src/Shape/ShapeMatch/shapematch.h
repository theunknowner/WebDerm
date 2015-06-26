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
	vector<vector<Islands> > upIslandVec;
	vector<vector<Islands> > dbIslandVec;
	vector<vector<Islands> > groupIslandsByShade(ShadeShape &ss);
	void sortIslandsByArea(vector<vector<Islands> > &islandVec);
	void sortIslandsByShape(vector<vector<Islands> > &islandVec);

	vector<String> shapeNames = {"Strip","Default","Blotch","Incomp_Disc",
			"Comp_Disc","Incomp_Donut","Comp_Donut"};
public:
	void test(ShadeShape &ss);
	float match(ShadeShape upSS, ShadeShape dbSS);
	void showIslands();
};

#endif /* SHAPEMATCH_H_ */
