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
class ShadeShapeMatch : public ShapeMatch, public ShadeMatch {
private:
	//[shape][shade][idx]
	vector<vector<vector<Islands> > > upIslandVec;
	vector<vector<vector<Islands> > > dbIslandVec;
	vector<vector<vector<Islands> > > groupIslandsByShade(ShadeShape &ss);
	vector<vector<vector<Islands> > > groupIslandsByShape(ShadeShape &ss);
	void sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec);
	vector<vector<vector<String> > > createKeyLabels(vector<vector<vector<Islands> > > &islandVec);
	std::map<String,float> createPropAreaMap(vector<vector<vector<Islands> > > &islandVec, float totalArea);
	void fillPropAreaMapGaps(std::map<String,float> &upMap, std::map<String,float> &dbMap);
	float dotProduct(std::map<String,float> &upMap, std::map<String,float> &dbMap);
public:
	void test(ShadeShape &ss);
	void test_match(ShadeShape upSS, ShadeShape dbSS);
	float match(ShadeShape upSS, ShadeShape dbSS);
	void printLabels(map<String,float> &labels);
};

#endif /* SHADESHAPEMATCH_H_ */
