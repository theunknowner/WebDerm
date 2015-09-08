/*
 * ssr_match.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "ssr_match.h"
#include "shadeshaperelation.h"
#include "../Labels/labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/**************************** PRIVATE FUNCTIONS ********************************/

float ShadeShapeRelationMatch::entropy(int count) {
	if(count==0) {
		return -1.0;
	} else {
		return log2(count);
	}
}

//! computes the entropy value for each label
float ShadeShapeRelationMatch::entropy(vector<vector<vector<vector<int> > > > srmCountUP,vector<vector<vector<vector<pair<int,int>> > > > srmAreaUP, Labels &upLabels, vector<vector<vector<vector<int> > > > srmCountDB, vector<vector<vector<vector<pair<int,int>> > > > srmAreaDB, Labels &dbLabels) {
	assert(srmCountUP.size()==srmCountDB.size());
	int maxNeighborLevel = 0; //place holder
	FILE * fp;
	String filename = "entropy_output_" + upLabels.name() + "_" + dbLabels.name() + ".txt";
	fp = fopen(filename.c_str(),"w");
	float totalEntropy = 0.0;
	int maxTotalArea = max(upLabels.totalArea(),dbLabels.totalArea());
	float sumOfArea=0.0;
	// for loop i: labels for the y axis
	for(unsigned int i=0; i<srmCountUP.size(); i++) {
		String labelUP1 = upLabels.at(i);
		// for loop j: labels for x axis
		for(unsigned int j=0; j<srmCountUP.at(i).size(); j++) {
			String labelUP2 = upLabels.at(j);
			// for loop k: counts for relations "y [op] x"
			for(unsigned int k=0; k<srmCountUP.at(i).at(j).size(); k++) {
				String relOp = this->rel_op.at(k);
				int totalCountUP = 0, totalCountDB = 0;
				int totalAreaUP = 0, totalAreaDB = 0;
				if(k==SURR_BY || k==SURR_BY_INV) {
					// for loop m: neighbor levels
					for(int m=0; m<this->relOpLevelSize; m++) {
						totalCountUP = 0; totalCountDB = 0;
						totalAreaUP = 0; totalAreaDB = 0;
						int countUP = srmCountUP.at(i).at(j).at(k).at(m);
						int countDB = srmCountDB.at(i).at(j).at(k).at(m);
						int areaUP1 = srmAreaUP.at(i).at(j).at(k).at(m).first;
						int areaUP2 = srmAreaUP.at(i).at(j).at(k).at(m).second;
						int areaDB1 = srmAreaDB.at(i).at(j).at(k).at(m).first;
						int areaDB2 = srmAreaDB.at(i).at(j).at(k).at(m).second;
						if(countUP>0 || countDB>0) {
							float entropyUP = this->entropy(countUP);
							float entropyDB = this->entropy(countDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float areaValUP = totalAreaUP, areaValDB = totalAreaDB;
							if(maxNeighborLevel>5) maxNeighborLevel = 5;

							if(k==SURR_BY) {
								// for loop y: sum of area for y label with "k" relation at "m" neighbor level
								for(unsigned int y=0; y<srmCountUP.size(); y++) {
									totalCountUP += srmCountUP.at(y).at(j).at(k).at(m);
									totalCountDB += srmCountDB.at(y).at(j).at(k).at(m);
									totalAreaUP += srmAreaUP.at(y).at(j).at(k).at(m).first;
									totalAreaDB += srmAreaDB.at(y).at(j).at(k).at(m).first;
								}
								areaValUP = ((float)areaUP1 + ((float)areaUP2 * ((float)areaUP1/totalAreaUP))) * pow(this->rVal[maxNeighborLevel],m);
								areaValDB = ((float)areaDB1 + ((float)areaDB2 * ((float)areaDB1/totalAreaDB))) * pow(this->rVal[maxNeighborLevel],m);
							}
							if(k==SURR_BY_INV) {
								// for loop y: sum of area for x label with "k" relation at "m" neighbor level
								for(unsigned int x=0; x<srmCountUP.size(); x++) {
									totalCountUP += srmCountUP.at(i).at(x).at(k).at(m);
									totalCountDB += srmCountDB.at(i).at(x).at(k).at(m);
									totalAreaUP += srmAreaUP.at(i).at(x).at(k).at(m).second ;
									totalAreaDB += srmAreaDB.at(i).at(x).at(k).at(m).second;
								}
								areaValUP = ((float)areaUP2 + ((float)areaUP1 * ((float)areaUP2/totalAreaUP))) * pow(this->rVal[maxNeighborLevel],m);
								areaValDB = ((float)areaDB2 + ((float)areaDB1 * ((float)areaDB2/totalAreaDB))) * pow(this->rVal[maxNeighborLevel],m);
							}
							if(std::isnan(areaValUP)) areaValUP=0;
							if(std::isnan(areaValDB)) areaValDB=0;
							float areaVal = min(areaValUP,areaValDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal;
							totalEntropy += weightedEntropy;
							sumOfArea += min(areaValUP,areaValDB);
							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp,"TotalAreaUP: %d, TotalAreaDB: %d\n",totalAreaUP,totalAreaDB);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"SumOfArea: %f\n",sumOfArea);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						}
					}
				} else {
					if(k==DIR) {
						int countUP=0, countDB=0;
						int areaUP1=0, areaUP2=0, areaDB1 = 0, areaDB2=0;
						for(int m=0; m<this->relOpLevelSize; m++) {
							countUP += srmCountUP.at(i).at(j).at(k).at(m);
							countDB += srmCountDB.at(i).at(j).at(k).at(m);
							areaUP1 += srmAreaUP.at(i).at(j).at(k).at(m).first;
							areaUP2 += srmAreaUP.at(i).at(j).at(k).at(m).second;
							areaDB1 += srmAreaDB.at(i).at(j).at(k).at(m).first;
							areaDB2 += srmAreaDB.at(i).at(j).at(k).at(m).second;
						}
						if(countUP>0 || countDB>0) {
							if(areaUP1>areaUP2) {
								for(unsigned int x=0; x<srmAreaUP.size(); x++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaUP += srmAreaUP.at(i).at(x).at(k).at(m).second;
										totalCountUP += srmCountUP.at(i).at(x).at(k).at(m);
									}
								}
							}
							else if(areaUP1<=areaUP2) {
								for(unsigned int y=0; y<srmAreaUP.size(); y++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaUP += srmAreaUP.at(y).at(j).at(k).at(m).first;
										totalCountUP += srmCountUP.at(y).at(j).at(k).at(m);
									}
								}
							}
							if(areaDB1>areaDB2) {
								for(unsigned int x=0; x<srmAreaDB.size(); x++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaDB += srmAreaDB.at(i).at(x).at(k).at(m).second;
										totalCountDB += srmCountDB.at(i).at(x).at(k).at(m);
									}
								}
							}
							else if(areaDB1<=areaDB2) {
								for(unsigned int y=0; y<srmAreaDB.size(); y++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaDB += srmAreaDB.at(y).at(j).at(k).at(m).first;
										totalCountDB += srmCountDB.at(y).at(j).at(k).at(m);
									}
								}
							}
							float areaRatioUP = (float)min(areaUP1,areaUP2) / totalAreaUP;
							float areaRatioDB = (float)min(areaDB1,areaDB2) / totalAreaDB;
							float areaValUP = min(areaUP1,areaUP2) + (areaRatioUP) * (float)max(areaUP1,areaUP2);
							float areaValDB = min(areaDB1,areaDB2) + (areaRatioDB) * (float)max(areaDB1,areaDB2);
							if(std::isnan(areaValUP)) areaValUP=0;
							if(std::isnan(areaValDB)) areaValDB=0;
							float entropyUP = this->entropy(countUP);
							float entropyDB = this->entropy(countDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float areaVal = min(areaValUP,areaValDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal;
							totalEntropy += weightedEntropy;
							sumOfArea += areaVal;
							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp,"AreaValUP: %f, AreaValDB: %f\n",areaValUP,areaValDB);
								fprintf(fp,"TotalAreaUP: %d, TotalAreaDB: %d\n",totalAreaUP,totalAreaDB);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"SumOfArea: %f\n",sumOfArea);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						}
					} // end if DIR
					else { // else INDIR
						for(int m=0; m<this->relOpLevelSize; m++) {
							totalCountUP += srmCountUP.at(i).at(j).at(k).at(m);
							totalCountDB += srmCountDB.at(i).at(j).at(k).at(m);
							totalAreaUP += srmAreaUP.at(i).at(j).at(k).at(m).first + srmAreaUP.at(i).at(j).at(k).at(m).second;
							totalAreaDB += srmAreaDB.at(i).at(j).at(k).at(m).first + srmAreaDB.at(i).at(j).at(k).at(m).second;
						}
						if(totalCountUP>0 || totalCountDB>0) {
							float entropyUP = this->entropy(totalCountUP);
							float entropyDB = this->entropy(totalCountDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float areaVal = min(totalAreaUP,totalAreaDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal;
							totalEntropy += weightedEntropy;
							sumOfArea += min(totalAreaUP,totalAreaDB);
							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",totalCountUP,entropyUP,totalCountDB,entropyDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP: %d, AreaDB: %d\n",totalAreaUP,totalAreaDB);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"SumOfArea: %f\n",sumOfArea);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						}
					} // end else INDIR
				} // end else DIR/INDIR
			} // end for loop k
		} // end for loop j
	} // end for loop i
	fclose(fp);

	// maxTotalArea should not be less if areas do not repeat
	if(maxTotalArea<sumOfArea)
		totalEntropy *= ((float)maxTotalArea / sumOfArea);
	return totalEntropy;
}

//! computes the entropy value for each label
float ShadeShapeRelationMatch::entropy(pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<pair<int,int>> > > >> &srmPairUP, Labels &upLabels, pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<pair<int,int>> > > >> &srmPairDB, Labels &dbLabels) {
	vector<vector<vector<vector<int> > > > srmCountUP = srmPairUP.first;
	vector<vector<vector<vector<pair<int,int>> > > > srmAreaUP = srmPairUP.second;
	vector<vector<vector<vector<int> > > > srmCountDB = srmPairDB.first;
	vector<vector<vector<vector<pair<int,int>> > > > srmAreaDB = srmPairDB.second;

	float totalEntropy = this->entropy(srmCountUP,srmAreaUP,upLabels,srmCountDB,srmAreaDB,dbLabels);
	return totalEntropy;
}

float ShadeShapeRelationMatch::entropy(ShadeShapeRelation &ssrUP, ShadeShapeRelation &ssrDB) {
	Srm srmUP = ssrUP.get_srm();
	Srm srmDB = ssrDB.get_srm();
	auto srmCountUP = srmUP.downscaleSrmCount();
	auto srmCountDB = srmDB.downscaleSrmCount();
	assert(srmCountUP.size()>0 && srmCountDB.size()>0);
	assert(srmCountUP.size()==srmCountDB.size());
	auto srmAreaUP = srmUP.downscaleSrmArea();
	auto srmAreaDB = srmDB.downscaleSrmArea();
	auto upMergedLabels = srmUP.getMergedLabels();
	auto dbMergedLabels = srmDB.getMergedLabels();
	auto upLabels = srmUP.getLabels();
	auto dbLabels = srmDB.getLabels();
	int maxNeighborLevelUP = srmUP.maxNeighborLevel();
	int maxNeighborLevelDB = srmDB.maxNeighborLevel();
	FILE * fp;
	String filename = "entropy_output_" + ssrUP.name() + "_" + ssrDB.name() + ".txt";
	fp = fopen(filename.c_str(),"w");
	float totalEntropy = 0.0;
	int maxTotalArea = max(upMergedLabels.totalArea(),dbMergedLabels.totalArea());
	float sumOfArea=0.0;
	// for loop i: labels for the y axis
	for(unsigned int i=0; i<srmCountUP.size(); i++) {
		String labelUP1 = upMergedLabels.at(i);
		// for loop j: labels for x axis
		for(unsigned int j=0; j<srmCountUP.at(i).size(); j++) {
			String labelUP2 = upMergedLabels.at(j);
			// for loop k: counts for relations "y [op] x"
			for(unsigned int k=0; k<srmCountUP.at(i).at(j).size(); k++) {
				String relOp = this->rel_op.at(k);
				int totalCountUP = 0, totalCountDB = 0;
				int totalAreaUP = 0, totalAreaDB = 0;
				if(k==SURR_BY || k==SURR_BY_INV) {
					// for loop m: neighbor level of separation
					for(int m=0; m<this->relOpLevelSize; m++) {
						int countUP = srmCountUP.at(i).at(j).at(k).at(m);
						int countDB = srmCountDB.at(i).at(j).at(k).at(m);
						float areaUP=0., areaDB=0.;
						if(countUP>0 || countDB>0) {
							float entropyUP = this->entropy(countUP);
							float entropyDB = this->entropy(countDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float areaValUP = totalAreaUP, areaValDB = totalAreaDB;
							if(maxNeighborLevelUP>5) maxNeighborLevelUP = 5;
							if(maxNeighborLevelDB>5) maxNeighborLevelDB = 5;
							if(k==SURR_BY) {
								//for UP
								for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
									try {
										float totalDenomAreaUP=0.;
										String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
										int areaX = upLabels.area(xLabel); // get area
										int xIdx = srmUP.getIndex(xLabel); // get pos from srm
										for(unsigned int yIdx=0; yIdx<srmUP.size(); yIdx++) {
											try {
												int rel_op_idx = srmUP.relation(yIdx,xIdx);
												if(rel_op_idx==k) {
													totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).first;
													totalCountUP++;
												}
											} catch (const std::out_of_range &oor) {
												printf("ShadeShapeRelationMatch::entropy() out of range!\n");
												printf("1a\n");
												printf("xLabel: %s\n",xLabel.c_str());
												printf("xIdx: %d\n",xIdx);
												exit(1);
											}
										}
										int areaY=0;
										for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
											String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
											String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
											if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
												areaY += upLabels.area(yLabel);
											}
										}
										areaUP += areaY + (areaX * (areaY/totalDenomAreaUP));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("1\n");
										printf("x: %d\n",x);
										exit(1);
									}
								}
								// for DB
								for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
									try {
										float totalDenomAreaDB=0.;
										String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
										int areaX = dbLabels.area(xLabel);
										int xIdx = srmDB.getIndex(xLabel);
										for(unsigned int yIdx=0; yIdx<srmDB.size(); yIdx++) {
											int rel_op_idx = srmDB.relation(yIdx,xIdx);
											if(rel_op_idx==k) {
												totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).first;
												totalCountDB++;
											}
										}
										int areaY=0;
										for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
											String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
											String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
											if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
												areaY += dbLabels.area(yLabel);
											}
										}
										areaDB += areaY + (areaX * (areaY/totalDenomAreaDB));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("2\n");
										exit(1);
									}
								}
								areaValUP = areaUP * pow(this->rVal[maxNeighborLevelUP],m);
								areaValDB = areaDB * pow(this->rVal[maxNeighborLevelDB],m);
							}
							if(k==SURR_BY_INV) {
								for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
									try {
										float totalDenomAreaUP=0.;
										String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
										int yIdx = srmUP.getIndex(yLabel);
										int areaY = upLabels.area(yLabel);
										for(unsigned int xIdx=0; xIdx<srmUP.size(); xIdx++) {
											int rel_op_idx = srmUP.relation(yIdx,xIdx);
											if(rel_op_idx==k) {
												totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).second;
												totalCountUP++;
												/*if(labelUP1=="0_Strip_s4" && labelUP2=="1_Default_s2" && m==1) {
													printf("yIdx: %d, %s\n",yIdx,yLabel.c_str());
													printf("xIdx: %d, %s\n",xIdx,upLabels.at(xIdx).c_str());
													printf("rel_op_idx: %d\n",rel_op_idx);
													printf("srmRelationArea: %d\n",srmUP.relationArea(yIdx,xIdx).second);
													printf("totalDenomAreaUP: %f\n",totalDenomAreaUP);

												}*/
											}
										}
										int areaX=0;
										for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
											String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
											String equationKey = yLabel+"<"+relOp+">"+xLabel;
											if(srmUP.equationMap.find(equationKey)!=srmUP.equationMap.end()) {
												areaX += upLabels.area(xLabel);
											}
										}
										areaUP += areaX + (areaY * (areaX/totalDenomAreaUP));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("3\n");
										exit(1);
									}
								}
								for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
									try {
										float totalDenomAreaDB=0.;
										String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
										int yIdx = srmDB.getIndex(yLabel);
										int areaY = dbLabels.area(yLabel);
										for(unsigned int xIdx=0; xIdx<srmDB.size(); xIdx++) {
											int rel_op_idx = srmDB.relation(yIdx,xIdx);
											if(rel_op_idx==k) {
												totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).second;
												totalCountDB++;
											}
										}
										int areaX=0;
										for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
											String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
											String equationKey = yLabel+"<"+relOp+">"+xLabel;
											if(srmDB.equationMap.find(equationKey)!=srmDB.equationMap.end()) {
												areaX += dbLabels.area(xLabel);
											}
										}
										areaDB += areaX + (areaY * (areaX/totalDenomAreaDB));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("4\n");
										exit(1);
									}
								}
								areaValUP = areaUP * pow(this->rVal[maxNeighborLevelUP],m);
								areaValDB = areaDB * pow(this->rVal[maxNeighborLevelDB],m);
							}
							if(std::isnan(areaValUP)) areaValUP=0;
							if(std::isnan(areaValDB)) areaValDB=0;
							float areaVal = min(areaValUP,areaValDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal;
							totalEntropy += weightedEntropy;
							sumOfArea += min(areaValUP,areaValDB);
							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s] : Level %d\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str(),m);
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"MaxNeighborLevelUP: %d, MaxNeighborLevelDB: %d\n",maxNeighborLevelUP,maxNeighborLevelDB);
								fprintf(fp,"AreaValUP: %f, AreaValDB: %f\n",areaValUP,areaValDB);
								fprintf(fp,"TotalAreaUP: %d, TotalAreaDB: %d\n",totalAreaUP,totalAreaDB);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"SumOfArea: %f\n",sumOfArea);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						}
					}
				} else {
					if(k==DIR) {
						int countUP=0, countDB=0;
						int areaUP1=0, areaUP2=0, areaDB1 = 0, areaDB2=0;
						for(int m=0; m<this->relOpLevelSize; m++) {
							countUP += srmCountUP.at(i).at(j).at(k).at(m);
							countDB += srmCountDB.at(i).at(j).at(k).at(m);
							areaUP1 += srmAreaUP.at(i).at(j).at(k).at(m).first;
							areaUP2 += srmAreaUP.at(i).at(j).at(k).at(m).second;
							areaDB1 += srmAreaDB.at(i).at(j).at(k).at(m).first;
							areaDB2 += srmAreaDB.at(i).at(j).at(k).at(m).second;
						}
						if(countUP>0 || countDB>0) {
							if(areaUP1>areaUP2) {
								for(unsigned int x=0; x<srmAreaUP.size(); x++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaUP += srmAreaUP.at(i).at(x).at(k).at(m).second;
										totalCountUP += srmCountUP.at(i).at(x).at(k).at(m);
									}
								}
							}
							else if(areaUP1<=areaUP2) {
								for(unsigned int y=0; y<srmAreaUP.size(); y++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaUP += srmAreaUP.at(y).at(j).at(k).at(m).first;
										totalCountUP += srmCountUP.at(y).at(j).at(k).at(m);
									}
								}
							}
							if(areaDB1>areaDB2) {
								for(unsigned int x=0; x<srmAreaDB.size(); x++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaDB += srmAreaDB.at(i).at(x).at(k).at(m).second;
										totalCountDB += srmCountDB.at(i).at(x).at(k).at(m);
									}
								}
							}
							else if(areaDB1<=areaDB2) {
								for(unsigned int y=0; y<srmAreaDB.size(); y++) {
									for(int m=0; m<this->relOpLevelSize; m++) {
										totalAreaDB += srmAreaDB.at(y).at(j).at(k).at(m).first;
										totalCountDB += srmCountDB.at(y).at(j).at(k).at(m);
									}
								}
							}
							float areaRatioUP = (float)min(areaUP1,areaUP2) / totalAreaUP;
							float areaRatioDB = (float)min(areaDB1,areaDB2) / totalAreaDB;
							float areaValUP = min(areaUP1,areaUP2) + (areaRatioUP) * (float)max(areaUP1,areaUP2);
							float areaValDB = min(areaDB1,areaDB2) + (areaRatioDB) * (float)max(areaDB1,areaDB2);
							if(std::isnan(areaValUP)) areaValUP=0;
							if(std::isnan(areaValDB)) areaValDB=0;
							float entropyUP = this->entropy(countUP);
							float entropyDB = this->entropy(countDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float areaVal = min(areaValUP,areaValDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal;
							totalEntropy += weightedEntropy;
							sumOfArea += areaVal;
							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp,"AreaValUP: %f, AreaValDB: %f\n",areaValUP,areaValDB);
								fprintf(fp,"TotalAreaUP: %d, TotalAreaDB: %d\n",totalAreaUP,totalAreaDB);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"SumOfArea: %f\n",sumOfArea);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						}
					} // end if DIR
					else { // else INDIR
						for(int m=0; m<this->relOpLevelSize; m++) {
							totalCountUP += srmCountUP.at(i).at(j).at(k).at(m);
							totalCountDB += srmCountDB.at(i).at(j).at(k).at(m);
							totalAreaUP += srmAreaUP.at(i).at(j).at(k).at(m).first + srmAreaUP.at(i).at(j).at(k).at(m).second;
							totalAreaDB += srmAreaDB.at(i).at(j).at(k).at(m).first + srmAreaDB.at(i).at(j).at(k).at(m).second;
						}
						if(totalCountUP>0 || totalCountDB>0) {
							float entropyUP = this->entropy(totalCountUP);
							float entropyDB = this->entropy(totalCountDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float areaVal = min(totalAreaUP,totalAreaDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal;
							totalEntropy += weightedEntropy;
							sumOfArea += min(totalAreaUP,totalAreaDB);
							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",totalCountUP,entropyUP,totalCountDB,entropyDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP: %d, AreaDB: %d\n",totalAreaUP,totalAreaDB);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"SumOfArea: %f\n",sumOfArea);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						}
					} // end else INDIR
				} // end else DIR/INDIR
			} // end for loop k
		} // end for loop j
	} // end for loop i
	fclose(fp);

	// maxTotalArea should not be less if areas do not repeat
	if(maxTotalArea<sumOfArea)
		totalEntropy *= ((float)maxTotalArea / sumOfArea);
	return totalEntropy;
}


/*************************** PUBLIC FUNCTIONS ***********************************/

float ShadeShapeRelationMatch::srm_match(pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<pair<int,int>> > > >> &srmPairUP, Labels &upMergedLabels, pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<pair<int,int>> > > >> &srmPairDB, Labels &dbMergedLabels) {
	float matchVal = this->entropy(srmPairUP,upMergedLabels,srmPairDB,dbMergedLabels);
	return matchVal;
}

float ShadeShapeRelationMatch::srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels) {
	ssrUP.get_srm().downScaleSrm();
	ssrDB.get_srm().downScaleSrm();
	float matchVal = this->entropy(ssrUP,ssrDB);
	//float matchVal = this->srm_match(srmPairUP,upMergedLabels,srmPairDB,dbMergedLabels);
	//this->writeDownScaleSrm(ssrUP.name(),srmPairUP,upMergedLabels,upLabels.totalArea());
	//this->writeDownScaleSrm(ssrDB.name(),srmPairDB,dbMergedLabels,dbLabels.totalArea());
	//this->printDownScaleSrm(srm3dUP,upMergedLabels);
	//cout << "-----------------------" << endl;
	//this->printDownScaleSrm(srm3dDB,dbMergedLabels);
	return matchVal;
}

void ShadeShapeRelationMatch::importDownScaleSrms(String file, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &labels) {
	fstream fs(file);
	if(fs.is_open()) {
		map<String,pair<int,float> > srmLabels;
		String temp;
		char beginDelimit = '[';
		char endDelimit = ']';
		char separator = ':';
		vector<String> vec;
		vector<String> vec2;
		while(getline(fs,temp)) {
			if(temp.find("TotalArea")==string::npos) {
				getSubstr(temp,separator,vec);
				Func::getSubstr(vec.at(0),beginDelimit,endDelimit,vec2);
				for(unsigned int i=0; i<vec2.size(); i++) {
					if(i==0 || i==2) {
						if(srmLabels.find(vec2.at(i))==srmLabels.end()) {
							srmLabels[vec2.at(i)] = std::make_pair(1,1.0);
						}
					}
				}
			}
		}
		fs.close();
		fs.open(file);
		vector<vector<vector<int> > > srmCount(srmLabels.size(),vector<vector<int> >(srmLabels.size(),vector<int>(this->rel_op.size(),0)));
		vector<vector<vector<int> > > srmArea(srmLabels.size(),vector<vector<int> >(srmLabels.size(),vector<int>(this->rel_op.size(),0)));
		while(getline(fs,temp)) {
			getSubstr(temp,separator,vec);
			if(temp.find("TotalArea")==string::npos) {
				Func::getSubstr(vec.at(0),beginDelimit,endDelimit,vec2);
				String labelY = vec2.at(0);
				String labelX = vec2.at(2);
				String relOp = vec2.at(1);
				int area = atoi(vec.at(1).c_str());
				int count = atoi(vec.at(2).c_str());
				auto itY = srmLabels.find(labelY);
				auto itX = srmLabels.find(labelX);
				int y = distance(srmLabels.begin(),itY);
				int x = distance(srmLabels.begin(),itX);
				int relOpIdx = this->getRelOpIndex(relOp);
				srmCount.at(y).at(x).at(relOpIdx) = count;
				srmArea.at(y).at(x).at(relOpIdx) = area;
			} else {
				int totalArea = atoi(vec.at(1).c_str());
				labels.totalArea() = totalArea;
			}
		}
		fs.close();
		srmPair = std::make_pair(srmCount,srmArea);
		labels.setLabels(srmLabels);
	}
	//this->printDownScaleSrm(srmPair,labels);
}
