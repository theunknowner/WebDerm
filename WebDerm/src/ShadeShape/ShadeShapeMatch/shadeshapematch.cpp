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
float ShadeShapeMatch::dotProduct(Labels &upLabels, Labels &dbLabels) {
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
		if(upLabels.isShapeShifted(itUP->first)) {
			int prevShapeNum = upLabels.getPrevShapeNum(itUP->first);
			int shapeNum = upLabels.getShapeNum(itUP->first);
			if(prevShapeNum>=0 && shapeNum>=0) {
				penalty = spm.getShiftPenalty(prevShapeNum,shapeNum);
			} else {
				printf("ShadeShapeMatch::dotProduct() %s label does not exist!\n",itUP->first.c_str());
				printf("PrevShapeNum: %d, CurrShapeNum: %d\n",prevShapeNum,shapeNum);
			}
		}
		numerSum += (itUP->second.second * itDB->second.second) * penalty;
		denomSumUP += pow(itUP->second.second,2);
		denomSumDB += pow(itDB->second.second,2);
	}
	float results = numerSum / sqrt(denomSumUP) * sqrt(denomSumDB);
	return results;
}

//! TR1 metric using dot product (Holder's Inequality)
float ShadeShapeMatch::tr1(Labels &upLabels, Labels &dbLabels) {
	float matchVal = this->dotProduct(upLabels,dbLabels);
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
	vector<vector<vector<Islands> > > largestIslandVec2;
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
			float largestResult = initialScore;
			largestIslandVec2 = islandVec2;
			int count=0;

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
					printf("LargestResult: %f\n",largestResult);
					printf("StoreIdx.size(): %lu\n",shadematch.getStoredFeatIslIdx().size());
					printf("isStored: %d\n",shadematch.isFeatIslStored());
					cout << "-------------------------" << endl;
				}
				/////// End Debug Print //////
				if(shadematch.isFeatIslStored()) {
					count++;
					if(count<=3 && results>largestResult) {
						count=0;
						largestImg = matchSS.image();
						largestIslandVec2 = islandVec2;
					}
					else if(count>=3 && results<=largestResult) {
						islandVec2 = largestIslandVec2;
						break;
					}
				}
			}
		}
		vector<vector<int> > islandVecIdxSorted = shapematch.getIslandVecIdxByArea(islandVec2);
		int shapeNum1 = islandVecIdxSorted.at(0).at(0);
		int shapeNum2 = islandVecIdxSorted.at(1).at(0);
		int ruleSize1 = ShapeMatch::shiftingRules.at(shapeNum1).size();
		int ruleSize2 = ShapeMatch::shiftingRules.at(shapeNum2).size();
		//! -1 = no shifting
		for(int shapeShift1=-1; shapeShift1<ruleSize1; shapeShift1++) {
			islandVec3 = islandVec2;
			bool flag1 = false;
			String newShape1 = "NONE";
			if(shapeShift1>=0) {
				newShape1 = ShapeMatch::shiftingRules.at(shapeNum1).at(shapeShift1);
				int newShapeIdx1 = shapematch.getShapeIndex(newShape1);
				flag1 = shapematch.shape_translation(islandVec3,shapeNum1,newShapeIdx1,0);
			}
			for(int shapeShift2=-1; shapeShift2<ruleSize2; shapeShift2++) {
				vector<vector<vector<Islands> > > islandVec4 = islandVec3;
				bool flag2 = false;
				String newShape2 = "NONE";
				if(shapeShift2>=0) {
					newShape2 = ShapeMatch::shiftingRules.at(shapeNum2).at(shapeShift2);
					int newShapeIdx2 = shapematch.getShapeIndex(newShape2);
					flag2 = shapematch.shape_translation(islandVec4,shapeNum2,newShapeIdx2,1);
				}
				if((shapeShift1==-1 && shapeShift2==-1) || (flag1==true || flag2==true)) {
					this->sortIslandsByArea(islandVec4);
					upLabels = Labels(islandVec4,upTotalArea,upSS.name());
					upLabelsFilled = upLabels;
					dbLabelsFilled = dbLabels;
					this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
					float tr1_score = this->tr1(upLabelsFilled,dbLabelsFilled);
					tr1_score = shadematch.applyShiftPenalty(upSS,tr1_score,shadeShift);

					if(shapeShift1==-1 && shapeShift2==-1) {
						if(shadematch.SHIFT().at(shadeShift)=="SHIFT_NONE") {
							initialScore = tr1_score;
						}
					}
					if(tr1_score>maxResult) {
						maxResult = tr1_score;
						largestLabelsUP = upLabelsFilled;
						largestLabelsDB = dbLabelsFilled;
						largestIslandVec = islandVec4;
						maxMatchImg = largestImg;
					}

					///// Debug Print /////
					if(this->debugMode>=2) {
						printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
						printf("CurrShape1: %s, ",shapematch.shapeName(shapeNum1).c_str());
						printf("NewShape1: %s\n",newShape1.c_str());
						printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
						printf("CurrShape2: %s, ",shapematch.shapeName(shapeNum2).c_str());
						printf("NewShape2: %s\n",newShape2.c_str());
						printf("Flag1: %d, Flag2: %d\n",flag1,flag2);
						printf("TR1: %f\n",tr1_score);
						Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled,1);
						cout << "-------------------------" << endl;
					}
					///// End Debug Print /////
				}// end if flag1 || flag2
			}// end for shapeShift2
		}// end for shapeShift1
	}// end shadeShift

	Labels::writeCompareLabels(largestLabelsUP,largestLabelsDB,1);
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
