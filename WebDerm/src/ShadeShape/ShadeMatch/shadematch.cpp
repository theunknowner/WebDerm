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

bool ShadeMatch::shade_translation(vector<vector<vector<Islands> > > &islandVec, float thresh, int shiftType, int shiftAmt) {
	if(SHIFT[shiftType]!="SHIFT_NONE") {
		vector<vector<int> > indexVec2d;
		vector<int> areaVec;
		vector<int> origPos;
		vector<int> indexVec;
		for(unsigned int i=0; i<islandVec.size(); i++) {
			for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
				for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
					int area = islandVec.at(i).at(j).at(k).area();
					areaVec.push_back(area);
					indexVec.push_back(i);
					indexVec.push_back(j);
					indexVec.push_back(k);
					indexVec2d.push_back(indexVec);
					indexVec.clear();
				}
			}
		}
		jaysort(areaVec,origPos);
		vector<vector<vector<Islands> > > newIslandVec = islandVec;
		int sum = 0;
		int shade_shift=0;
		for(int i=areaVec.size()-1; i>=0; i--) {
			int pos = origPos.at(i);
			int shape = indexVec2d.at(pos).at(0);
			int shade = indexVec2d.at(pos).at(1);
			int idx = indexVec2d.at(pos).at(2);
			if(SHIFT[shiftType] == "SHIFT_LEFT")
				shade_shift = max(shade-shiftAmt,0);
			if(SHIFT[shiftType] == "SHIFT_RIGHT")
				shade_shift = min(shade+shiftAmt,this->maxNumOfShades-1);

			if(shade_shift!=shade) {
				newIslandVec.at(shape).at(shade_shift).push_back(islandVec.at(shape).at(shade).at(idx));
				newIslandVec.at(shape).at(shade).erase(newIslandVec.at(shape).at(shade).begin()+idx);
			}

			//printf("shape%d_s%d_%d: %d\n",shape,shade,idx,areaVec.at(i));
			//printf("Shade %d: NewShade %d\n",shade,shade_shift);
			sum += areaVec.at(i);
			if(sum>thresh) break;
		}
		islandVec = newIslandVec;
	}
	return true;
}

bool ShadeMatch::shade_translation(vector<vector<vector<Islands> > > &islandVec, int shiftType, int shiftAmt) {
	static int prevShiftType = -1;
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
	if(SHIFT[shiftType]!="SHIFT_NONE") {
		if(prevShiftType!=shiftType) {
			for(unsigned int i=0; i<islandVec.size(); i++) {
				for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
					for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
						int area = islandVec.at(i).at(j).at(k).area();
						areaVec.push_back(area);
						indexVec.push_back(i);
						indexVec.push_back(j);
						indexVec.push_back(k);
						indexVec2d.push_back(indexVec);
						indexVec.clear();
					}
				}
			}
			jaysort(areaVec,origPos);
		}
		vector<vector<vector<Islands> > > newIslandVec = islandVec;
		int shade_shift=0;
		static int index = 1;
		if(prevShiftType != shiftType)	index = 1;
		prevShiftType = shiftType;
		printf("index: %d\n",index);
		for(int i=areaVec.size()-index; i>=0;) {
			int pos = origPos.at(i);
			int shape = indexVec2d.at(pos).at(0);
			int shade = indexVec2d.at(pos).at(1);
			int idx = indexVec2d.at(pos).at(2);
			if(SHIFT[shiftType] == "SHIFT_LEFT")
				shade_shift = max(shade-shiftAmt,0);
			if(SHIFT[shiftType] == "SHIFT_RIGHT")
				shade_shift = min(shade+shiftAmt,this->maxNumOfShades-1);

			if(shade_shift!=shade) {
				newIslandVec.at(shape).at(shade_shift).push_back(islandVec.at(shape).at(shade).at(idx));
				newIslandVec.at(shape).at(shade).erase(newIslandVec.at(shape).at(shade).begin()+idx);
			}
			printf("shape%d_s%d_%d -> ",shape,shade,idx);
			printf("shape%d_s%d_%lu\n",shape,shade_shift,newIslandVec.at(shape).at(shade_shift).size()-1);
			index++;
			islandVec = newIslandVec;
			return true;
		}
	}
	return false;
}

//! continuous shade_translation of islands until shiftType changes
bool ShadeMatch::shade_translation(ShadeShape &ss, int shiftType, int shiftAmt) {
	static int prevShiftType = 0;
	if(SHIFT[shiftType]=="SHIFT_NONE") {
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
	int newShadeInd=0;
	static int index = 1;
	if(prevShiftType != shiftType)	index = 1;
	//printf("prevShift: %s, currShift: %s\n",ShadeMatch::SHIFT[prevShiftType].c_str(),ShadeMatch::SHIFT[shiftType].c_str());
	prevShiftType = shiftType;
	//printf("index: %d\n",index);
	int n =  areaVec.size()-index;
	index++;
	if(n>=0) {
		int pos=0,featNum=0,islNum=0,shade=0,shadeInd=0;
		int newShade=0;
		try {
			pos = origPos.at(n);
			featNum = indexVec2d.at(pos).at(0);
			islNum = indexVec2d.at(pos).at(1);
			shade = ss.feature(featNum).island(islNum).shade();
			shadeInd = ss.getIndexOfShade(shade);
			if(SHIFT[shiftType] == "SHIFT_LEFT")
				newShadeInd = max(shadeInd-shiftAmt,0);
			if(SHIFT[shiftType] == "SHIFT_RIGHT")
				newShadeInd = min(shadeInd+shiftAmt,this->maxNumOfShades-1);

			if(newShadeInd!=shadeInd) {
				newShade = ss.shade(newShadeInd);
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
			}
			return true;
		} catch (const std::out_of_range &oor) {
			printf("ShadeMatch::shade_translation() out of range!\n");
			printf("%s\n",SHIFT[shiftType].c_str());
			printf("origPos.size(): %lu\n",origPos.size());
			printf("indexVec2d.size(): %lu\n",indexVec2d.size());
			printf("n: %d\n",n);
			printf("pos: %d\n",pos);
			printf("shade: %d\n",shade);
			printf("shadeInd: %d\n",shadeInd);
			printf("newShadeInd: %d\n",newShadeInd);
			printf("newShade:%d\n",newShade);
			printf("maxNumOfShades: %d\n",this->maxNumOfShades);
			exit(1);
		}
	}
	return false;
}

float ShadeMatch::applyShiftPenalty(float score, int shiftAmt) {
	float penalty = -0.3 * min(shiftAmt,1); //not permanent
	float newScore = score * pow(2.0,penalty);
	return newScore;
}

