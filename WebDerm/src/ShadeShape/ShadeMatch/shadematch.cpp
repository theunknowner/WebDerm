/*
 * shadematch.cpp
 *
 *  Created on: Jul 1, 2015
 *      Author: jason
 */

#include "shadematch.h"
#include "../shadeshape.h"
#include "../islands.h"
#include "../../Algorithms/jaysort.h"
#include "../../functions.h"

vector<float> ShadeMatch::shadeWeightsVec;

/*************** PRIVATE FUNCTIONS *******************/

/*************** PUBLIC FUNCTIONS ********************/

void ShadeMatch::setMaxShades(vector<int> shadeVec1, vector<int> shadeVec2) {
	this->maxNumOfShades = max(shadeVec1.size(),shadeVec2.size());

	ShadeMatch::shadeWeightsVec.clear();
	ShadeMatch::shadeWeightsVec.resize(this->maxNumOfShades,0.0);
	const float initWeight = 5.0;
	float weight = initWeight;
	int shades = ceil(this->maxNumOfShades/2.0);
	int j = this->maxNumOfShades-1;
	float totalWeight = 0.0;
	for(int i=0; i<shades; i++) {
		if(i>0) weight = initWeight / (i*initWeight);
		ShadeMatch::shadeWeightsVec.at(i) = weight;
		ShadeMatch::shadeWeightsVec.at(j) = weight;
		if(i!=j) {
			totalWeight += ShadeMatch::shadeWeightsVec.at(i) + ShadeMatch::shadeWeightsVec.at(j);
		} else {
			totalWeight += ShadeMatch::shadeWeightsVec.at(i);
		}
		j--;
	}
	for(unsigned int i=0; i<ShadeMatch::shadeWeightsVec.size(); i++) {
		//ShadeMatch::shadeWeightsVec.at(i) /= totalWeight;
		ShadeMatch::shadeWeightsVec.at(i) = 1.0; //> set weights to 1 for now
	}
}

//! continuous shade_translation1 of islands until shiftType changes
//! starting with the largest island
bool ShadeMatch::shade_translation1(ShadeShape &ss, int shiftType, int shiftAmt) {
	static int prevShiftType = 0;
	if(_SHIFT[shiftType]=="SHIFT_NONE") {
		prevShiftType = shiftType;
		return true;
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
							ss.feature(i).island(j).image().copyTo(shiftedImg,ss.feature(i).island(j).image());
						}
					}
				}
				shiftedImg.copyTo(ss.image(),shiftedImg);
				ss.getImageData().setImage(ss.image());
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
	return true;
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

//! shifts all the shades to either left(darker)  or right(lighter)
vector<vector<vector<Islands> > > ShadeMatch::shiftShades(vector<vector<vector<Islands> > > &islandVec, int shiftType) {
	if(shiftType==SHIFT_NONE)
		return islandVec;

	vector<vector<vector<Islands> > > newIslandVec(islandVec.size(),vector<vector<Islands> >(this->maxNumOfShades,vector<Islands>(0,Islands())));

	for(unsigned int shape=0; shape<islandVec.size(); shape++) {
		for(unsigned int shade=0; shade<islandVec.at(shape).size(); shade++) {
			//> if SHIFT_LEFT get max, if SHIFT_RIGHT get min
			int new_shade = shiftType==SHIFT_LEFT ? max((int)(shade-1),0) : min((int)(shade+1),this->maxNumOfShades-1);
			try {
				for(unsigned int isl=0; isl<islandVec.at(shape).at(shade).size(); isl++) {
					newIslandVec.at(shape).at(new_shade).push_back(islandVec.at(shape).at(shade).at(isl));
				}
			}
			catch(const std::out_of_range &oor) {
				printf("ShadeMatch::shiftShades(): out of range!\n");
				printf("shift_type: %d\n",shiftType);
				printf("shade: %d\n",shade);
				printf("new_shade: %d\n",new_shade);
				printf("newIslVec Size: %lu\n",newIslandVec.size());
				printf("newIslVec Size2: %lu\n",newIslandVec.at(shape).size());
				exit(1);
			}
		}
	}
	return newIslandVec;
}

float ShadeMatch::applyShadeWeights(int shadeLevel) {
	return ShadeMatch::shadeWeightsVec.at(shadeLevel);
}

//! shifts the lightest shades darker by one shade
//! or shifts the darkest shades lighter by one shade
bool ShadeMatch::shade_translation2(ShadeShape &ss, int shiftType, int shiftAmt) {
	if(shiftType==SHIFT_NONE)
		return false;

	this->featIslIdxStoreVec.clear();
	vector<int> indexVec;
	Mat shiftedImg = Mat::zeros(ss.image().size(),ss.image().type());
	for(int i=0; i<ss.numOfFeatures(); i++) {
		bool isShifted = false;
		for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
			int shade = ss.feature(i).island(j).shade();
			int shadeLevel = ss.getIndexOfShade(shade);
			if(shiftType==SHIFT_LEFT && shadeLevel==this->maxNumOfShades-1) {
				int new_shade = ss.shade(shadeLevel-shiftAmt);
				ss.feature(i).island(j).set_island_shade(new_shade);
				isShifted = true;
			}
			if(shiftType==SHIFT_RIGHT && shadeLevel==0) {
				int new_shade = ss.shade(shadeLevel+shiftAmt);
				ss.feature(i).island(j).set_island_shade(new_shade);
				isShifted = true;
			}
			if(isShifted) {
				indexVec.push_back(i);
				indexVec.push_back(j);
				this->featIslIdxStoreVec.push_back(indexVec);
				indexVec.clear();
				ss.feature(i).island(j).image().copyTo(shiftedImg,ss.feature(i).island(j).image());
			}
			isShifted = false;
		}
	}
	shiftedImg.copyTo(ss.image(),shiftedImg);
	ss.getImageData().setImage(ss.image());

	return true;
}
