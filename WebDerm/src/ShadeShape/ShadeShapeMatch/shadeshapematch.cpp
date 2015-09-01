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
	ShapeMatch spm;
	int shapeSize = spm.numOfShapes();
	vector<vector<vector<Islands> > > islandVec(ss.numOfShades(),vector<vector<Islands> >(shapeSize,vector<Islands>(0,Islands())));
	for(int i=0; i<ss.numOfShades(); i++) {
		int shadeVal = ss.shade(i);
		for(int j=0; j<shapeSize; j++) {
			String shape = spm.shapeName(j);
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
	ShapeMatch spm;
	int shapeSize = spm.numOfShapes();
	vector<vector<vector<Islands> > > islandVec(shapeSize,vector<vector<Islands> >(ss.numOfShades(),vector<Islands>(0,Islands())));
	for(int i=0; i<shapeSize; i++) {
		String shape = spm.shapeName(i);
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
void ShadeShapeMatch::fillMissingLabels(Labels &upLabels, Labels &dbLabels) {
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
//! applies shape shift penalties during calculations for the shapes shifted
float ShadeShapeMatch::dotProduct(Labels &upLabels, Labels &dbLabels, int shapeNum1, int shapeNum2) {
	ShapeMatch spm;
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
		float penalty = 1.0;
		if(shapeNum1>=0 && shapeNum2>=0) {
			String shapeUP = upLabels.getShape(itUP->first);
			if(upLabels.isShapeShifted(itUP->first)) {
				penalty = spm.getShiftPenalty(shapeNum1,shapeNum2);
			}
		}
		numerSum += (itUP->second.second * itDB->second.second) * penalty;
		denomSumUP += pow(itUP->second.second,2);
		denomSumDB += pow(itDB->second.second,2);
	}
	float results = numerSum / (pow(denomSumUP,0.25) * pow(denomSumDB,0.25));
	return results;
}

//! TR1 metric using dot product (Holder's Inequality)
float ShadeShapeMatch::tr1(Labels &upLabels, Labels &dbLabels, int shapeNum1, int shapeNum2) {
	float matchVal = this->dotProduct(upLabels,dbLabels,shapeNum1,shapeNum2);
	return matchVal;
}

//! TR2 metric using SRM(Spatial Relations Matrix) comparison
float ShadeShapeMatch::tr2(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels) {
	assert(upLabels.size()>0 && dbLabels.size()>0);
	assert(upLabels.size()==dbLabels.size());
	ShadeShapeRelationMatch ssrm;
	float matchVal = ssrm.srm_match(ssrUP,upLabels,ssrDB,dbLabels);
	return matchVal;
}

/******************* PUBLIC FUNCTIONS ******************/

void ShadeShapeMatch::test(ShadeShape &ss) {
	ShadeMatch sdm;
	float totalArea = ss.area();
	sdm.setMaxShades(ss.get_shades(),ss.get_shades());
	vector<vector<vector<Islands> > > islandVec = this->groupIslandsByShape(ss);
	this->sortIslandsByArea(islandVec);
	Labels lbls = Labels(islandVec,totalArea);
	lbls.printLabels();
	cout << "----------------------------" << endl;
	ShadeShapeRelation ssr;
	ssr.spatial_relation(ss,lbls,islandVec);
}

//same as match but for testing changes
float ShadeShapeMatch::test_match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch shapematch;
	ShadeMatch shadematch;
	shadematch.setMaxShades(upSS.get_shades(),dbSS.get_shades());
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
	this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
	Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
	cout << "--------------------------" << endl;
	vector<vector<float> > resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	vector<vector<vector<Islands> > > prevIslandVec2;
	vector<vector<vector<Islands> > > islandVec3;
	float maxResult = 0.0;
	Labels largestLabelsUP;
	Labels largestLabelsDB ;
	vector<vector<vector<Islands> > > largestIslandVec;
	Mat largestImg = upSS.image(), maxMatchImg;
	for(unsigned int shadeShift=0; shadeShift<shadematch.SHIFT().size(); shadeShift++) {
		islandVec2 = this->upIslandVec;
		if(shadematch.SHIFT().at(shadeShift)!="SHIFT_NONE") {
			float prevResults = 0.0;
			ShadeShape newUpSS(upSS.image(),upSS.name());
			while(shadematch.shade_translation(newUpSS,shadeShift)) {
				ShadeShape matchSS(newUpSS.image(),newUpSS.name());
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				upLabels = Labels(islandVec2,matchSS.area(),matchSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
				float results = this->tr1(upLabelsFilled,dbLabelsFilled);

				/*** Debug Print ***/
				if(this->debugMode==1) {
					printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
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
		for(unsigned int shapeShift=0; shapeShift<shapematch.SHIFT().size(); shapeShift++) {
			if(shapematch.SHIFT()[shapeShift]=="SHIFT_START") {
				for(unsigned int shapeNum=0; shapeNum<islandVec2.size(); shapeNum++) {
					bool flag = false;
					for(unsigned int j=0; j<ShapeMatch::shiftingRules.at(shapeNum).size(); j++) {
						islandVec3 = islandVec2;
						String newShape = ShapeMatch::shiftingRules.at(shapeNum).at(j);
						int newShapeIdx = shapematch.getShapeIndex(newShape);
						flag = shapematch.shape_translation2(islandVec3,shapeNum,newShapeIdx);
						if(flag==true) {
							this->sortIslandsByArea(islandVec3);
							upLabels = Labels(islandVec3,upTotalArea,upSS.name());
							upLabelsFilled = upLabels;
							dbLabelsFilled = dbLabels;
							this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
							float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
							tr1_score = shadematch.applyShiftPenalty(upSS,tr1_score,shadeShift);

							if(tr1_score>maxResult) {
								maxResult = tr1_score;
								largestLabelsUP = upLabelsFilled;
								largestLabelsDB = dbLabelsFilled;
								largestIslandVec = islandVec3;
								maxMatchImg = largestImg;
							}

							/*** Debug Print ***/
							if(this->debugMode>=2) {
								printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
								printf("CurrShape: %s, ",shapematch.shapeName(shapeNum).c_str());
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
				this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
				float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
				tr1_score = shadematch.applyShiftPenalty(upSS,tr1_score,shadeShift);

				if(tr1_score>maxResult) {
					maxResult = tr1_score;
					largestLabelsUP = upLabelsFilled;
					largestLabelsDB = dbLabelsFilled;
					largestIslandVec = islandVec2;
					maxMatchImg = largestImg;
				}

				/*** Debug Print ***/
				if(this->debugMode>=1) {
					printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
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

	//Labels::writeCompareLabels(largestLabelsUP,largestLabelsDB);
	//imwrite(upSS.name()+"_"+dbSS.name()+"_max_match_image.png",maxMatchImg);
	Labels::printCompareLabels(largestLabelsUP,largestLabelsDB);
	cout << "---------------------------" << endl;
	ShadeShapeRelation ssrUP;
	ssrUP.spatial_relation(upSS,largestLabelsUP,largestIslandVec);
	ShadeShapeRelation ssrDB;
	ssrDB.spatial_relation(dbSS,largestLabelsDB,this->dbIslandVec);
	//float tr2_score = this->tr2(ssrUP,largestLabelsUP,ssrDB,largestLabelsDB);
	//float results = maxResult * tr2_score;
	//vector<float> vec = {results,maxResult,tr2_score};
	//resultVec.push_back(vec);
	//return *max_element(resultVec.begin(),resultVec.end());
	return 0.0;
}

vector<float> ShadeShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch shapematch;
	ShadeMatch shadematch;
	shadematch.setMaxShades(upSS.get_shades(),dbSS.get_shades());
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
	this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
	Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
	cout << "--------------------------" << endl;
	vector<vector<float> > resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	vector<vector<vector<Islands> > > prevIslandVec2;
	vector<vector<vector<Islands> > > islandVec3;
	float initialScore = 0.0;
	float maxResult = 0.0;
	Labels largestLabelsUP;
	Labels largestLabelsDB ;
	vector<vector<vector<Islands> > > largestIslandVec;
	Mat largestImg = upSS.image(), maxMatchImg;

	for(unsigned int shadeShift=0; shadeShift<shadematch.SHIFT().size(); shadeShift++) {
		islandVec2 = this->upIslandVec;
		if(shadematch.SHIFT().at(shadeShift)!="SHIFT_NONE") {
			float prevResults = initialScore;
			ShadeShape newUpSS(upSS.image(),upSS.name());
			while(shadematch.shade_translation(newUpSS,shadeShift)) {
				ShadeShape matchSS(newUpSS.image(),newUpSS.name());
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				upLabels = Labels(islandVec2,matchSS.area(),matchSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
				float results = this->tr1(upLabelsFilled,dbLabelsFilled);

				////// Debug Print //////
				if(this->debugMode==1) {
					printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
					printf("Results: %f\n",results);
					Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
					cout << shadematch.getStoredFeatIslIdx().size() << endl;
					cout << "-------------------------" << endl;
				}
				/////// End Debug Print //////
				if(results>prevResults) {
					prevResults = results;
					prevIslandVec2 = islandVec2;
					largestImg = matchSS.image();
				}
				else if(results==0.0 && prevResults==initialScore) {
					prevResults = results;
				}
				else if(results==prevResults && prevResults==initialScore) {
					prevResults = results;
				}
				else if(results>0 && results==prevResults && prevResults!=initialScore) {
					islandVec2 = prevIslandVec2;
					//shadematch.getStoredFeatIslIdx().pop_back();
					break;
				}
				else {
					islandVec2 = prevIslandVec2;
					//shadematch.getStoredFeatIslIdx().pop_back();
					break;
				}
			}
		}
		for(unsigned int shapeShift=0; shapeShift<1; shapeShift++) {
			if(shapematch.SHIFT()[shapeShift]=="SHIFT_START") {
				for(unsigned int shapeNum=0; shapeNum<islandVec2.size(); shapeNum++) {
					bool flag = false;
					for(unsigned int j=0; j<ShapeMatch::shiftingRules.at(shapeNum).size(); j++) {
						islandVec3 = islandVec2;
						String newShape = ShapeMatch::shiftingRules.at(shapeNum).at(j);
						int newShapeIdx = shapematch.getShapeIndex(newShape);
						flag = shapematch.shape_translation2(islandVec3,shapeNum,newShapeIdx);
						if(flag==true) {
							this->sortIslandsByArea(islandVec3);
							upLabels = Labels(islandVec3,upTotalArea,upSS.name());
							upLabelsFilled = upLabels;
							dbLabelsFilled = dbLabels;
							this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
							float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled,shapeNum,newShapeIdx);
							tr1_score = shadematch.applyShiftPenalty(upSS,tr1_score,shadeShift);
							if(tr1_score>maxResult) {
								maxResult = tr1_score;
								largestLabelsUP = upLabelsFilled;
								largestLabelsDB = dbLabelsFilled;
								largestIslandVec = islandVec3;
								maxMatchImg = largestImg;
							}

							///// Debug Print /////
							if(this->debugMode>=2) {
								printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
								printf("CurrShape: %s, ",shapematch.shapeName(shapeNum).c_str());
								printf("NewShape: %s\n",newShape.c_str());
								//printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_score,results);
								printf("TR1: %f\n",tr1_score);
								Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
								cout << "-------------------------" << endl;
							}
							///// End Debug Print /////
						}
					}
				}
			} else {
				upLabels = Labels(islandVec2,upSS.area(),upSS.name());
				upLabelsFilled = upLabels;
				dbLabelsFilled = dbLabels;
				this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
				float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
				tr1_score = shadematch.applyShiftPenalty(upSS,tr1_score,shadeShift);
				if(shadematch.SHIFT().at(shadeShift)=="SHIFT_NONE") {
					initialScore = tr1_score;
				}
				if(tr1_score>maxResult) {
					maxResult = tr1_score;
					largestLabelsUP = upLabelsFilled;
					largestLabelsDB = dbLabelsFilled;
					largestIslandVec = islandVec2;
					maxMatchImg = largestImg;
				}

				///// Debug Print /////
				if(this->debugMode>=1) {
					printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
					printf("CurrShape: NONE, ");
					printf("NewShape: NONE\n");
					//printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_score,results);
					printf("TR1: %f\n",tr1_score);
					Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
					cout << "-------------------------" << endl;
				}
				///// End Debug Print /////
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
