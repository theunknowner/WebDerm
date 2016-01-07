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
	map<String,bool> labelShapeShiftMap;
	map<String,int> labelShapeNumMap;
	map<String,int> labelPrevShapeNumMap;
	map<String,int> labelShadeLevelMap;
	map<String, vector<float> > labelStatSignMap;
	map<String,Islands> labelIslandMap;
	map<String,int> labelShadeLumMap;
	String labelName;
	int labelTotalArea;
	void calcTotalArea();

public:
	Labels();
	Labels(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name="");
	void create(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name="");
	map<String,pair<int,float> >& getMap();
	map<String,int>& getShadeLevelMap();
	map<String, int>& getShapeMap();
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
	String getShape(String label);
	int getShade(String label);
	int getShadeLevel(String label);
	void setShadeLevel(String label, int level);
	bool isShapeShifted(String label);
	int getShapeNum(String label);
	int getPrevShapeNum(String label);
	vector<float> getStatSign(String label);
	map<String,vector<float>>& getStatSignMap();
	Islands& getIsland(String label);
	static void printCompareLabels(Labels &labels1, Labels &labels2, int markShifted=0);
	static void writeCompareLabels(String name, Labels &labels1, Labels &labels2, int markShifted=0);
	static void printCompareStatSign(Labels &labels1, Labels &labels2, String label);
	static void writeCompareStatSign(Labels &labels1, Labels &labels2, String label, String fileType="txt");
};

#endif /* LABELS_H_ */
