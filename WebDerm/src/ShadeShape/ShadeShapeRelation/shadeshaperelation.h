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
#include "srm.h"

class ShadeShape;
class Islands;
class Labels;
class ShadeShapeRelation : public ShapeMatch, public Srm {

protected:
	String ssr_name;
	Srm srm;

public:
	void generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	void spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	Srm& get_srm();
	String name();
	int getRelOpIndex(String relOp);
};

#endif /* SHADESHAPERELATION_H_ */
