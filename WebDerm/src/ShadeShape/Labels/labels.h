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
	String labelName;
	int labelTotalArea;
	void calcTotalArea();

public:
	Labels();
	Labels(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name="");
	void create(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name="");
	map<String,pair<int,float> >& getMap();
	void setLabels(map<String,pair<int,float> > labels);
	int area(String label);
	int area(int num);
	int& totalArea();
	float relativeArea(String label);
	float relativeArea(int num);
	size_t size();
	String at(int num);
	String& name();
	void printLabels();
	int getIndex(String label);
	static void printCompareLabels(Labels &labels1, Labels &labels2);
	static void writeCompareLabels(Labels &labels1, Labels &labels2);
};

#endif /* LABELS_H_ */
