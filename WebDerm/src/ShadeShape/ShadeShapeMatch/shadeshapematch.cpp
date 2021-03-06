/*
 * shadeshapematch.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#include "shadeshapematch.h"
#include "../../functions.h"
#include "../../Algorithms/jaysort.h"
#include "../ShapeMatch/shapematch.h"
#include "../ShadeMatch/shadematch.h"
#include "../shadeshape.h"
#include "../ShadeShapeRelation/shadeshaperelation.h"
#include "../Labels/labels.h"
#include "../ShadeShapeRelation/ssr_match.h"
#include "../../PrintStream/printstream.h"

using namespace ip;

/******************* PRIVATE FUNCTIONS ******************/

//! groups the islands of a feature by shade then shape
vector<vector<vector<Islands> > > ShadeShapeMatch::groupIslandsByShade(ShadeShape &ss) {
	ShapeMatch spm;
	int shapeSize = spm.numOfShapes();
	vector<vector<vector<Islands> > > islandVec(ss.numOfShades(),vector<vector<Islands> >(shapeSize,vector<Islands>(0,Islands())));
	for(int i=0; i<ss.numOfShades(); i++) {
		int shadeVal = ss.shade(i);
		for(int j=0; j<shapeSize; j++) {
			String shape = spm.getShapeName(j);
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
		String shape = spm.getShapeName(i);
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
	map<String,int> &upShadeLevelMap = upLabels.getShadeLevelMap();
	map<String,int> &dbShadeLevelMap = dbLabels.getShadeLevelMap();
	map<String,int> &upShapeNumMap = upLabels.getShapeMap();
	map<String,int> &dbShapeNumMap = dbLabels.getShapeMap();
	for(auto it=upMap.begin(); it!=upMap.end(); it++) {
		String label = it->first;
		if(dbMap.find(label)==dbMap.end()) {
			dbMap[label] = std::make_pair(0,0.0);
			dbShadeLevelMap[label] = upLabels.getShadeLevel(label);
			dbShapeNumMap[label] = upLabels.getShapeNum(label);
		}
	}
	for(auto it=dbMap.begin(); it!=dbMap.end(); it++) {
		String label = it->first;
		if(upMap.find(label)==upMap.end()) {
			upMap[label] = std::make_pair(0,0.0);
			upShadeLevelMap[label] = dbLabels.getShadeLevel(label);
			upShapeNumMap[label] = dbLabels.getShapeNum(label);
		}
	}
}

//! dot product using holder's inequality
//! TR1
//! applies shape shift penalties during calculations for the shapes shifted
float ShadeShapeMatch::dotProduct(Labels &upLabels, Labels &dbLabels) {
	ShapeMatch spm;
	ShadeMatch sdm;
	if(upLabels.size()!=dbLabels.size()) {
		cout << "ShapeMatch::dotProduct(): upLabels && dbLabels not same size!!!" << endl;
		exit(1);
	}
	float numerSum = 0.0;
	float denomSumUP = 0.0;
	float denomSumDB = 0.0;
	map<String,pair<int,float> > upMap = upLabels.getMap();
	map<String,pair<int,float> > dbMap = dbLabels.getMap();
	this->shapeTranslateCount.resize(spm.numOfShapes(),vector<int>(spm.numOfShapes(),0)); // 10 shapes
	for(auto itUP=upMap.begin(), itDB=dbMap.begin(); itUP!=upMap.end(), itDB!=dbMap.end(); itUP++, itDB++) {
		String label = itUP->first;
		float penalty = 1.0;
		int shapeNum = upLabels.getShapeNum(label);
		if(shapeNum==-1) shapeNum = dbLabels.getShapeNum(label);
		if(upLabels.isShapeShifted(label)) {
			int prevShapeNum = upLabels.getPrevShapeNum(label);
			if(prevShapeNum>=0 && shapeNum>=0) {
				penalty = spm.getShiftPenalty(prevShapeNum,shapeNum);
			} else {
				printf("ShadeShapeMatch::dotProduct() %s label does not exist!\n",itUP->first.c_str());
				printf("PrevShapeNum: %d, :CurrShapeNum: %d\n",prevShapeNum,shapeNum);
			}
		}
		int shadeLevel = upLabels.getShadeLevel(label);
		float shadeWeight = sdm.applyShadeWeights(shadeLevel);
		float shapeWeight = spm.applyShapeWeight(shapeNum);
		numerSum += (itUP->second.second * itDB->second.second) * penalty * shadeWeight * shapeWeight;
		denomSumUP += pow(itUP->second.second,2);
		denomSumDB += pow(itDB->second.second,2);
	}
	float denomSum = sqrt(denomSumUP) * sqrt(denomSumDB);
	float results = numerSum / denomSum;
	return results;
}

float ShadeShapeMatch::tr1ForShade(Labels &upLabels, Labels &dbLabels) {
	assert(upLabels.size()==dbLabels.size());
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
	float denomSum = sqrt(denomSumUP) * sqrt(denomSumDB);
	float results = numerSum / denomSum;
	return results;
}

//! TR1 metric using dot product (Holder's Inequality)
float ShadeShapeMatch::tr1(Labels &upLabels, Labels &dbLabels) {
	float matchVal = this->dotProduct(upLabels,dbLabels);
	return matchVal;
}

//! TR2 metric using SRM(Spatial Relations Matrix) comparison
//! returns vector holding pos[0] = match score; pos[1] = mismatch score.
vector<float> ShadeShapeMatch::tr2(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels, String nStr) {
	assert(upLabels.size()>0 && dbLabels.size()>0);
	assert(upLabels.size()==dbLabels.size());
	ShadeShapeRelationMatch ssrm;
	ssrm.srm_match(ssrUP,upLabels,ssrDB,dbLabels,nStr);
	float matchScore = ssrm.getMatchScore();
	float mismatchScore = ssrm.getMismatchScore();

	//store the ESG variable data to PrintStream to be printed out later
	PrintStream esgPS = ssrm.getEsgPrintStream();
	if(this->esgPS_Map.find(nStr)==this->esgPS_Map.end()) {
		this->esgPS_Map[nStr] = esgPS;
	}
	//store the entropy variable data to PrintStream to be printed out later
	PrintStream entropyPS = ssrm.getEntropyPrintStream();
	if(this->entropyPS_Map.find(nStr)==this->entropyPS_Map.end()) {
		this->entropyPS_Map[nStr] = entropyPS;
	}
	PrintStream mismatchEntropyPS = ssrm.getMismatchEntropyPrintStream();
	if(this->mismatchEntropyPS_Map.find(nStr)==this->mismatchEntropyPS_Map.end()) {
		this->mismatchEntropyPS_Map[nStr] = mismatchEntropyPS;
	}
	PrintStream noEntropyPS = ssrm.getNoEntropyPrintStream();
	if(this->noEntropyPS_Map.find(nStr)==this->noEntropyPS_Map.end()) {
		this->noEntropyPS_Map[nStr] = noEntropyPS;
	}

	vector<float> results = {matchScore,mismatchScore};

	return results;
}

/******************* PUBLIC FUNCTIONS ******************/

void ShadeShapeMatch::test(ShadeShape &ss) {
	ShadeMatch sdm;
	float totalArea = ss.area();
	sdm.setMaxShades(ss.get_shades(),ss.get_shades());
	vector<vector<vector<Islands> > > islandVec = this->groupIslandsByShape(ss);
	this->sortIslandsByArea(islandVec);
	Labels lbls = Labels(islandVec,totalArea);
	//lbls.printLabels();
	//cout << "----------------------------" << endl;
	//ShadeShapeRelation ssr;
	//ssr.spatial_relation(ss,lbls,islandVec,1);
}

//same as match but for testing changes
float ShadeShapeMatch::test_match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch shapematch;
	ShadeMatch shadematch;
	String newNameUP = upSS.name()+"_"+dbSS.name();
	String newNameDB = dbSS.name()+"_"+upSS.name();
	shadematch.setMaxShades(upSS.get_shades(),dbSS.get_shades());
	float upTotalArea = upSS.areaPostDensityConnector();
	float dbTotalArea = dbSS.areaPostDensityConnector();
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

	ShadeShapeRelation ssrUP;
	ssrUP.spatial_relation(upSS,upLabelsFilled,this->upIslandVec,0,upSS.name());
	//ShadeShapeRelation ssrDB;
	//ssrDB.spatial_relation(dbSS,dbLabelsFilled,this->dbIslandVec,0,dbSS.name());
	//ShadeShapeRelationMatch ssrm;
	ssrUP.get_srm().downScaleSrm();
	ssrUP.get_srm().printDownScaleSrm("9_Unknown_s4","5_Excavated_s1");

	return 0.0;
}

vector<float> ShadeShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	ShapeMatch shapematch;
	ShadeMatch shadematch;
	String newNameUP = upSS.name()+"_"+dbSS.name();
	String newNameDB = dbSS.name()+"_"+upSS.name();
	shadematch.setMaxShades(upSS.get_shades(),dbSS.get_shades());
	float upTotalArea = upSS.areaPostDensityConnector();
	float dbTotalArea = dbSS.areaPostDensityConnector();
	this->upIslandVec = this->groupIslandsByShape(upSS);
	this->dbIslandVec = this->groupIslandsByShape(dbSS);
	this->sortIslandsByArea(this->dbIslandVec);
	this->sortIslandsByArea(this->upIslandVec);
	Labels upLabels(this->upIslandVec,upTotalArea,upSS.name());
	Labels dbLabels(this->dbIslandVec,dbTotalArea,dbSS.name());
	Labels upLabelsFilled = upLabels;
	Labels dbLabelsFilled = dbLabels;
	this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
	//Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
	//cout << "--------------------------" << endl;
	vector<vector<float> > resultVec;
	vector<vector<vector<Islands> > > islandVec2;
	vector<vector<vector<Islands> > > islandVec3;
	float maxResults = 0.0, finalTR1 = 0.0, finalTR2_match = 0.0, finalTR2_mismatch = 0.0;
	Labels largestLabelsUP = upLabelsFilled;
	Labels largestLabelsDB = dbLabelsFilled ;
	vector<vector<vector<Islands> > > largestIslandVec;
	Mat largestImg = upSS.image(), maxMatchImg = upSS.image();
	String maxNStr = "";

	float prevScore = tr1ForShade(upLabelsFilled,dbLabelsFilled); //> initialize
	prevScore = roundDecimal(prevScore,6);
	for(int n=0; n<3; n++) {
		for(unsigned int shadeShift=0; shadeShift<shadematch.SHIFT().size(); shadeShift++) {
			bool isShifted = false;
			islandVec2 = this->upIslandVec;
			float largestResult = prevScore;
			vector<vector<vector<Islands> > > largestIslandVec2 = islandVec2;
			ImageData upId = upSS.getImageData();
			ShadeShape newUpSS(upId,false);
			if(n==0) {
				while(shadematch.shade_translation1(newUpSS,shadeShift)) {
					ShadeShape matchSS(newUpSS.getImageData(),false);
					islandVec2 = this->groupIslandsByShape(matchSS);
					this->sortIslandsByArea(islandVec2);
					upLabels = Labels(islandVec2,matchSS.area(),matchSS.name());
					upLabelsFilled = upLabels;
					dbLabelsFilled = dbLabels;
					this->fillMissingLabels(upLabelsFilled,dbLabelsFilled);
					float results = this->tr1ForShade(upLabelsFilled,dbLabelsFilled);
					results = roundDecimal(results,6);
					///////////////////////////////////////////////
					//################ Debug Print ####################//
					if(this->debugMode>=1) {
						printf("n: %d\n",n);
						printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
						printf("Results: %f\n",results);
						printf("PrevScore: %f\n",prevScore);
						Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled);
						printf("LargestResult: %f\n",largestResult);
						printf("StoreIdx.size(): %lu\n",shadematch.getStoredFeatIslIdx().size());
						printf("isStored: %d\n",shadematch.isFeatIslStored());
						cout << "-------------------------" << endl;
					}
					//############### End Debug Print ###################//
					////////////////////////////////////////////////
					if(results>largestResult || (results>=largestResult && shadeShift==ShadeMatch::SHIFT_NONE)) {
						isShifted = true;
						largestResult = results;
						largestIslandVec2 = islandVec2;
						largestImg = matchSS.image();
						if(shadeShift==ShadeMatch::SHIFT_NONE) {
							islandVec2 = largestIslandVec2;
							break;
						}
					} else {
						if(shadematch.isFeatIslStored()) {
							shadematch.getStoredFeatIslIdx().pop_back();
						}
						islandVec2 = largestIslandVec2;
						break;
					}
				} // end while
			} else  if(n==1 && shadeShift==ShadeMatch::SHIFT_LEFT) {
				isShifted = shadematch.shade_translation2(newUpSS,shadeShift);
				ShadeShape matchSS(newUpSS.getImageData(),false);
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				largestImg = matchSS.image().clone();
			} else if(n==2 && shadeShift==ShadeMatch::SHIFT_RIGHT) {
				isShifted = shadematch.shade_translation2(newUpSS,shadeShift);
				ShadeShape matchSS(newUpSS.getImageData(),false);
				islandVec2 = this->groupIslandsByShape(matchSS);
				this->sortIslandsByArea(islandVec2);
				largestImg = matchSS.image().clone();
			}
			// start shape shifting
			if(isShifted) {
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
						try {
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
								String nStr = "n"+toString(n)+"_shd"+toString(shadeShift)+"_shp"+toString(shapeShift1)+toString(shapeShift2);
								ShadeShapeRelation ssrUP;
								ssrUP.spatial_relation(upSS,upLabelsFilled,islandVec4,1,newNameUP);
								ShadeShapeRelation ssrDB;
								ssrDB.spatial_relation(dbSS,dbLabelsFilled,this->dbIslandVec,1,newNameDB);
								vector<float> tr2_scores = this->tr2(ssrUP,upLabelsFilled,ssrDB,dbLabelsFilled,nStr);
								float results = tr1_score * tr2_scores.at(0);

								//printf("%s\n",nStr.c_str());
								//printf("TR1: %f x TR2: %f = %f\n",tr1_score,tr2_scores.at(0),results);
								String labelFilename = upLabelsFilled.name()+"_"+dbLabelsFilled.name()+"_labels_"+nStr;
								Labels::writeCompareLabels(labelFilename,upLabelsFilled,dbLabelsFilled,tr1_score,1);

								if(results>maxResults) {
									maxResults = results;
									finalTR1 = tr1_score;
									finalTR2_match = tr2_scores.at(0);
									finalTR2_mismatch = tr2_scores.at(1);
									largestLabelsUP = upLabelsFilled;
									largestLabelsDB = dbLabelsFilled;
									largestIslandVec = islandVec4;
									maxMatchImg = largestImg;
									maxNStr = nStr; //use for labeling image
								}

								///// Debug Print /////
								if(this->debugMode>=2) {
									printf("n: %d\n", n);
									printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
									printf("CurrShape1: %s, ",shapematch.getShapeName(shapeNum1).c_str());
									printf("NewShape1: %s\n",newShape1.c_str());
									printf("ShadeShift: %s, ",shadematch.SHIFT()[shadeShift].c_str());
									printf("CurrShape2: %s, ",shapematch.getShapeName(shapeNum2).c_str());
									printf("NewShape2: %s\n",newShape2.c_str());
									printf("Flag1: %d, Flag2: %d\n",flag1,flag2);
									printf("TR1: %f\n",tr1_score);
									printf("TR2: %f\n",tr2_scores.at(0));
									printf("Result: %f\n",results);
									Labels::printCompareLabels(upLabelsFilled,dbLabelsFilled,1);
									cout << "-------------------------" << endl;
								}
								///// End Debug Print /////
							}// end if flag1 || flag2
						} catch (const std::out_of_range &oor) {
							printf("ShapeShift1: %d\n",shapeShift1);
							printf("ShapeShift2: %d\n",shapeShift2);
							printf("CurrShape1: %s\n",shapematch.getShapeName(shapeNum1).c_str());
							printf("CurrShape2: %s\n",shapematch.getShapeName(shapeNum2).c_str());
							printf("NewShape1: %s\n",newShape1.c_str());
							exit(1);
						}
					}// end for shapeShift2
				}// end for shapeShift1
			}// end if shifted condition
		}// end shadeShift
	}// end n

	/*** Shape STT counter ***/
	auto largestUpMap = largestLabelsUP.getMap();
	for(auto it=largestUpMap.begin(); it!=largestUpMap.end(); it++) {
		if(largestLabelsUP.isShapeShifted(it->first)) {
			int prevShape = largestLabelsUP.getPrevShapeNum(it->first);
			int currShape = largestLabelsUP.getShapeNum(it->first);
			this->shapeTranslateCount.at(prevShape).at(currShape)=1;
		}
	}
	/*****************************/
	if(maxResults>0) {
		String labelFilename = largestLabelsUP.name()+"_"+largestLabelsDB.name()+"_tr1_max_match_labels";
		Labels::writeCompareLabels(labelFilename,largestLabelsUP,largestLabelsDB,-1,1);
		imwrite(newNameUP+"_max_match_image_"+maxNStr+".png",maxMatchImg);
		upSS.getImageData().writePrevSize(newNameUP+"_max_match_image_"+maxNStr);
		this->esgPS_Map.at(maxNStr).writePrintStream(newNameUP+"_ESG_"+maxNStr+".txt");
		this->entropyPS_Map.at(maxNStr).writePrintStream(newNameUP+"_entropy_output_"+maxNStr+".txt");
		this->mismatchEntropyPS_Map.at(maxNStr).writePrintStream(newNameUP+"_mismatch_entropy_output_"+maxNStr+".txt");
		this->noEntropyPS_Map.at(maxNStr).writePrintStream(newNameUP+"_no_entropy_"+maxNStr+".txt");
	}
	vector<float> vec = {maxResults,finalTR1,finalTR2_match,finalTR2_mismatch};
	resultVec.push_back(vec);
	return *max_element(resultVec.begin(),resultVec.end());
}

void ShadeShapeMatch::debug_mode(int mode) {
	this->debugMode = mode;
}

void ShadeShapeMatch::countShapeTranslations(vector<vector<int> > &shapeTranslateCount) {
	assert(this->shapeTranslateCount.size()==shapeTranslateCount.size());

	for(unsigned int i=0; i<this->shapeTranslateCount.size(); i++) {
		for(unsigned int j=0; j<this->shapeTranslateCount.at(i).size(); j++) {
			if(this->shapeTranslateCount.at(i).at(j)>0) {
				shapeTranslateCount.at(i).at(j) += this->shapeTranslateCount.at(i).at(j);
			}
		}
	}
}
