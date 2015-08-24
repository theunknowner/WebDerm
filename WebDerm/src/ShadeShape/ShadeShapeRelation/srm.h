/*
 * srm.h
 *
 *  Created on: Aug 24, 2015
 *      Author: jason
 */

#ifndef SRM_H_
#define SRM_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../Labels/labels.h"

class Labels;
class Srm {
private:
	Labels labels;
	Labels mergedLabels;
	void setup_relationMatrix(Labels &labels);
	Labels mergeLabels();
	int max_neighbor_level;
	int relOpLevelSize = 50;

protected:
	vector<String> rel_op = {"NULL","INDIR","DIR","SURR_BY_INV","SURR_BY"};
	enum{NONE=0, INDIR=1, DIR=2, SURR_BY_INV=3, SURR_BY=4};
	vector<vector<int> > relationOpMatrix;
	vector<vector<int> > neighborLevelMatrix;
	vector<vector<int> > relationCountMatrix;

public:
	Srm();
	Srm(Labels &labels);
	int& relation(int index1, int index2);
	int& relationCount(int index1, int index2);
	int& neighborLevel(int index1, int index2);
	size_t size();
	int maxNeighborLevel();
	void writeRelationMatrix(Labels &labels,String name);
	//pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<pair<int,int>> > > >> downScaleSrm(Labels &labels, Labels &mergedLabels);
	void downScaleSrm();
	Labels getLabels();
	Labels getMergedLabels();

	/**** For downscaling srm ****/
	vector<vector<vector<vector<int> > > > dsSrmCount;
	vector<vector<vector<vector<pair<int,int>> > > > dsSrmArea;
	map<String,vector<String>> mergedLabelContainer; //holds the individual labels that belongs to a merged label
};

#endif /* SRM_H_ */
