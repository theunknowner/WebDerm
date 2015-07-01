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

bool ShadeMatch::shade_translation(vector<vector<vector<Islands> > > &islandVec, float totalArea) {
	float thresh = totalArea / 3.0;
	vector<vector<vector<Islands> > > newIslandVec = islandVec;
	vector<vector<int> > indexVec2d;
	vector<int> areaVec;
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
	vector<int> origPos;
	jaysort(areaVec,origPos);
	int sum = 0;
	for(int i=areaVec.size()-1; i>=0; i--) {
		int pos = origPos.at(i);
		int shape = indexVec2d.at(pos).at(0);
		int shade = indexVec2d.at(pos).at(1);
		int idx = indexVec2d.at(pos).at(2);
		int shade_shift = min(shade-1,0);
		newIslandVec.at(shape).at(shade_shift).push_back(islandVec.at(shape).at(shade).at(idx));
		newIslandVec.at(shape).at(shade).erase(newIslandVec.at(shape).at(shade).begin()+idx);

		sum += areaVec.at(i);
		if(sum>thresh) break;

		printf("shape%d_s%d_%d: %d\n",indexVec2d.at(pos).at(0),indexVec2d.at(pos).at(1),indexVec2d.at(pos).at(2),areaVec.at(i));
	}
	return false;
}
