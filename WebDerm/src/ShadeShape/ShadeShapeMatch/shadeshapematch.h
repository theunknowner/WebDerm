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

class ShadeShapeMatch : public ShapeMatch, public ShadeMatch {
private:
	int debugMode = 0;
	//[shape][shade][idx]
	vector<vector<vector<Islands> > > upIslandVec;
	vector<vector<vector<Islands> > > dbIslandVec;
	vector<vector<vector<Islands> > > groupIslandsByShade(ShadeShape &ss);
	vector<vector<vector<Islands> > > groupIslandsByShape(ShadeShape &ss);
	void sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec);
	void fillPropAreaMapGaps(Labels &upLabels, Labels &dbLabels);
	float dotProduct(Labels &upLabels, Labels &dbLabels);
	float tr1_match(ShadeShape &upSS, ShadeShape &dbSS);
	float tr2_match(ShadeShape &upSS, ShadeShape &dbSS);
	float tr1(Labels &upLabels, Labels &dbLabels);
	float tr2(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels);
public:
	void test(ShadeShape &ss);
	float test_match(ShadeShape upSS, ShadeShape dbSS);
	float match(ShadeShape upSS, ShadeShape dbSS);
	vector<float> match2(ShadeShape upSS, ShadeShape dbSS);
	void printLabels(map<String,float> &labels);
	void debug_mode(int mode);
};

#endif /* SHADESHAPEMATCH_H_ */
