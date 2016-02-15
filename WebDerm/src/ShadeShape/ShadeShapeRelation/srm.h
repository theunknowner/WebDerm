/*
 * srm.h
 *
 *  Created on: Aug 24, 2015
 *      Author: jason
 */

#ifndef SRM_H_
#define SRM_H_

#include "../../global.h"
#include "../Labels/labels.h"

class Labels;
class ShadeShape;

class Srm {
private:
	Labels labels;
	Labels mergedLabels;
	void setup_relationMatrix(Labels &labels);
	Labels mergeLabels();
	int max_neighbor_level;
	vector<vector<int> > relationOpMatrix;
	vector<vector<int> > neighborLevelMatrix;
	vector<vector<int> > relationCountMatrix;
	vector<vector<int> > relationTouchCountMatrix;
	vector<vector<pair<int,int>> > relationAreaMatrix;
	vector<vector<map<float,float>> > relationDistanceMatrix;
	vector<vector<float> > relationCountPercentMatrix;

protected:
	int relOpLevelSize = 4;
	vector<String> rel_op = {"NULL","INDIR","DIR","SURR_BY_INV","SURR_BY"};
	enum{NONE=0, INDIR=1, DIR=2, SURR_BY_INV=3, SURR_BY=4};

public:
	Srm();
	Srm(ShadeShape &ss, Labels &labels);
	int& relation(int index1, int index2);
	int& relationCount(int index1, int index2);
	int& neighborLevel(int index1, int index2);
	int& relationTouchCount(int index1, int index2);
	float& relationCountPercent(int index1, int index2);
	pair<int,int>& relationArea(int index1, int index2);
	float getRelationDistance(int index1, int index2, float degree);
	void setRelationDistance(int index1, int index2, float degree, float val);
	size_t size();
	int& maxNeighborLevel();
	void writeRelationMatrix(Labels &labels,String name);
	void writeNeighborLevelMatrix(Labels &labels, String name);
	void writeRelationCountMatrix(Labels &labels, String name);
	void downScaleSrm();
	Labels getLabels();
	Labels getMergedLabels();
	vector<vector<vector<vector<int> > > >& downscaleSrmCount();
	vector<vector<vector<vector<pair<int,int>> > > >& downscaleSrmArea();
	int getIndex(String label);
	int area(String label);
	void printDownScaleSrm(String label1, String label2);
private:
	/**** For downscaling srm ****/
	vector<vector<vector<vector<int> > > > dsSrmCount;
	vector<vector<vector<vector<pair<int,int>> > > > dsSrmArea;
public:
	//! holds the individual labels that belongs to a merged label
	//! first->y; second->x
	vector<vector<vector<vector<pair<vector<String>,vector<String>> > > > > mergedLabelContainer;
	//! Key = x<op>y, holds area of x and y in a pair
	map<String,pair<int,int>> equationMap;
	vector<vector<vector<vector<float> > > > mergedRelationDistance;

};

#endif /* SRM_H_ */
