/*
 * shadeshapematch.h
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#ifndef SHADESHAPEMATCH_H_
#define SHADESHAPEMATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../shadeshape.h"
#include "../islands.h"
#include "../ShapeMatch/shapematch.h"
#include "../ShadeMatch/shadematch.h"

class ShadeShape;
class Islands;
class Labels;
class ShadeShapeRelation;

class ShadeShapeMatch {
private:
	int debugMode = 0;
	//[shape][shade][idx]
	vector<vector<vector<Islands> > > upIslandVec;
	vector<vector<vector<Islands> > > dbIslandVec;
	vector<vector<vector<Islands> > > groupIslandsByShade(ShadeShape &ss);
	vector<vector<vector<Islands> > > groupIslandsByShape(ShadeShape &ss);
	void sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec);
	void fillMissingLabels(Labels &upLabels, Labels &dbLabels);
	float dotProduct(Labels &upLabels, Labels &dbLabels, int shapeNum1=-1, int shapeNum2=-1);
	float tr1(Labels &upLabels, Labels &dbLabels, int shapeNum1=-1, int shapeNum2=-1);
	float tr2(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels);
public:
	void test(ShadeShape &ss);
	float test_match(ShadeShape upSS, ShadeShape dbSS);
	vector<float> match(ShadeShape upSS, ShadeShape dbSS);
	void printLabels(map<String,float> &labels);
	void debug_mode(int mode);
};

#endif /* SHADESHAPEMATCH_H_ */
