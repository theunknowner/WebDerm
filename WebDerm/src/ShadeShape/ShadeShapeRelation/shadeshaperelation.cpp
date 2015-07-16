/*
 * shadeshaperelation.cpp
 *
 *  Created on: Jul 16, 2015
 *      Author: jason
 */

#include "shadeshaperelation.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ShadeShape/shadeshape.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/******************** PRIVATE FUNCTIONS **********************/

void ShadeShapeRelation::setup_relationMatrix(map<String,float> &labels) {
	this->relationMatrix.clear();
	this->relationMatrix.resize(labels.size(),vector<String>(labels.size(),"NULL"));
	this->markedMap.clear();
	this->markedMap.resize(labels.size(),vector<int>(labels.size(),0));
}

/******************** PUBLIC FUNCTIONS ***********************/
void ShadeShapeRelation::surrounded_by(ShadeShape &ss, map<String,float> &labels, vector<vector<vector<Islands> > > &islandVec) {
	this->setup_relationMatrix(labels);
	vector<int> vecWidth(labels.size(),0);
	for(unsigned int shape=0; shape<islandVec.size(); shape++) {
		for(unsigned int shade=0; shade<islandVec.at(shape).size(); shade++) {
			for(unsigned int num=0; num<islandVec.at(shape).at(shade).size(); num++) {
				Islands isl = islandVec.at(shape).at(shade).at(num);
				String label = isl.labelName();
				int index = distance(labels.begin(),labels.find(label));
				Point center = isl.centerOfMass();
				int row=center.y, col=center.x;
				for(double theta=0.0; theta<360.0; theta+=15.0) {
					double deg = theta * M_PI / 180.0; //convert to degrees
					while(row<isl.image().rows && col<isl.image().cols) {
						String coords = toString(col)+","+toString(row);
						for(unsigned int shape2=0; shape2<islandVec.size(); shape2++) {
							for(unsigned int shade2=0; shade2<islandVec.at(shape2).size(); shade2++) {
								for(unsigned int num2=0; num2<islandVec.at(shape2).at(shade2).size(); num2++) {
									bool inside = false;
									Islands isl2 = islandVec.at(shape2).at(shade2).at(num2);
									String label2 = isl2.labelName();
									int index2 =  distance(labels.begin(),labels.find(label2));
									if(isl2.coordinates().find(coords)!=isl2.coordinates().end() && &isl!=&isl2)
										inside = true;
									if(inside) {
										vecWidth.at(index2)++;
										goto jump_out;
									}
								}
							}
						}// end shape2 loop
jump_out:
						col+=round(cos(deg));
						row+=round(sin(deg));
					}// end while
				}
			}
		}
	}
}
