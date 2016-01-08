/*
 * shapematch.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#include "shapematch.h"
#include "../islands.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"

vector<vector<String> > ShapeMatch::shiftingRules;
vector<vector<float> > ShapeMatch::shiftingPenalties;
bool ShapeMatch::THRESH_IMPORTED = false;

vector<float> ShapeMatch::shapeWeightsVec;
vector<float> ShapeMatch::shapeWeightsVec2;

/******************* PUBLIC FUNCTIONS ******************/

ShapeMatch::ShapeMatch() {
	if(!this->THRESH_IMPORTED)
		this->THRESH_IMPORTED = this->importThresholds();
}

bool ShapeMatch::importThresholds() {
	if(!ShapeMatch::THRESH_IMPORTED) {
		String folderName = "Thresholds/";
		String filename = folderName+"shape_shifting_rules.csv";
		String filename2 = folderName+"shape_penalties.csv";
		String filename3 = folderName+"shape_weights.csv";
		assert(fs::exists(filename)==true);
		assert(fs::exists(filename2)==true);
		assert(fs::exists(filename3)==true);
		fstream fs(filename);
		fstream fs2(filename2);
		fstream fs3(filename3);
		if(fs.is_open() && fs2.is_open() && fs3.is_open()) {
			String temp;
			vector<String> vec;
			vector<String> vec2;
			vector<float> vec3;
			while(getline(fs,temp)) {
				getSubstr(temp,',',vec);
				for(unsigned int i=0; i<vec.size(); i++) {
					if(vec.at(i)!="") {
						if(i>0) {
							vec2.push_back(vec.at(i));
						}
					}
				}
				ShapeMatch::shiftingRules.push_back(vec2);
				vec2.clear();
			}
			getline(fs2,temp);
			while(getline(fs2,temp)) {
				getSubstr(temp,',',vec);
				for(unsigned int i=0; i<vec.size(); i++) {
					if(i>0) {
						vec3.push_back(atof(vec.at(i).c_str()));
					}
				}
				ShapeMatch::shiftingPenalties.push_back(vec3);
				vec3.clear();
			}
			getline(fs3,temp);
			while(getline(fs3,temp)) {
				getSubstr(temp,',',vec);
				ShapeMatch::shapeWeightsVec.push_back(atof(vec.at(1).c_str()));
				ShapeMatch::shapeWeightsVec2.push_back(atof(vec.at(2).c_str()));
			}
			assert(ShapeMatch::shapeNames.size()==ShapeMatch::shiftingPenalties.size());
			assert(ShapeMatch::shapeNames.size()==ShapeMatch::shiftingRules.size());
			assert(ShapeMatch::shapeNames.size()==ShapeMatch::shapeWeightsVec.size());
			assert(ShapeMatch::shapeNames.size()==ShapeMatch::shapeWeightsVec2.size());
			fs.close();
			fs2.close();
			fs3.close();
			return true;
		} else {
			cout << "Importing ShapeMatch Thresholds failed!" << endl;
			return false;
		}
	}
	return true;
}

vector<vector<int> > ShapeMatch::getIslandVecIdxByArea(vector<vector<vector<Islands> > > &islandVec) {
	vector<vector<int> > islandVecIdx;
	vector<int> idxVec;
	vector<int> areaVec;
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				int area = islandVec.at(i).at(j).at(k).area();
				areaVec.push_back(area);
				idxVec.push_back(i);
				idxVec.push_back(j);
				idxVec.push_back(k);
				islandVecIdx.push_back(idxVec);
				idxVec.clear();
			}
		}
	}
	assert(areaVec.size()>0);
	vector<int> origPos;
	jaysort(areaVec,origPos,1);
	vector<vector<int> > islandVecIdxSorted;
	for(unsigned int i=0; i<areaVec.size(); i++) {
		islandVecIdxSorted.push_back(islandVecIdx.at(origPos.at(i)));
	}
	return islandVecIdxSorted;
}


//! shifts island specified by rank, 0=default=largest
bool ShapeMatch::shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int newShape, int rank) {
	vector<vector<vector<Islands> > > newIslandVec = islandVec;
	vector<vector<int> > islandVecIdxSorted = this->getIslandVecIdxByArea(islandVec);
	int shape = islandVecIdxSorted.at(rank).at(0);
	int shade = islandVecIdxSorted.at(rank).at(1);
	int isl = islandVecIdxSorted.at(rank).at(2);
	assert(shape==shapeNum);

	int new_shape = newShape;
	islandVec.at(shapeNum).at(shade).at(isl).isShapeShifted() = true;
	newIslandVec.at(new_shape).at(shade).push_back(islandVec.at(shapeNum).at(shade).at(isl));
	newIslandVec.at(new_shape).at(shade).back().prevShape() = shapeNum;
	newIslandVec.at(new_shape).at(shade).back().shape() = new_shape;
	newIslandVec.at(shapeNum).at(shade).erase(newIslandVec.at(shapeNum).at(shade).begin()+isl);
	islandVec = newIslandVec;
	return true;
}

//! shifts specified shape to the left
//! only shifts the largest shape no matter what shade
bool ShapeMatch::shape_translation2(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int newShape, int shiftAmt) {
	vector<vector<vector<Islands> > > newIslandVec = islandVec;
	vector<int> areaVec;
	bool empty = true;
	for(unsigned int j=0; j<islandVec.at(shapeNum).size(); j++) {
		if(islandVec.at(shapeNum).at(j).size()>0) {
			int area = islandVec.at(shapeNum).at(j).front().area();
			areaVec.push_back(area);
			empty = false;
		}
		else {
			areaVec.push_back(0);
		}
	}
	if(!empty) {
		for(int i=0; i<shiftAmt; i++) {
			auto it = max_element(areaVec.begin(), areaVec.end());
			int max_shade_pos = it - areaVec.begin();
			int new_shape = newShape;
			if(areaVec.at(max_shade_pos)>0) {
				islandVec.at(shapeNum).at(max_shade_pos).front().isShapeShifted() = true;
				newIslandVec.at(new_shape).at(max_shade_pos).push_back(islandVec.at(shapeNum).at(max_shade_pos).front());
				newIslandVec.at(new_shape).at(max_shade_pos).back().prevShape() = shapeNum;
				newIslandVec.at(new_shape).at(max_shade_pos).back().shape() = new_shape;
				newIslandVec.at(shapeNum).at(max_shade_pos).erase(newIslandVec.at(shapeNum).at(max_shade_pos).begin());
				areaVec.at(max_shade_pos) = 0;
			}
		}
		islandVec = newIslandVec;
		return true;
	}
	return false;
}

void ShapeMatch::showIslands(vector<vector<vector<Islands> > > &islandVec) {
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				Mat img = islandVec.at(i).at(j).at(k).image();
				printf("Shade %d: ",i);
				cout << islandVec.at(i).at(j).at(k).shape_name() << endl;
				//imgshow(this->upIslandVec.at(i).at(j).image(),1);
			}
		}
	}
}

void ShapeMatch::printIslandAreas(vector<vector<vector<Islands> > > &islandVec) {
	cout << "**** PRINT ISLANDVEC AREAS ****" << endl;
	for(unsigned int i=0; i<islandVec.size(); i++) {
		printf("shape%d:\n",i);
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			printf("s%d: ",j);
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				int area = islandVec.at(i).at(j).at(k).area();
				printf("%d,",area);
			}
			cout << endl;
		}
	}
}

int ShapeMatch::numOfShapes() {
	return ShapeMatch::shapeNames.size();
}

//! assigns an island a new shape
//! does not re-sort by area
void ShapeMatch::moveShape(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int shadeNum, int islNum, int newShape) {
	islandVec.at(newShape).at(shadeNum).push_back(islandVec.at(shapeNum).at(shadeNum).at(islNum));
	islandVec.at(shapeNum).at(shadeNum).erase(islandVec.at(shapeNum).at(shadeNum).begin()+islNum);
}

vector<String> ShapeMatch::SHIFT() {
	return this->_SHIFT;
}

void ShapeMatch::printRules() {
	for(unsigned int i=0; i<ShapeMatch::shiftingRules.size(); i++) {
		printf("%s: ",ShapeMatch::shapeNames.at(i).c_str());
		for(unsigned int j=0; j<ShapeMatch::shiftingRules.at(i).size(); j++) {
			printf("%s, ",ShapeMatch::shiftingRules.at(i).at(j).c_str());
		}
		printf("\n");
	}
}

void ShapeMatch::printPenalties() {
	for(unsigned int i=0; i<ShapeMatch::shiftingPenalties.size(); i++) {
		for(unsigned int j=0; j<ShapeMatch::shiftingPenalties.at(i).size(); j++) {
			printf("%0.2f, ",ShapeMatch::shiftingPenalties.at(i).at(j));
		}
		printf("\n");
	}
}

void ShapeMatch::printWeights() {
	for(unsigned int i=0; i<ShapeMatch::shapeWeightsVec.size(); i++) {
		printf("%s: %f\n",ShapeMatch::shapeNames.at(i).c_str(),ShapeMatch::shapeWeightsVec.at(i));
	}
}

float ShapeMatch::applyShiftPenalty(float score, int shapeNum, int shapeNum2) {
	float penalty = ShapeMatch::shiftingPenalties.at(shapeNum).at(shapeNum2);
	float newScore = score * pow(2.0,penalty);
	return newScore;
}

float ShapeMatch::getShiftPenalty(int shapeNum, int shapeNum2) {
	float weight = ShapeMatch::shiftingPenalties.at(shapeNum).at(shapeNum2);
	float penalty = pow(2.0,weight);
	return penalty;
}

float ShapeMatch::applyShapeWeight(int shapeNum) {
	try {
		return ShapeMatch::shapeWeightsVec.at(shapeNum);
	} catch (const std::out_of_range &oor) {
		printf("ShapeMatch::applyShapeWeight() out of range!\n");
		printf("ShapeNum: %d\n", shapeNum);
		printf("ShapeWeightVec.size(): %lu\n",ShapeMatch::shapeWeightsVec.size());
		exit(1);
	}
}
