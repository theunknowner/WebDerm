/*
 * shadeshapematch.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#include "shadeshapematch.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"
#include "../ShapeMatch/shapematch.h"
#include "../ShadeMatch/shadematch.h"
#include "../shadeshape.h"
#include "../ShadeShapeRelation/shadeshaperelation.h"

/******************* PRIVATE FUNCTIONS ******************/

//! groups the islands of a feature by shade then shape
vector<vector<vector<Islands> > > ShadeShapeMatch::groupIslandsByShade(ShadeShape &ss) {
	int shapeSize = this->numOfShapes();
	vector<vector<vector<Islands> > > islandVec(ss.numOfShades(),vector<vector<Islands> >(shapeSize,vector<Islands>(0,Islands())));
	for(int i=0; i<ss.numOfShades(); i++) {
		int shadeVal = ss.shade(i);
		for(int j=0; j<shapeSize; j++) {
			String shape =this->shapeName(j);
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
vector<vector<vector<Islands> > > ShadeShapeMatch::groupIslandsByShape(ShadeShape &ss) {
	int shapeSize = this->numOfShapes();
	vector<vector<vector<Islands> > > islandVec(shapeSize,vector<vector<Islands> >(ss.numOfShades(),vector<Islands>(0,Islands())));
	for(int i=0; i<shapeSize; i++) {
		String shape = this->shapeName(i);
		for(int j=0; j<ss.numOfShades(); j++) {
			int shadeVal = ss.shade(j);
			for(int m=0; m<ss.numOfFeatures(); m++) {
				for(int n=0; n<ss.feature(m).numOfIslands(); n++) {
					int shadeVal2 = ss.feature(m).island(n).shade();
					String shape2 = ss.feature(m).island(n).shape_name();
					if(shadeVal2==shadeVal && shape==shape2) {
						islandVec.at(i).at(j).push_back(ss.feature(m).island(n));
						islandVec.at(i).at(j).back().shape() = i;
					}
				}
			}
		}
	}
	return islandVec;
}

//! sorts the islands by area from largest to smallest
void ShadeShapeMatch::sortIslandsByArea(vector<vector<vector<Islands> > > &islandVec) {
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
vector<vector<vector<String> > > ShadeShapeMatch::createKeyLabels(vector<vector<vector<Islands> > > &islandVec) {
	std::unordered_map<String,int> map;
	String label;
	vector<vector<vector<String> > > labels(islandVec.size(),vector<vector<String> >(islandVec.at(0).size(),vector<String>(0,"")));
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				//label = "s"+toString(j)+"_shape"+toString(i)+"_";
				label = "shape"+toString(i)+"_s"+toString(j)+"_";
				label += Func::addDigitsForLabel(k,"0",3);
				islandVec.at(i).at(j).at(k).labelName() = label;
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
map<String,float> ShadeShapeMatch::createPropAreaMap(vector<vector<vector<Islands> > > &islandVec,float totalArea) {
	map<String,float> map;
	vector<vector<vector<String> > > labels = this->createKeyLabels(islandVec);
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
void ShadeShapeMatch::fillPropAreaMapGaps(std::map<String,float> &upMap, std::map<String,float> &dbMap) {
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
float ShadeShapeMatch::dotProduct(std::map<String,float> &upMap, std::map<String,float> &dbMap) {
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

void ShadeShapeMatch::test(ShadeShape &ss) {
	float totalArea = ss.area();
	this->maxNumOfShades = ss.numOfShades();
	vector<vector<vector<Islands> > > islandVec = this->groupIslandsByShape(ss);
	this->sortIslandsByArea(islandVec);
	std::map<String,float> map = this->createPropAreaMap(islandVec,totalArea);
	std::map<String,float>::iterator it;
	for(it=map.begin(); it!=map.end(); it++) {
		cout << it->first << ": " << it->second << endl;
	}
	cout << "------------------" << endl;
	//ShadeShapeRelation ssr;
	//ssr.spatial_relation(ss,map,islandVec);
}

void ShadeShapeMatch::test_match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch smatch;
	ShadeMatch shadematch;
	this->maxNumOfShades = max(upSS.numOfShades(),dbSS.numOfShades());
	float upTotalArea = upSS.area();
	float dbTotalArea = dbSS.area();
	this->upIslandVec = this->groupIslandsByShape(upSS);
	this->dbIslandVec = this->groupIslandsByShape(dbSS);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByArea(this->upIslandVec);
	std::map<String,float> upMap, upMapFilled, dbMapFilled;
	std::map<String,float> dbMap = this->createPropAreaMap(this->dbIslandVec,dbTotalArea);

	vector<float> resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	for(unsigned int shadeShift=0; shadeShift<ShadeMatch::SHIFT.size(); shadeShift++) {
		ShadeShape newUpSS = upSS;
		if(shadeShift==0) {
			islandVec2 = this->groupIslandsByShape(newUpSS);
			this->sortIslandsByArea(islandVec2);
			upMap = this->createPropAreaMap(islandVec2,upTotalArea);
			upMapFilled = upMap;
			dbMapFilled = dbMap;
			this->fillPropAreaMapGaps(upMapFilled,dbMapFilled);
			float results = this->dotProduct(upMapFilled,dbMapFilled);
			resultVec.push_back(results);
			std::map<String,float>::iterator itUP;
			std::map<String,float>::iterator itDB;
			for(itUP=upMapFilled.begin(),itDB=dbMapFilled.begin();itUP!=upMapFilled.end(),itDB!=dbMapFilled.end(); itUP++, itDB++) {
				printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second,itDB->first.c_str(),itDB->second);
			}
			printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
			printf("Results: %f\n",results);
		}
		else {
			while(this->shade_translation(newUpSS,shadeShift)) {
				ShadeShape matchSS;
				matchSS.extract(newUpSS.image());
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				upMap = this->createPropAreaMap(islandVec2,upTotalArea);
				upMapFilled = upMap;
				dbMapFilled = dbMap;
				this->fillPropAreaMapGaps(upMapFilled,dbMapFilled);
				float results = this->dotProduct(upMapFilled,dbMapFilled);
				if(ShadeMatch::SHIFT.at(shadeShift)!="SHIFT_NONE") {
					std::map<String,float>::iterator itUP;
					std::map<String,float>::iterator itDB;
					for(itUP=upMapFilled.begin(),itDB=dbMapFilled.begin();itUP!=upMapFilled.end(),itDB!=dbMapFilled.end(); itUP++, itDB++) {
						printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second,itDB->first.c_str(),itDB->second);
					}
				}
				printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
				printf("Results: %f\n",results);
				if(results>resultVec.back())
					resultVec.push_back(results);
				else
					break;
			}
		}
	}
}

float ShadeShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch smatch;
	ShadeMatch shadematch;
	this->setMaxNumOfShades(max(upSS.numOfShades(),dbSS.numOfShades()));
	float upTotalArea = upSS.area();
	float dbTotalArea = dbSS.area();
	this->upIslandVec = this->groupIslandsByShape(upSS);
	this->dbIslandVec = this->groupIslandsByShape(dbSS);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByArea(this->upIslandVec);
	std::map<String,float> upMap;
	std::map<String,float> dbMap;

	vector<float> resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	vector<vector<vector<Islands> > > islandVec3;
	vector<vector<vector<Islands> > > islandVecTemp;
	float thresh = upTotalArea * 0.60;
	for(unsigned int shadeShift=0; shadeShift<ShadeMatch::SHIFT.size(); shadeShift++) {
		islandVec2 = this->upIslandVec;
		this->shade_translation(islandVec2,thresh,shadeShift);
		for(unsigned int shapeShift=0; shapeShift<ShapeMatch::SHIFT.size(); shapeShift++) {
			islandVec3 = islandVec2;
			if(ShapeMatch::SHIFT.at(shapeShift)=="SHIFT_NONE") {
				this->sortIslandsByArea(islandVec3);
				upMap = this->createPropAreaMap(islandVec3,upTotalArea);
				dbMap = this->createPropAreaMap(this->dbIslandVec,dbTotalArea);
				this->fillPropAreaMapGaps(upMap,dbMap);
				if(ShadeMatch::SHIFT.at(shadeShift)=="SHIFT_NONE" && ShapeMatch::SHIFT.at(shapeShift)=="SHIFT_NONE") {
					std::map<String,float>::iterator itUP;
					std::map<String,float>::iterator itDB;
					for(itUP=upMap.begin(),itDB=dbMap.begin();itUP!=upMap.end(),itDB!=dbMap.end(); itUP++, itDB++) {
						printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second,itDB->first.c_str(),itDB->second);
					}
				}
				float results = this->dotProduct(upMap,dbMap);
				resultVec.push_back(results);
				printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
				printf("ShapeShift: %s, ",ShapeMatch::SHIFT[shapeShift].c_str());
				printf("ShapeNum: NULL, ");
				printf("Results: %f\n",results);
			}
			else {
				for(unsigned int shapeNum=0; shapeNum<islandVec3.size(); shapeNum++) {
					islandVecTemp = islandVec3;
					bool flag = false;
					if(ShapeMatch::SHIFT.at(shapeShift)=="SHIFT_LEFT") {
						if(shapeNum==2 || shapeNum==3 || shapeNum==4 || shapeNum==6) {
							//printf("ShadeShift: %d, ShapeShift: %d, ShapeNum: %d\n",shadeShift,shapeShift,shapeNum);
							flag =  this->shape_translation(islandVecTemp,shapeNum,shapeShift);
						}
					}
					if(ShapeMatch::SHIFT.at(shapeShift)=="SHIFT_RIGHT") {
						if(shapeNum==1 || shapeNum==2 || shapeNum==3 || shapeNum==5) {
							//printf("ShadeShift: %d, ShapeShift: %d, ShapeNum: %d\n",shadeShift,shapeShift,shapeNum);
							flag = this->shape_translation(islandVecTemp,shapeNum,shapeShift);
						}
					}
					if(flag==true) {
						this->sortIslandsByArea(islandVecTemp);
						upMap = this->createPropAreaMap(islandVecTemp,upTotalArea);
						dbMap = this->createPropAreaMap(this->dbIslandVec,dbTotalArea);
						this->fillPropAreaMapGaps(upMap,dbMap);

						float results = this->dotProduct(upMap,dbMap);
						resultVec.push_back(results);
						printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
						printf("ShapeShift: %s, ",ShapeMatch::SHIFT[shapeShift].c_str());
						printf("ShapeNum: %d, ", shapeNum);
						printf("Results: %f\n",results);
					}
				}//end for loop shapeNum
			}
		}
	}
	/*
	cout << "-----------------------------" << endl;
	for(unsigned int i=0; i<resultVec.size(); i++) {
		float results = resultVec.at(i);
		if(i>0)
			results *= pow(2,-1); // may change
		printf("Results %d: %f\n",i+1,results);
	}
	 */

	return 0.0;
}
