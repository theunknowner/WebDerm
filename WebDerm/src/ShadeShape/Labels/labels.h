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

class Islands;
class Labels {
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
	float relativeArea(String label);
	int size();
	void printLabels();
};

#endif /* LABELS_H_ */