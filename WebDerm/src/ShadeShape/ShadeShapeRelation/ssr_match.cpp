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
#include "../ShapeMatch/shapematch.h"
#include "../StatSign/statsign.h"
#include "esg.h"

/**************************** PRIVATE FUNCTIONS ********************************/

float ShadeShapeRelationMatch::entropy(float count) {
	if(count==0.0) {
		return -1.0;
	} else if(count>0.0 && count<1.0) {
		return count - 1.0;
	} else {
		return log2(count);
	}
}

float ShadeShapeRelationMatch::contrastWeight(float esg, float relArea) {

	float result = esg * relArea;
	return result;
}

float ShadeShapeRelationMatch::calculateArcScore(float score1, float score2) {
	float val = abs(score1-score2);
	float m = 3.0;
	float result = 1.0 - ((exp(val/m) - 1.0) / (exp(val/m) + 1.0) / 2.0);
	return result;
}

void ShadeShapeRelationMatch::match(ShadeShapeRelation &ssrUP, ShadeShapeRelation &ssrDB, String nStr) {
	Srm srmUP = ssrUP.get_srm();
	Srm srmDB = ssrDB.get_srm();
	auto srmCountUP = srmUP.downscaleSrmCount();
	auto srmCountDB = srmDB.downscaleSrmCount();
	assert(srmCountUP.size()>0 && srmCountDB.size()>0);
	assert(srmCountUP.size()==srmCountDB.size());
	auto srmAreaUP = srmUP.downscaleSrmArea();
	auto srmAreaDB = srmDB.downscaleSrmArea();
	Labels upMergedLabels = srmUP.getMergedLabels();
	Labels dbMergedLabels = srmDB.getMergedLabels();
	Labels upLabels = srmUP.getLabels();
	Labels dbLabels = srmDB.getLabels();
	int maxNeighborLevelUP = srmUP.maxNeighborLevel();
	int maxNeighborLevelDB = srmDB.maxNeighborLevel();
	if(maxNeighborLevelUP>5) maxNeighborLevelUP = 5;
	if(maxNeighborLevelDB>5) maxNeighborLevelDB = 5;
	ShapeMatch shapematch;
	StatSign statsign;
	FILE * fp;
	FILE * fp2;
	FILE * fp3;
	String filename = ssrUP.name() + "_entropy_output_"+nStr+".txt";
	String filename2 = ssrUP.name() + "_mismatch_entropy_output_"+nStr+".txt";
	String filename3 = ssrUP.name() + "_no_entropy_"+nStr+".txt";
	fp = fopen(filename.c_str(),"w");
	fp2 = fopen(filename2.c_str(),"w");
	fp3 = fopen(filename3.c_str(),"w");
	float totalMatchScore = 0.0, totalMismatchScore=0.0;
	int maxTotalArea = max(upMergedLabels.totalArea(),dbMergedLabels.totalArea());

	float negArcScore = -100.0;
	float posArcScore = 100.0;
	// for loop i: labels for the y axis
	for(unsigned int i=0; i<srmCountUP.size(); i++) {
		String label1 = upMergedLabels.at(i);
		bool label1Condition = label1.find("Excavated")!=string::npos || label1.find("Default")!=string::npos || label1.find("Blotch")!=string::npos;
		// for loop j: labels for x axis
		for(unsigned int j=0; j<srmCountUP.at(i).size(); j++) {
			String label2 = upMergedLabels.at(j);
			bool label2Condition = label2.find("Excavated")!=string::npos || label2.find("Default")!=string::npos  || label1.find("Blotch")!=string::npos;
			// for loop k: counts for relations "y [op] x"
			for(unsigned int k=0; k<srmCountUP.at(i).at(j).size(); k++) {
				String relOp = this->rel_op.at(k);
				int totalCountUP = 0, totalCountDB = 0;
				float totalAreaUP = 0.0, totalAreaDB = 0.0;
				if(k==SURR_BY || k==SURR_BY_INV) {
					// for loop m: neighbor level of separation
					for(int m=0; m<this->relOpLevelSize; m++) {
						float penalizedY = 1.0, penalizedX = 1.0;
						int countUP = srmCountUP.at(i).at(j).at(k).at(m);
						int countDB = srmCountDB.at(i).at(j).at(k).at(m);
						float areaUP=0., areaDB=0.;
						//> used for match calculation
						int maxIslandAreaUP=0, maxIslandAreaDB=0;
						float sumIslandAreaUP=0.0, sumIslandAreaDB=0.0;
						float sumArcScoreUP1=0.0, sumArcScoreUP2=0.0, sumArcScoreDB1=0.0, sumArcScoreDB2=0.0;
						float deltaArcScore1=1.0, deltaArcScore2 = 1.0;
						//> used for dot product calculation
						float dotProduct1 = 1.0, dotProduct2 = 1.0;
						float adjustedDP1 = 1.0, adjustedDP2 = 1.0;
						float weight1=0.0, weight2 = 0.0;
						vector<float> sumStatSignUP1(17,0), sumStatSignUP2(17,0), sumStatSignDB1(17,0), sumStatSignDB2(17,0);
						vector<float> statSignUP1(17,0.0), statSignUP2(17,0.0), statSignDB1(17,0.0), statSignDB2(17,0.0);
						//> used for constrast weight
						float contrastWeightUP = 0.0, contrastWeightDB = 0.0;
						float sumOfAreaUP = 0.0, sumOfAreaDB = 0.0;
						if(countUP>0 || countDB>0) {
							float areaValUP = totalAreaUP, areaValDB = totalAreaDB;
							if(k==SURR_BY) {
								//> UP initial calculations
								if (countUP>0) {
									for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
										String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
										if(label1Condition) {
											for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
												String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
												String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
												if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignY = upLabels.getStatSign(yLabel);
													if(statSignY.size()>0 && statSignY.size()==sumStatSignUP1.size()) {
														for(unsigned int n=0; n<statSignY.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignUP1.at(n) += statSignY.at(n);
														}
													}
													//> for sub island match calculations, arc score
													if(label1.find("Excavated")!=string::npos) {
														for(int n=0; n<upLabels.getIsland(yLabel).numOfSubIslands(); n++) {
															String shape_name = upLabels.getIsland(yLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = upLabels.getIsland(yLabel).subIsland(n).area();
																float bigArea = upLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreUP1 += score;
															}
														}
													}
												}// end if equationMap
											} // end for y srmUP
										} // end if label1Condition
										//> for dot product calculations (REI only so far)
										if(label2Condition) {
											vector<float> statSignX = upLabels.getStatSign(xLabel);
											if(statSignX.size()>0 && statSignX.size()==sumStatSignUP2.size()) {
												for(unsigned int n=0; n<statSignX.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignUP2.at(n) += statSignX.at(n);
												}
											}
										}
										//> for sub island match calculations
										if(label2.find("Excavated")!=string::npos) {
											for(int n=0; n<upLabels.getIsland(xLabel).numOfSubIslands(); n++) {
												String shape_name = upLabels.getIsland(xLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = upLabels.getIsland(xLabel).subIsland(n).area();
													float bigArea = upLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreUP2 += score;
												}
											}
										}
									}
								} // end if countUP > 0
								//> end UP initial calculations
								//DB initial calculations
								if (countDB>0) {
									for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
										String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
										if(label1Condition) {
											for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
												String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
												String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignY = dbLabels.getStatSign(yLabel);
													if(statSignY.size()>0 && statSignY.size()==sumStatSignDB1.size()) {
														for(unsigned int n=0; n<statSignY.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignDB1.at(n) += statSignY.at(n);
														}
													}
													//> for sub island match calculations, arc score
													if(label1.find("Excavated")!=string::npos) {
														for(int n=0; n<dbLabels.getIsland(yLabel).numOfSubIslands(); n++) {
															String shape_name = dbLabels.getIsland(yLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = dbLabels.getIsland(yLabel).subIsland(n).area();
																float bigArea = dbLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreDB1 += score;
															}
														}
													}
												}// end if equationMap
											}// end for y srmDB
										}// end if label1Condition
										//> for dot product calculations (REI only so far)
										if(label2Condition) {
											vector<float> statSignX = dbLabels.getStatSign(xLabel);
											if(statSignX.size()>0 && statSignX.size()==sumStatSignDB2.size()) {
												for(unsigned int n=0; n<statSignX.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignDB2.at(n) += statSignX.at(n);
												}
											}
										}
										//> for sub island match calculations
										if(label2.find("Excavated")!=string::npos) {
											for(int n=0; n<dbLabels.getIsland(xLabel).numOfSubIslands(); n++) {
												String shape_name = dbLabels.getIsland(xLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = dbLabels.getIsland(xLabel).subIsland(n).area();
													float bigArea = dbLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreDB2 += score;
												}
											}
										}
									}
								}// end if countDB > 0
								// end DB initial calculation
								//> StatSign using Dot Product (Scheme I) <//
								if(label1Condition) {
									dotProduct1 = statsign.dotProduct(sumStatSignUP1,sumStatSignDB1);
									adjustedDP1 = statsign.adjustValue(dotProduct1);

								}
								if(label2Condition) {
									dotProduct2 = statsign.dotProduct(sumStatSignUP2,sumStatSignDB2);
									adjustedDP2 = statsign.adjustValue(dotProduct2);
								}
								//> ************ End StatSign ************ <//
								weight1 = this->getShapeWeight(upMergedLabels.getShapeNum(label1),adjustedDP1);
								weight2 = this->getShapeWeight(upMergedLabels.getShapeNum(label2),adjustedDP2);
								deltaArcScore1 = this->calculateArcScore(sumArcScoreUP1,sumArcScoreDB1);
								deltaArcScore2 = this->calculateArcScore(sumArcScoreUP2,sumArcScoreDB2);
								//for UP
								if (countUP>0) {
									for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
										try {
											float totalDenomAreaUP=0.;
											String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
											int areaX = upLabels.area(xLabel); // get area
											int xIdx = srmUP.getIndex(xLabel); // get pos from srm
											for(unsigned int yIdx=0; yIdx<srmUP.size(); yIdx++) {
												try {
													unsigned int rel_op_idx = srmUP.relation(yIdx,xIdx);
													//for all relations
													if(rel_op_idx>NONE && rel_op_idx!=SURR_BY_INV) {
														totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).first;
														totalCountUP++;
													}
												} catch (const std::out_of_range &oor) {
													printf("ShadeShapeRelationMatch::entropy() out of range!\n");
													printf("SURR_BY: UP 1a\n");
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
													float penaltyY = 1.0;
													//> check if shape is shifted to apply penalty <//
													if(upLabels.isShapeShifted(yLabel)) {
														int shapeNumY = upLabels.getShapeNum(yLabel);
														int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
														penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
														penalizedY = penaltyY;
													}
													areaY += upLabels.area(yLabel) * penaltyY;

													//> for match calculations
													sumIslandAreaUP += upLabels.area(yLabel);
													maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(yLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(upLabels.getShade(yLabel),upLabels.getShade(xLabel));
													int index1 = srmUP.getIndex(yLabel);
													int index2 = srmUP.getIndex(xLabel);
													float dist = srmUP.getRelationDistance(index1,index2,-2.0);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = upLabels.area(yLabel);
													float area2 = upLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaUP += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmUP.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightUP += ctWt;
													/*if(label1=="4_Blotch_s4" && label2=="5_Excavated_s1" && k==SURR_BY && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** UP *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",upLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmUP.relationCountPercent(index1,index2));
														printf("ContrastWeightUP: %f\n",contrastWeightUP);
													}
													if(label1=="4_Blotch_s3" && label2=="5_Excavated_s1" && k==SURR_BY && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** UP *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",upLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmUP.relationCountPercent(index1,index2));
														printf("ContrastWeightUP: %f\n",contrastWeightUP);
													}
													if(label1=="5_Excavated_s1" && label2=="5_Excavated_s2" && k==SURR_BY && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** UP *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",upLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmUP.relationCountPercent(index1,index2));
														printf("ContrastWeightUP: %f\n",contrastWeightUP);
													}*/
												}// end of equationMap
											} // end y mergedLabelContainer UP

											float penaltyX = 1.0;
											if(upLabels.isShapeShifted(xLabel)) {
												int shapeNumX = upLabels.getShapeNum(xLabel);
												int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
												penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
												penalizedX = penaltyX;
											}
											areaUP += (areaY  * weight1 * deltaArcScore1)+ (areaX * penaltyX * weight2 * deltaArcScore2 * (areaY/totalDenomAreaUP));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("SURR_BY: UP 1b\n");
											printf("x: %d\n",x);
											exit(1);
										}
									} // end x mergedLabelContatiner UP
								} // end if countUP > 0
								// for DB
								if (countDB>0) {
									for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
										try {
											float totalDenomAreaDB=0.;
											String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
											int areaX = dbLabels.area(xLabel);
											int xIdx = srmDB.getIndex(xLabel);
											for(unsigned int yIdx=0; yIdx<srmDB.size(); yIdx++) {
												unsigned int rel_op_idx = srmDB.relation(yIdx,xIdx);
												if(rel_op_idx>NONE && rel_op_idx!=SURR_BY_INV) {
													//for all relations
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

													//> used for match calculations
													sumIslandAreaDB += dbLabels.area(yLabel);
													maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(yLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(dbLabels.getShade(yLabel),dbLabels.getShade(xLabel));
													int index1 = srmDB.getIndex(yLabel);
													int index2 = srmDB.getIndex(xLabel);
													float dist = srmDB.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = dbLabels.area(yLabel);
													float area2 = dbLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaDB += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmDB.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightDB += ctWt;
													/*if(label1=="4_Blotch_s4" && label2=="5_Excavated_s1" && k==SURR_BY && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** DB *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",dbLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmDB.relationCountPercent(index1,index2));
														printf("ContrastWeightDB: %f\n",contrastWeightDB);
													}
													if(label1=="4_Blotch_s3" && label2=="5_Excavated_s1" && k==SURR_BY && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** DB *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",dbLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmDB.relationCountPercent(index1,index2));
														printf("ContrastWeightDB: %f\n",contrastWeightDB);
													}
													if(label1=="5_Excavated_s1" && label2=="5_Excavated_s2" && k==SURR_BY && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** DB *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",dbLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmDB.relationCountPercent(index1,index2));
														printf("ContrastWeightDB: %f\n",contrastWeightDB);
													}*/
												}// end if equationMap
											} // end y mergedLabelContainer DB

											areaDB += (areaY * weight1 * deltaArcScore1) + (areaX * weight2 * deltaArcScore2 * (areaY/totalDenomAreaDB));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("SURR_BY: DB\n");
											exit(1);
										}
									} // end x mergedLabelContainer DB
								} // end if countDB>0
								areaValUP = areaUP * pow(this->rVal[maxNeighborLevelUP],m);
								areaValDB = areaDB * pow(this->rVal[maxNeighborLevelDB],m);
							}
							if(k==SURR_BY_INV) {
								//> UP initial calculations
								if (countUP>0) {
									for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
										String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
										if(label2Condition) {
											for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
												String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
												String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
												if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignX = upLabels.getStatSign(xLabel);
													if(statSignX.size()>0 && statSignX.size()==sumStatSignUP2.size()) {
														for(unsigned int n=0; n<statSignX.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignUP2.at(n) += statSignX.at(n);
														}
													}
													//> for sub island match calculations, arc score
													if(label2.find("Excavated")!=string::npos) {
														for(int n=0; n<upLabels.getIsland(xLabel).numOfSubIslands(); n++) {
															String shape_name = upLabels.getIsland(xLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = upLabels.getIsland(xLabel).subIsland(n).area();
																float bigArea = upLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreUP2 += score;
															}
														}
													}
												}// end if equationMap
											}// end for x srmUP
										}// end if label2Condition
										//> for dot product calculations (REI only so far)
										if(label1Condition) {
											vector<float> statSignY = upLabels.getStatSign(yLabel);
											if(statSignY.size()>0 && statSignY.size()==sumStatSignUP1.size()) {
												for(unsigned int n=0; n<statSignY.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignUP1.at(n) += statSignY.at(n);
												}
											}
										}
										//> for sub island match calculations, arc score
										if(label1.find("Excavated")!=string::npos) {
											for(int n=0; n<upLabels.getIsland(yLabel).numOfSubIslands(); n++) {
												String shape_name = upLabels.getIsland(yLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = upLabels.getIsland(yLabel).subIsland(n).area();
													float bigArea = upLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreUP1 += score;
												}
											}
										}
									}
								}// end if countUP > 0
								//> end UP initial calculations
								//DB initial calculations
								if (countDB>0) {
									for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
										String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
										if(label2Condition) {
											for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
												String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
												String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignX = dbLabels.getStatSign(xLabel);
													if(statSignX.size()>0 && statSignX.size()==sumStatSignDB2.size()) {
														for(unsigned int n=0; n<statSignX.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignDB2.at(n) += statSignX.at(n);
														}
													}
													//> for sub island match calculations, arc score
													if(label2.find("Excavated")!=string::npos) {
														for(int n=0; n<dbLabels.getIsland(xLabel).numOfSubIslands(); n++) {
															String shape_name = dbLabels.getIsland(xLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = dbLabels.getIsland(xLabel).subIsland(n).area();
																float bigArea = dbLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreDB2 += score;
															}
														}
													}
												}// end of equationMap
											}// end for x srmDB
										}// end if label2Condition
										//> for dot product calculations (REI only so far)
										if(label1Condition) {
											vector<float> statSignY = dbLabels.getStatSign(yLabel);
											if(statSignY.size()>0 && statSignY.size()==sumStatSignDB1.size()) {
												for(unsigned int n=0; n<statSignY.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignDB1.at(n) += statSignY.at(n);
												}
											}
										}
										//> for sub island match calculations, arc score
										if(label1.find("Excavated")!=string::npos) {
											for(int n=0; n<dbLabels.getIsland(yLabel).numOfSubIslands(); n++) {
												String shape_name = dbLabels.getIsland(yLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = dbLabels.getIsland(yLabel).subIsland(n).area();
													float bigArea = dbLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreDB1 += score;
												}
											}
										}
									}// end for y srmDB
								}
								// end DB initial calculation
								//> StatSign using Dot Product (Scheme I) <//
								if(label1Condition){
									dotProduct1 = statsign.dotProduct(sumStatSignUP1,sumStatSignDB1);
									adjustedDP1 = statsign.adjustValue(dotProduct1);
								}
								if(label2Condition) {
									dotProduct2 = statsign.dotProduct(sumStatSignUP2,sumStatSignDB2);
									adjustedDP2 = statsign.adjustValue(dotProduct2);
								}
								//> ************ End StatSign ************ <//
								weight1 = this->getShapeWeight(upMergedLabels.getShapeNum(label1),adjustedDP1);
								weight2 = this->getShapeWeight(upMergedLabels.getShapeNum(label2),adjustedDP2);
								deltaArcScore1 = this->calculateArcScore(sumArcScoreUP1,sumArcScoreDB1);
								deltaArcScore2 = this->calculateArcScore(sumArcScoreUP2,sumArcScoreDB2);
								// for UP
								if (countUP>0) {
									for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
										try {
											float totalDenomAreaUP=0.;
											String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
											int yIdx = srmUP.getIndex(yLabel);
											int areaY = upLabels.area(yLabel);
											for(unsigned int xIdx=0; xIdx<srmUP.size(); xIdx++) {
												unsigned int rel_op_idx = srmUP.relation(yIdx,xIdx);
												//for all relations
												if(rel_op_idx>NONE && rel_op_idx!=SURR_BY) {
													totalCountUP++;
													totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).second;
													/*if(label1=="5_Excavated_s1" && label2=="7_Default_s3" && m==2 && nStr=="n0_shd0_shp-1-1") {
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
													float penaltyX = 1.0;
													if(upLabels.isShapeShifted(xLabel)) {
														int shapeNumX = upLabels.getShapeNum(xLabel);
														int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
														penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
														penalizedX = penaltyX;
													}
													areaX += upLabels.area(xLabel) * penaltyX;

													sumIslandAreaUP += upLabels.area(xLabel);
													maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(xLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(upLabels.getShade(yLabel),upLabels.getShade(xLabel));
													int index1 = srmUP.getIndex(yLabel);
													int index2 = srmUP.getIndex(xLabel);
													float dist = srmUP.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = upLabels.area(yLabel);
													float area2 = upLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaUP += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmUP.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightUP += ctWt;
													/*if(label1=="5_Excavated_s1" && label2=="7_Default_s3" && k==SURR_BY_INV && m==2 && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** UP *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",upLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmUP.relationCountPercent(index1,index2));
														printf("ContrastWeightUP: %f\n",contrastWeightUP);
													}*/
												}// end if equationMap
											}// end for x srmUP

											float penaltyY = 1.0;
											if(upLabels.isShapeShifted(yLabel)) {
												int shapeNumY = upLabels.getShapeNum(yLabel);
												int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
												penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
												penalizedY = penaltyY;
											}
											areaUP += (areaX * weight2 * deltaArcScore2) + (areaY * penaltyY * weight1 * deltaArcScore1 *(areaX/totalDenomAreaUP));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("SURR_BY_INV: UP\n");
											exit(1);
										}
									}// end for y srmUP
								}// end if countUP > 0
								// for DB
								if (countDB>0) {
									for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
										try {
											float totalDenomAreaDB=0.;
											String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
											int yIdx = srmDB.getIndex(yLabel);
											int areaY = dbLabels.area(yLabel);
											for(unsigned int xIdx=0; xIdx<srmDB.size(); xIdx++) {
												unsigned int rel_op_idx = srmDB.relation(yIdx,xIdx);
												//for all relations
												if(rel_op_idx>NONE && rel_op_idx!=SURR_BY) {
													totalCountDB++;
													totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).second;
												}
											}
											int areaX=0;
											for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
												String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
												String equationKey = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKey)!=srmDB.equationMap.end()) {
													areaX += dbLabels.area(xLabel);

													//> for match calculations
													sumIslandAreaDB += dbLabels.area(xLabel);
													maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(xLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(dbLabels.getShade(yLabel),dbLabels.getShade(xLabel));
													int index1 = srmDB.getIndex(yLabel);
													int index2 = srmDB.getIndex(xLabel);
													float dist = srmDB.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = dbLabels.area(yLabel);
													float area2 = dbLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaDB += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmDB.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightDB += ctWt;
													/*if(label1=="0_Comp-Disc_s3" && label2=="1_Incomp-Disc_s4" && k==SURR_BY_INV && nStr=="n0_shd0_shp-1-1") {
														printf("[%s][%s][%s]\n",label1.c_str(),this->rel_op.at(k).c_str(),label2.c_str());
														printf("**** DB *****");
														printf("ShadeDiff: %d\n",shadeDiff);
														printf("AvgDist: %f\n",dist);
														printf("RelAreaY: %.0f, RelAreaX: %.0f\n",area1,area2);
														printf("TotalArea: %d\n",dbLabels.totalArea());
														printf("RelArea: %f\n",relArea);
														printf("EsgVal: %f\n",esg.esgVal);
														printf("ctWt: %f\n",ctWt);
														printf("RelationCountPercent: %f\n",srmDB.relationCountPercent(index1,index2));
														printf("ContrastWeightDB: %f\n",contrastWeightDB);
													}*/
												}// end if equationMap
											} // end x mergedLabelContainer DB

											areaDB += (areaX * weight2 * deltaArcScore2) + (areaY * weight1 * deltaArcScore1 *(areaX/totalDenomAreaDB));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("SURR_BY_INV: DB\n");
											exit(1);
										}
									} // end y mergedLabelContainer
								}// end if countDB > 0
								areaValUP = areaUP * pow(this->rVal[maxNeighborLevelUP],m);
								areaValDB = areaDB * pow(this->rVal[maxNeighborLevelDB],m);
							} // end SURR_BY_INV
							//> E-Value using count, base on relArea <//
							float countProportionUP = sumIslandAreaUP/maxIslandAreaUP;
							float countProportionDB = sumIslandAreaDB/maxIslandAreaDB;
							if(std::isnan(countProportionUP)) countProportionUP = 0.0;
							if(std::isnan(countProportionDB)) countProportionDB = 0.0;
							float entropyUP = this->entropy(countProportionUP);
							float entropyDB = this->entropy(countProportionDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							//> ********* End E-Value ************ <//
							//> Expected value of Shade Gradient (ESG) <//
							contrastWeightUP /= sumOfAreaUP;
							contrastWeightDB /= sumOfAreaDB;
							if(std::isnan(contrastWeightUP)) contrastWeightUP=0.0;
							if(std::isnan(contrastWeightDB)) contrastWeightDB=0.0;
							float contrastWeight = min(contrastWeightUP,contrastWeightDB);
							//> ********** End (ESG) *********** <//
							if(std::isnan(areaValUP)) areaValUP=0.0;
							if(std::isnan(areaValDB)) areaValDB=0.0;
							float areaVal = min(areaValUP,areaValDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal * contrastWeight;
							totalMatchScore += weightedEntropy;

							float weightedMismatchEntropy  = 0.0;
							if(countUP>0 && countDB==0) {
								weightedMismatchEntropy = contrastWeightUP * (areaValUP / upMergedLabels.totalArea());
							}
							if(countDB>0 && countUP==0) {
								weightedMismatchEntropy = contrastWeightDB * (areaValDB / dbMergedLabels.totalArea());
							}
							totalMismatchScore += weightedMismatchEntropy;

							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s] : Level %d\n", label1.c_str(),relOp.c_str(),label2.c_str(),m);
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"sumIslandAreaUP: %f, sumIslandAreaDB: %f\n",sumIslandAreaUP,sumIslandAreaDB);
								fprintf(fp,"maxIslandAreaUP: %d, maxIslandAreaDB: %d\n",maxIslandAreaUP,maxIslandAreaDB);
								fprintf(fp,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"MaxNeighborLevelUP: %d, MaxNeighborLevelDB: %d\n",maxNeighborLevelUP,maxNeighborLevelDB);
								fprintf(fp,"AreaValUP: %f, AreaValDB: %f\n",areaValUP,areaValDB);
								fprintf(fp,"PenaltyY: %f, PenaltyX: %f\n",penalizedY, penalizedX);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp,"DeltaArcScore1: %f, DeltaArcScore2: %f\n",deltaArcScore1,deltaArcScore2);
								fprintf(fp,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalMatchScore: %f\n",totalMatchScore);
								fprintf(fp,"----------------------------------\n");
							}
							if(weightedMismatchEntropy>0) {
								fprintf(fp2,"[%s][%s][%s] : Level %d\n", label1.c_str(),relOp.c_str(),label2.c_str(),m);
								fprintf(fp2,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp2,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp2,"sumIslandAreaUP: %f, sumIslandAreaDB: %f\n",sumIslandAreaUP,sumIslandAreaDB);
								fprintf(fp2,"maxIslandAreaUP: %d, maxIslandAreaDB: %d\n",maxIslandAreaUP,maxIslandAreaDB);
								fprintf(fp2,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp2,"EntropyVal: %f\n",entropyVal);
								fprintf(fp2,"MaxNeighborLevelUP: %d, MaxNeighborLevelDB: %d\n",maxNeighborLevelUP,maxNeighborLevelDB);
								fprintf(fp2,"AreaValUP: %f, AreaValDB: %f\n",areaValUP,areaValDB);
								fprintf(fp2,"PenaltyY: %f, PenaltyX: %f\n",penalizedY, penalizedX);
								fprintf(fp2,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp2,"RelArea: %f\n",relArea);
								fprintf(fp2,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp2,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp2,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp2,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp2,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp2,"DeltaArcScore1: %f, DeltaArcScore2: %f\n",deltaArcScore1,deltaArcScore2);
								fprintf(fp2,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp2,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp2,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp2,"WeightedMismatchEntropy: %f\n",weightedMismatchEntropy);
								fprintf(fp2,"TotalMismatchScore: %f\n",totalMismatchScore);
								fprintf(fp2,"----------------------------------\n");
							}
							if(weightedEntropy<=0 || std::isnan(weightedEntropy)) {
								fprintf(fp3,"[%s][%s][%s] : Level %d\n", label1.c_str(),relOp.c_str(),label2.c_str(),m);
								fprintf(fp3,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp3,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp3,"sumIslandAreaUP: %f, sumIslandAreaDB: %f\n",sumIslandAreaUP,sumIslandAreaDB);
								fprintf(fp3,"maxIslandAreaUP: %d, maxIslandAreaDB: %d\n",maxIslandAreaUP,maxIslandAreaDB);
								fprintf(fp3,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp3,"EntropyVal: %f\n",entropyVal);
								fprintf(fp3,"MaxNeighborLevelUP: %d, MaxNeighborLevelDB: %d\n",maxNeighborLevelUP,maxNeighborLevelDB);
								fprintf(fp3,"AreaValUP: %f, AreaValDB: %f\n",areaValUP,areaValDB);
								fprintf(fp3,"PenaltyY: %f, PenaltyX: %f\n",penalizedY, penalizedX);
								fprintf(fp3,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp3,"RelArea: %f\n",relArea);
								fprintf(fp3,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp3,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp3,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp3,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp3,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp3,"DeltaArcScore1: %f, DeltaArcScore2: %f\n",deltaArcScore1,deltaArcScore2);
								fprintf(fp3,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp3,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp3,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp3,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp3,"TotalMatchScore: %f\n",totalMatchScore);
								fprintf(fp3,"----------------------------------\n");
							}
						} // if countUP || countDB
					} // end for loop m
				} // end if k==SURR_BY || SURR_BY_INV
				else {
					if(k==DIR) { // constant m=1 || neighborNum=1
						const int neighborNum = 1;
						float penalizedY = 1.0 , penalizedX = 1.0;
						int countUP = srmCountUP.at(i).at(j).at(k).at(neighborNum);
						int countDB = srmCountDB.at(i).at(j).at(k).at(neighborNum);
						int areaUP1 = srmAreaUP.at(i).at(j).at(k).at(neighborNum).first;
						int areaUP2 = srmAreaUP.at(i).at(j).at(k).at(neighborNum).second;
						int areaDB1 = srmAreaDB.at(i).at(j).at(k).at(neighborNum).first;
						int areaDB2 = srmAreaDB.at(i).at(j).at(k).at(neighborNum).second;
						float areaUP=0.0, areaDB=0.0;
						int maxIslandAreaUP = 0, maxIslandAreaDB = 0;
						float sumIslandAreaUP=0.0, sumIslandAreaDB = 0.0;
						float sumArcScoreUP1=0.0, sumArcScoreUP2=0.0, sumArcScoreDB1=0.0, sumArcScoreDB2=0.0;
						float deltaArcScore1=1.0, deltaArcScore2=1.0;
						//> for dot product and weights
						float dotProduct1 = 1.0, dotProduct2 = 1.0;
						float adjustedDP1 = 1.0, adjustedDP2 = 1.0;
						float weight1 = 1.0, weight2 = 1.0;
						vector<float> sumStatSignUP1(17,0), sumStatSignUP2(17,0), sumStatSignDB1(17,0), sumStatSignDB2(17,0);
						vector<float> statSignUP1(17,0.0), statSignUP2(17,0.0), statSignDB1(17,0.0), statSignDB2(17,0.0);
						//> for contrast weight
						float contrastWeightUP=0.0, contrastWeightDB=0.0;
						float sumOfAreaUP = 0.0, sumOfAreaDB = 0.0;
						if(countUP>0 || countDB>0) {
							if (countUP>0) {
								if(areaUP1<=areaUP2) {
									//> UP initial calculations 1
									for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
										String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
										if(label1Condition) {
											for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
												String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
												String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
												if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignY = upLabels.getStatSign(yLabel);
													if(statSignY.size()>0 && statSignY.size()==sumStatSignUP1.size()) {
														for(unsigned int n=0; n<statSignY.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignUP1.at(n) += statSignY.at(n);
														}
													}

													//> for sub island match calculations
													if(label1.find("Excavated")!=string::npos) {
														for(int n=0; n<upLabels.getIsland(yLabel).numOfSubIslands(); n++) {
															String shape_name = upLabels.getIsland(yLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = upLabels.getIsland(yLabel).subIsland(n).area();
																float bigArea = upLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreUP1 += score;
															}
														}
													}
												}
											}
										}
										//> for dot product calculations (REI only so far)
										if(label2Condition) {
											vector<float> statSignX = upLabels.getStatSign(xLabel);
											if(statSignX.size()>0 && statSignX.size()==sumStatSignUP2.size()) {
												for(unsigned int n=0; n<statSignX.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignUP2.at(n) += statSignX.at(n);
												}
											}
										}

										//> for sub island match calculations
										if(label2.find("Excavated")!=string::npos) {
											for(int n=0; n<upLabels.getIsland(xLabel).numOfSubIslands(); n++) {
												String shape_name = upLabels.getIsland(xLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = upLabels.getIsland(xLabel).subIsland(n).area();
													float bigArea = upLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreUP2 += score;
												}
											}
										}
									}
								}
								//> end UP initial calculations
								if(areaUP1>areaUP2) {
									//> UP initial calculations 2
									for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
										String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
										if(label2Condition) {
											for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
												String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
												String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
												if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignX = upLabels.getStatSign(xLabel);
													if(statSignX.size()>0 && statSignX.size()==sumStatSignUP2.size()) {
														for(unsigned int n=0; n<statSignX.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignUP2.at(n) += statSignX.at(n);
														}
													}
													//> for sub island match calculations
													if(label2.find("Excavated")!=string::npos) {
														for(int n=0; n<upLabels.getIsland(xLabel).numOfSubIslands(); n++) {
															String shape_name = upLabels.getIsland(xLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = upLabels.getIsland(xLabel).subIsland(n).area();
																float bigArea = upLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreUP2 += score;
															}
														}
													}
												}
											}
										}
										//> for dot product calculations (REI only so far)
										if(label1Condition) {
											vector<float> statSignY = upLabels.getStatSign(yLabel);
											if(statSignY.size()>0 && statSignY.size()==sumStatSignUP1.size()) {
												for(unsigned int n=0; n<statSignY.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignUP1.at(n) += statSignY.at(n);
												}
											}
										}
										if(label1.find("Excavated")!=string::npos) {
											for(int n=0; n<upLabels.getIsland(yLabel).numOfSubIslands(); n++) {
												String shape_name = upLabels.getIsland(yLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = upLabels.getIsland(yLabel).subIsland(n).area();
													float bigArea = upLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreUP1 += score;
												}
											}
										}
									}
									//> end UP initial calculations
								} //> end areaUP1>areaUP2
							}// end if countUP > 0
							if (countDB>0) {
								if(areaDB1<=areaDB2) {
									//DB initial calculations 1
									for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
										String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
										if(label1Condition) {
											for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
												String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
												String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignY = dbLabels.getStatSign(yLabel);
													if(statSignY.size()>0 && statSignY.size()==sumStatSignDB1.size()) {
														for(unsigned int n=0; n<statSignY.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignDB1.at(n) += statSignY.at(n);
														}
													}
													//> for sub island match calculations
													if(label1.find("Excavated")!=string::npos) {
														for(int n=0; n<dbLabels.getIsland(yLabel).numOfSubIslands(); n++) {
															String shape_name = dbLabels.getIsland(yLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = dbLabels.getIsland(yLabel).subIsland(n).area();
																float bigArea = dbLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreDB1 += score;
															}
														}
													}
												}
											}
										}
										//> for dot product calculations (REI only so far)
										if(label2Condition) {
											vector<float> statSignX = dbLabels.getStatSign(xLabel);
											if(statSignX.size()>0 && statSignX.size()==sumStatSignDB2.size()) {
												for(unsigned int n=0; n<statSignX.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignDB2.at(n) = statSignX.at(n);
												}
											}
										}
										//> for sub island match calculations
										if(label2.find("Excavated")!=string::npos) {
											for(int n=0; n<dbLabels.getIsland(xLabel).numOfSubIslands(); n++) {
												String shape_name = dbLabels.getIsland(xLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = dbLabels.getIsland(xLabel).subIsland(n).area();
													float bigArea = dbLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreDB2 += score;
												}
											}
										}
									}
									// end DB initial calculation
								} // end areaDB1<=areaDB2
								if(areaDB1>areaDB2) {
									//DB initial calculations 2
									for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
										String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
										if(label2Condition) {
											for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
												String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
												String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
													//> for dot product calculations (REI only so far)
													vector<float> statSignX = dbLabels.getStatSign(xLabel);
													if(statSignX.size()>0 && statSignX.size()==sumStatSignDB2.size()) {
														for(unsigned int n=0; n<statSignX.size(); n++) {
															//change from "+=" -> "=" for individual application
															sumStatSignDB2.at(n) += statSignX.at(n);
														}
													}
													//> for sub island match calculations
													if(label2.find("Excavated")!=string::npos) {
														for(int n=0; n<dbLabels.getIsland(xLabel).numOfSubIslands(); n++) {
															String shape_name = dbLabels.getIsland(xLabel).subIsland(n).shape_name();
															if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																int smallArea = dbLabels.getIsland(xLabel).subIsland(n).area();
																float bigArea = dbLabels.totalArea();
																float relArea = smallArea/bigArea;
																float val = negArcScore;
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	val = posArcScore;
																}
																float score = val * relArea;
																sumArcScoreDB2 += score;
															}
														}
													}
												}
											}
										}
										//> for dot product calculations (REI only so far)
										if(label1Condition) {
											vector<float> statSignY = dbLabels.getStatSign(yLabel);
											if(statSignY.size()>0 && statSignY.size()==sumStatSignDB1.size()) {
												for(unsigned int n=0; n<statSignY.size(); n++) {
													//change from "+=" -> "=" for individual application
													sumStatSignDB1.at(n) += statSignY.at(n);
												}
											}
										}
										//> for sub island match calculations
										if(label1.find("Excavated")!=string::npos) {
											for(int n=0; n<dbLabels.getIsland(yLabel).numOfSubIslands(); n++) {
												String shape_name = dbLabels.getIsland(yLabel).subIsland(n).shape_name();
												if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
													int smallArea = dbLabels.getIsland(yLabel).subIsland(n).area();
													float bigArea = dbLabels.totalArea();
													float relArea = smallArea/bigArea;
													float val = negArcScore;
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
														val = posArcScore;
													}
													float score = val * relArea;
													sumArcScoreDB1 += score;
												}
											}
										}
									}
									// end DB initial calculation 2
								} // end areaDB1>areaDB2
							}// end if countDB > 0
							//> StatSign using Dot Product (Scheme I) <//
							if(label1Condition){
								dotProduct1 = statsign.dotProduct(sumStatSignUP1,sumStatSignDB1);
								adjustedDP1 = statsign.adjustValue(dotProduct1);
							}
							if(label2Condition) {
								dotProduct2 = statsign.dotProduct(sumStatSignUP2,sumStatSignDB2);
								adjustedDP2 = statsign.adjustValue(dotProduct2);
							}
							//> ************ End StatSign ************ <//
							weight1 = this->getShapeWeight(upMergedLabels.getShapeNum(label1),adjustedDP1);
							weight2 = this->getShapeWeight(upMergedLabels.getShapeNum(label2),adjustedDP2);
							deltaArcScore1 = this->calculateArcScore(sumArcScoreUP1,sumArcScoreDB1);
							deltaArcScore2 = this->calculateArcScore(sumArcScoreUP2,sumArcScoreDB2);

							//> Y <OP> X
							if (countUP>0) {
								if(areaUP1<=areaUP2) {
									/*! for UP */
									for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
										try {
											float totalDenomAreaUP=0.;
											String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
											int areaX = upLabels.area(xLabel); // get area
											int xIdx = srmUP.getIndex(xLabel); // get pos from srm
											for(unsigned int yIdx=0; yIdx<srmUP.size(); yIdx++) {
												try {
													unsigned int rel_op_idx = srmUP.relation(yIdx,xIdx);
													//for all relations
													if(rel_op_idx>NONE && rel_op_idx!=SURR_BY_INV) {
														totalCountUP++; //total relations for areaUP2
														totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).first;
													}
												} catch (const std::out_of_range &oor) {
													printf("ShadeShapeRelationMatch::entropy() out of range!\n");
													printf("DIR: UP 1a\n");
													printf("xLabel: %s\n",xLabel.c_str());
													printf("xIdx: %d\n",xIdx);
													exit(1);
												}
											}
											int areaY=0;
											for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
												String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
												String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
												if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
													float penaltyY = 1.0;
													if(upLabels.isShapeShifted(yLabel)) {
														int shapeNumY = upLabels.getShapeNum(yLabel);
														int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
														penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
														penalizedY = penaltyY;
													}
													areaY += upLabels.area(yLabel) * penaltyY;

													//> for match calculations
													sumIslandAreaUP += upLabels.area(yLabel);
													maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(yLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(upLabels.getShade(yLabel),upLabels.getShade(xLabel));
													int index1 = srmUP.getIndex(yLabel);
													int index2 = srmUP.getIndex(xLabel);
													float dist = srmUP.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = upLabels.area(yLabel);
													float area2 = upLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaUP += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmUP.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightUP += ctWt;
												}// end if equationMap
											}// end for y srmUP

											float penaltyX = 1.0;
											if(upLabels.isShapeShifted(xLabel)) {
												int shapeNumX = upLabels.getShapeNum(xLabel);
												int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
												penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
												penalizedX = penaltyX;
											}
											areaUP += (areaY * weight1 * deltaArcScore1) + (areaX * penaltyX * weight2 * deltaArcScore2 *(areaY/totalDenomAreaUP));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("DIR: UP 1b\n");
											printf("x: %d\n",x);
											exit(1);
										}
									} // end for UP
								} // end if areaUP1 <= areaUP2
								//> X <OP> Y
								else if(areaUP1>areaUP2) {
									/*! for UP */
									for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
										try {
											float totalDenomAreaUP=0.;
											String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
											int yIdx = srmUP.getIndex(yLabel);
											int areaY = upLabels.area(yLabel);
											for(unsigned int xIdx=0; xIdx<srmUP.size(); xIdx++) {
												unsigned int rel_op_idx = srmUP.relation(yIdx,xIdx);
												//for all relations
												if(rel_op_idx>NONE && rel_op_idx!=SURR_BY) {
													totalCountUP++; //total relations for areaUP1
													totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).second;
												}
												/*if(label1=="0_Strip_s4" && label2=="1_Default_s2" && m==1) {
														printf("yIdx: %d, %s\n",yIdx,yLabel.c_str());
														printf("xIdx: %d, %s\n",xIdx,upLabels.at(xIdx).c_str());
														printf("rel_op_idx: %d\n",rel_op_idx);
														printf("srmRelationArea: %d\n",srmUP.relationArea(yIdx,xIdx).second);
														printf("totalDenomAreaUP: %f\n",totalDenomAreaUP);

													}*/
											}
											int areaX=0;
											for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
												String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
												String equationKey = yLabel+"<"+relOp+">"+xLabel;
												if(srmUP.equationMap.find(equationKey)!=srmUP.equationMap.end()) {
													float penaltyX = 1.0;
													if(upLabels.isShapeShifted(xLabel)) {
														int shapeNumX = upLabels.getShapeNum(xLabel);
														int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
														penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
														penalizedX = penaltyX;
													}
													areaX += upLabels.area(xLabel) * penaltyX;

													//> for match calculations
													sumIslandAreaUP += upLabels.area(xLabel);
													maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(xLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(upLabels.getShade(yLabel),upLabels.getShade(xLabel));
													int index1 = srmUP.getIndex(yLabel);
													int index2 = srmUP.getIndex(xLabel);
													float dist = srmUP.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = upLabels.area(yLabel);
													float area2 = upLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaUP += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmUP.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightUP += ctWt;
												}// end if equationMap
											}// end for x srmUP

											float penaltyY = 1.0;
											if(upLabels.isShapeShifted(yLabel)) {
												int shapeNumY = upLabels.getShapeNum(yLabel);
												int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
												penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
												penalizedY = penaltyY;
											}
											areaUP += (areaX * weight2 * deltaArcScore2) + (areaY * penaltyY * weight1 * deltaArcScore1 *(areaX/totalDenomAreaUP));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("DIR: UP 2\n");
											exit(1);
										}
									} // end for UP mergedLabelContainer
								} // end else if areaUP1 > areaUP2
							}
							if (countDB>0) {
								if(areaDB1<=areaDB2) {
									/*! for DB */
									for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
										try {
											float totalDenomAreaDB=0.;
											String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
											int areaX = dbLabels.area(xLabel);
											int xIdx = srmDB.getIndex(xLabel);
											for(unsigned int yIdx=0; yIdx<srmDB.size(); yIdx++) {
												unsigned int rel_op_idx = srmDB.relation(yIdx,xIdx);
												//for all relations
												if(rel_op_idx>NONE && rel_op_idx!=SURR_BY_INV) {
													totalCountDB++; //total relations for areaDB1
													totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).second;
												}
											}
											int areaY=0;
											for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
												String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
												String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
													areaY += dbLabels.area(yLabel);

													//>for match calculations
													sumIslandAreaDB += dbLabels.area(yLabel);
													maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(yLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(dbLabels.getShade(yLabel),dbLabels.getShade(xLabel));
													int index1 = srmDB.getIndex(yLabel);
													int index2 = srmDB.getIndex(xLabel);
													float dist = srmDB.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = dbLabels.area(yLabel);
													float area2 = dbLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaDB += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmDB.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightDB += ctWt;
												} //end equationMap
											} //end y srmDB mergedContainer

											areaDB += (areaY * weight1 * deltaArcScore1) + (areaX * weight2 * deltaArcScore2 *(areaY/totalDenomAreaDB));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("DIR: DB 1\n");
											exit(1);
										}
									} // end for DB
								} // end else if areaDB1 <= areaDB2
								else if(areaDB1>areaDB2) {
									/*! for DB */
									for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.size(); y++) {
										try {
											float totalDenomAreaDB=0.;
											String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).first.at(y);
											int yIdx = srmDB.getIndex(yLabel);
											int areaY = dbLabels.area(yLabel);
											for(unsigned int xIdx=0; xIdx<srmDB.size(); xIdx++) {
												unsigned int rel_op_idx = srmDB.relation(yIdx,xIdx);
												//for all relations
												if(rel_op_idx>NONE && rel_op_idx!=SURR_BY) {
													totalCountDB++; //total relations for areaUP1
													totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).second;
												}
											}
											int areaX=0;
											for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.size(); x++) {
												String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(neighborNum).second.at(x);
												String equationKey = yLabel+"<"+relOp+">"+xLabel;
												if(srmDB.equationMap.find(equationKey)!=srmDB.equationMap.end()) {
													areaX += dbLabels.area(xLabel);

													//>for match calculations
													sumIslandAreaDB += dbLabels.area(xLabel);
													maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(xLabel));

													//> Expected value of Shade Gradient (ESG) <//
													Esg esg;
													int shadeDiff = esg.shadeDiff(dbLabels.getShade(yLabel),dbLabels.getShade(xLabel));
													int index1 = srmDB.getIndex(yLabel);
													int index2 = srmDB.getIndex(xLabel);
													float dist = srmDB.getRelationDistance(index1,index2,-2);
													esg.calculate(dist,shadeDiff);
													esg.esgVal = max(0.0f,esg.esgVal);
													float area1 = dbLabels.area(yLabel);
													float area2 = dbLabels.area(xLabel);
													float relArea = area1 + area2;
													sumOfAreaDB += relArea;
													float ctWt = this->contrastWeight(esg.esgVal,relArea);
													ctWt *= srmDB.relationCountPercent(index1,index2);
													if(std::isnan(ctWt)) ctWt=0.0;
													contrastWeightDB += ctWt;
												}// end if equationMap
											}// end for x srmDB

											areaDB += (areaX * weight2 * deltaArcScore2) + (areaY * weight1 * deltaArcScore1 *(areaX/totalDenomAreaDB));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("DIR: DB 2\n");
											exit(1);
										}
									} // end for DB
								} // end else if areaDB1 > areaDB2
							}
							areaUP = areaUP * this->rVal[maxNeighborLevelUP];
							areaDB = areaDB * this->rVal[maxNeighborLevelDB];

							//> E-Value using count base on relArea <//
							float countProportionUP = sumIslandAreaUP/maxIslandAreaUP;
							float countProportionDB = sumIslandAreaDB/maxIslandAreaDB;
							if(std::isnan(countProportionUP)) countProportionUP = 0.0;
							if(std::isnan(countProportionDB)) countProportionDB = 0.0;
							float entropyUP = this->entropy(countProportionUP);
							float entropyDB = this->entropy(countProportionDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							//> ********* End E-Value ************ <//
							//> Expected value of Shade Gradient (ESG) <//
							contrastWeightUP /= sumOfAreaUP;
							contrastWeightDB /= sumOfAreaDB;
							if(std::isnan(contrastWeightUP)) contrastWeightUP = 0.0;
							if(std::isnan(contrastWeightDB)) contrastWeightDB = 0.0;
							float contrastWeight = min(contrastWeightUP,contrastWeightDB);

							if(std::isnan(areaUP)) areaUP=0;
							if(std::isnan(areaDB)) areaDB=0;
							float areaVal = min(areaUP,areaDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal * contrastWeight;
							totalMatchScore += weightedEntropy;

							float weightedMismatchEntropy  = 0.0;
							if(countUP>0 && countDB==0) {
								weightedMismatchEntropy = contrastWeightUP * (areaUP / upMergedLabels.totalArea());
							}
							if(countDB>0 && countUP==0) {
								weightedMismatchEntropy = contrastWeightDB * (areaDB / dbMergedLabels.totalArea());
							}
							totalMismatchScore += weightedMismatchEntropy;

							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", label1.c_str(),relOp.c_str(),label2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp,"AreaUP: %f, AreaDB: %f\n",areaUP,areaDB);
								fprintf(fp,"PenaltyY: %f, PenaltyX: %f\n",penalizedY,penalizedX);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp,"DeltaArcScore1: %f, DeltaArcScore2: %f\n",deltaArcScore1,deltaArcScore2);
								fprintf(fp,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalMatchScore: %f\n",totalMatchScore);
								fprintf(fp,"----------------------------------\n");
							}
							if(weightedMismatchEntropy>0) {
								fprintf(fp2,"[%s][%s][%s]\n", label1.c_str(),relOp.c_str(),label2.c_str());
								fprintf(fp2,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp2,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp2,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp2,"EntropyVal: %f\n",entropyVal);
								fprintf(fp2,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp2,"AreaUP: %f, AreaDB: %f\n",areaUP,areaDB);
								fprintf(fp2,"PenaltyY: %f, PenaltyX: %f\n",penalizedY,penalizedX);
								fprintf(fp2,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp2,"RelArea: %f\n",relArea);
								fprintf(fp2,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp2,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp2,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp2,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp2,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp2,"DeltaArcScore1: %f, DeltaArcScore2: %f\n",deltaArcScore1,deltaArcScore2);
								fprintf(fp2,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp2,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp2,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp2,"WeightedMismatchEntropy: %f\n",weightedMismatchEntropy);
								fprintf(fp2,"TotalMismatchScore: %f\n",totalMismatchScore);
								fprintf(fp2,"----------------------------------\n");
							}
							if(weightedEntropy<=0 || std::isnan(weightedEntropy)) {
								fprintf(fp3,"[%s][%s][%s]\n", label1.c_str(),relOp.c_str(),label2.c_str());
								fprintf(fp3,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp3,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp3,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp3,"EntropyVal: %f\n",entropyVal);
								fprintf(fp3,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp3,"AreaUP: %f, AreaDB: %f\n",areaUP,areaDB);
								fprintf(fp3,"PenaltyY: %f, PenaltyX: %f\n",penalizedY,penalizedX);
								fprintf(fp3,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp3,"RelArea: %f\n",relArea);
								fprintf(fp3,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp3,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp3,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp3,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp3,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp3,"DeltaArcScore1: %f, DeltaArcScore2: %f\n",deltaArcScore1,deltaArcScore2);
								fprintf(fp3,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp3,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp3,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp3,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp3,"TotalMatchScore: %f\n",totalMatchScore);
								fprintf(fp3,"----------------------------------\n");
							}
						} // end if countUP || countDB
					} // end if DIR
					else { // INDIR
						float penalizedY = 1.0 , penalizedX = 1.0;
						int countUP=0, countDB=0;
						int areaUP1=0, areaUP2=0, areaDB1=0, areaDB2=0;
						for(int m=0; m<this->relOpLevelSize; m++) {
							countUP += srmCountUP.at(i).at(j).at(k).at(m);
							countDB += srmCountDB.at(i).at(j).at(k).at(m);
							areaUP1 += srmAreaUP.at(i).at(j).at(k).at(m).first;
							areaUP2 += srmAreaUP.at(i).at(j).at(k).at(m).second;
							areaDB1 += srmAreaDB.at(i).at(j).at(k).at(m).first;
							areaDB2 += srmAreaDB.at(i).at(j).at(k).at(m).second;
						}
						float areaUP=0.0, areaDB=0.0;
						int maxIslandAreaUP = 0, maxIslandAreaDB = 0;
						float sumIslandAreaUP=0.0, sumIslandAreaDB=0.0;
						float sumArcScoreUP1=0.0, sumArcScoreUP2=0.0, sumArcScoreDB1=0.0, sumArcScoreDB2=0.0;
						float deltaArcScore1=1.0, deltaArcScore2=1.0;
						//> for dot products and weights
						float dotProduct1 = 1.0, dotProduct2 = 1.0;
						float adjustedDP1 = 1.0, adjustedDP2 = 1.0;
						float weight1 = 1.0, weight2 = 1.0;
						vector<float> sumStatSignUP1(17,0), sumStatSignUP2(17,0), sumStatSignDB1(17,0), sumStatSignDB2(17,0);
						vector<float> statSignUP1(17,0.0), statSignUP2(17,0.0), statSignDB1(17,0.0), statSignDB2(17,0.0);
						//> for contrast weight
						float contrastWeightUP=0.0, contrastWeightDB=0.0;
						float sumOfAreaUP = 0.0, sumOfAreaDB = 0.0;
						if(countUP>0 || countDB>0) {
							for(int m=0; m<this->relOpLevelSize; m++) {
								if (countUP>0) {
									if(areaUP1<=areaUP2) {
										//> UP initial calculations 1
										for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
											String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
											if(label1Condition) {
												for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
													String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
													String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
													if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
														//> for dot product calculations (REI only so far)
														vector<float> statSignY = upLabels.getStatSign(yLabel);
														if(statSignY.size()>0 && statSignY.size()==sumStatSignUP1.size()) {
															for(unsigned int n=0; n<statSignY.size(); n++) {
																sumStatSignUP1.at(n) += statSignY.at(n);
															}
														}
														//> for sub island match calculations
														if(label1.find("Excavated")!=string::npos) {
															for(int n=0; n<upLabels.getIsland(yLabel).numOfSubIslands(); n++) {
																String shape_name = upLabels.getIsland(yLabel).subIsland(n).shape_name();
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	int smallArea = upLabels.getIsland(yLabel).subIsland(n).area();
																	float bigArea = upLabels.totalArea();
																	float relArea = smallArea/bigArea;
																	float val = negArcScore;
																	if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																		val = posArcScore;
																	}
																	float score = val * relArea;
																	sumArcScoreUP1 += score;
																}
															}
														}
													}
												}
											}
											//> for dot product calculations (REI only so far)
											if(label2Condition) {
												vector<float> statSignX = upLabels.getStatSign(xLabel);
												if(statSignX.size()>0 && statSignX.size()==sumStatSignUP2.size()) {
													for(unsigned int n=0; n<statSignX.size(); n++) {
														sumStatSignUP2.at(n) += statSignX.at(n);
													}
												}
											}
											//> for sub island match calculations
											if(label2.find("Excavated")!=string::npos) {
												for(int n=0; n<upLabels.getIsland(xLabel).numOfSubIslands(); n++) {
													String shape_name = upLabels.getIsland(xLabel).subIsland(n).shape_name();
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
														int smallArea = upLabels.getIsland(xLabel).subIsland(n).area();
														float bigArea = upLabels.totalArea();
														float relArea = smallArea/bigArea;
														float val = negArcScore;
														if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
															val = posArcScore;
														}
														float score = val * relArea;
														sumArcScoreUP2 += score;
													}
												}
											}
										}
										//> end UP initial calculations 1
									}
									if(areaUP1>areaUP2) {
										//> UP initial calculations 2
										for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
											String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
											if(label2Condition) {
												for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
													String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
													String equationKeyUP = yLabel+"<"+relOp+">"+xLabel;
													if(srmUP.equationMap.find(equationKeyUP)!=srmUP.equationMap.end()) {
														//> for dot product calculations (REI only so far)
														vector<float> statSignX = upLabels.getStatSign(xLabel);
														if(statSignX.size()>0 && statSignX.size()==sumStatSignUP2.size()) {
															for(unsigned int n=0; n<statSignX.size(); n++) {
																sumStatSignUP2.at(n) += statSignX.at(n);
															}
														}
														//> for sub island match calculations
														if(label2.find("Excavated")!=string::npos) {
															for(int n=0; n<upLabels.getIsland(xLabel).numOfSubIslands(); n++) {
																String shape_name = upLabels.getIsland(xLabel).subIsland(n).shape_name();
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	int smallArea = upLabels.getIsland(xLabel).subIsland(n).area();
																	float bigArea = upLabels.totalArea();
																	float relArea = smallArea/bigArea;
																	float val = negArcScore;
																	if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																		val = posArcScore;
																	}
																	float score = val * relArea;
																	sumArcScoreUP2 += score;
																}
															}
														}
													}// end if equationMap
												} // end for x srmUP
											} // end label2Condition
											//> for dot product calculations (REI only so far)
											if(label1Condition) {
												vector<float> statSignY = upLabels.getStatSign(yLabel);
												if(statSignY.size()>0 && statSignY.size()==sumStatSignUP1.size()) {
													for(unsigned int n=0; n<statSignY.size(); n++) {
														sumStatSignUP1.at(n) += statSignY.at(n);
													}
												}
											}
											//> for sub island match calculations
											if(label1.find("Excavated")!=string::npos) {
												for(int n=0; n<upLabels.getIsland(yLabel).numOfSubIslands(); n++) {
													String shape_name = upLabels.getIsland(yLabel).subIsland(n).shape_name();
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
														int smallArea = upLabels.getIsland(yLabel).subIsland(n).area();
														float bigArea = upLabels.totalArea();
														float relArea = smallArea/bigArea;
														float val = negArcScore;
														if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
															val = posArcScore;
														}
														float score = val * relArea;
														sumArcScoreUP1 += score;
													}
												}
											}
										}
										//> end UP initial calculations
									} //> end areaUP1>areaUP2
								}// end if countUP > 0
								if (countDB>0) {
									if(areaDB1<=areaDB2) {
										//DB initial calculations 1
										for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
											String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
											if(label1Condition) {
												for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
													String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
													String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
													if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
														//> for dot product calculations (REI only so far)
														vector<float> statSignY = dbLabels.getStatSign(yLabel);
														if(statSignY.size()>0 && statSignY.size()==sumStatSignDB1.size()) {
															for(unsigned int n=0; n<statSignY.size(); n++) {
																sumStatSignDB1.at(n) += statSignY.at(n);
															}
														}
														//> for sub island match calculations
														if(label1.find("Excavated")!=string::npos) {
															for(int n=0; n<dbLabels.getIsland(yLabel).numOfSubIslands(); n++) {
																String shape_name = dbLabels.getIsland(yLabel).subIsland(n).shape_name();
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	int smallArea = dbLabels.getIsland(yLabel).subIsland(n).area();
																	float bigArea = dbLabels.totalArea();
																	float relArea = smallArea/bigArea;
																	float val = negArcScore;
																	if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																		val = posArcScore;
																	}
																	float score = val * relArea;
																	sumArcScoreDB1 += score;
																}
															}
														}
													}// end if equationMap
												}//end for y srmDB
											}// end if label1Condition
											//> for dot product calculations (REI only so far)
											if(label2Condition) {
												vector<float> statSignX = dbLabels.getStatSign(xLabel);
												if(statSignX.size()>0 && statSignX.size()==sumStatSignDB2.size()) {
													for(unsigned int n=0; n<statSignX.size(); n++) {
														sumStatSignDB2.at(n) += statSignX.at(n);
													}
												}
											}
											//> for sub island match calculations
											if(label2.find("Excavated")!=string::npos) {
												for(int n=0; n<dbLabels.getIsland(xLabel).numOfSubIslands(); n++) {
													String shape_name = dbLabels.getIsland(xLabel).subIsland(n).shape_name();
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
														int smallArea = dbLabels.getIsland(xLabel).subIsland(n).area();
														float bigArea = dbLabels.totalArea();
														float relArea = smallArea/bigArea;
														float val = negArcScore;
														if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
															val = posArcScore;
														}
														float score = val * relArea;
														sumArcScoreDB2 += score;
													}
												}
											}
										}// end for x srmDB
										//> end DB initial calculation 1
									} // end areaDB1<=areaDB2
									if(areaDB1>areaDB2) {
										//DB initial calculations 2
										for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
											String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
											if(label2Condition) {
												for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
													String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
													String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
													if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
														//> for dot product calculations (REI only so far)
														vector<float> statSignX = dbLabels.getStatSign(xLabel);
														if(statSignX.size()>0 && statSignX.size()==sumStatSignDB2.size()) {
															for(unsigned int n=0; n<statSignX.size(); n++) {
																sumStatSignDB2.at(n) += statSignX.at(n);
															}
														}
														if(label2.find("Excavated")!=string::npos) {
															for(int n=0; n<dbLabels.getIsland(xLabel).numOfSubIslands(); n++) {
																String shape_name = dbLabels.getIsland(xLabel).subIsland(n).shape_name();
																if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
																	int smallArea = dbLabels.getIsland(xLabel).subIsland(n).area();
																	float bigArea = dbLabels.totalArea();
																	float relArea = smallArea/bigArea;
																	float val = negArcScore;
																	if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
																		val = posArcScore;
																	}
																	float score = val * relArea;
																	sumArcScoreDB2 += score;
																}
															}
														}
													}// end if equationMap
												}// end for x srmDB
											}// end if label2Condition
											//> for dot product calculations (REI only so far)
											if(label1Condition) {
												vector<float> statSignY = dbLabels.getStatSign(yLabel);
												if(statSignY.size()>0 && statSignY.size()==sumStatSignDB1.size()) {
													for(unsigned int n=0; n<statSignY.size(); n++) {
														sumStatSignDB1.at(n) += statSignY.at(n);
													}
												}
											}
											if(label1.find("Excavated")!=string::npos) {
												for(int n=0; n<dbLabels.getIsland(yLabel).numOfSubIslands(); n++) {
													String shape_name = dbLabels.getIsland(yLabel).subIsland(n).shape_name();
													if(shape_name.find("Disc")!=string::npos || shape_name.find("Blotch")!=string::npos || shape_name.find("Donut")!=string::npos) {
														int smallArea = dbLabels.getIsland(yLabel).subIsland(n).area();
														float bigArea = dbLabels.totalArea();
														float relArea = smallArea/bigArea;
														float val = negArcScore;
														if(shape_name.find("Disc")!=string::npos || shape_name.find("Donut")!=string::npos) {
															val = posArcScore;
														}
														float score = val * relArea;
														sumArcScoreDB1 += score;
													}
												}
											}
										}// end for y srmDB
										//> end DB initial calculation 2
									} // end if areaDB1>areaDB2
								}// end if countDB > 0
								//> StatSign using Dot Product (Scheme I) <//
								if(label1Condition) {
									dotProduct1 = statsign.dotProduct(sumStatSignUP1,sumStatSignDB1);
									adjustedDP1 = statsign.adjustValue(dotProduct1);
								}
								if(label2Condition) {
									dotProduct2 = statsign.dotProduct(sumStatSignUP2,sumStatSignDB2);
									adjustedDP2 = statsign.adjustValue(dotProduct2);
								}
								//> ************ End StatSign ************ <//
								weight1 = this->getShapeWeight(upMergedLabels.getShapeNum(label1),adjustedDP1);
								weight2 = this->getShapeWeight(upMergedLabels.getShapeNum(label2),adjustedDP2);
								deltaArcScore1 = this->calculateArcScore(sumArcScoreUP1,sumArcScoreDB1);
								deltaArcScore2 = this->calculateArcScore(sumArcScoreUP2,sumArcScoreDB2);
							}
							for(int m=0; m<this->relOpLevelSize; m++) {
								// Y <OP> X
								if (countUP>0) {
									if(areaUP1<=areaUP2) {
										/*! for UP */
										for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
											try {
												float totalDenomAreaUP=0.;
												String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
												int areaX = upLabels.area(xLabel); // get area
												int xIdx = srmUP.getIndex(xLabel); // get pos from srm
												for(unsigned int yIdx=0; yIdx<srmUP.size(); yIdx++) {
													try {
														unsigned int rel_op_idx = srmUP.relation(yIdx,xIdx);
														//for all relations
														if(rel_op_idx>NONE && rel_op_idx!=SURR_BY_INV) {
															totalCountUP++; //total relations for areaUP2
															totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).first;
														}
													} catch (const std::out_of_range &oor) {
														printf("ShadeShapeRelationMatch::entropy() out of range!\n");
														printf("INDIR: UP 1a\n");
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
														float penaltyY = 1.0;
														if(upLabels.isShapeShifted(yLabel)) {
															int shapeNumY = upLabels.getShapeNum(yLabel);
															int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
															penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
															penalizedY = penaltyY;
														}
														areaY += upLabels.area(yLabel) * penaltyY;

														//> for match calculations
														sumIslandAreaUP += upLabels.area(yLabel);
														maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(yLabel));

														//> Expected value of Shade Gradient (ESG) <//
														Esg esg;
														int shadeDiff = esg.shadeDiff(upLabels.getShade(yLabel),upLabels.getShade(xLabel));
														int index1 = srmUP.getIndex(yLabel);
														int index2 = srmUP.getIndex(xLabel);
														float dist = srmUP.getRelationDistance(index1,index2,-2);
														esg.calculate(dist,shadeDiff);
														esg.esgVal = max(0.0f,esg.esgVal);
														float area1 = upLabels.area(yLabel);
														float area2 = upLabels.area(xLabel);
														float relArea = area1 + area2;
														sumOfAreaUP += relArea;
														float ctWt = this->contrastWeight(esg.esgVal,relArea);
														ctWt *= (4.0/24.0); // experimental
														if(std::isnan(ctWt)) ctWt=0.0;
														contrastWeightUP += ctWt;
													}// end if equationMap
												}// end for y srmUP

												float penaltyX = 1.0;
												if(upLabels.isShapeShifted(xLabel)) {
													int shapeNumX = upLabels.getShapeNum(xLabel);
													int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
													penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
													penalizedX = penaltyX;
												}
												areaUP += (areaY * weight1 * deltaArcScore1) + (areaX * penaltyX * weight2 * deltaArcScore2 *(areaY/totalDenomAreaUP));
											} catch (const std::out_of_range &oor) {
												printf("ShadeShapeRelationMatch::entropy() out of range!\n");
												printf("INDIR: UP 1b\n");
												printf("x: %d\n",x);
												exit(1);
											}
										} // end for srmUP
									} // end if areaUP1 <= areaUP2
									// X <OP> Y
									else if(areaUP1>areaUP2) {
										/*! for srmUP */
										for(unsigned int y=0; y<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
											try {
												float totalDenomAreaUP=0.;
												String yLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
												int yIdx = srmUP.getIndex(yLabel);
												int areaY = upLabels.area(yLabel);
												for(unsigned int xIdx=0; xIdx<srmUP.size(); xIdx++) {
													unsigned int rel_op_idx = srmUP.relation(yIdx,xIdx);
													//for all relations
													if(rel_op_idx>NONE && rel_op_idx!=SURR_BY) {
														totalCountUP++; //total relations for areaUP1
														totalDenomAreaUP += srmUP.relationArea(yIdx,xIdx).second;
													}
												}
												int areaX=0;
												for(unsigned int x=0; x<srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
													String xLabel = srmUP.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
													String equationKey = yLabel+"<"+relOp+">"+xLabel;
													if(srmUP.equationMap.find(equationKey)!=srmUP.equationMap.end()) {
														float penaltyX = 1.0;
														if(upLabels.isShapeShifted(xLabel)) {
															int shapeNumX = upLabels.getShapeNum(xLabel);
															int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
															penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
															penalizedX = penaltyX;
														}
														areaX += upLabels.area(xLabel) * penaltyX;

														//> for match calculations
														sumIslandAreaUP += upLabels.area(xLabel);
														maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(xLabel));

														//> Expected value of Shade Gradient (ESG) <//
														Esg esg;
														int shadeDiff = esg.shadeDiff(upLabels.getShade(yLabel),upLabels.getShade(xLabel));
														int index1 = srmUP.getIndex(yLabel);
														int index2 = srmUP.getIndex(xLabel);
														float dist = srmUP.getRelationDistance(index1,index2,-2);
														esg.calculate(dist,shadeDiff);
														esg.esgVal = max(0.0f,esg.esgVal);
														float area1 = upLabels.area(yLabel);
														float area2 = upLabels.area(xLabel);
														float relArea = area1 + area2;
														sumOfAreaUP += relArea;
														float ctWt = this->contrastWeight(esg.esgVal,relArea);
														ctWt *= (4.0/24.0);
														if(std::isnan(ctWt)) ctWt=0.0;
														contrastWeightUP += ctWt;
													}// end if equationMap
												}// end for x srmUP

												float penaltyY = 1.0;
												if(upLabels.isShapeShifted(yLabel)) {
													int shapeNumY = upLabels.getShapeNum(yLabel);
													int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
													penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
													penalizedY = penaltyY;
												}
												areaUP += (areaX *weight2 * deltaArcScore2) + (areaY * penaltyY * weight1 * deltaArcScore1 *(areaX/totalDenomAreaUP));
											} catch (const std::out_of_range &oor) {
												printf("ShadeShapeRelationMatch::entropy() out of range!\n");
												printf("INDIR: UP 2\n");
												exit(1);
											}
										} // end for srmUP
									} // end else if areaUP1 > areaUP2
								}// end if countUP > 0
								if (countDB>0) {
									if(areaDB1<=areaDB2) {
										/*! for DB */
										for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
											try {
												float totalDenomAreaDB=0.;
												String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
												int areaX = dbLabels.area(xLabel);
												int xIdx = srmDB.getIndex(xLabel);
												for(unsigned int yIdx=0; yIdx<srmDB.size(); yIdx++) {
													unsigned int rel_op_idx = srmDB.relation(yIdx,xIdx);
													//for all relations
													if(rel_op_idx>NONE && rel_op_idx!=SURR_BY_INV) {
														totalCountDB++; //total relations for areaDB1
														totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).second;
													}
												}
												int areaY=0;
												for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
													String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
													String equationKeyDB = yLabel+"<"+relOp+">"+xLabel;
													if(srmDB.equationMap.find(equationKeyDB)!=srmDB.equationMap.end()) {
														areaY += dbLabels.area(yLabel);

														//>for match calculations
														sumIslandAreaDB += dbLabels.area(yLabel);
														maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(yLabel));

														//> Expected value of Shade Gradient (ESG) <//
														Esg esg;
														int shadeDiff = esg.shadeDiff(dbLabels.getShade(yLabel),dbLabels.getShade(xLabel));
														int index1 = srmDB.getIndex(yLabel);
														int index2 = srmDB.getIndex(xLabel);
														float dist = srmDB.getRelationDistance(index1,index2,-2);
														esg.calculate(dist,shadeDiff);
														esg.esgVal = max(0.0f,esg.esgVal);
														float area1 = dbLabels.area(yLabel);
														float area2 = dbLabels.area(xLabel);
														float relArea = area1 + area2;
														sumOfAreaDB += relArea;
														float ctWt = this->contrastWeight(esg.esgVal,relArea);
														ctWt *= (4.0/24.0);
														if(std::isnan(ctWt)) ctWt=0.0;
														contrastWeightDB += ctWt;
													}// end if equationMap
												}// end for y srmDB

												areaDB += (areaY * weight1 * deltaArcScore1) + (areaX * weight2 * deltaArcScore2 *(areaY/totalDenomAreaDB));
											} catch (const std::out_of_range &oor) {
												printf("ShadeShapeRelationMatch::entropy() out of range!\n");
												printf("INDIR: DB 1\n");
												exit(1);
											}
										} // end for srmDB
									} // end else if areaDB1 <= areaDB2
									else if(areaDB1>areaDB2) {
										/*! for DB */
										for(unsigned int y=0; y<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.size(); y++) {
											try {
												float totalDenomAreaDB=0.;
												String yLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).first.at(y);
												int yIdx = srmDB.getIndex(yLabel);
												int areaY = dbLabels.area(yLabel);
												for(unsigned int xIdx=0; xIdx<srmDB.size(); xIdx++) {
													unsigned int rel_op_idx = srmDB.relation(yIdx,xIdx);
													//for all relations
													if(rel_op_idx>NONE && rel_op_idx!=SURR_BY) {
														totalCountDB++; //total relations for areaUP1
														totalDenomAreaDB += srmDB.relationArea(yIdx,xIdx).second;
													}
												}
												int areaX=0;
												for(unsigned int x=0; x<srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.size(); x++) {
													String xLabel = srmDB.mergedLabelContainer.at(i).at(j).at(k).at(m).second.at(x);
													String equationKey = yLabel+"<"+relOp+">"+xLabel;
													if(srmDB.equationMap.find(equationKey)!=srmDB.equationMap.end()) {
														areaX += dbLabels.area(xLabel);

														//>for match calculations
														sumIslandAreaDB += dbLabels.area(xLabel);
														maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(xLabel));

														//> Expected value of Shade Gradient (ESG) <//
														Esg esg;
														int shadeDiff = esg.shadeDiff(dbLabels.getShade(yLabel),dbLabels.getShade(xLabel));
														int index1 = srmDB.getIndex(yLabel);
														int index2 = srmDB.getIndex(xLabel);
														float dist = srmDB.getRelationDistance(index1,index2,-2);
														esg.calculate(dist,shadeDiff);
														esg.esgVal = max(0.0f,esg.esgVal);
														float area1 = dbLabels.area(yLabel);
														float area2 = dbLabels.area(xLabel);
														float relArea = area1 + area2;
														sumOfAreaDB += relArea;
														float ctWt = this->contrastWeight(esg.esgVal,relArea);
														ctWt *= (4.0/24.0);
														if(std::isnan(ctWt)) ctWt=0.0;
														contrastWeightDB += ctWt;
													}// end if equationMap
												}// end for x srmDB

												areaDB += (areaX * weight2 * deltaArcScore2) + (areaY * weight1 * deltaArcScore1 *(areaX/totalDenomAreaDB));
											} catch (const std::out_of_range &oor) {
												printf("ShadeShapeRelationMatch::entropy() out of range!\n");
												printf("INDIR: DB 2\n");
												exit(1);
											}
										} // end for srmDB
									} // end else if areaDB1 > areaDB2
								}// end if countDB > 0
							} // end for m

							areaUP = areaUP * this->rVal[maxNeighborLevelUP];
							areaDB = areaDB * this->rVal[maxNeighborLevelDB];
							//> E-Value using count base on relArea <//
							float countProportionUP = sumIslandAreaUP/maxIslandAreaUP;
							float countProportionDB = sumIslandAreaDB/maxIslandAreaDB;
							if(std::isnan(countProportionUP)) countProportionUP = 0.0;
							if(std::isnan(countProportionDB)) countProportionDB = 0.0;
							float entropyUP = this->entropy(countProportionUP);
							float entropyDB = this->entropy(countProportionDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							//> ********* End E-Value ************ <//
							//> Expected value of Shade Gradient (ESG) <//
							contrastWeightUP /= sumOfAreaUP;
							contrastWeightDB /= sumOfAreaDB;
							if(std::isnan(contrastWeightUP)) contrastWeightUP = 0.0;
							if(std::isnan(contrastWeightDB)) contrastWeightDB = 0.0;
							float contrastWeight = min(contrastWeightUP,contrastWeightDB);

							if(std::isnan(areaUP)) areaUP=0;
							if(std::isnan(areaDB)) areaDB=0;
							float areaVal = min(areaUP,areaDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal * contrastWeight;
							totalMatchScore += weightedEntropy;

							float weightedMismatchEntropy  = 0.0;
							if(countUP>0 && countDB==0) {
								weightedMismatchEntropy = contrastWeightUP * (areaUP / upMergedLabels.totalArea());
							}
							if(countDB>0 && countUP==0) {
								weightedMismatchEntropy = contrastWeightDB * (areaDB / dbMergedLabels.totalArea());
							}
							totalMismatchScore += weightedMismatchEntropy;

							if(weightedEntropy>0) {
								fprintf(fp,"[%s][%s][%s]\n", label1.c_str(),relOp.c_str(),label2.c_str());
								fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp,"EntropyVal: %f\n",entropyVal);
								fprintf(fp,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp,"AreaUP: %f, AreaDB: %f\n",areaUP,areaDB);
								fprintf(fp,"PenaltyY: %f, PenaltyX: %f\n",penalizedY,penalizedX);
								fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp,"RelArea: %f\n",relArea);
								fprintf(fp,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp,"DeltaArcScore1: %f, DeltaArcScore2: %f\n", deltaArcScore1,deltaArcScore2);
								fprintf(fp,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalMatchScore: %f\n",totalMatchScore);
								fprintf(fp,"----------------------------------\n");
							}
							if(weightedMismatchEntropy>0) {
								fprintf(fp2,"[%s][%s][%s]\n", label1.c_str(),relOp.c_str(),label2.c_str());
								fprintf(fp2,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp2,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp2,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp2,"EntropyVal: %f\n",entropyVal);
								fprintf(fp2,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp2,"AreaUP: %f, AreaDB: %f\n",areaUP,areaDB);
								fprintf(fp2,"PenaltyY: %f, PenaltyX: %f\n",penalizedY,penalizedX);
								fprintf(fp2,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp2,"RelArea: %f\n",relArea);
								fprintf(fp2,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp2,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp2,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp2,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp2,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp2,"DeltaArcScore1: %f, DeltaArcScore2: %f\n", deltaArcScore1,deltaArcScore2);
								fprintf(fp2,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp2,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp2,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp2,"WeightedMismatchEntropy: %f\n",weightedMismatchEntropy);
								fprintf(fp2,"TotalMismatchScore: %f\n",totalMismatchScore);
								fprintf(fp2,"----------------------------------\n");
							}
							if(weightedEntropy<=0 || std::isnan(weightedEntropy)) {
								fprintf(fp3,"[%s][%s][%s]\n", label1.c_str(),relOp.c_str(),label2.c_str());
								fprintf(fp3,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
								fprintf(fp3,"TotalCountUP: %d, TotalCountDB: %d\n",totalCountUP,totalCountDB);
								fprintf(fp3,"RelativeCountUP: %f, RelativeCountDB: %f\n",countProportionUP,countProportionDB);
								fprintf(fp3,"EntropyVal: %f\n",entropyVal);
								fprintf(fp3,"AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
								fprintf(fp3,"AreaUP: %f, AreaDB: %f\n",areaUP,areaDB);
								fprintf(fp3,"PenaltyY: %f, PenaltyX: %f\n",penalizedY,penalizedX);
								fprintf(fp3,"MaxTotalArea: %d\n",maxTotalArea);
								fprintf(fp3,"RelArea: %f\n",relArea);
								fprintf(fp3,"ContrastWeightUP: %f, ContrastWeightDB: %f\n",contrastWeightUP,contrastWeightDB);
								fprintf(fp3,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp3,"SumOfAreaUP: %f, SumOfAreaDB: %f\n",sumOfAreaUP,sumOfAreaDB);
								fprintf(fp3,"ArcScoreUP1: %f, ArcScoreDB1: %f\n",sumArcScoreUP1,sumArcScoreDB1);
								fprintf(fp3,"ArcScoreUP2: %f, ArcScoreDB2: %f\n",sumArcScoreUP2,sumArcScoreDB2);
								fprintf(fp3,"DeltaArcScore1: %f, DeltaArcScore2: %f\n", deltaArcScore1,deltaArcScore2);
								fprintf(fp3,"DotProduct1: %f, DotProduct2: %f\n",dotProduct1,dotProduct2);
								fprintf(fp3,"AdjustedDP1: %f, AdjustedDP2: %f\n",adjustedDP1,adjustedDP2);
								fprintf(fp3,"WeightY: %f, WeightX: %f\n",weight1,weight2);
								fprintf(fp3,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp3,"TotalMatchScore: %f\n",totalMatchScore);
								fprintf(fp3,"----------------------------------\n");
							}
						} // end if countUP || countDB
					} // end else INDIR
				} // end else DIR/INDIR
			} // end for loop k
		} // end for loop j
	} // end for loop i
	fclose(fp);
	fclose(fp2);

	this->matchScore = totalMatchScore;
	this->mismatchScore = totalMismatchScore;
}


/*************************** PUBLIC FUNCTIONS ***********************************/

void ShadeShapeRelationMatch::srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels, String nStr) {
	ssrUP.get_srm().downScaleSrm();
	ssrDB.get_srm().downScaleSrm();
	this->match(ssrUP,ssrDB,nStr);
	//float matchVal = this->srm_match(srmPairUP,upMergedLabels,srmPairDB,dbMergedLabels);
	//this->writeDownScaleSrm(ssrUP.name(),srmPairUP,upMergedLabels,upLabels.totalArea());
	//this->writeDownScaleSrm(ssrDB.name(),srmPairDB,dbMergedLabels,dbLabels.totalArea());
	//this->printDownScaleSrm(srm3dUP,upMergedLabels);
	//cout << "-----------------------" << endl;
	//this->printDownScaleSrm(srm3dDB,dbMergedLabels);
}

//! run srm_match() before this
float ShadeShapeRelationMatch::getMatchScore() {
	return this->matchScore;
}

//! run srm_match() before this
float ShadeShapeRelationMatch::getMismatchScore() {
	return this->mismatchScore;
}

//! return shape weights
float ShadeShapeRelationMatch::getShapeWeight(int shape, float prop) {
	if(shape!=5)  { //> 5=excavated
		return this->shapeWeight[shape];
	}
	float shapeWt = this->shapeWeight[shape];
	float weight = (0.13 + (0.87 * prop)) * shapeWt;
	return weight;
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
