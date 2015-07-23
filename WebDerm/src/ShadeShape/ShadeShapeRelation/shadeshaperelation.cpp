/*
 * shadeshaperelation.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: jason
 */

#include "shadeshaperelation.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ShadeShape/shadeshape.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"

/******************** PRIVATE FUNCTIONS **********************/

void ShadeShapeRelation::setup_relationMatrix(map<String,float> &labels) {
	this->relationMatrix.clear();
	this->relationMatrix.resize(labels.size(),vector<String>(labels.size(),"NULL"));
	this->markedMap.clear();
	this->markedMap.resize(labels.size(),vector<int>(labels.size(),0));
	this->relationCount.clear();
	this->relationCount.resize(labels.size(),vector<int>(labels.size(),0));
}

/******************** PUBLIC FUNCTIONS ***********************/

void ShadeShapeRelation::spatial_relation(ShadeShape &ss, map<String,float> &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->setup_relationMatrix(labels);
	const int visibilityThresh = 3;
	const int surroundedThreshUpper = 14;
	const int surroundedThreshLower = 3;
	const float directNeighborDistThresh = 5.0;
	int minWidthForVisibility = 0;
	Point beginCoords, endCoords;
	for(unsigned int shape1=0; shape1<islandVec.size(); shape1++) {
		for(unsigned int shade1=0; shade1<islandVec.at(shape1).size(); shade1++) {
			for(unsigned int num1=0; num1<islandVec.at(shape1).at(shade1).size(); num1++) {
				Islands isl1 = islandVec.at(shape1).at(shade1).at(num1);
				String label1 = isl1.labelName();
				int index1 = distance(labels.begin(),labels.find(label1));
				Point center = isl1.centerOfMass();
				String coords1 = toString(center.x)+","+toString(center.y);
				vector<vector<int> > neighborNumVec(labels.size(),vector<int>(labels.size(),0));
				vector<vector<float> > neighborDistVec(labels.size(),vector<float>(labels.size(),140.0));
				//printf("%s: %s\n",label1.c_str(),coords1.c_str());
				for(double theta=0.0; theta<360.0; theta+=15.0) {
					vector<int> vecWidth(labels.size(),0);
					int neighborNum = 0;
					int row=center.y, col=center.x;
					float fRow = center.y, fCol = center.x;
					double deg = theta * M_PI / 180.0; //convert to degrees
					int insideIsland1 = isl1.coordinates().find(coords1)!=isl1.coordinates().end() ? ENTERED : OUTSIDE;
					while(row<isl1.image().rows && col<isl1.image().cols && row>=0 && col>=0) {
						String coords2 = toString(col)+","+toString(row);
						if(insideIsland1==OUTSIDE) {
							if(isl1.coordinates().find(coords2)!=isl1.coordinates().end()) {
								insideIsland1 = ENTERED;
							}
						} else if(insideIsland1==ENTERED) {
							if(isl1.coordinates().find(coords2)!=isl1.coordinates().end()) {
								vecWidth.at(index1)++;
								minWidthForVisibility = min(vecWidth.at(index1),visibilityThresh);
								beginCoords = Point(col,row);
							}
							else {
								insideIsland1 = EXITED;
							}
						} else if(insideIsland1==EXITED) {
							for(unsigned int shape2=0; shape2<islandVec.size(); shape2++) {
								for(unsigned int shade2=0; shade2<islandVec.at(shape2).size(); shade2++) {
									for(unsigned int num2=0; num2<islandVec.at(shape2).at(shade2).size(); num2++) {
										if(shape1!=shape2 || shade1!=shade2 || num1!=num2) { //checks so island1 != island2
											Islands isl2 = islandVec.at(shape2).at(shade2).at(num2);
											String label2 = isl2.labelName();
											int insideIsland2 = OUTSIDE;
											int index2 =  distance(labels.begin(),labels.find(label2));
											if(markedMap.at(index1).at(index2)==0) {
												if(isl2.coordinates().find(coords2)!=isl2.coordinates().end()) {
													if(insideIsland2==OUTSIDE) {
														insideIsland2 = ENTERED;
														neighborNum++;
														neighborNumVec.at(index1).at(index2) = neighborNum;
														endCoords = Point(col,row);
														float dist = MyMath::eucDist(beginCoords,endCoords);
														neighborDistVec.at(index1).at(index2) = min(neighborDistVec.at(index1).at(index2),dist);
														if(index1==0 && index2==1) {
															cout << "Degree: " << theta << endl;
															cout << "Begin: " << beginCoords << ", " << "End: " << endCoords << endl;
															cout << "Dist: " << dist << endl;
														}
													} else if(insideIsland2==ENTERED) {
														insideIsland2 = INSIDE;
													}

													vecWidth.at(index2)++;
													goto jump_out;
												}
											}
										}
									}// end num2 loop
								}// end shade2 loop
							}// end shape2 loop
						}// end if(insideIsland1)
						jump_out:
						fCol += cos(deg);
						fRow += sin(deg);
						if(((int)theta%45) == 0) { //fixes col,row increments
							col += round(cos(deg));
							row += round(sin(deg));
						} else {
							col = round(fCol);
							row = round(fRow);
						}
					}// end while

					//counting the islands that are visible
					for(unsigned int index2=0; index2<vecWidth.size(); index2++) {
						if(index2!=index1) {
							if(vecWidth.at(index2)>=minWidthForVisibility)
								relationCount.at(index1).at(index2)++;
						}
					}
				}// end theta loop

				//check if island is surrounded by other islands
				for(unsigned int index2=0; index2<relationCount.at(index1).size(); index2++) {
					if(relationCount.at(index1).at(index2)>=surroundedThreshUpper) {
						this->relationMatrix.at(index1).at(index2) = this->rel_op[SURR_BY];
						this->relationMatrix.at(index2).at(index1) = this->rel_op[SURR_BY_INV];
						this->markedMap.at(index1).at(index2) = 1;
						this->markedMap.at(index2).at(index1) = 1;
					}
					if(this->relationCount.at(index1).at(index2)<surroundedThreshUpper) {
						if(this->relationCount.at(index1).at(index2)>=surroundedThreshLower) {
							float dist = neighborDistVec.at(index1).at(index2);
							if(neighborNumVec.at(index1).at(index2)==1 && dist<directNeighborDistThresh) {
								this->relationMatrix.at(index1).at(index2) = this->rel_op[DIR];
								this->relationMatrix.at(index2).at(index1) = this->rel_op[DIR];
							}
						}
					}
					if(index1==0 && index2==1) {
						printf("Rel_Count: %d, Dist: %f\n",this->relationCount.at(index1).at(index2),neighborDistVec.at(index1).at(index2));
					}
				}
			}// end num1 loop
		}// end shade1 loop
	}// end shape1 loop
	this->writeRelationMatrix(labels);
}

void ShadeShapeRelation::writeRelationMatrix(map<String,float> &labels) {
	FILE * fp;
	fp = fopen("relation_matrix.csv","w");
	map<String,float>::iterator it;
	fprintf(fp,",");
	for(it=labels.begin(); it!=labels.end(); it++) {
		fprintf(fp,"%s,",it->first.c_str());
	}
	fprintf(fp,"\n");
	it = labels.begin();
	for(unsigned int i=0; i<this->relationMatrix.size(); i++) {
		fprintf(fp,"%s,",it->first.c_str());
		it++;
		for(unsigned int j=0; j<this->relationMatrix.at(i).size(); j++) {
			fprintf(fp,"%s,",this->relationMatrix.at(i).at(j).c_str());
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}
