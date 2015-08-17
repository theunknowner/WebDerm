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
	assert(labels.size()>0);
	this->relationMatrix.clear();
	this->relationMatrix.resize(labels.size(),vector<vector<int> >(labels.size(),vector<int>(2,0)));
	this->relationCount.clear();
	this->relationCount.resize(labels.size(),vector<int>(labels.size(),0));
}

/******************** PUBLIC FUNCTIONS ***********************/

void ShadeShapeRelation::generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec) {
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
				vector<vector<float> > neighborDistVec(lbls.size(),vector<float>(lbls.size(),140.0));
				vector<vector<vector<float> > > neighborNumCount(lbls.size(),vector<vector<float> >(lbls.size(),vector<float>(0,0.0)));
				//printf("%s: %s\n",label1.c_str(),coords1.c_str());
				for(double theta=0.0; theta<360.0; theta+=15.0) {
					vector<int> vecWidth(labels.size(),0);
					float neighborNum = 0.;
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
											if(centerDist<=thresh && relationMatrix.at(index1).at(index2).at(0)==NONE) {
												this->relationMatrix.at(index1).at(index2).at(0) = INDIR;
												this->relationMatrix.at(index2).at(index1).at(0) = INDIR;
											}
											if(relationMatrix.at(index1).at(index2).at(0)<=DIR) {
												if(insideIsland2==ENTERED || insideIsland2==INSIDE) {
													if(prevIslandLabel==label2) insideIsland2=INSIDE;
													if(insideIsland2==ENTERED) {
														neighborNum++;
														neighborNumCount.at(index1).at(index2).push_back(neighborNum);
														endCoords = Point(col,row);
														float dist = MyMath::eucDist(beginCoords,endCoords);
														neighborDistVec.at(index1).at(index2) = min(neighborDistVec.at(index1).at(index2),dist);
													}
													prevIslandLabel = label2;
													vecWidth.at(index2)++;
													goto jump_out;
												}
											}
											if(relationMatrix.at(index1).at(index2).at(0)>=SURR_BY_INV && isl1.area()<isl2.area()) {
												if(insideIsland2==ENTERED || insideIsland2==INSIDE) {
													if(prevIslandLabel==label2) insideIsland2=INSIDE;
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
					int neighborNumber=0;
					if(neighborNumCount.at(index1).at(index2).size()>0) {
						neighborNumber = majority(neighborNumCount.at(index1).at(index2));
					}
					this->relationMatrix.at(index1).at(index2).at(1) = max(neighborNumber,this->relationMatrix.at(index1).at(index2).at(1));
					this->relationMatrix.at(index2).at(index1).at(1) = max(neighborNumber,this->relationMatrix.at(index2).at(index1).at(1));
					if(relationCount.at(index1).at(index2)>=surroundedThreshUpper) {
						this->relationMatrix.at(index1).at(index2).at(0) = SURR_BY;
						this->relationMatrix.at(index2).at(index1).at(0) = SURR_BY_INV;
					}
					if(this->relationCount.at(index1).at(index2)<surroundedThreshUpper) {
						if(this->relationCount.at(index1).at(index2)>=surroundedThreshLower) {
							float dist = neighborDistVec.at(index1).at(index2);
							if(neighborNumber==1 && dist<directNeighborDistThresh) {
								this->relationMatrix.at(index1).at(index2).at(0) = DIR;
								this->relationMatrix.at(index2).at(index1).at(0) = DIR;
							}
						}
					}
				}
			}// end num1 loop
		}// end shade1 loop
	}// end shape1 loop
}

/*
 * returns SSR object containing all info on the ShadeShape's spatial relationships
 * accessible by the defined functions
 */
void ShadeShapeRelation::spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->ssr_name = ss.name();
	this->generate_srm(ss,labels,islandVec);
	this->writeRelationMatrix(labels,ss.name());
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
			int rel_op_idx = this->relationMatrix.at(i).at(j).at(0);
			String relOp = this->rel_op.at(rel_op_idx);
			fprintf(fp,"%s,",relOp.c_str());
			//fprintf(fp,"%d,",rel_op_idx);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

vector<vector<vector<int> > >& ShadeShapeRelation::get_srm() {
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

/* merges labels with the same shape and shade and
 * counts the relationship between the merged labels
 * along with areas for that relationship
 */
pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> ShadeShapeRelation::downScaleSrm(Labels &labels, Labels &mergedLabels) {
	map<String,pair<int,float> > labelMap = labels.getLabels();
	map<String,pair<int,float> > merged_labels = mergedLabels.getLabels();

	vector<vector<vector<vector<map<String,int>> > > > srmMarkMap(merged_labels.size(),vector<vector<vector<map<String,int>> > >(merged_labels.size(),vector<vector<map<String,int>> >(this->rel_op.size(),vector<map<String,int>>(relOpLevelSize,map<String,int>()))));
	vector<vector<vector<vector<int> > > > srmCount4D = Func::createVector4D(merged_labels.size(),merged_labels.size(),this->rel_op.size(),relOpLevelSize,0);
	vector<vector<vector<vector<int> > > > srmArea4D = Func::createVector4D(merged_labels.size(),merged_labels.size(),this->rel_op.size(),relOpLevelSize,0);

	for(auto itY=labelMap.begin(); itY!=labelMap.end(); itY++) {
		int y = distance(labelMap.begin(),itY);
		String newLabelY = itY->first.substr(0,itY->first.length()-4);
		auto itY2 = merged_labels.find(newLabelY);
		int y2 = distance(merged_labels.begin(),itY2);
		int areaY = itY->second.first;
		auto itX = itY;
		itX++;
		for(; itX!=labelMap.end(); itX++) {
			int x = distance(labelMap.begin(), itX);
			String newLabelX = itX->first.substr(0,itX->first.length()-4);
			auto itX2 = merged_labels.find(newLabelX);
			int x2 = distance(merged_labels.begin(),itX2);
			int rel_op_idx = this->relationMatrix.at(y).at(x).at(0);
			int neighborNum = this->relationMatrix.at(y).at(x).at(1);
			if(rel_op_idx>0) { //ignores "NULL" relations
				int areaX = itX->second.first;
				srmCount4D.at(y2).at(x2).at(rel_op_idx).at(neighborNum)++;
				if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itX->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
					srmArea4D.at(y2).at(x2).at(rel_op_idx).at(neighborNum) += areaX;
					srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itX->first] = 1;
				}
				if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itY->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
					srmArea4D.at(y2).at(x2).at(rel_op_idx).at(neighborNum)+= areaY;
					srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itY->first] = 1;
				}

			}
		}
	}
	auto srm4dPair = std::make_pair(srmCount4D,srmArea4D);
	return srm4dPair;
}
