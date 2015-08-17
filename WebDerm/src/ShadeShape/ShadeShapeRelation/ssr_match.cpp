/*
 * ssr_match.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "ssr_match.h"
#include "shadeshaperelation.h"
#include "../Labels/labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/**************************** PRIVATE FUNCTIONS ********************************/

Labels ShadeShapeRelationMatch::mergeLabels(Labels &labels) {
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
	newLabels.setLabels(merged_labels);
	return newLabels;
}

/* merges labels with the same shape and shade and
 * counts the relationship between the merged labels
 * along with areas for that relationship
 */
pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> ShadeShapeRelationMatch::downScaleSrm(ShadeShapeRelation &ssr, vector<vector<vector<int> > > &srm, Labels &labels, Labels &mergedLabels) {
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
			int rel_op_idx = srm.at(y).at(x).at(0);
			int neighborNum = srm.at(y).at(x).at(1);
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

float ShadeShapeRelationMatch::entropy(int count) {
	if(count==0) {
		return -1.0;
	} else {
		return log2(count);
	}
}

//! computes the entropy value for each label
float ShadeShapeRelationMatch::entropy(vector<vector<vector<vector<int> > > > srmCountUP,vector<vector<vector<vector<int> > > > srmAreaUP, Labels &upLabels, vector<vector<vector<vector<int> > > > srmCountDB, vector<vector<vector<vector<int> > > > srmAreaDB, Labels &dbLabels) {
	assert(srmCountUP.size()==srmCountDB.size());
	FILE * fp;
	String filename = "entropy_output_" + upLabels.name() + "_" + dbLabels.name() + ".txt";
	fp = fopen(filename.c_str(),"w");
	float totalEntropy = 0.0;
	int maxTotalArea = max(upLabels.totalArea(),dbLabels.totalArea());
	int sumOfArea=0;
	for(unsigned int i=0; i<srmCountUP.size(); i++) {
		String labelUP1 = upLabels.at(i);
		for(unsigned int j=0; j<srmCountUP.at(i).size(); j++) {
			String labelUP2 = upLabels.at(j);
			for(unsigned int k=0; k<srmCountUP.at(i).at(j).size(); k++) {
				String relOp = this->rel_op.at(k);
				int totalCountUP = 0, totalCountDB = 0;
				int totalAreaUP = 0, totalAreaDB = 0;
				for(int m=0; m<this->relOpLevelSize; m++) {
					totalCountUP += srmCountUP.at(i).at(j).at(k).at(m);
					totalCountDB += srmCountDB.at(i).at(j).at(k).at(m);
					totalAreaUP += srmAreaUP.at(i).at(j).at(k).at(m);
					totalAreaDB += srmAreaDB.at(i).at(j).at(k).at(m);
				}
				for(int m=0; m<this->relOpLevelSize; m++) {
					int countUP = srmCountUP.at(i).at(j).at(k).at(m);
					int countDB = srmCountDB.at(i).at(j).at(k).at(m);
					int areaUP = srmAreaUP.at(i).at(j).at(k).at(m);
					int areaDB = srmAreaDB.at(i).at(j).at(k).at(m);
					if(countUP>0 || countDB>0) {
						float entropyUP = this->entropy(countUP);
						float entropyDB = this->entropy(countDB);
						float entropyVal = (min(entropyUP,entropyDB)+1.0) / (max(entropyUP,entropyDB)+1.0);
						float areaValUP = totalAreaUP, areaValDB = totalAreaDB;
						if(k==SURR_BY) {
							int level = m<5 ? m : 5;
							areaValUP = (float)areaUP * ((float)countUP/totalCountUP) * this->rVal[level];
							areaValDB = (float)areaDB * ((float)countDB/totalCountDB) * this->rVal[level];
							/*if(i==3 && j==4 && k==4) {
								//printf("%d: %d : %d : %f : %f\n",m,countUP,areaUP,areaValUP,this->rVal[level]);
								printf("Level: %d\n",m);
								printf("CountUP: %d\n",countUP);
								printf("TotalCountUP: %d\n",totalCountUP);
								printf("AreaUP: %d\n",areaUP);
								printf("AreaValUP: %f\n",areaValUP);
								printf("R: %f\n",this->rVal[level]);
							}*/
						}
						float areaVal = min(areaValUP,areaValDB);
						float relArea = areaVal / maxTotalArea;
						float weightedEntropy = relArea * entropyVal;
						totalEntropy += weightedEntropy;
						sumOfArea += min(areaUP,areaDB);
						if(weightedEntropy>0) {
							fprintf(fp,"[%s][%s][%s]\n", labelUP1.c_str(),relOp.c_str(),labelUP2.c_str());
							fprintf(fp,"CountUP: %d, EntUP: %f, CountDB: %d, EntDB: %f\n",countUP,entropyUP,countDB,entropyDB);
							fprintf(fp,"EntropyVal: %f\n",entropyVal);
							fprintf(fp,"AreaUP: %d, AreaDB: %d\n",areaUP,areaDB);
							fprintf(fp,"MaxTotalArea: %d\n",maxTotalArea);
							fprintf(fp,"RelArea: %f\n",relArea);
							fprintf(fp,"SumOfArea: %d\n",sumOfArea);
							fprintf(fp,"WeightedEntropy: %f\n",weightedEntropy);
							fprintf(fp,"TotalEntropy: %f\n",totalEntropy);
							fprintf(fp,"----------------------------------\n");
						}
					}
				}
			}
		}
	}
	fclose(fp);
	if(maxTotalArea<sumOfArea)
		totalEntropy *= ((float)maxTotalArea / sumOfArea);
	return totalEntropy;
}

//! computes the entropy value for each label
float ShadeShapeRelationMatch::entropy(pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairUP, Labels &upLabels, pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairDB, Labels &dbLabels) {
	vector<vector<vector<vector<int> > > > srmCountUP = srmPairUP.first;
	vector<vector<vector<vector<int> > > > srmAreaUP = srmPairUP.second;
	vector<vector<vector<vector<int> > > > srmCountDB = srmPairDB.first;
	vector<vector<vector<vector<int> > > > srmAreaDB = srmPairDB.second;

	float totalEntropy = this->entropy(srmCountUP,srmAreaUP,upLabels,srmCountDB,srmAreaDB,dbLabels);
	return totalEntropy;
}


/*************************** PUBLIC FUNCTIONS ***********************************/

float ShadeShapeRelationMatch::srm_match(pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairUP, Labels &upMergedLabels, pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<int> > > >> &srmPairDB, Labels &dbMergedLabels) {
	float matchVal = this->entropy(srmPairUP,upMergedLabels,srmPairDB,dbMergedLabels);
	return matchVal;
}

float ShadeShapeRelationMatch::srm_match(ShadeShapeRelation &ssrUP, Labels &upLabels, ShadeShapeRelation &ssrDB, Labels &dbLabels) {
	vector<vector<vector<int> > > srmUP = ssrUP.get_srm();
	vector<vector<vector<int> > > srmDB = ssrDB.get_srm();
	Labels upMergedLabels = this->mergeLabels(upLabels);
	Labels dbMergedLabels = this->mergeLabels(dbLabels);
	auto srmPairUP = ssrUP.downScaleSrm(upLabels,upMergedLabels);
	auto srmPairDB = ssrDB.downScaleSrm(dbLabels,dbMergedLabels);
	float matchVal = this->srm_match(srmPairUP,upMergedLabels,srmPairDB,dbMergedLabels);
	//this->writeDownScaleSrm(ssrUP.name(),srmPairUP,upMergedLabels,upLabels.totalArea());
	//this->writeDownScaleSrm(ssrDB.name(),srmPairDB,dbMergedLabels,dbLabels.totalArea());
	//this->printDownScaleSrm(srm3dUP,upMergedLabels);
	//cout << "-----------------------" << endl;
	//this->printDownScaleSrm(srm3dDB,dbMergedLabels);
	return matchVal;
}

void ShadeShapeRelationMatch::writeDownScaleSrm(String name, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels, int totalArea) {
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
	fprintf(fp,"TotalArea: %d\n",totalArea);
	fclose(fp);
}

void ShadeShapeRelationMatch::printDownScaleSrm(pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &mergedLabels) {
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
					printf("[%s][%s][%s]: %d : %d\n",itY->first.c_str(),relOp.c_str(),itX->first.c_str(),area,count);
				}
			}
		}
	}
}

void ShadeShapeRelationMatch::importDownScaleSrms(String file, pair<vector<vector<vector<int> > >,vector<vector<vector<int> > >> &srmPair, Labels &labels) {
	fstream fs(file);
	if(fs.is_open()) {
		map<String,pair<int,float> > srmLabels;
		String temp;
		char beginDelimit = '[';
		char endDelimit = ']';
		char separator = ':';
		vector<String> vec;
		vector<String> vec2;
		int area=0, count=0;
		while(getline(fs,temp)) {
			if(temp.find("TotalArea")==string::npos) {
				getSubstr(temp,separator,vec);
				Func::getSubstr(vec.at(0),beginDelimit,endDelimit,vec2);
				for(unsigned int i=0; i<vec2.size(); i++) {
					if(i==0 || i==2) {
						if(srmLabels.find(vec2.at(i))==srmLabels.end()) {
							srmLabels[vec2.at(i)] = std::make_pair(1,1.0);
						}
					}
				}
			}
		}
		fs.close();
		fs.open(file);
		vector<vector<vector<int> > > srmCount(srmLabels.size(),vector<vector<int> >(srmLabels.size(),vector<int>(this->rel_op.size(),0)));
		vector<vector<vector<int> > > srmArea(srmLabels.size(),vector<vector<int> >(srmLabels.size(),vector<int>(this->rel_op.size(),0)));
		while(getline(fs,temp)) {
			getSubstr(temp,separator,vec);
			if(temp.find("TotalArea")==string::npos) {
				Func::getSubstr(vec.at(0),beginDelimit,endDelimit,vec2);
				String labelY = vec2.at(0);
				String labelX = vec2.at(2);
				String relOp = vec2.at(1);
				int area = atoi(vec.at(1).c_str());
				int count = atoi(vec.at(2).c_str());
				auto itY = srmLabels.find(labelY);
				auto itX = srmLabels.find(labelX);
				int y = distance(srmLabels.begin(),itY);
				int x = distance(srmLabels.begin(),itX);
				int relOpIdx = this->getRelOpIndex(relOp);
				srmCount.at(y).at(x).at(relOpIdx) = count;
				srmArea.at(y).at(x).at(relOpIdx) = area;
			} else {
				int totalArea = atoi(vec.at(1).c_str());
				labels.totalArea() = totalArea;
			}
		}
		fs.close();
		srmPair = std::make_pair(srmCount,srmArea);
		labels.setLabels(srmLabels);
	}
	//this->printDownScaleSrm(srmPair,labels);
}
