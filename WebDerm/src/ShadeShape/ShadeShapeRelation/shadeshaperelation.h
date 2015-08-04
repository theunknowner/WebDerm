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
	vector<String> rel_op = {"NULL","INDIR","DIR","SURR_BY_INV","SURR_BY"};
	enum{NONE=0, INDIR=1, DIR=2, SURR_BY_INV=3, SURR_BY=4};
	enum{ENTERED, INSIDE, OUTSIDE, EXITED};
	vector<vector<int> > relationMatrix;
	vector<vector<int> > relationCount;
	String ssr_name;

private:
	void setup_relationMatrix(Labels &labels);

	/************ ssr_match.cpp **************/
	Labels mergeLabels(Labels &labels, vector<vector<int> > &srm);
	pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> downScaleSrm(ShadeShapeRelation &ssr, vector<vector<int> > &srm,Labels &labels, Labels &mergedLabels);
	float entropy(int count);
	float entropy(pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPairUP, Labels &upLabels, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPairDB, Labels &dbLabels);


public:
	vector<vector<int> > generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	void spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec);
	void writeRelationMatrix(Labels &labels,String name);
	vector<vector<int> >& get_srm();
	String name();

	/************ ssr_match.cpp **************/
	float srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels);
	void writeDownScaleSrm(String name, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels);
	void printDownScaleSrm(pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels);
};

#endif /* SHADESHAPERELATION_H_ */
