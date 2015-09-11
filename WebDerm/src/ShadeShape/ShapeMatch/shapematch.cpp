/*
 * shapematch.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#include "shapematch.h"
#include "../islands.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

vector<String> ShapeMatch::shapeNames;
vector<vector<String> > ShapeMatch::shiftingRules;
vector<vector<float> > ShapeMatch::shiftingPenalties;
bool ShapeMatch::THRESH_IMPORTED = false;

/******************* PUBLIC FUNCTIONS ******************/

ShapeMatch::ShapeMatch() {
	if(!this->THRESH_IMPORTED)
		this->THRESH_IMPORTED = this->importThresholds();
}

bool ShapeMatch::importThresholds() {
	if(!this->THRESH_IMPORTED) {
		String folderName = "Thresholds/";
		String filename = folderName+"shape_shifting_rules.csv";
		String filename2 = folderName+"shape_penalties.csv";
		assert(fs::exists(filename)==true);
		assert(fs::exists(filename2)==true);
		fstream fs(filename);
		fstream fs2(filename2);
		if(fs.is_open() && fs2.is_open()) {
			String temp;
			vector<String> vec;
			vector<String> vec2;
			vector<float> vec3;
			while(getline(fs,temp)) {
				getSubstr(temp,',',vec);
				for(unsigned int i=0; i<vec.size(); i++) {
					if(vec.at(i)!="") {
						if(i==0) {
							this->shapeNames.push_back(vec.at(i));
						} else {
							vec2.push_back(vec.at(i));
						}
					}
				}
				this->shiftingRules.push_back(vec2);
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
				this->shiftingPenalties.push_back(vec3);
				vec3.clear();
			}
			assert(this->shapeNames.size()==this->shiftingPenalties.size());
			assert(this->shapeNames.size()==this->shiftingRules.size());
			fs.close();
			fs2.close();
			return true;
		} else {
			cout << "Importing ShapeMatch Thresholds failed!" << endl;
			return false;
		}
	}
	return true;
}

// shifts specified shape to the left
bool ShapeMatch::shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int shiftType) {
	if(ShapeMatch::_SHIFT[shiftType]!="SHIFT_NONE") {
		//printf("%d: %lu\n",shapeNum,islandVec.at(shapeNum).size());
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
			vector<int>::iterator it = max_element(areaVec.begin(),areaVec.end());
			int max_pos = it - areaVec.begin();
			int new_shape = shapeNum;
			if(ShapeMatch::_SHIFT[shiftType] == "SHIFT_LEFT")
				new_shape = shapeNum - 1;
			if(ShapeMatch::_SHIFT[shiftType] == "SHIFT_RIGHT")
				new_shape = shapeNum + 1;
			//printf("shape%d: %d | NewShape: %d\n",shapeNum,max_pos,new_shape);
			newIslandVec.at(new_shape).at(max_pos).push_back(islandVec.at(shapeNum).at(max_pos).front());
			newIslandVec.at(shapeNum).at(max_pos).erase(newIslandVec.at(shapeNum).at(max_pos).begin());
			islandVec = newIslandVec;
			return true;
		}
	}
	return false;
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
				islandVec.at(shapeNum).at(max_shade_pos).front().prevShape() = shapeNum;
				newIslandVec.at(new_shape).at(max_shade_pos).push_back(islandVec.at(shapeNum).at(max_shade_pos).front());
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
	return this->shapeNames.size();
}

String ShapeMatch::shapeName(int num) {
	return this->shapeNames.at(num);
}

int ShapeMatch::getShapeIndex(String shape) {
	auto it = std::find(ShapeMatch::shapeNames.begin(),ShapeMatch::shapeNames.end(),shape);
	if(it!=ShapeMatch::shapeNames.end())
		return distance(ShapeMatch::shapeNames.begin(),it);
	else {
		printf("ShapeMatch:getShapeIndex() %s does not exist\n",shape.c_str());
		exit(1);
	}
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
