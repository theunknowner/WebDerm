/*
 * labels.h
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#ifndef LABELS_H_
#define LABELS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../islands.h"
#include "../ShapeMatch/shapematch.h"

class Islands;
class Labels : public ShapeMatch{
private:
	map<String, pair<int,float> > labelMap;
	map<String,int> _areaLabels;
	map<String,float> _relAreaLabels;
public:
	Labels();
	Labels(vector<vector<vector<Islands> > > &islandVec, float totalArea);
	void create(vector<vector<vector<Islands> > > &islandVec, float totalArea);
	map<String,pair<int,float> >& getLabels();
	map<String,int>& areaLabels();
	map<String,float>& relAreaLabels();
	int area(String label);
	int area(int num);
	int totalArea();
	float relativeArea(String label);
	float relativeArea(int num);
	float totalRelArea();
	int size();
	String at(int num);
	void printLabels();
};

#endif /* LABELS_H_ */
