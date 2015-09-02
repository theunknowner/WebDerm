/*
 * shadematch.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#include "shadematch.h"
#include "../shadeshape.h"
#include "../islands.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/*************** PRIVATE FUNCTIONS *******************/

/*************** PUBLIC FUNCTIONS ********************/

void ShadeMatch::setMaxShades(vector<int> shadeVec1, vector<int> shadeVec2) {
	this->maxNumOfShades = max(shadeVec1.size(),shadeVec2.size());
}

//! continuous shade_translation of islands until shiftType changes
bool ShadeMatch::shade_translation(ShadeShape &ss, int shiftType, int shiftAmt) {
	static int prevShiftType = 0;
	if(_SHIFT[shiftType]=="SHIFT_NONE") {
		prevShiftType = shiftType;
		return false;
	}

	static vector<int> areaVec;
	static vector<int> origPos;
	static vector<int> indexVec;
	static vector<vector<int> > indexVec2d;
	if(prevShiftType != shiftType) {
		areaVec.clear();
		origPos.clear();
		indexVec.clear();
		indexVec2d.clear();
	}
	if(prevShiftType!=shiftType) {
		this->featIslIdxStoreVec.clear();
		for(int i=0; i<ss.numOfFeatures(); i++) {
			for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
				int area = ss.feature(i).island(j).area();
				areaVec.push_back(area);
				indexVec.push_back(i);
				indexVec.push_back(j);
				indexVec2d.push_back(indexVec);
				indexVec.clear();
			}
		}
		jaysort(areaVec,origPos);
	}
	int newShadeIdx=0;
	static int index = 1;
	if(prevShiftType != shiftType)	index = 1;
	//printf("prevShift: %s, currShift: %s\n",ShadeMatch::SHIFT[prevShiftType].c_str(),ShadeMatch::SHIFT[shiftType].c_str());
	prevShiftType = shiftType;
	//printf("index: %d\n",index);
	int n =  areaVec.size()-index;
	index++;
	if(n>=0) {
		int pos=0,featNum=0,islNum=0,shade=0,shadeIdx=0;
		int newShade=0;
		try {
			pos = origPos.at(n);
			featNum = indexVec2d.at(pos).at(0);
			islNum = indexVec2d.at(pos).at(1);
			shade = ss.feature(featNum).island(islNum).shade();
			shadeIdx = ss.getIndexOfShade(shade);
			if(this->_SHIFT[shiftType] == "SHIFT_LEFT")
				newShadeIdx = max(shadeIdx-shiftAmt,0);
			if(this->_SHIFT[shiftType] == "SHIFT_RIGHT")
				newShadeIdx = min(shadeIdx+shiftAmt,this->maxNumOfShades-1);

			if(newShadeIdx!=shadeIdx) {
				newShade = ss.shade(newShadeIdx);
				Mat shiftedImg(ss.image().size(),ss.image().type(),Scalar(0));
				for(int i=0; i<ss.numOfFeatures(); i++) {
					for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
						if(i==featNum && j==islNum) {
							ss.feature(i).island(j).set_island_shade(newShade);
						}
						ss.feature(i).island(j).image().copyTo(shiftedImg,ss.feature(i).island(j).image());
					}
				}
				ss.image() = shiftedImg;
				this->featIslIdxStoreVec.push_back(indexVec2d.at(pos));
				this->featIslStored = true;
				return true;
			}
			this->featIslStored = false;
			return true;
		} catch (const std::out_of_range &oor) {
			printf("ShadeMatch::shade_translation() out of range!\n");
			printf("%s\n",_SHIFT[shiftType].c_str());
			printf("origPos.size(): %lu\n",origPos.size());
			printf("indexVec2d.size(): %lu\n",indexVec2d.size());
			printf("n: %d\n",n);
			printf("pos: %d\n",pos);
			printf("shade: %d\n",shade);
			printf("shadeInd: %d\n",shadeIdx);
			printf("newShadeInd: %d\n",newShadeIdx);
			printf("newShade:%d\n",newShade);
			printf("maxNumOfShades: %d\n",this->maxNumOfShades);
			exit(1);
		}
	}
	return false;
}

vector<vector<int> >& ShadeMatch::getStoredFeatIslIdx() {
	return this->featIslIdxStoreVec;
}

bool& ShadeMatch::isFeatIslStored() {
	return this->featIslStored;
}

vector<String> ShadeMatch::SHIFT() {
	return this->_SHIFT;
}

float ShadeMatch::applyShiftPenalty(ShadeShape &ss, float score, int shiftAmt) {
	float totalRelArea = 0.0;
	for(unsigned int i=0; i<this->featIslIdxStoreVec.size(); i++) {
		int featNum = this->featIslIdxStoreVec.at(i).at(0);
		int islNum = this->featIslIdxStoreVec.at(i).at(1);
		int area = ss.feature(featNum).island(islNum).area();
		//cout << area << endl;
		float relArea = (float)area / ss.area();
		totalRelArea += relArea;
	}
	float penalty = pow(2.0,(-2.0*totalRelArea));
	return score * penalty;
}
