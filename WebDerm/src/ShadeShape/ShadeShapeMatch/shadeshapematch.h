/*
 * shadeshapematch.h
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#ifndef SHADESHAPEMATCH_H_
#define SHADESHAPEMATCH_H_

#include "../../global.h"
#include "../shadeshape.h"
#include "../islands.h"
#include "../ShapeMatch/shapematch.h"
#include "../ShadeMatch/shadematch.h"

class ShadeShape;
class Islands;
class Labels;
class ShadeShapeRelation;
class PrintStream;

class ShadeShapeMatch {
private:
	int debugMode = 0;
	//[shape][shade][idx]
	vector<vector<vector<Islands> > > upIslandVec;
	vector<vector<vector<Islands> > > dbIslandVec;

	vector<vector<int> > shapeTranslateCount;

	vector<vector<vector<Islands> > > groupIslandsByShade(ShadeShape &ss);
	vector<vector<vector<Islands> > > groupIslandsByShape(ShadeShape &ss);
	void sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec);
	void fillMissingLabels(Labels &upLabels, Labels &dbLabels);
	float dotProduct(Labels &upLabels, Labels &dbLabels);
	float tr1ForShade(Labels &upLabels, Labels &dbLabels);
	float tr1(Labels &upLabels, Labels &dbLabels);
	vector<float> tr2(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels, String nStr="");

	map<String,PrintStream> esgPS_Map;
	map<String,PrintStream> entropyPS_Map;
	map<String,PrintStream> mismatchEntropyPS_Map;
	map<String,PrintStream> noEntropyPS_Map;

public:
	void test(ShadeShape &ss);
	float test_match(ShadeShape upSS, ShadeShape dbSS);
	vector<float> match(ShadeShape upSS, ShadeShape dbSS);
	void debug_mode(int mode);

	void countShapeTranslations(vector<vector<int> > &shapeTranslateCount);
};

#endif /* SHADESHAPEMATCH_H_ */
