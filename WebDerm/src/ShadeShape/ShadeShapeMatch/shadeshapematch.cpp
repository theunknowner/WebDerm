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
#include "../ShadeShapeRelation/ssr_match.h"

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

//! compare two labels and fill in missing labels
void ShadeShapeMatch::fillPropAreaMapGaps(Labels &upLabels, Labels &dbLabels) {
	map<String,pair<int,float> > &upMap = upLabels.getMap();
	map<String,pair<int,float> > &dbMap = dbLabels.getMap();
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
//! TR1
float ShadeShapeMatch::dotProduct(Labels &upLabels, Labels &dbLabels) {
	if(upLabels.size()!=dbLabels.size()) {
		cout << "ShapeMatch::dotProduct(): upLabels && dbLabels not same size!!!" << endl;
		exit(1);
	}
	float numerSum = 0.0;
	float denomSumUP = 0.0;
	float denomSumDB = 0.0;
	map<String,pair<int,float> > upMap = upLabels.getMap();
	map<String,pair<int,float> > dbMap = dbLabels.getMap();
	for(auto itUP=upMap.begin(), itDB=dbMap.begin(); itUP!=upMap.end(), itDB!=dbMap.end(); itUP++, itDB++) {
		numerSum += (itUP->second.second * itDB->second.second);
		denomSumUP += pow(itUP->second.second,2);
		denomSumDB += pow(itDB->second.second,2);
	}
	float results = numerSum / (sqrt(denomSumUP) * sqrt(denomSumDB));
	return results;
}

//! TR1 matching using dot product && STT
float ShadeShapeMatch::tr1_match(ShadeShape &upSS, ShadeShape &dbSS) {
	ShapeMatch smatch;
	ShadeMatch shadematch;
	this->setMaxShades(upSS.get_shades(),dbSS.get_shades());
	float upTotalArea = upSS.area();
	float dbTotalArea = dbSS.area();
	this->upIslandVec = this->groupIslandsByShape(upSS);
	this->dbIslandVec = this->groupIslandsByShape(dbSS);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByArea(this->upIslandVec);

	Labels upLabels(this->upIslandVec,upTotalArea,upSS.name());
	Labels dbLabels(this->dbIslandVec,dbTotalArea,dbSS.name());
	map<String,pair<int,float> > upMap; //for printf
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
				upMap = upLabels.getMap();
				dbMap = dbLabels.getMap();
				float results = this->dotProduct(upLabels,dbLabels);
				resultVec.push_back(results);
				if(this->debugMode>=2) {
					if(ShadeMatch::SHIFT.at(shadeShift)=="SHIFT_NONE" && ShapeMatch::SHIFT.at(shapeShift)=="SHIFT_NONE") {
						for(auto itUP=upMap.begin(),itDB=dbMap.begin();itUP!=upMap.end(),itDB!=dbMap.end(); itUP++, itDB++) {
							printf("%s: %f | %s: %f\n",itUP->first.c_str(),itUP->second.second,itDB->first.c_str(),itDB->second.second);
						}
					}
					printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
					printf("ShapeShift: %s, ",ShapeMatch::SHIFT[shapeShift].c_str());
					printf("ShapeNum: NULL, ");
					printf("Results: %f\n",results);
				}
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

						if(this->debugMode>=2) {
							printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
							printf("ShapeShift: %s, ",ShapeMatch::SHIFT[shapeShift].c_str());
							printf("ShapeNum: %d, ", shapeNum);
							printf("Results: %f\n",results);
						}

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

	return matchVal;
}

//! TR2 using SRMs(Spatial Relations Matrix)
float ShadeShapeMatch::tr2_match(ShadeShape &upSS, ShadeShape &dbSS) {
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
	ShadeShapeRelationMatch ssrm;
	float matchVal = ssrm.srm_match(ssrUP,upLabelsFilled,ssrDB,dbLabelsFilled);
	return matchVal;
}

//! TR1 metric using dot product (Holder's Inequality)
float ShadeShapeMatch::tr1(Labels &upLabels, Labels &dbLabels) {
	float matchVal = this->dotProduct(upLabels,dbLabels);
	return matchVal;
}

//! TR2 metric using SRM(Spatial Relations Matrix) comparison
float ShadeShapeMatch::tr2(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels) {
	assert(upLabels.size()==dbLabels.size());
	ShadeShapeRelationMatch ssrm;
	float matchVal = ssrm.srm_match(ssrUP,upLabels,ssrDB,dbLabels);
	return matchVal;
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
	Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
	cout << "--------------------------" << endl;
	vector<float> resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	vector<vector<vector<Islands> > > prevIslandVec2;
	vector<vector<vector<Islands> > > islandVec3;
	vector<vector<vector<Islands> > > islandVecTemp;
	for(unsigned int shadeShift=0; shadeShift<ShadeMatch::SHIFT.size(); shadeShift++) {
		islandVec2 = this->upIslandVec;
		if(ShadeMatch::SHIFT.at(shadeShift)!="SHIFT_NONE") {
			float prevResults = 0.0;
			ShadeShape newUpSS(upSS.image(),upSS.name());
			while(this->shade_translation(newUpSS,shadeShift)) {
				ShadeShape matchSS(newUpSS.image(),newUpSS.name());
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				upLabels = Labels(islandVec2,newUpSS.area(),newUpSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
				float results = this->tr1(upLabelsFilled,dbLabelsFilled);

				/*** Debug Print ***/
				if(this->debugMode==1) {
					Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
					printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
					printf("Results: %f\n",results);
					cout << "-------------------------" << endl;
				}
				/*** End Debug Print ***/

				if(results>prevResults) {
					prevResults = results;
					prevIslandVec2 = islandVec2;
				}
				else {
					islandVec2 = prevIslandVec2;
					break;
				}
			}
		}
		for(unsigned int shapeShift=0; shapeShift<ShapeMatch::SHIFT.size(); shapeShift++) {
			if(ShapeMatch::SHIFT[shapeShift]=="SHIFT_START") {
				for(unsigned int shapeNum=0; shapeNum<islandVec2.size(); shapeNum++) {
					bool flag = false;
					for(unsigned int j=0; j<ShapeMatch::shiftingRules.at(shapeNum).size(); j++) {
						islandVec3 = islandVec2;
						String newShape = ShapeMatch::shiftingRules.at(shapeNum).at(j);
						int newShapeIdx = this->getShapeIndex(newShape);
						flag = this->shape_translation2(islandVec3,shapeNum,newShapeIdx);
						if(flag==true) {
							this->sortIslandsByArea(islandVec3);
							upLabels = Labels(islandVec3,upTotalArea,upSS.name());
							upLabelsFilled = upLabels;
							dbLabelsFilled = dbLabels;
							this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
							float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
							tr1_score = ShadeMatch::applyShiftPenalty(tr1_score,shadeShift);
							tr1_score = ShapeMatch::applyShiftPenalty(tr1_score,shapeNum,newShapeIdx);
							ShadeShapeRelation ssrUP;
							ssrUP.spatial_relation(upSS,upLabelsFilled,islandVec3);
							ShadeShapeRelation ssrDB;
							ssrDB.spatial_relation(dbSS,dbLabelsFilled,this->dbIslandVec);
							float tr2_score = this->tr2(ssrUP,upLabelsFilled,ssrDB,dbLabelsFilled);
							float results = tr1_score * tr2_score;
							resultVec.push_back(results);

							/*** Debug Print ***/
							if(this->debugMode>=2) {
								printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
								printf("CurrShape: %s, ",this->shapeName(shapeNum).c_str());
								printf("NewShape: %s\n",newShape.c_str());
								printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_score,results);
								//printf("TR1: %f = %f\n",tr1_score,results);
								Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
								cout << "-------------------------" << endl;
							}
							/*** End Debug Print ***/
						}
					}
				}
			} else {
				upLabels = Labels(islandVec2,upSS.area(),upSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
				float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
				tr1_score = ShadeMatch::applyShiftPenalty(tr1_score,shadeShift);
				ShadeShapeRelation ssrUP;
				ssrUP.spatial_relation(upSS,upLabelsFilled,islandVec2);
				ShadeShapeRelation ssrDB;
				ssrDB.spatial_relation(dbSS,dbLabelsFilled,this->dbIslandVec);
				float tr2_score = this->tr2(ssrUP,upLabelsFilled,ssrDB,dbLabelsFilled);
				float results = tr1_score * tr2_score;
				resultVec.push_back(results);
				/*** Debug Print ***/
				if(this->debugMode>=1) {
					printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
					printf("CurrShape: NONE, ");
					printf("NewShape: NONE\n");
					printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_score,results);
					//printf("TR1: %f = %f\n",tr1_score,results);
					Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
					cout << "-------------------------" << endl;
				}
				/*** End Debug Print ***/
			}
		}
	}

	float matchVal = *max_element(resultVec.begin(),resultVec.end());
	return matchVal;
}

vector<float> ShadeShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch smatch;
	ShadeMatch shadematch;
	this->setMaxShades(upSS.get_shades(),dbSS.get_shades());
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
	//Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
	//cout << "--------------------------" << endl;
	vector<vector<float> > resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	vector<vector<vector<Islands> > > prevIslandVec2;
	vector<vector<vector<Islands> > > islandVec3;
	float maxResult = 0.0;
	Labels largestLabelsUP;
	Labels largestLabelsDB;
	vector<vector<vector<Islands> > > largestIslandVec;
	Mat largestImg = upSS.image(), maxMatchImg;
	for(unsigned int shadeShift=0; shadeShift<ShadeMatch::SHIFT.size(); shadeShift++) {
		islandVec2 = this->upIslandVec;
		if(ShadeMatch::SHIFT.at(shadeShift)!="SHIFT_NONE") {
			float prevResults = 0.0;
			ShadeShape newUpSS(upSS.image(),upSS.name());
			while(this->shade_translation(newUpSS,shadeShift)) {
				ShadeShape matchSS(newUpSS.image(),newUpSS.name());
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				upLabels = Labels(islandVec2,matchSS.area(),matchSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
				float results = this->tr1(upLabelsFilled,dbLabelsFilled);

				/*** Debug Print ***/
				if(this->debugMode==1) {
					printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
					printf("Results: %f\n",results);
					Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
					cout << "-------------------------" << endl;
				}
				/*** End Debug Print ***/
				if(results>prevResults) {
					prevResults = results;
					prevIslandVec2 = islandVec2;
					largestImg = matchSS.image();
				}
				else if(results==0.0 && prevResults==0.0) {
					prevResults = results;
				}
				else {
					islandVec2 = prevIslandVec2;
					break;
				}
			}
		}
		for(unsigned int shapeShift=0; shapeShift<ShapeMatch::SHIFT.size(); shapeShift++) {
			if(ShapeMatch::SHIFT[shapeShift]=="SHIFT_START") {
				for(unsigned int shapeNum=0; shapeNum<islandVec2.size(); shapeNum++) {
					bool flag = false;
					for(unsigned int j=0; j<ShapeMatch::shiftingRules.at(shapeNum).size(); j++) {
						islandVec3 = islandVec2;
						String newShape = ShapeMatch::shiftingRules.at(shapeNum).at(j);
						int newShapeIdx = this->getShapeIndex(newShape);
						flag = this->shape_translation2(islandVec3,shapeNum,newShapeIdx);
						if(flag==true) {
							this->sortIslandsByArea(islandVec3);
							upLabels = Labels(islandVec3,upTotalArea,upSS.name());
							upLabelsFilled = upLabels;
							dbLabelsFilled = dbLabels;
							this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
							float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
							tr1_score = ShadeMatch::applyShiftPenalty(tr1_score,shadeShift);
							tr1_score = ShapeMatch::applyShiftPenalty(tr1_score,shapeNum,newShapeIdx);

							if(tr1_score>maxResult) {
								maxResult = tr1_score;
								largestLabelsUP = upLabelsFilled;
								largestLabelsDB = dbLabelsFilled;
								largestIslandVec = islandVec3;
								maxMatchImg = largestImg;
							}

							/*** Debug Print ***/
							if(this->debugMode>=2) {
								printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
								printf("CurrShape: %s, ",this->shapeName(shapeNum).c_str());
								printf("NewShape: %s\n",newShape.c_str());
								//printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_score,results);
								printf("TR1: %f\n",tr1_score);
								Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
								cout << "-------------------------" << endl;
							}
							/*** End Debug Print ***/
						}
					}
				}
			} else {
				upLabels = Labels(islandVec2,upSS.area(),upSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillPropAreaMapGaps(upLabelsFilled,dbLabelsFilled);
				float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
				tr1_score = ShadeMatch::applyShiftPenalty(tr1_score,shadeShift);

				if(tr1_score>maxResult) {
					maxResult = tr1_score;
					largestLabelsUP = upLabelsFilled;
					largestLabelsDB = dbLabelsFilled;
					largestIslandVec = islandVec2;
					maxMatchImg = largestImg;
				}

				/*** Debug Print ***/
				if(this->debugMode>=1) {
					printf("ShadeShift: %s, ",ShadeMatch::SHIFT[shadeShift].c_str());
					printf("CurrShape: NONE, ");
					printf("NewShape: NONE\n");
					//printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_score,results);
					printf("TR1: %f\n",tr1_score);
					Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
					cout << "-------------------------" << endl;
				}
				/*** End Debug Print ***/
			}
		}
	}

	Labels::writeCompareLabels(largestLabelsUP,largestLabelsDB);
	imwrite(upSS.name()+"_"+dbSS.name()+"_max_match_image.png",maxMatchImg);
	ShadeShapeRelation ssrUP;
	ssrUP.spatial_relation(upSS,largestLabelsUP,largestIslandVec);
	ShadeShapeRelation ssrDB;
	ssrDB.spatial_relation(dbSS,largestLabelsDB,this->dbIslandVec);
	float tr2_score = this->tr2(ssrUP,largestLabelsUP,ssrDB,largestLabelsDB);
	float results = maxResult * tr2_score;
	vector<float> vec = {results,maxResult,tr2_score};
	resultVec.push_back(vec);
	return *max_element(resultVec.begin(),resultVec.end());
}

void ShadeShapeMatch::printLabels(map<String,float> &labels) {
	for(auto it=labels.begin(); it!=labels.end(); it++) {
		int index = distance(labels.begin(),it);
		printf("%d) ",index);
		cout << it->first << ": " << it->second << endl;
	}
	cout << "------------------" << endl;
}

void ShadeShapeMatch::debug_mode(int mode) {
	this->debugMode = mode;
}
