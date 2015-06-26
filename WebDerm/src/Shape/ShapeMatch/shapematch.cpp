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

//! sorts the islands by area
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
		for(unsigned int n=0; n<origPos.size(); n++) {
			newIslandVec.push_back(islandVec.at(i).at(origPos.at(n)));
		}
		islandVec.at(i) = newIslandVec;
	}
}

void ShapeMatch::sortIslandsByShape(vector<vector<Islands> > &islandVec) {
	for(unsigned int i=0; i<islandVec.size(); i++) {
		vector<Islands> newIslandVec;
		for(unsigned int j=0; j<this->shapeNames.size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).size(); k++) {
				String shapeName = islandVec.at(i).at(k).shape_name();
				if(shapeName==shapeNames.at(j)) {
					newIslandVec.push_back(islandVec.at(i).at(k));
				}
			}
		}
		islandVec.at(i) = newIslandVec;
	}
}

/******************* PUBLIC FUNCTIONS ******************/

void ShapeMatch::test(ShadeShape &ss) {
	this->upIslandVec = this->groupIslandsByShade(ss);
	this->sortIslandsByArea(this->upIslandVec);
	this->sortIslandsByShape(this->upIslandVec);
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
