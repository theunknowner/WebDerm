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
protected:
	String rel_op[4] = {"SURR_BY","SURR_BY_INV","INDIR","DIR"};
	enum{SURR_BY, SURR_BY_INV, INDIR, DIR};
	enum{ENTERED, INSIDE, OUTSIDE, EXITED};
	vector<vector<String> > relationMatrix;
	vector<vector<int> > relationCount;
	vector<vector<int> > markedMap;
	void setup_relationMatrix(map<String,float> &labels);

public:
	void surrounded_by(ShadeShape &ss, map<String,float> &labels, vector<vector<vector<Islands> > > &islandVec);
	void writeRelationMatrix(map<String,float> &labels);
};

#endif /* SHADESHAPERELATION_H_ */
