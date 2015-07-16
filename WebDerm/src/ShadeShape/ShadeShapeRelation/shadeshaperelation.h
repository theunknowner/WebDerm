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
	vector<vector<String> > relationMatrix;
	vector<vector<int> > markedMap;
	void setup_relationMatrix(map<String,int> &labels);

public:
	void surrounded_by(ShadeShape &ss, map<String,int> &labels, vector<vector<vector<Islands> > > &islandVec);
};

#endif /* SHADESHAPERELATION_H_ */
