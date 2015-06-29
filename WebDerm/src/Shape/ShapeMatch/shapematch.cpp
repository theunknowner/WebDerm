/*
 * shapematch.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#include "shapematch.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "../ShadeShape/shadeshape.h"
#include "../ShadeShape/islands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"


/******************* PRIVATE FUNCTIONS ******************/

//! sorts the islands of a feature by area size
vector<vector<Islands> > ShapeMatch::groupIslandsByShade(ShadeShape &ss) {
	vector<vector<Islands> > islandVec(ss.numOfShades(),vector<Islands>(0,Islands()));
	int numOfFeatures = ss.numOfFeatures();
	for(int i=0; i<ss.numOfShades(); i++) {
		int shadeVal = ss.shade(i);
		for(int j=0; j<numOfFeatures; j++) {
			int numOfIslands = ss.feature(j).numOfIslands();
			for(int k=0; k<numOfIslands; k++) {
				int shadeVal2 = ss.feature(j).island(k).shade();
				if(shadeVal == shadeVal2)
					islandVec.at(i).push_back(ss.feature(j).island(k));
			}
		}
	}
	return islandVec;
}

//! sorts the islands by area from largest to smallest
void ShapeMatch::sortIslandsByArea(vector<vector<Islands> > &islandVec) {
	for(unsigned int i=0; i<islandVec.size(); i++) {
		vector<int> areaVec;
		vector<int> origPos;
		vector<Islands> newIslandVec;
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			int area = islandVec.at(i).at(j).area();
			areaVec.push_back(area);
		}
		jaysort(areaVec,origPos);
		for(int n=origPos.size()-1; n>=0; n--) {
			newIslandVec.push_back(islandVec.at(i).at(origPos.at(n)));
		}
		islandVec.at(i) = newIslandVec;
	}
}

//! sort islands by shape defined by shapeNames
void ShapeMatch::sortIslandsByShape(vector<vector<Islands> > &islandVec) {
	for(unsigned int i=0; i<islandVec.size(); i++) {
		vector<Islands> newIslandVec;
		for(unsigned int j=0; j<this->shapeNames.size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).size(); k++) {
				String shapeName = islandVec.at(i).at(k).shape_name();
				if(shapeName==shapeNames.at(j)) {
					islandVec.at(i).at(k).set_shape(j);
					newIslandVec.push_back(islandVec.at(i).at(k));
				}
			}
		}
		islandVec.at(i) = newIslandVec;
	}
}

//! create key labels for map
vector<vector<String> > ShapeMatch::createLabels(vector<vector<Islands> > &islandVec) {
	std::unordered_map<String,int> map;
	String label;
	vector<vector<String> > labels(islandVec.size(),vector<String>(0,""));
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			int shape = islandVec.at(i).at(j).shape();
			int n=0;
			while(1) {
				if(n<10) {
					label = "s"+toString(i)+"_shape"+toString(shape)+"_0"+toString(n);
				}
				else
					label = "s"+toString(i)+"_shape"+toString(shape)+"_"+toString(n);
				if(map.find(label)==map.end()) {
					map[label] = 1;
					labels.at(i).push_back(label);
					break;
				}
				else n++;
			}
		}
	}
	return labels;
}

//! create map using labels and proportional area
map<String,float> ShapeMatch::createPropAreaMap(vector<vector<Islands> > &islandVec,float totalArea) {
	map<String,float> map;
	vector<vector<String> > labels = this->createLabels(islandVec);
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			String label = labels.at(i).at(j);
			float propArea = islandVec.at(i).at(j).area() / totalArea;
			//printf("%s: %f, %d\n",label.c_str(),propArea,islandVec.at(i).at(j).area());
			if(map.find(label)==map.end()) {
				map[label] = propArea;
			}
			else {
				cout << "Key already exist!!!" << endl;
				cout << label << endl;
				exit(1);
			}
		}
	}
	return map;
}

//! compare two maps and fill in missing data for each map
void ShapeMatch::fillPropAreaMapGaps(std::map<String,float> &upMap, std::map<String,float> &dbMap) {
	std::map<String, float>::iterator it;
	for(it=upMap.begin(); it!=upMap.end(); it++) {
		String label = it->first;
		if(dbMap.find(label)==dbMap.end()) {
			dbMap[label] = 0;
		}
	}
	for(it=dbMap.begin(); it!=dbMap.end(); it++) {
		String label = it->first;
		if(upMap.find(label)==upMap.end()) {
			upMap[label] = 0;
		}
	}
}

//! dot product using holder's inequality
float ShapeMatch::dotProduct(std::map<String,float> &upMap, std::map<String,float> &dbMap) {
	if(upMap.size()!=dbMap.size()) {
		cout << "ShapeMatch::dotProduct(): upMap && dbMap not same size!!!" << endl;
		exit(1);
	}
	std::map<String,float>::iterator itUP;
	std::map<String,float>::iterator itDB;
	float numerSum = 0.0;
	float denomSumUP = 0.0;
	float denomSumDB = 0.0;
	for(itUP=upMap.begin(), itDB=dbMap.begin(); itUP!=upMap.end(), itDB!=dbMap.end(); itUP++, itDB++) {
		numerSum += (itUP->second * itDB->second);
		denomSumUP += pow(itUP->second,2);
		denomSumDB += pow(itDB->second,2);
	}
	float results = numerSum / (sqrt(denomSumUP) * sqrt(denomSumDB));
	return results;
}

/******************* PUBLIC FUNCTIONS ******************/

void ShapeMatch::test(ShadeShape &ss) {
	this->upIslandVec = this->groupIslandsByShade(ss);
	this->sortIslandsByArea(this->upIslandVec);
	this->sortIslandsByShape(this->upIslandVec);
	float totalArea = ss.area();
	std::map<String,float> map = this->createPropAreaMap(this->upIslandVec,totalArea);
	std::map<String,float>::iterator it;
	for(it=map.begin(); it!=map.end(); it++) {
		cout << it->first << ": " << it->second << endl;
	}
}

float ShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	TestML ml;
	this->upIslandVec = this->groupIslandsByShade(upSS);
	this->dbIslandVec = this->groupIslandsByShade(dbSS);
	this->sortIslandsByArea(this->upIslandVec);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByShape(this->upIslandVec);
	this->sortIslandsByShape(this->dbIslandVec);
	float upTotalArea = upSS.area();
	float dbTotalArea = dbSS.area();
	std::map<String,float> upMap = this->createPropAreaMap(this->upIslandVec,upTotalArea);
	std::map<String,float> dbMap = this->createPropAreaMap(this->dbIslandVec,dbTotalArea);
	this->fillPropAreaMapGaps(upMap,dbMap);
	std::map<String,float>::iterator itUP;
	std::map<String,float>::iterator itDB;
	for(itUP=upMap.begin(),itDB=dbMap.begin();itUP!=upMap.end(),itDB!=dbMap.end(); itUP++, itDB++) {
		printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second,itDB->first.c_str(),itDB->second);
	}

	float results = this->dotProduct(upMap,dbMap);

	return results;
}

void ShapeMatch::showIslands() {
	for(unsigned int i=0; i<this->upIslandVec.size(); i++) {
		for(unsigned int j=0; j<this->upIslandVec.at(i).size(); j++) {
			Mat img = this->upIslandVec.at(i).at(j).image();
			printf("Shade %d: ",i);
			cout << this->upIslandVec.at(i).at(j).shape_name() << endl;
			//imgshow(this->upIslandVec.at(i).at(j).image(),1);
		}
	}
}
