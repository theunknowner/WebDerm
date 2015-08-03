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
vector<vector<vector<int> > > ShadeShapeRelation::downScaleSrm(ShadeShapeRelation &ssr, vector<vector<int> > &srm, Labels &labels, Labels &mergedLabels) {
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

	String filename = ssr.name() + "_downscale_srm.txt";
	FILE *fp;
	fp = fopen(filename.c_str(),"w");

	for(auto itY=merged_labels.begin(); itY!=merged_labels.end(); itY++) {
		int y = distance(merged_labels.begin(),itY);
		for(auto itX=merged_labels.begin(); itX!=merged_labels.end(); itX++) {
			int x = distance(merged_labels.begin(), itX);
			for(unsigned int z=0; z<srmCount3D.at(y).at(x).size(); z++) {
				String relOp = this->rel_op.at(z);
				int count = srmCount3D.at(y).at(x).at(z);
				if(count>0) {
					//printf("[%s][%s][%s]: %d\n",itY->first.c_str(),relOp.c_str(),itX->first.c_str(),count);
					fprintf(fp,"[%s][%s][%s]: %d\n",itY->first.c_str(),relOp.c_str(),itX->first.c_str(),count);
				}
			}
		}
	}
	fclose(fp);
	return srmCount3D;
}

float ShadeShapeRelation::entropy(int count) {
	if(count==0) {
		return -1.0;
	} else {
		return log2(count);
	}
}

//computes the entropy value for each label
float ShadeShapeRelation::entropy(vector<vector<vector<int> > > &srmUP, Labels &upLabels, vector<vector<vector<int> > > &srmDB, Labels &dbLabels) {
	assert(srmUP.size()==srmDB.size());

	float totalEntropy = 0.0;
	int maxTotalArea = max(upLabels.totalArea(),dbLabels.totalArea());
	for(unsigned int i=0; i<srmUP.size(); i++) {
		int areaUP1 = upLabels.area(i);
		int areaDB1 = dbLabels.area(i);
		String labelUP1 = upLabels.at(i);
		for(unsigned int j=0; j<srmUP.at(i).size(); j++) {
			int areaUP2 = upLabels.area(j);
			int areaDB2 = dbLabels.area(j);
			String labelUP2 = upLabels.at(j);
			int countUP = 0;
			int countDB = 0;
			for(unsigned int k=0; k<srmUP.at(i).at(j).size(); k++) {
				countUP = srmUP.at(i).at(j).at(k);
				countDB = srmDB.at(i).at(j).at(k);
				String relOp = this->rel_op.at(k);
				if(countUP>0 || countDB>0) {
					float entropyUP = this->entropy(countUP);
					float entropyDB = this->entropy(countDB);
					float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
					float areaVal = min(areaUP1+areaUP2,areaDB1+areaDB2);
					printf("[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
					printf("CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
					printf("EntropyVal: %f\n",entropyVal);
					printf("AreaUP1: %d, AreaUP2: %d, AreaDB1: %d, AreaDB2: %d\n",areaUP1,areaUP2,areaDB1,areaDB2);
					printf("AreaVal: %f\n",areaVal);
					printf("MaxTotalArea: %d\n",maxTotalArea);
					areaVal /= maxTotalArea;
					printf("NewAreaVal: %f\n",areaVal);

					totalEntropy += (areaVal * entropyVal);
					printf("TotalEntropy: %f\n",totalEntropy);
				}
			}
		}
	}
	return totalEntropy;
}

float ShadeShapeRelation::srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels) {
	Labels upMergedLabels = this->mergeLabels(upLabels);
	Labels dbMergedLabels = this->mergeLabels(dbLabels);
	vector<vector<int> > srmUP = ssrUP.get_srm();
	vector<vector<int> > srmDB = ssrDB.get_srm();
	vector<vector<vector<int> > > srm3dUP = this->downScaleSrm(ssrUP,srmUP,upLabels,upMergedLabels);
	//cout << "-------------------" << endl;
	vector<vector<vector<int> > > srm3dDB = this->downScaleSrm(ssrDB,srmDB,dbLabels,dbMergedLabels);
	float matchVal = this->entropy(srm3dUP,upMergedLabels,srm3dDB,dbMergedLabels);
	return matchVal;
}
