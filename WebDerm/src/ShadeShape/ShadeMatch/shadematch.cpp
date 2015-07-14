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

/*************** PRIVATE FUNCTIONS *******************/

/*************** PUBLIC FUNCTIONS ********************/

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
