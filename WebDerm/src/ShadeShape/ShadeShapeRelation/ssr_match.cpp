/*
 * ssr_match.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "shadeshaperelation.h"
#include "../Labels/labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

Labels ShadeShapeRelation::mergeLabels(Labels &labels, vector<vector<int> > &srm) {
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
 * along with areas for that relationship
 */
pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> ShadeShapeRelation::downScaleSrm(ShadeShapeRelation &ssr, vector<vector<int> > &srm, Labels &labels, Labels &mergedLabels) {
	map<String,pair<int,float> > labelMap = labels.getLabels();
	map<String,pair<int,float> > merged_labels = mergedLabels.getLabels();

	vector<vector<vector<int> > >  srmCount3D(merged_labels.size(),vector<vector<int> >(merged_labels.size(),vector<int>(this->rel_op.size(),0)));
	vector<vector<vector<int> > > srmArea3D(merged_labels.size(),vector<vector<int> >(merged_labels.size(),vector<int>(this->rel_op.size(),0)));
	vector<vector<vector<map<String,int>> > > srmMarkMap(merged_labels.size(),vector<vector<map<String,int>> >(merged_labels.size(),vector<map<String,int>>(this->rel_op.size(),map<String,int>())));
	for(auto itY=labelMap.begin(); itY!=labelMap.end(); itY++) {
		int y = distance(labelMap.begin(),itY);
		String newLabelY = itY->first.substr(0,itY->first.length()-4);
		auto itY2 = merged_labels.find(newLabelY);
		int y2 = distance(merged_labels.begin(),itY2);
		int areaY = itY->second.first;
		auto itX = itY;
		itX++;
		for(; itX!=labelMap.end(); itX++) {
			int x = distance(labelMap.begin(), itX);
			String newLabelX = itX->first.substr(0,itX->first.length()-4);
			auto itX2 = merged_labels.find(newLabelX);
			int x2 = distance(merged_labels.begin(),itX2);
			int rel_op_idx = srm.at(y).at(x);
			if(rel_op_idx>0) { //ignores "NULL" relations
				int areaX = itX->second.first;
				srmCount3D.at(y2).at(x2).at(rel_op_idx)++;
				if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).find(itX->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).end()) {
					srmArea3D.at(y2).at(x2).at(rel_op_idx) += areaX;
					srmMarkMap.at(y2).at(x2).at(rel_op_idx)[itX->first] = 1;
				}
				if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).find(itY->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).end()) {
					srmArea3D.at(y2).at(x2).at(rel_op_idx) += areaY;
					srmMarkMap.at(y2).at(x2).at(rel_op_idx)[itY->first] = 1;
				}
			}
		}
	}
	auto srm3dPair = std::make_pair(srmCount3D,srmArea3D);
	return srm3dPair;
}

float ShadeShapeRelation::entropy(int count) {
	if(count==0) {
		return -1.0;
	} else {
		return log2(count);
	}
}

//computes the entropy value for each label
float ShadeShapeRelation::entropy(pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPairUP, Labels &upLabels, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPairDB, Labels &dbLabels) {
	vector<vector<vector<int> > > srmCountUP = srmPairUP.first;
	vector<vector<vector<int> > > srmAreaUP = srmPairUP.second;
	vector<vector<vector<int> > > srmCountDB = srmPairDB.first;
	vector<vector<vector<int> > > srmAreaDB = srmPairDB.second;

	assert(srmCountUP.size()==srmCountDB.size());
	FILE * fp;
	fp = fopen("entropy_output.txt","w");
	float totalEntropy = 0.0;
	int maxTotalArea = max(upLabels.totalArea(),dbLabels.totalArea());
	for(unsigned int i=0; i<srmCountUP.size(); i++) {
		String labelUP1 = upLabels.at(i);
		for(unsigned int j=0; j<srmCountUP.at(i).size(); j++) {
			String labelUP2 = upLabels.at(j);
			for(unsigned int k=0; k<srmCountUP.at(i).at(j).size(); k++) {
				int countUP = srmCountUP.at(i).at(j).at(k);
				int countDB = srmCountDB.at(i).at(j).at(k);
				int areaUP = srmAreaUP.at(i).at(j).at(k);
				int areaDB = srmAreaDB.at(i).at(j).at(k);
				String relOp = this->rel_op.at(k);
				if(countUP>0 || countDB>0) {
					float entropyUP = this->entropy(countUP);
					float entropyDB = this->entropy(countDB);
					float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
					float areaVal = min(areaUP,areaDB);
					fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
					fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
					fprintf(fp,"EntropyVal: %f\n",entropyVal);
					fprintf(fp,"AreaUP: %d, AreaDB: %d\n",areaUP,areaDB);
					fprintf(fp,"AreaVal: %f\n",areaVal);
					fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
					areaVal /= maxTotalArea;
					fprintf(fp,"RelArea: %f\n",areaVal);

					totalEntropy += (areaVal * entropyVal);
					fprintf(fp,"WeightedEntropy: %f\n",areaVal * entropyVal);
					fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
				}
			}
		}
	}
	fclose(fp);
	return totalEntropy;
}

float ShadeShapeRelation::srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels) {
	vector<vector<int> > srmUP = ssrUP.get_srm();
	vector<vector<int> > srmDB = ssrDB.get_srm();
	Labels upMergedLabels = this->mergeLabels(upLabels,srmUP);
	Labels dbMergedLabels = this->mergeLabels(dbLabels,srmDB);
	auto srmPairUP = this->downScaleSrm(ssrUP,srmUP,upLabels,upMergedLabels);
	auto srmPairDB = this->downScaleSrm(ssrDB,srmDB,dbLabels,dbMergedLabels);
	float matchVal = this->entropy(srmPairUP,upMergedLabels,srmPairDB,dbMergedLabels);
	this->writeDownScaleSrm(ssrUP.name(),srmPairUP,upMergedLabels);
	this->writeDownScaleSrm(ssrDB.name(),srmPairDB,dbMergedLabels);
	//this->printDownScaleSrm(srm3dUP,upMergedLabels);
	//cout << "-----------------------" << endl;
	//this->printDownScaleSrm(srm3dDB,dbMergedLabels);
	return matchVal;
}

void ShadeShapeRelation::writeDownScaleSrm(String name, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels) {
	String filename = name + "_downscale_srm.txt";
	FILE *fp;
	fp = fopen(filename.c_str(),"w");
	auto merged_labels = mergedLabels.getLabels();
	vector<vector<vector<int> > > srmCount = srmPair.first;
	vector<vector<vector<int> > > srmArea = srmPair.second;
	for(auto itY=merged_labels.begin(); itY!=merged_labels.end(); itY++) {
		int y = distance(merged_labels.begin(),itY);
		for(auto itX=merged_labels.begin(); itX!=merged_labels.end(); itX++) {
			int x = distance(merged_labels.begin(), itX);
			for(unsigned int z=0; z<srmCount.at(y).at(x).size(); z++) {
				String relOp = this->rel_op.at(z);
				int count = srmCount.at(y).at(x).at(z);
				int area = srmArea.at(y).at(x).at(z);
				if(count>0) {
					fprintf(fp,"[%s][%s][%s]: %d : %d\n",itY->first.c_str(),relOp.c_str(),itX->first.c_str(),area,count);
				}
			}
		}
	}
	fclose(fp);
}

void ShadeShapeRelation::printDownScaleSrm(pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels) {
	auto merged_labels = mergedLabels.getLabels();
	vector<vector<vector<int> > > srmCount = srmPair.first;
	vector<vector<vector<int> > > srmArea = srmPair.second;
	for(auto itY=merged_labels.begin(); itY!=merged_labels.end(); itY++) {
		int y = distance(merged_labels.begin(),itY);
		for(auto itX=merged_labels.begin(); itX!=merged_labels.end(); itX++) {
			int x = distance(merged_labels.begin(), itX);
			for(unsigned int z=0; z<srmCount.at(y).at(x).size(); z++) {
				String relOp = this->rel_op.at(z);
				int count = srmCount.at(y).at(x).at(z);
				int area = srmArea.at(y).at(x).at(z);
				if(count>0) {
					printf("[%s][%s][%s]: %d : %d\n",itY->first.c_str(),relOp.c_str(),itX->first.c_str(),count,area);
				}
			}
		}
	}
}
