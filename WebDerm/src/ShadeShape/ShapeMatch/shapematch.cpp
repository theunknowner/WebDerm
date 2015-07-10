/*
 * shapematch.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#include "shapematch.h"
#include "../islands.h"

/******************* PUBLIC FUNCTIONS ******************/

// shifts specified shape to the left
bool ShapeMatch::shape_translation(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int shiftType) {
	if(SHIFT[shiftType]!="SHIFT_NONE") {
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
			if(SHIFT[shiftType] == "SHIFT_LEFT")
				new_shape = shapeNum - 1;
			if(SHIFT[shiftType] == "SHIFT_RIGHT")
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

//! assigns an island a new shape
//! does not re-sort by area
void ShapeMatch::moveShape(vector<vector<vector<Islands> > > &islandVec, int shapeNum, int shadeNum, int islNum, int newShape) {
	islandVec.at(newShape).at(shadeNum).push_back(islandVec.at(shapeNum).at(shadeNum).at(islNum));
	islandVec.at(shapeNum).at(shadeNum).erase(islandVec.at(shapeNum).at(shadeNum).begin()+islNum);
}
