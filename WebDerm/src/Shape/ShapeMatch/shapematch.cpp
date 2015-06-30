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

//! groups the islands of a feature by shade then shape
vector<vector<vector<Islands> > > ShapeMatch::groupIslandsByShade(ShadeShape &ss) {
	unsigned int shapeSize = this->shapeNames.size();
	vector<vector<vector<Islands> > > islandVec(ss.numOfShades(),vector<vector<Islands> >(shapeSize,vector<Islands>(0,Islands())));
	for(int i=0; i<ss.numOfShades(); i++) {
		int shadeVal = ss.shade(i);
		for(unsigned int j=0; j<shapeSize; j++) {
			String shape = this->shapeNames.at(j);
			for(int m=0; m<ss.numOfFeatures(); m++) {
				for(int n=0; n<ss.feature(m).numOfIslands(); n++) {
					int shadeVal2 = ss.feature(m).island(n).shade();
					String shape2 = ss.feature(m).island(n).shape_name();
					if(shadeVal2==shadeVal && shape==shape2) {
						islandVec.at(i).at(j).push_back(ss.feature(m).island(n));
					}
				}
			}
		}
	}
	return islandVec;
}

//! groups the islands of a feature by shape then shade
vector<vector<vector<Islands> > > ShapeMatch::groupIslandsByShape(ShadeShape &ss) {
	unsigned int shapeSize = this->shapeNames.size();
	vector<vector<vector<Islands> > > islandVec(shapeSize,vector<vector<Islands> >(ss.numOfShades(),vector<Islands>(0,Islands())));
	for(unsigned int i=0; i<shapeSize; i++) {
		String shape = this->shapeNames.at(i);
		for(int j=0; j<ss.numOfShades(); j++) {
			int shadeVal = ss.shade(j);
			for(int m=0; m<ss.numOfFeatures(); m++) {
				for(int n=0; n<ss.feature(m).numOfIslands(); n++) {
					int shadeVal2 = ss.feature(m).island(n).shade();
					String shape2 = ss.feature(m).island(n).shape_name();
					if(shadeVal2==shadeVal && shape==shape2) {
						islandVec.at(i).at(j).push_back(ss.feature(m).island(n));
						islandVec.at(i).at(j).back().set_shape(i);
					}
				}
			}
		}
	}
	return islandVec;
}

//! sorts the islands by area from largest to smallest
void ShapeMatch::sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec) {
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			vector<int> areaVec;
			vector<int> origPos;
			vector<Islands> newIslandVec;
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				int area = islandVec.at(i).at(j).at(k).area();
				areaVec.push_back(area);
			}
			jaysort(areaVec,origPos);
			for(int n=origPos.size()-1; n>=0; n--) {
				newIslandVec.push_back(islandVec.at(i).at(j).at(origPos.at(n)));
			}
			islandVec.at(i).at(j) = newIslandVec;
		}
	}
}

//! create key labels for map
//! assume islands are group by shape first
vector<vector<vector<String> > > ShapeMatch::createLabels(vector<vector<vector<Islands> > > &islandVec) {
	std::unordered_map<String,int> map;
	String label;
	vector<vector<vector<String> > > labels(islandVec.size(),vector<vector<String> >(islandVec.at(0).size(),vector<String>(0,"")));
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				//label = "s"+toString(i)+"_shape"+toString(shape)+"_";
				label = "shape"+toString(i)+"_s"+toString(j)+"_";
				label += Func::addDigitsForLabel(k,"0",2);
				if(map.find(label)==map.end()) {
					map[label] = 1;
					labels.at(i).at(j).push_back(label);

				}
			}
		}
	}
	return labels;
}

//! create map using labels and proportional area
map<String,float> ShapeMatch::createPropAreaMap(vector<vector<vector<Islands> > > &islandVec,float totalArea) {
	map<String,float> map;
	vector<vector<vector<String> > > labels = this->createLabels(islandVec);
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				String label = labels.at(i).at(j).at(k);
				float propArea = islandVec.at(i).at(j).at(k).area() / totalArea;
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

// shifts specified shape to the left
void ShapeMatch::shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum) {
	vector<vector<vector<Islands> > > newIslandVec = islandVec;
	vector<int> areaVec;
	if(shapeNum==4)
		cout << islandVec.at(shapeNum).size() << endl;
	if(islandVec.at(shapeNum).size()>0) {
		for(unsigned int j=0; j<islandVec.at(shapeNum).size(); j++) {
			if(islandVec.at(shapeNum).at(j).size()>0) {
				int area = islandVec.at(shapeNum).at(j).front().area();
				areaVec.push_back(area);
			}
			else
				areaVec.push_back(0);
		}
		vector<int>::iterator it = max_element(areaVec.begin(),areaVec.end());
		int max_pos = it - areaVec.begin();
		int new_shape = shapeNum-1;

		//printf("shape%d: %d | NewShape: %d\n",shapeNum,max_pos,new_shape);
		newIslandVec.at(new_shape).at(max_pos).push_back(islandVec.at(shapeNum).at(max_pos).front());
		newIslandVec.at(shapeNum).at(max_pos).erase(newIslandVec.at(shapeNum).at(max_pos).begin());
		islandVec = newIslandVec;
	}
}

/******************* PUBLIC FUNCTIONS ******************/

void ShapeMatch::test(ShadeShape &ss) {
	float totalArea = ss.area();
	this->upIslandVec = this->groupIslandsByShape(ss);
	this->sortIslandsByArea(this->upIslandVec);
	std::map<String,float> map = this->createPropAreaMap(this->upIslandVec,totalArea);
	std::map<String,float>::iterator it;
	for(it=map.begin(); it!=map.end(); it++) {
		cout << it->first << ": " << it->second << endl;
	}
	cout << "------------------" << endl;
	vector<vector<vector<Islands> > > islandVec = this->upIslandVec;
	this->shape_translation(this->upIslandVec,2);
	map = this->createPropAreaMap(islandVec,totalArea);
	for(it=map.begin(); it!=map.end(); it++) {
		cout << it->first << ": " << it->second << endl;
	}
	this->sortIslandsByArea(islandVec);
	cout << "------------------" << endl;
	map = this->createPropAreaMap(islandVec,totalArea);
	for(it=map.begin(); it!=map.end(); it++) {
		cout << it->first << ": " << it->second << endl;
	}
}

float ShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	float upTotalArea = upSS.area();
	float dbTotalArea = dbSS.area();
	this->upIslandVec = this->groupIslandsByShape(upSS);
	this->dbIslandVec = this->groupIslandsByShape(dbSS);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByArea(this->upIslandVec);

	// initial matching with no shape_transform_translation
	vector<float> resultVec;
	vector<vector<vector<Islands> > > islandVec = this->upIslandVec;
	this->sortIslandsByArea(islandVec);
	std::map<String,float> upMap = this->createPropAreaMap(islandVec,upTotalArea);
	std::map<String,float> dbMap = this->createPropAreaMap(this->dbIslandVec,dbTotalArea);
	this->fillPropAreaMapGaps(upMap,dbMap);
	std::map<String,float>::iterator itUP;
	std::map<String,float>::iterator itDB;
	for(itUP=upMap.begin(),itDB=dbMap.begin();itUP!=upMap.end(),itDB!=dbMap.end(); itUP++, itDB++) {
		printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second,itDB->first.c_str(),itDB->second);
	}
	float results = this->dotProduct(upMap,dbMap);
	resultVec.push_back(results);

	// start of shape_transform_translation(STT)
	for(unsigned int i=0; i<this->upIslandVec.size(); i++) {
		islandVec = this->upIslandVec;
		if(i==2 || i==3 || i==4 || i==6) { // shape num
			shape_translation(islandVec,i);
			this->sortIslandsByArea(islandVec);
			upMap = this->createPropAreaMap(islandVec,upTotalArea);
			dbMap = this->createPropAreaMap(this->dbIslandVec,dbTotalArea);
			this->fillPropAreaMapGaps(upMap,dbMap);

			results = this->dotProduct(upMap,dbMap);
			resultVec.push_back(results);
		}
	}

	cout << "-----------------------------" << endl;
	for(unsigned int i=0; i<resultVec.size(); i++) {
		float results = resultVec.at(i);
		if(i>0)
			results *= pow(2,-1);
		printf("Results %d: %f\n",i+1,results);
	}
	return 0.0;
}

void ShapeMatch::showIslands() {
	for(unsigned int i=0; i<this->upIslandVec.size(); i++) {
		for(unsigned int j=0; j<this->upIslandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<this->upIslandVec.at(i).at(j).size(); k++) {
				Mat img = this->upIslandVec.at(i).at(j).at(k).image();
				printf("Shade %d: ",i);
				cout << this->upIslandVec.at(i).at(j).at(k).shape_name() << endl;
				//imgshow(this->upIslandVec.at(i).at(j).image(),1);
			}
		}
	}
}

void ShapeMatch::printIslandAreas() {
	for(unsigned int i=0; i<this->upIslandVec.size(); i++) {
		printf("s%d: ",i);
		for(unsigned int j=0; j<this->upIslandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<this->upIslandVec.at(i).at(j).size(); k++) {
				int area = this->upIslandVec.at(i).at(j).at(k).area();
				printf("%d, ",area);
			}
		}
		cout << endl;
	}
}
