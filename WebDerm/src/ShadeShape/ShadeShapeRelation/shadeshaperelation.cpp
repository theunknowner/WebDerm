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
#include "/home/jason/git/WebDerm/WebDerm/src/State/state.h"
#include "srm.h"

using namespace ip;

/******************** PUBLIC FUNCTIONS ***********************/

void ShadeShapeRelation::generate_srm(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->srm = Srm(ss,labels);
	map<String,pair<int,float> > lbls = labels.getMap();
	const float interval = 15.0;
	const int visibilityThresh = 3;
	const float surroundedThreshUpper = 0.58; //> 14/24; 360.0 deg/15.0 deg = 24;
	const float surroundedThreshLower = 0.125; //> 3/24
	const float alpha = 0.25;
	int minWidthForVisibility = 0;
	Point beginCoords, endCoords;
	for(unsigned int shape1=0; shape1<islandVec.size(); shape1++) {
		for(unsigned int shade1=0; shade1<islandVec.at(shape1).size(); shade1++) {
			for(unsigned int num1=0; num1<islandVec.at(shape1).at(shade1).size(); num1++) {
				Islands isl1 = islandVec.at(shape1).at(shade1).at(num1);
				String label1 = isl1.labelName();
				int index1 = distance(lbls.begin(),lbls.find(label1));
				int area1 = isl1.area();
				Point center = isl1.centerOfMass();
				String coords1 = toString(center.x)+","+toString(center.y);
				vector<vector<vector<int> > > neighborNumCount(lbls.size(),vector<vector<int> >(lbls.size(),vector<int>(0,0)));
				int totalTimesEntered = 0;
				for(double theta=0.0; theta<360.0; theta+=interval) {
					vector<int> vecWidth(labels.size(),0);
					vector<int> islandHitVec; //stores the labels and index for islands that has been entered
					map<String,int> islandHitMap;
					vector<vector<int> > touchCountVec(lbls.size(),vector<int>(lbls.size(),0));
					int neighborNum = 0;
					String prevLabel2="";
					int row=center.y, col=center.x;
					float fRow = center.y, fCol = center.x;
					double deg = theta * M_PI / 180.0; //convert from radians to degrees
					int state = isl1.containsCoordinate(coords1) ? State::ENTERED : State::OUTSIDE;
					State insideIsland1(state);
					while(row<isl1.image().rows && col<isl1.image().cols && row>=0 && col>=0) {
						String coords2 = toString(col)+","+toString(row);
						if(insideIsland1.currentState()==State::OUTSIDE) {
							if(isl1.containsCoordinate(coords2)) {
								insideIsland1.setState(State::ENTERED);
							}
						}
						if(insideIsland1.currentState()==State::ENTERED || insideIsland1.currentState()==State::INSIDE) {
							if(insideIsland1.currentState()==State::ENTERED) {
								totalTimesEntered++;
								insideIsland1.setState(State::INSIDE);
							}
							if(isl1.containsCoordinate(coords2)) {
								vecWidth.at(index1)++;
								minWidthForVisibility = min(vecWidth.at(index1),visibilityThresh);
								beginCoords = Point(col,row);
							}
							else {
								insideIsland1.setState(State::EXITED);
							}
						}
						if(insideIsland1.currentState()==State::EXITED) {
							for(unsigned int shape2=0; shape2<islandVec.size(); shape2++) {
								for(unsigned int shade2=0; shade2<islandVec.at(shape2).size(); shade2++) {
									for(unsigned int num2=0; num2<islandVec.at(shape2).at(shade2).size(); num2++) {
										Islands isl2 = islandVec.at(shape2).at(shade2).at(num2);
										String label2 = isl2.labelName();
										int state2 = isl2.containsCoordinate(coords2) ? State::ENTERED : State::OUTSIDE;
										State insideIsland2(state2);
										if(insideIsland2.currentState()==State::ENTERED || insideIsland2.currentState()==State::INSIDE) {
											if(label2==prevLabel2) insideIsland2.setState(State::INSIDE);
											if(shape1==shape2 && shade1==shade2 && num1==num2) { //revert if island1 enters itself
												if(insideIsland2.currentState()==State::ENTERED) {
													neighborNum = 0;
													for(unsigned int i=0; i<islandHitVec.size(); i++) {
														int index2 = islandHitVec.at(i);
														neighborNumCount.at(index1).at(index2).pop_back();
														islandHitMap.erase(labels.at(index2));
													}
													islandHitVec.clear();
												}
											} else {
												int index2 =  distance(lbls.begin(),lbls.find(label2));
												if(insideIsland2.currentState()==State::ENTERED) {
													if(srm.getRelationDistance(index1,index2,theta)==0) {
														float dist = MyMath::eucDist(beginCoords,Point(col,row));
														srm.setRelationDistance(index1,index2,theta,dist);
														srm.setRelationDistance(index2,index1,theta,dist);
													}
												}
												int area2 = isl2.area();
												Point center2 = isl2.centerOfMass();
												float centerDist = abs(MyMath::eucDist(center,center2));
												float relArea1 = (float)isl1.area() / ss.image().total();
												float relArea2 = (float)isl2.area() / ss.image().total();
												float thresh = 250.0 * pow(relArea1*relArea2, alpha);
												if(centerDist<=thresh && srm.relation(index1,index2)==NONE) {
													srm.relation(index1,index2) = INDIR;
													srm.relation(index2,index1) = INDIR;
													srm.relationArea(index1,index2) = std::make_pair(area1,area2);
												}
												if(srm.relation(index1,index2)<=DIR) {
													if(insideIsland2.currentState()==State::ENTERED) {
														endCoords = Point(col,row);
														float dist = MyMath::eucDist(beginCoords,endCoords);
														if(islandHitMap.find(label2)==islandHitMap.end()) {
															neighborNum++;
															islandHitVec.push_back(index2);
															islandHitMap[label2] = neighborNum;
															neighborNumCount.at(index1).at(index2).push_back(neighborNum);
															if(dist<2) {
																touchCountVec.at(index1).at(index2)=1;
															}
															/*
															if(index1==37 && index2==32 && theta==90.0) {
																printf("%s | %s\n",label1.c_str(),label2.c_str());
																printf("Center: %s\n",coords1.c_str());
																printf("Deg: %f\n",theta);
																printf("Coords: %s\n",coords2.c_str());
																printf("BeginCoord: (%d,%d)\n",beginCoords.x,beginCoords.y);
																printf("EndCoord: (%d,%d)\n",endCoords.x,endCoords.y);
																printf("Dist: %f\n",dist);
																printf("TouchCount: %d\n",srm.relationTouchCount(index1,index2));
															}*/
														}
														/*if(index1==45 && index2==84) {
															printf("%s | %s\n",label1.c_str(),label2.c_str());
															printf("Deg: %f, %s\n",theta,coords2.c_str());
															printf("NeighborNum: %d\n",neighborNum);
															cout << "------------------" << endl;
														}*/
													}
													prevLabel2 = label2;
													vecWidth.at(index2)++;
													goto jump_out;
												}
											}
											prevLabel2 = label2;
										} //end if insideIsland2.currentState() == ENTERED || INSIDE
									}// end num2 loop
								}// end shade2 loop
							}// end shape2 loop
						}// end if insideIsland1.currentState(0 == EXITED
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
							if(vecWidth.at(index2)>=minWidthForVisibility && islandHitMap.find(labels.at(index2))!=islandHitMap.end())
								srm.relationCount(index1,index2)++;
						}
						srm.relationTouchCount(index1,index2) += touchCountVec.at(index1).at(index2);
					}
				}// end theta loop
				//check if island is surrounded by other islands
				for(unsigned int index2=0; index2<srm.size(); index2++) {
					int neighborNumber=0;
					int area2 = labels.area(index2);
					if(neighborNumCount.at(index1).at(index2).size()>0) {
						neighborNumber = majority(neighborNumCount.at(index1).at(index2));
					}
					float countPercent = (float)srm.relationCount(index1,index2) / totalTimesEntered;
					float touchCountPercent = (float)srm.relationTouchCount(index1,index2) / totalTimesEntered;
					/*
					if(index1==37 && index2==32) {
						printf("%s | %s\n",label1.c_str(),labels.at(index2).c_str());
						printf("Center: (%d,%d)\n",center.x,center.y);
						printf("Relation: %s\n",this->rel_op.at(srm.relation(index1,index2)).c_str());
						printf("Area1: %d, Area2: %d\n",area1,area2);
						printf("Count: %d(%f)\n",srm.relationCount(index1,index2),countPercent);
						printf("TouchCount: %d(%f)\n",srm.relationTouchCount(index1,index2),touchCountPercent);
						printf("NeighborNumCount.Size(): %lu\n",neighborNumCount.at(index1).at(index2).size());
						printf("Majority: %d\n", neighborNumber);
						printf("TotalTimesEntered: %d\n",totalTimesEntered);
					}*/
					if(countPercent>=surroundedThreshUpper) {
						srm.relation(index1,index2) = SURR_BY;
						srm.relation(index2,index1) = SURR_BY_INV;
						srm.relationCountPercent(index1,index2) = countPercent;
						srm.relationCountPercent(index2,index1) = countPercent;
					}
					else if(touchCountPercent<surroundedThreshUpper && touchCountPercent>=surroundedThreshLower) {
						if(neighborNumber==1) {
							srm.relation(index1,index2) = DIR;
							srm.relation(index2,index1) = DIR;
							srm.relationCountPercent(index1,index2) = touchCountPercent;
							srm.relationCountPercent(index2,index1) = touchCountPercent;
						}
					}
					else if(countPercent<surroundedThreshUpper && countPercent>=surroundedThreshLower && touchCountPercent>=surroundedThreshUpper) {
						if(neighborNumber==1) {
							srm.relation(index1,index2) = DIR;
							srm.relation(index2,index1) = DIR;
							srm.relationCountPercent(index1,index2) = countPercent;
							srm.relationCountPercent(index2,index1) = countPercent;
						}
					}
					int maxNeighborLevelCap = 2;
					srm.neighborLevel(index1,index2) = max(neighborNumber,srm.neighborLevel(index1,index2));
					srm.neighborLevel(index2,index1) = max(neighborNumber,srm.neighborLevel(index2,index1));
					srm.neighborLevel(index1,index2) = min(srm.neighborLevel(index1,index2),maxNeighborLevelCap);
					srm.neighborLevel(index2,index1) = min(srm.neighborLevel(index2,index1),maxNeighborLevelCap);
					srm.maxNeighborLevel() = max(srm.maxNeighborLevel(),srm.neighborLevel(index1,index2));
					srm.relationArea(index1,index2) = std::make_pair(area1,area2);
				}
			}// end num1 loop
		}// end shade1 loop
	}// end shape1 loop
}

/*
 * returns SSR object containing all info on the ShadeShape's spatial relationships
 * accessible by the defined functions
 */
void ShadeShapeRelation::spatial_relation(ShadeShape &ss, Labels &labels, vector<vector<vector<Islands> > > &islandVec, int write, String newName) {
	this->ssr_name = newName!="" ? newName : ss.name();
		this->generate_srm(ss,labels,islandVec);
	if(write) {
		this->srm.writeRelationMatrix(labels,this->ssr_name);
		this->srm.writeNeighborLevelMatrix(labels,this->ssr_name);
		this->srm.writeRelationCountMatrix(labels,this->ssr_name);
	}
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

//! use after running generate_srm() function
Srm& ShadeShapeRelation::get_srm() {
	return this->srm;
}

void ShadeShapeRelation::importSrmFiles(String name) {
	String srm_file = name+"_srm.csv";
	String los_file = name+"_los.csv";
	String count_file = name+ "_count.csv";
	fstream fs(srm_file);
	fstream fs2(los_file);
	fstream fs3(count_file);
	if(fs.is_open()) {
		String temp;
		vector<String> vec;
		getline(fs,temp);
		getSubstr(temp,',',vec);
		for(unsigned int i=0; i<vec.size(); i++) {

		}
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {

			}
		}
		fs.close();
	}
}

void ShadeShapeRelation::dbg_import_srm(bool setImport) {
	this->isImported = setImport;
}
