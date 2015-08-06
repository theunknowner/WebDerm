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
#include "../Labels/labels.h"

/******************** PRIVATE FUNCTIONS **********************/

void ShadeShapeRelation::setup_relationMatrix(Labels &labels) {
	this->relationMatrix.clear();
	this->relationMatrix.resize(labels.size(),vector<int>(labels.size(),0));
	this->relationCount.clear();
	this->relationCount.resize(labels.size(),vector<int>(labels.size(),0));
}

/******************** PUBLIC FUNCTIONS ***********************/

vector<vector<int> > ShadeShapeRelation::generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->setup_relationMatrix(labels);
	map<String,pair<int,float> > lbls = labels.getLabels();
	const int visibilityThresh = 3;
	const int surroundedThreshUpper = 14;
	const int surroundedThreshLower = 3;
	const float directNeighborDistThresh = 5.0;
	const float alpha = 0.25;
	int minWidthForVisibility = 0;
	Point beginCoords, endCoords;
	for(unsigned int shape1=0; shape1<islandVec.size(); shape1++) {
		for(unsigned int shade1=0; shade1<islandVec.at(shape1).size(); shade1++) {
			for(unsigned int num1=0; num1<islandVec.at(shape1).at(shade1).size(); num1++) {
				Islands isl1 = islandVec.at(shape1).at(shade1).at(num1);
				String label1 = isl1.labelName();
				int index1 = distance(lbls.begin(),lbls.find(label1));
				Point center = isl1.centerOfMass();
				String coords1 = toString(center.x)+","+toString(center.y);
				vector<vector<int> > neighborNumVec(lbls.size(),vector<int>(lbls.size(),0));
				vector<vector<float> > neighborDistVec(lbls.size(),vector<float>(lbls.size(),140.0));
				//printf("%s: %s\n",label1.c_str(),coords1.c_str());
				for(double theta=0.0; theta<360.0; theta+=15.0) {
					vector<int> vecWidth(labels.size(),0);
					int neighborNum = 0;
					String prevIslandLabel="";
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
						}
						if(insideIsland1==ENTERED) {
							if(isl1.coordinates().find(coords2)!=isl1.coordinates().end()) {
								vecWidth.at(index1)++;
								minWidthForVisibility = min(vecWidth.at(index1),visibilityThresh);
								beginCoords = Point(col,row);
							}
							else {
								insideIsland1 = EXITED;
							}
						}
						if(insideIsland1==EXITED) {
							for(unsigned int shape2=0; shape2<islandVec.size(); shape2++) {
								for(unsigned int shade2=0; shade2<islandVec.at(shape2).size(); shade2++) {
									for(unsigned int num2=0; num2<islandVec.at(shape2).at(shade2).size(); num2++) {
										if(shape1!=shape2 || shade1!=shade2 || num1!=num2) { //checks so island1 != island2
											Islands isl2 = islandVec.at(shape2).at(shade2).at(num2);
											String label2 = isl2.labelName();
											int index2 =  distance(lbls.begin(),lbls.find(label2));
											int insideIsland2 = isl2.coordinates().find(coords2)!=isl2.coordinates().end() ? ENTERED : OUTSIDE;
											/*if(index1==2 && index2==24) {
												printf("Deg: %f, Coords2: %s, ",theta,coords2.c_str());
												printf("State: %d\n",insideIsland2);
											}*/
											Point center2 = isl2.centerOfMass();
											float centerDist = abs(MyMath::eucDist(center,center2));
											float relArea1 = (float)isl1.area() / ss.image().total();
											float relArea2 = (float)isl2.area() / ss.image().total();
											float thresh = 250.0 * pow(relArea1*relArea2, alpha);
											if(centerDist<=thresh && relationMatrix.at(index1).at(index2)==NONE) {
												this->relationMatrix.at(index1).at(index2) = INDIR;
												this->relationMatrix.at(index2).at(index1) = INDIR;
											}
											if(relationMatrix.at(index1).at(index2)<=DIR) {
												if(insideIsland2==ENTERED || insideIsland2==INSIDE) {
													if(prevIslandLabel==label2) insideIsland2=INSIDE;
													if(insideIsland2==ENTERED) {
														neighborNum++;
														neighborNumVec.at(index1).at(index2) = neighborNum;
														endCoords = Point(col,row);
														float dist = MyMath::eucDist(beginCoords,endCoords);
														neighborDistVec.at(index1).at(index2) = min(neighborDistVec.at(index1).at(index2),dist);
													}
													prevIslandLabel = label2;
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
						int area1 = labels.area(index1);
						int area2 = labels.area(index2);
						if(area2>area1) {
							this->relationMatrix.at(index1).at(index2) = SURR_BY;
							this->relationMatrix.at(index2).at(index1) = SURR_BY_INV;
						}
					}
					if(this->relationCount.at(index1).at(index2)<surroundedThreshUpper) {
						if(this->relationCount.at(index1).at(index2)>=surroundedThreshLower) {
							float dist = neighborDistVec.at(index1).at(index2);
							if(neighborNumVec.at(index1).at(index2)==1 && dist<directNeighborDistThresh) {
								this->relationMatrix.at(index1).at(index2) = DIR;
								this->relationMatrix.at(index2).at(index1) = DIR;
							}
						}
					}
				}
			}// end num1 loop
		}// end shade1 loop
	}// end shape1 loop
	return this->relationMatrix;
}

/*
 * returns SSR object containing all info on the ShadeShape's spatial relationships
 * accessible by the defined functions
 */
void ShadeShapeRelation::spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->ssr_name = ss.name();
	this->generate_srm(ss,labels,islandVec);
	//this->writeRelationMatrix(labels,ss.name());
}

void ShadeShapeRelation::writeRelationMatrix(Labels &labels, String name) {
	name += "_srm.csv";
	FILE * fp;
	fp = fopen(name.c_str(),"w");
	map<String,pair<int,float> > labelMap = labels.getLabels();
	fprintf(fp,",");
	for(auto it=labelMap.begin(); it!=labelMap.end(); it++) {
		fprintf(fp,"%s,",it->first.c_str());
	}
	fprintf(fp,"\n");
	auto it = labelMap.begin();
	for(unsigned int i=0; i<this->relationMatrix.size(); i++) {
		fprintf(fp,"%s,",it->first.c_str());
		it++;
		for(unsigned int j=0; j<this->relationMatrix.at(i).size(); j++) {
			int rel_op_idx = this->relationMatrix.at(i).at(j);
			String relOp = this->rel_op.at(rel_op_idx);
			fprintf(fp,"%s,",relOp.c_str());
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

vector<vector<int> >& ShadeShapeRelation::get_srm() {
	return this->relationMatrix;
}

String ShadeShapeRelation::name() {
	return this->ssr_name;
}

//! returns the index of the specified relation_operator
int ShadeShapeRelation::getRelOpIndex(String relOp) {
	for(unsigned int i=0; i<this->rel_op.size(); i++) {
		if(relOp == this->rel_op.at(i))
			return i;
	}
	return -1;
}
