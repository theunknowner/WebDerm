/*
 * shadeshaperelation.h
 *
 *  Created on: Jul 16, 2015
 *      Author: jason
 */

#ifndef SHADESHAPERELATION_H_
#define SHADESHAPERELATION_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../ShapeMatch/shapematch.h"

class ShadeShape;
class Islands;
class Labels;
class ShadeShapeRelation : public ShapeMatch{

protected:
	const int relOpLevelSize = 10;
	vector<String> rel_op = {"NULL","INDIR","DIR","SURR_BY_INV","SURR_BY"};
	enum{NONE=0, INDIR=1, DIR=2, SURR_BY_INV=3, SURR_BY=4};
	enum{ENTERED, INSIDE, OUTSIDE, EXITED};
	vector<vector<vector<int> > > relationMatrix;
	vector<vector<int> > relationCount;

	String ssr_name;

private:
	void setup_relationMatrix(Labels &labels);


public:
	void generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	void spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	void writeRelationMatrix(Labels &labels,String name);
	vector<vector<vector<int> > >& get_srm();
	String name();
	int getRelOpIndex(String relOp);
	pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> downScaleSrm(Labels &labels, Labels &mergedLabels);
};

#endif /* SHADESHAPERELATION_H_ */
