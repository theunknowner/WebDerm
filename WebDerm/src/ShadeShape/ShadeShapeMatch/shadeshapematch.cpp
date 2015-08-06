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
#include "../Labels/labels.h"

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
void ShadeShapeMatch::fillPropAreaMapGaps(Labels &upLabels, Labels &dbLabels) {
	map<String,pair<int,float> > &upMap = upLabels.getLabels();
	map<String,pair<int,float> > &dbMap = dbLabels.getLabels();
	for(auto it=upMap.begin(); it!=upMap.end(); it++) {
		String label = it->first;
		if(dbMap.find(label)==dbMap.end()) {
			dbMap[label] = std::make_pair(0,0.0);
		}
	}
	for(auto it=dbMap.begin(); it!=dbMap.end(); it++) {
		String label = it->first;
		if(upMap.find(label)==upMap.end()) {
			upMap[label] = std::make_pair(0,0.0);
		}
	}
}

//! dot product using holder's inequality
float ShadeShapeMatch::dotProduct(Labels &upLabels, Labels &dbLabels) {
	if(upLabels.size()!=dbLabels.size()) {
		cout << "ShapeMatch::dotProduct(): upLabels && dbLabels not same size!!!" << endl;
		exit(1);
	}
	float numerSum = 0.0;
	float denomSumUP = 0.0;
	float denomSumDB = 0.0;
	map<String,pair<int,float> > upMap = upLabels.getLabels();
	map<String,pair<int,float> > dbMap = dbLabels.getLabels();
	for(auto itUP=upMap.begin(), itDB=dbMap.begin(); itUP!=upMap.end(), itDB!=dbMap.end(); itUP++, itDB++) {
		numerSum += (itUP->second.second * itDB->second.second);
		denomSumUP += pow(itUP->second.second,2);
		denomSumDB += pow(itDB->second.second,2);
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
	Labels lbls = Labels(islandVec,totalArea);
	lbls.printLabels();
	ShadeShapeRelation ssr;
	ssr.spatial_relation(ss,lbls,islandVec);
}

float ShadeShapeMatch::test_match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch smatch;
	ShadeMatch shadematch;
	this->maxNumOfShades = max(upSS.numOfShades(),dbSS.numOfShades());
	float upTotalArea = upSS.area();
	float dbTotalArea = dbSS.area();
	this->upIslandVec = this->groupIslandsByShape(upSS);
	this->dbIslandVec = this->groupIslandsByShape(dbSS);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByArea(this->upIslandVec);
	Labels upLabels(this->upIslandVec,upTotalArea,upSS.name());
	Labels dbLabels(this->dbIslandVec,dbTotalArea,dbSS.name());
	Labels upLabelsFilled = upLabels;
	Labels dbLabelsFilled = dbLabels;
	this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
	ShadeShapeRelation ssrUP;
	ssrUP.spatial_relation(upSS,upLabelsFilled,this->upIslandVec);
	ShadeShapeRelation ssrDB;
	ssrDB.spatial_relation(dbSS,dbLabelsFilled,this->dbIslandVec);
	float matchVal = ssrUP.srm_match(ssrUP,upLabelsFilled,ssrDB,dbLabelsFilled);
	FILE * fp;
	String filename = upSS.name() + "_" + dbSS.name() + "_tr2_match_score.txt";
	fp = fopen(filename.c_str(),"w");
	fprintf(fp,"%f\n",matchVal);
	fclose(fp);
	cout << "Match: " << matchVal << endl;
	return matchVal;
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

	Labels upLabels;
	Labels dbLabels;
	map<String,pair<int,float> > upMap;
	map<String,pair<int,float> > dbMap;
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
				upLabels = Labels(islandVec3,upTotalArea);
				dbLabels = Labels(this->dbIslandVec,dbTotalArea);
				this->fillPropAreaMapGaps(upLabels,dbLabels);
				upMap = upLabels.getLabels();
				dbMap = dbLabels.getLabels();
				if(ShadeMatch::SHIFT.at(shadeShift)=="SHIFT_NONE" && ShapeMatch::SHIFT.at(shapeShift)=="SHIFT_NONE") {
					for(auto itUP=upMap.begin(),itDB=dbMap.begin();itUP!=upMap.end(),itDB!=dbMap.end(); itUP++, itDB++) {
						printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second.second,itDB->first.c_str(),itDB->second.second);
					}
				}
				float results = this->dotProduct(upLabels,dbLabels);
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
						upLabels = Labels(islandVecTemp,upTotalArea);
						dbLabels = Labels(this->dbIslandVec,dbTotalArea);
						this->fillPropAreaMapGaps(upLabels,dbLabels);

						float results = this->dotProduct(upLabels,dbLabels);
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

	float matchVal = *max_element(resultVec.begin(),resultVec.end());
	/*
	cout << "-----------------------------" << endl;
	for(unsigned int i=0; i<resultVec.size(); i++) {
		float results = resultVec.at(i);
		if(i>0)
			results *= pow(2,-1); // may change
		printf("Results %d: %f\n",i+1,results);
	}
	 */
	FILE * fp;
	String filename = upSS.name() + "_" + dbSS.name() + "_tr1_match_score.txt";
	fp = fopen(filename.c_str(),"w");
	fprintf(fp,"%f\n",matchVal);
	fclose(fp);

	return matchVal;
}

void ShadeShapeMatch::printLabels(map<String,float> &labels) {
	for(auto it=labels.begin(); it!=labels.end(); it++) {
		int index = distance(labels.begin(),it);
		printf("%d) ",index);
		cout << it->first << ": " << it->second << endl;
	}
	cout << "------------------" << endl;
}
