/*
 * ssr_match.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "shadeshaperelation.h"
#include "../Labels/labels.h"

Labels ShadeShapeRelation::mergeLabels(Labels &labels) {
	Labels newLabels = labels;
	map<String,pair<int,float> > labelMap = newLabels.getLabels();
	map<String,pair<int,float> > merged_labels;
	for(auto it=labelMap.begin(); it!=labelMap.end(); it++) {
		String newLabel = it->first.substr(0,it->first.length()-4);
		if(merged_labels.find(newLabel)==merged_labels.end()) {
			merged_labels[newLabel] = it->second;
		}
		else {
			merged_labels[newLabel].first += it->second.first;
			merged_labels[newLabel].second += it->second.second;
		}
	}
	newLabels.getLabels() = merged_labels;
	return newLabels;
}

/* merges labels with the same shape and shade and
 * counts the relationship between the merged labels
 */
vector<vector<vector<int> > > ShadeShapeRelation::downScaleShadeShapeLabels(vector<vector<int> > &srm, Labels &labels, Labels &mergedLabels) {
	map<String,pair<int,float> > labelMap = labels.getLabels();
	map<String,pair<int,float> > merged_labels = mergedLabels.getLabels();

	vector<vector<vector<int> > > srmCount3D(merged_labels.size(),vector<vector<int> >(merged_labels.size(),vector<int>(this->rel_op.size(),0)));
	for(auto itY=labelMap.begin(); itY!=labelMap.end(); itY++) {
		int y = distance(labelMap.begin(),itY);
		String newLabel = itY->first.substr(0,itY->first.length()-4);
		auto itY2 = merged_labels.find(newLabel);
		int y2 = distance(merged_labels.begin(),itY2);
		auto itX = itY;
		itX++;
		for(; itX!=labelMap.end(); itX++) {
			int x = distance(labelMap.begin(), itX);
			newLabel = itX->first.substr(0,itX->first.length()-4);
			auto itX2 = merged_labels.find(newLabel);
			int x2 = distance(merged_labels.begin(),itX2);
			int rel_op_idx = srm.at(y).at(x);
			if(rel_op_idx>0) { //ignores "NULL" relations
				srmCount3D.at(y2).at(x2).at(rel_op_idx)++;
			}
		}
	}
	/*
	for(auto itY=merged_labels.begin(); itY!=merged_labels.end(); itY++) {
		int y = distance(merged_labels.begin(),itY);
		for(auto itX=merged_labels.begin(); itX!=merged_labels.end(); itX++) {
			int x = distance(merged_labels.begin(), itX);
			for(unsigned int z=0; z<srmCount3D.at(y).at(x).size(); z++) {
				String relOp = this->rel_op.at(z);
				int count = srmCount3D.at(y).at(x).at(z);
				if(count>0) {
					printf("[%s][%s][%s]: %d\n",itY->first.c_str(),itX->first.c_str(),relOp.c_str(),count);
				}
			}
		}
	}*/
	labels.getLabels() = merged_labels;
	return srmCount3D;
}

float ShadeShapeRelation::entropy(int count) {
	if(count==0) {
		return -0.75;
	} else if(count==1) {
		return 0.25;
	} else {
		return log10(count);
	}
}

//computes the entropy value for each label
float ShadeShapeRelation::entropy(ShadeShapeRelation &srrUP, vector<vector<vector<int> > > &srmUP, ShadeShapeRelation &srrDB, vector<vector<vector<int> > > &srmDB) {
	assert(srmUP.size()==srmDB.size());

	float totalEntropy = 1.0;
	for(unsigned int i=0; i<srmUP.size(); i++) {
		for(unsigned int j=0; j<srmUP.at(i).size(); j++) {
			int countUP = 0;
			int countDB = 0;
			for(unsigned int k=0; k<srmUP.at(i).at(j).size(); k++) {
				countUP = srmUP.at(i).at(j).at(k);
				countDB = srmDB.at(i).at(j).at(k);
				if(countUP>0 || countDB>0) {
					float entropyUP = this->entropy(countUP);
					float entropyDB = this->entropy(countDB);
					float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
					totalEntropy *= entropyVal;
				}
			}
		}
	}
	return totalEntropy;
}

float ShadeShapeRelation::srm_match(ShadeShapeRelation &srrDB, Labels &labels) {
	Labels mergedLabels = this->mergeLabels(labels);
	vector<vector<int> > srmDB = srrDB.get_srm();
	vector<vector<vector<int> > > srm3dUP = this->downScaleShadeShapeLabels(this->relationMatrix,labels,mergedLabels);
	vector<vector<vector<int> > > srm3dDB = this->downScaleShadeShapeLabels(srmDB,labels,mergedLabels);
	float matchVal = this->entropy(*this,srm3dUP,srrDB,srm3dDB);
	//printf("EntUP: %f\n",entropyUP);
	//printf("EntDB: %f\n",entropyDB);

	//return matchVal;
}
