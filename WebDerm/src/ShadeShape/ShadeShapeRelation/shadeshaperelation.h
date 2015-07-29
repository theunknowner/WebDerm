/*
 * shadeshaperelation.h
 *
 *  Created on: Jul 16, 2015
 *      Author: jason
 */

#ifndef SHADESHAPERELATION_H_
#define SHADESHAPERELATION_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class ShadeShape;
class Islands;
class ShadeShapeRelation {
public:
	void setup_relationMatrix(map<String,float> &labels);
	vector<vector<vector<int> > > downScaleShadeShapeLabels(vector<vector<int> > &srm,map<String,float> &labels);
	float entropy(int count);
	float entropy(vector<vector<vector<int> > > &srmUP, vector<vector<vector<int> > > &srmDB);

protected:
	vector<String> rel_op = {"NULL","INDIR","DIR","SURR_BY_INV","SURR_BY"};
	enum{NONE=0, INDIR=1, DIR=2, SURR_BY_INV=3, SURR_BY=4};
	enum{ENTERED, INSIDE, OUTSIDE, EXITED};
	vector<vector<int> > relationMatrix;
	vector<vector<int> > relationCount;

public:
	vector<vector<int> > spatial_relation(ShadeShape &ss, map<String,float> &labels, vector<vector<vector<Islands> > > &islandVec);
	float srm_match(vector<vector<int> > srmUP, vector<vector<int> > srmDB, map<String,float> &labels);
	void writeRelationMatrix(map<String,float> &labels);
};

#endif /* SHADESHAPERELATION_H_ */
