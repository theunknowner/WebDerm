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
	this->relationMatrix.resize(labels.size(),vector<int>(labels.size(),0));
	this->relationCount.clear();
	this->relationCount.resize(labels.size(),vector<int>(labels.size(),0));
}

/* merges labels with the same shape and shade and
 * counts the relationship between the merged labels
 */
vector<vector<vector<int> > > ShadeShapeRelation::downScaleShadeShapeLabels(vector<vector<int> > &srm, map<String,float> &labels) {
	map<String,int> merged_labels;
	for(auto it=labels.begin(); it!=labels.end(); it++) {
		String newLabel = it->first.substr(0,it->first.length()-4);
		if(merged_labels.find(newLabel)==merged_labels.end()) {
			merged_labels[newLabel] = 1;
		}
	}
	vector<vector<vector<int> > > srmCount3D(merged_labels.size(),vector<vector<int> >(merged_labels.size(),vector<int>(this->rel_op.size(),0)));
	for(auto itY=labels.begin(); itY!=labels.end(); itY++) {
		int y = distance(labels.begin(),itY);
		String newLabel = itY->first.substr(0,itY->first.length()-4);
		auto itY2 = merged_labels.find(newLabel);
		int y2 = distance(merged_labels.begin(),itY2);
		auto itX = itY;
		itX++;
		for(; itX!=labels.end(); itX++) {
			int x = distance(labels.begin(), itX);
			newLabel = itX->first.substr(0,itX->first.length()-4);
			auto itX2 = merged_labels.find(newLabel);
			int x2 = distance(merged_labels.begin(),itX2);
			int rel_op_idx = srm.at(y).at(x);
			if(rel_op_idx>0) { //ignores "NULL" relations
				srmCount3D.at(y2).at(x2).at(rel_op_idx)++;
			}
		}
	}
/*
	for(auto itY=merged_labels.begin(); itY!=merged_labels.end(); itY++) {
		int y = distance(merged_labels.begin(),itY);
		for(auto itX=merged_labels.begin(); itX!=merged_labels.end(); itX++) {
			int x = distance(merged_labels.begin(), itX);
			for(unsigned int z=0; z<srmCount3D.at(y).at(x).size(); z++) {
				String relOp = this->rel_op.at(z);
				int count = srmCount3D.at(y).at(x).at(z);
				if(count>0) {
					printf("[%s][%s][%s]: %d\n",itY->first.c_str(),itX->first.c_str(),relOp.c_str(),count);
				}
			}
		}
	}*/

	return srmCount3D;
}

float ShadeShapeRelation::entropy(int count) {
	if(count==0) {
		return -0.75;
	} else if(count==1) {
		return 0.25;
	} else {
		return log10(count);
	}
}

//computes the entropy value for each label
float ShadeShapeRelation::entropy(vector<vector<vector<int> > > &srmUP, vector<vector<vector<int> > > &srmDB) {
	assert(srmUP.size()==srmDB.size());

	float totalEntropy = 1.0;
	for(unsigned int i=0; i<srmUP.size(); i++) {
		for(unsigned int j=0; j<srmUP.at(i).size(); j++) {
			int countUP = 0;
			int countDB = 0;
			for(unsigned int k=0; k<srmUP.at(i).at(j).size(); k++) {
				countUP = srmUP.at(i).at(j).at(k);
				countDB = srmDB.at(i).at(j).at(k);
				if(countUP>0 || countDB>0) {
					float entropyUP = this->entropy(countUP);
					float entropyDB = this->entropy(countDB);
					float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
					totalEntropy *= entropyVal;
				}
			}
		}
	}
	return totalEntropy;
}

/******************** PUBLIC FUNCTIONS ***********************/

vector<vector<int> > ShadeShapeRelation::spatial_relation(ShadeShape &ss, map<String,float> &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->setup_relationMatrix(labels);
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
				int index1 = distance(labels.begin(),labels.find(label1));
				Point center = isl1.centerOfMass();
				String coords1 = toString(center.x)+","+toString(center.y);
				vector<vector<int> > neighborNumVec(labels.size(),vector<int>(labels.size(),0));
				vector<vector<float> > neighborDistVec(labels.size(),vector<float>(labels.size(),140.0));
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
											int index2 =  distance(labels.begin(),labels.find(label2));
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
						this->relationMatrix.at(index1).at(index2) = SURR_BY;
						this->relationMatrix.at(index2).at(index1) = SURR_BY_INV;
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
	//this->writeRelationMatrix(labels);
	return this->relationMatrix;
}

float ShadeShapeRelation::srm_match(vector<vector<int> > srmUP,vector<vector<int> > srmDB, map<String,float> &labels) {
	vector<vector<vector<int> > > srm3dUP = this->downScaleShadeShapeLabels(srmUP,labels);
	vector<vector<vector<int> > > srm3dDB = this->downScaleShadeShapeLabels(srmDB,labels);
	float matchVal = this->entropy(srm3dUP,srm3dDB);
	//printf("EntUP: %f\n",entropyUP);
	//printf("EntDB: %f\n",entropyDB);

	return matchVal;
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
			int rel_op_idx = this->relationMatrix.at(i).at(j);
			String relOp = this->rel_op.at(rel_op_idx);
			fprintf(fp,"%s,",relOp.c_str());
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}


