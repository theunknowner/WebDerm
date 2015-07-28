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
private:
	void setup_relationMatrix(map<String,float> &labels);
	void downScaleShadeShapeLabels(vector<vector<String> > &srm,map<String,float> &labels);

protected:
	vector<String> rel_op = {"NULL","INDIR","DIR","SURR_BY_INV","SURR_BY"};
	enum{NONE, INDIR, DIR, SURR_BY_INV, SURR_BY};
	enum{ENTERED, INSIDE, OUTSIDE, EXITED};
	vector<vector<String> > relationMatrix;
	vector<vector<int> > relationCount;
	vector<vector<int> > markedMap;

public:
	vector<vector<String> > spatial_relation(ShadeShape &ss, map<String,float> &labels, vector<vector<vector<Islands> > > &islandVec);
	float srm_match(vector<vector<String> > srmUP, vector<vector<String> > srmDB);
	void writeRelationMatrix(map<String,float> &labels);
};

#endif /* SHADESHAPERELATION_H_ */
