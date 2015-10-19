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

/**************************** PRIVATE FUNCTIONS ********************************/

float ShadeShapeRelationMatch::entropy(float count) {
	if(count==0) {
		return -1.0;
	} else {
		return log2(count);
	}
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
	ShapeMatch shapematch;
	FILE * fp;
	String filename = ssrUP.name() + "_entropy_output.txt";
	fp = fopen(filename.c_str(),"w");
	float totalEntropy = 0.0;
	int maxTotalArea = max(upMergedLabels.totalArea(),dbMergedLabels.totalArea());

	const float alpha = 3.2;
	// for loop i: labels for the y axis
	for(unsigned int i=0; i<srmCountUP.size(); i++) {
		String label1 = upMergedLabels.at(i);
		// for loop j: labels for x axis
		for(unsigned int j=0; j<srmCountUP.at(i).size(); j++) {
			String label2 = upMergedLabels.at(j);
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
						//> used for entropy calculation
						int maxIslandAreaUP=0, maxIslandAreaDB=0;
						float sumIslandAreaUP=0.0, sumIslandAreaDB=0.0;
						if(countUP>0 || countDB>0) {
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

												//> for entropy calculations
												sumIslandAreaUP += upLabels.area(yLabel);
												maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(yLabel));
											}
										}
										float penaltyX = 1.0;
										if(upLabels.isShapeShifted(xLabel)) {
											int shapeNumX = upLabels.getShapeNum(xLabel);
											int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
											penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
											penalizedX = penaltyX;
										}
										areaUP += areaY + (areaX * penaltyX * (areaY/totalDenomAreaUP));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("SURR_BY: UP 1b\n");
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

												//> used for entropy calculations
												sumIslandAreaDB += dbLabels.area(yLabel);
												maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(yLabel));
											}
										}
										areaDB += areaY + (areaX * (areaY/totalDenomAreaDB));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("SURR_BY: DB\n");
										exit(1);
									}
								}
								areaValUP = areaUP * pow(this->rVal[maxNeighborLevelUP],m);
								areaValDB = areaDB * pow(this->rVal[maxNeighborLevelDB],m);
							}
							if(k==SURR_BY_INV) {
								// for UP
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
												/*if(label1=="0_Strip_s4" && label2=="1_Default_s2" && m==1) {
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
											}
										}
										float penaltyY = 1.0;
										if(upLabels.isShapeShifted(yLabel)) {
											int shapeNumY = upLabels.getShapeNum(yLabel);
											int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
											penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
											penalizedY = penaltyY;
										}
										areaUP += areaX + (areaY * penaltyY * (areaX/totalDenomAreaUP));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("SURR_BY_INV: UP\n");
										exit(1);
									}
								}
								// for DB
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

												//> for entropy calculations
												sumIslandAreaDB += dbLabels.area(xLabel);
												maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(xLabel));
											}
										}
										areaDB += areaX + (areaY * (areaX/totalDenomAreaDB));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("SURR_BY_INV: DB\n");
										exit(1);
									}
								}
								areaValUP = areaUP * pow(this->rVal[maxNeighborLevelUP],m);
								areaValDB = areaDB * pow(this->rVal[maxNeighborLevelDB],m);
							}
							float countProportionUP = sumIslandAreaUP/maxIslandAreaUP;
							float countProportionDB = sumIslandAreaDB/maxIslandAreaDB;
							if(std::isnan(countProportionUP)) countProportionUP = 0.0;
							if(std::isnan(countProportionDB)) countProportionDB = 0.0;
							float entropyUP = this->entropy(countProportionUP);
							float entropyDB = this->entropy(countProportionDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float distAvgUP = std::accumulate(srmUP.mergedRelationDistance.at(i).at(j).at(k).begin(),srmUP.mergedRelationDistance.at(i).at(j).at(k).end(),0.0);
							distAvgUP /= srmUP.mergedRelationDistance.at(i).at(j).at(k).size();
							float distAvgDB = std::accumulate(srmDB.mergedRelationDistance.at(i).at(j).at(k).begin(),srmDB.mergedRelationDistance.at(i).at(j).at(k).end(),0.0);
							distAvgDB /= srmDB.mergedRelationDistance.at(i).at(j).at(k).size();
							//> Expected value of Shade Gradient <//
							int shadeDiffUP = abs(upMergedLabels.getShadeLevel(label1)-upMergedLabels.getShadeLevel(label2));
							int shadeDiffDB = abs(dbMergedLabels.getShadeLevel(label1)-dbMergedLabels.getShadeLevel(label2));
							float esgUP = shadeDiffUP / (distAvgUP + 1.0);
							float esgDB = shadeDiffDB / (distAvgDB + 1.0);
							float contrastWeightUP = pow(alpha,esgUP);
							float contrastWeightDB = pow(alpha,esgDB);
							if(std::isnan(contrastWeightUP)) contrastWeightUP = 1.0;
							if(std::isnan(contrastWeightDB)) contrastWeightDB = 1.0;
							float contrastWeight = min(contrastWeightUP,contrastWeightDB);
							if(std::isnan(areaValUP)) areaValUP=0;
							if(std::isnan(areaValDB)) areaValDB=0;
							float areaVal = min(areaValUP,areaValDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal * contrastWeight;
							totalEntropy += weightedEntropy;
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
								fprintf(fp,"DistAvgUP: %f, DistAvgDB: %f\n",distAvgUP,distAvgDB);
								fprintf(fp,"esgUP: %f, esgDB: %f\n",esgUP,esgDB);
								fprintf(fp,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
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
						if(countUP>0 || countDB>0) {
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

												//> for entropy calculations
												sumIslandAreaUP += upLabels.area(yLabel);
												maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(yLabel));
											}
										}
										float penaltyX = 1.0;
										if(upLabels.isShapeShifted(xLabel)) {
											int shapeNumX = upLabels.getShapeNum(xLabel);
											int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
											penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
											penalizedX = penaltyX;
										}
										areaUP += areaY + (areaX * penaltyX * (areaY/totalDenomAreaUP));
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("DIR: UP 1b\n");
										printf("x: %d\n",x);
										exit(1);
									}
								} // end for UP
							} // end if areaUP1 <= areaUP2
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

												//> for entropy calculations
												sumIslandAreaUP += upLabels.area(xLabel);
												maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(xLabel));
											}
										}
										float penaltyY = 1.0;
										if(upLabels.isShapeShifted(yLabel)) {
											int shapeNumY = upLabels.getShapeNum(yLabel);
											int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
											penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
											penalizedY = penaltyY;
										}
										areaUP += areaX + (areaY * penaltyY * (areaX/totalDenomAreaUP));
										areaUP = areaUP * this->rVal[maxNeighborLevelUP];
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("DIR: UP 2\n");
										exit(1);
									}
								} // end for UP
							} // end else if areaUP1 > areaUP2
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

												//>for entropy calculations
												sumIslandAreaDB += dbLabels.area(yLabel);
												maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(yLabel));
											}
										}
										areaDB += areaY + (areaX * (areaY/totalDenomAreaDB));
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

												//>for entropy calculations
												sumIslandAreaDB += dbLabels.area(xLabel);
												maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(xLabel));
											}
										}
										areaDB += areaX + (areaY * (areaX/totalDenomAreaDB));
										areaDB = areaDB * this->rVal[maxNeighborLevelDB];
									} catch (const std::out_of_range &oor) {
										printf("ShadeShapeRelationMatch::entropy() out of range!\n");
										printf("DIR: DB 2\n");
										exit(1);
									}
								} // end for DB
							} // end else if areaDB1 > areaDB2
							float countProportionUP = sumIslandAreaUP/maxIslandAreaUP;
							float countProportionDB = sumIslandAreaDB/maxIslandAreaDB;
							if(std::isnan(countProportionUP)) countProportionUP = 0.0;
							if(std::isnan(countProportionDB)) countProportionDB = 0.0;
							float entropyUP = this->entropy(countProportionUP);
							float entropyDB = this->entropy(countProportionDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float distAvgUP = std::accumulate(srmUP.mergedRelationDistance.at(i).at(j).at(k).begin(),srmUP.mergedRelationDistance.at(i).at(j).at(k).end(),0.0);
							distAvgUP /= srmUP.mergedRelationDistance.at(i).at(j).at(k).size();
							float distAvgDB = std::accumulate(srmDB.mergedRelationDistance.at(i).at(j).at(k).begin(),srmDB.mergedRelationDistance.at(i).at(j).at(k).end(),0.0);
							distAvgDB /= srmDB.mergedRelationDistance.at(i).at(j).at(k).size();
							//> Expected value of Shade Gradient <//
							int shadeDiffUP = abs(upMergedLabels.getShadeLevel(label1)-upMergedLabels.getShadeLevel(label2));
							int shadeDiffDB = abs(dbMergedLabels.getShadeLevel(label1)-dbMergedLabels.getShadeLevel(label2));
							float esgUP = shadeDiffUP / (distAvgUP + 1.0);
							float esgDB = shadeDiffDB / (distAvgDB + 1.0);
							float contrastWeightUP = pow(alpha,esgUP);
							float contrastWeightDB = pow(alpha,esgDB);
							if(std::isnan(contrastWeightUP)) contrastWeightUP = 1.0;
							if(std::isnan(contrastWeightDB)) contrastWeightDB = 1.0;
							float contrastWeight = min(contrastWeightUP,contrastWeightDB);
							if(std::isnan(areaUP)) areaUP=0;
							if(std::isnan(areaDB)) areaDB=0;
							float areaVal = min(areaUP,areaDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal * contrastWeight;
							totalEntropy += weightedEntropy;
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
								fprintf(fp,"DistAvgUP: %f, DistAvgDB: %f\n",distAvgUP,distAvgDB);
								fprintf(fp,"esgUP: %f, esgDB: %f\n",esgUP,esgDB);
								fprintf(fp,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
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
						float sumIslandAreaUP=0.0, sumIslandAreaDB = 0.0;
						if(countUP>0 || countDB>0) {
							if(areaUP1<=areaUP2) {
								for (int m=0; m<this->relOpLevelSize; m++) {
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

													//> for entropy calculations
													sumIslandAreaUP += upLabels.area(yLabel);
													maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(yLabel));
												}
											}
											float penaltyX = 1.0;
											if(upLabels.isShapeShifted(xLabel)) {
												int shapeNumX = upLabels.getShapeNum(xLabel);
												int prevShapeNumX = upLabels.getPrevShapeNum(xLabel);
												penaltyX = shapematch.getShiftPenalty(prevShapeNumX,shapeNumX);
												penalizedX = penaltyX;
											}
											areaUP += areaY + (areaX * penaltyX * (areaY/totalDenomAreaUP));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("INDIR: UP 1b\n");
											printf("x: %d\n",x);
											exit(1);
										}
									} // end for srmUP
								} //  for m
							} // end if areaUP1 <= areaUP2
							else if(areaUP1>areaUP2) {
								for (int m=0; m<this->relOpLevelSize; ++m) {
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
												/*if(label1=="0_Strip_s4" && label2=="1_Default_s2" && m==1) {
														printf("yIdx: %d, %s\n",yIdx,yLabel.c_str());
														printf("xIdx: %d, %s\n",xIdx,upLabels.at(xIdx).c_str());
														printf("rel_op_idx: %d\n",rel_op_idx);
														printf("srmRelationArea: %d\n",srmUP.relationArea(yIdx,xIdx).second);
														printf("totalDenomAreaUP: %f\n",totalDenomAreaUP);

													}*/
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

													//> for entropy calculations
													sumIslandAreaUP += upLabels.area(xLabel);
													maxIslandAreaUP = max(maxIslandAreaUP,upLabels.area(xLabel));
												}
											}
											float penaltyY = 1.0;
											if(upLabels.isShapeShifted(yLabel)) {
												int shapeNumY = upLabels.getShapeNum(yLabel);
												int prevShapeNumY = upLabels.getPrevShapeNum(yLabel);
												penaltyY = shapematch.getShiftPenalty(prevShapeNumY,shapeNumY);
												penalizedY = penaltyY;
											}
											areaUP += areaX + (areaY * penaltyY * (areaX/totalDenomAreaUP));
											areaUP = areaUP * this->rVal[maxNeighborLevelUP];
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("INDIR: UP 2\n");
											exit(1);
										}
									} // end for srmUP
								} // for m
							} // end else if areaUP1 > areaUP2
							if(areaDB1<=areaDB2) {
								for (int m=0; m<this->relOpLevelSize; m++) {
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

													//>for entropy calculations
													sumIslandAreaDB += dbLabels.area(yLabel);
													maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(yLabel));
												}
											}
											areaDB += areaY + (areaX * (areaY/totalDenomAreaDB));
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("INDIR: DB 1\n");
											exit(1);
										}
									} // end for srmDB
								} // end for m
							} // end else if areaDB1 <= areaDB2
							else if(areaDB1>areaDB2) {
								for (int m=0; m<this->relOpLevelSize; m++) {
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

													//>for entropy calculations
													sumIslandAreaDB += dbLabels.area(xLabel);
													maxIslandAreaDB = max(maxIslandAreaDB,dbLabels.area(xLabel));
												}
											}
											areaDB += areaX + (areaY * (areaX/totalDenomAreaDB));
											areaDB = areaDB * this->rVal[maxNeighborLevelDB];
										} catch (const std::out_of_range &oor) {
											printf("ShadeShapeRelationMatch::entropy() out of range!\n");
											printf("INDIR: DB 2\n");
											exit(1);
										}
									} // end for DB
								} // end for m
							} // end else if areaDB1 > areaDB2
							float countProportionUP = sumIslandAreaUP/maxIslandAreaUP;
							float countProportionDB = sumIslandAreaDB/maxIslandAreaDB;
							if(std::isnan(countProportionUP)) countProportionUP = 0.0;
							if(std::isnan(countProportionDB)) countProportionDB = 0.0;
							float entropyUP = this->entropy(countProportionUP);
							float entropyDB = this->entropy(countProportionDB);
							float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
							float distAvgUP = std::accumulate(srmUP.mergedRelationDistance.at(i).at(j).at(k).begin(),srmUP.mergedRelationDistance.at(i).at(j).at(k).end(),0.0);
							distAvgUP /= srmUP.mergedRelationDistance.at(i).at(j).at(k).size();
							float distAvgDB = std::accumulate(srmDB.mergedRelationDistance.at(i).at(j).at(k).begin(),srmDB.mergedRelationDistance.at(i).at(j).at(k).end(),0.0);
							distAvgDB /= srmDB.mergedRelationDistance.at(i).at(j).at(k).size();
							//> Expected value of Shade Gradient <//
							int shadeDiffUP = abs(upMergedLabels.getShadeLevel(label1)-upMergedLabels.getShadeLevel(label2));
							int shadeDiffDB = abs(dbMergedLabels.getShadeLevel(label1)-dbMergedLabels.getShadeLevel(label2));
							float esgUP = shadeDiffUP / (distAvgUP + 1.0);
							float esgDB = shadeDiffDB / (distAvgDB + 1.0);
							float contrastWeightUP = pow(alpha,esgUP);
							float contrastWeightDB = pow(alpha,esgDB);
							if(std::isnan(contrastWeightUP)) contrastWeightUP = 1.0;
							if(std::isnan(contrastWeightDB)) contrastWeightDB = 1.0;
							float contrastWeight = min(contrastWeightUP,contrastWeightDB);
							if(std::isnan(areaUP)) areaUP=0;
							if(std::isnan(areaDB)) areaDB=0;
							float areaVal = min(areaUP,areaDB);
							float relArea = areaVal / maxTotalArea;
							float weightedEntropy = relArea * entropyVal * contrastWeight;
							totalEntropy += weightedEntropy;
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
								fprintf(fp,"DistAvgUP: %f, DistAvgDB: %f\n",distAvgUP,distAvgDB);
								fprintf(fp,"esgUP: %f, esgDB: %f\n",esgUP,esgDB);
								fprintf(fp,"ContrastWeight: %f\n",contrastWeight);
								fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
								fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
								fprintf(fp,"----------------------------------\n");
							}
						} // end if countUP || countDB
					} // end else INDIR
				} // end else DIR/INDIR
			} // end for loop k
		} // end for loop j
	} // end for loop i
	fclose(fp);

	return totalEntropy;
}


/*************************** PUBLIC FUNCTIONS ***********************************/

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
