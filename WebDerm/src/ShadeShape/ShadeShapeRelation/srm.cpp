/*
 * srm.cpp
 *
 *  Created on: Aug 24, 2015
 *      Author: jason
 */

#include "srm.h"
#include "../Labels/labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/******************** PRIVATE FUNCTIONS **********************/

void Srm::setup_relationMatrix(Labels &labels) {
	assert(labels.size()>0);
	this->relationOpMatrix.clear();
	this->relationOpMatrix.resize(labels.size(),vector<int>(labels.size(),0));
	this->relationCountMatrix.clear();
	this->relationCountMatrix.resize(labels.size(),vector<int>(labels.size(),0));
	this->neighborLevelMatrix.clear();
	this->neighborLevelMatrix.resize(labels.size(),vector<int>(labels.size(),0));
}

Labels Srm::mergeLabels() {
	Labels newLabels = this->labels;
	map<String,pair<int,float> > labelMap = newLabels.getMap();
	map<String,pair<int,float> > merged_labels;
	for(auto it=labelMap.begin(); it!=labelMap.end(); it++) {
		String merged_key = it->first.substr(0,it->first.length()-4);

		if(merged_labels.find(merged_key)==merged_labels.end()) {
			merged_labels[merged_key] = it->second;
		}
		else {
			merged_labels[merged_key].first += it->second.first;
			merged_labels[merged_key].second += it->second.second;
		}
	}
	newLabels.setLabels(merged_labels);

	return newLabels;
}

/********************* PUBLIC FUNCTIONS ***********************/

Srm::Srm() {};
Srm::Srm(Labels &labels) {
	this->labels = labels;
	this->setup_relationMatrix(labels);
	this->max_neighbor_level=0;
}

//! return reference to SRM relationship between label1 & label2
int& Srm::relation(int index1, int index2) {
	return this->relationOpMatrix.at(index1).at(index2);
}

//! return reference to SRM relalationship count between label1 & label2;
int& Srm::relationCount(int index1, int index2) {
	return this->relationCountMatrix.at(index1).at(index2);
}

//! return reference to SRM neighbor level between label1 & label2
int& Srm::neighborLevel(int index1, int index2) {
	return this->neighborLevelMatrix.at(index1).at(index2);
}

size_t Srm::size() {
	return this->relationOpMatrix.size();
}
//! return max neighbor level
int Srm::maxNeighborLevel() {
	return this->max_neighbor_level;
}

void Srm::writeRelationMatrix(Labels &labels, String name) {
	name += "_srm.csv";
	FILE * fp;
	fp = fopen(name.c_str(),"w");
	map<String,pair<int,float> > labelMap = labels.getMap();
	fprintf(fp,",");
	for(auto it=labelMap.begin(); it!=labelMap.end(); it++) {
		fprintf(fp,"%s,",it->first.c_str());
	}
	fprintf(fp,"\n");
	auto it = labelMap.begin();
	for(unsigned int i=0; i<this->relationOpMatrix.size(); i++) {
		fprintf(fp,"%s,",it->first.c_str());
		it++;
		for(unsigned int j=0; j<this->relationOpMatrix.at(i).size(); j++) {
			int rel_op_idx = this->relationOpMatrix.at(i).at(j);
			String relOp = this->rel_op.at(rel_op_idx);
			fprintf(fp,"%s,",relOp.c_str());
			//fprintf(fp,"%d,",rel_op_idx);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

/* merges labels with the same shape and shade and
 * counts the relationship between the merged labels
 * along with areas for that relationship
 */
/*
pair<vector<vector<vector<vector<int> > > >,vector<vector<vector<vector<pair<int,int>> > > >> Srm::downScaleSrm(Labels &labels, Labels &mergedLabels) {
	map<String,pair<int,float> > labelMap = labels.getMap();
	map<String,pair<int,float> > merged_labels = mergedLabels.getMap();

	vector<vector<vector<vector<map<String,int>> > > > srmMarkMap(merged_labels.size(),vector<vector<vector<map<String,int>> > >(merged_labels.size(),vector<vector<map<String,int>> >(this->rel_op.size(),vector<map<String,int>>(relOpLevelSize,map<String,int>()))));
	vector<vector<vector<vector<int> > > > srmCount4D = Func::createVector4D(merged_labels.size(),merged_labels.size(),this->rel_op.size(),this->relOpLevelSize,0);
	vector<vector<vector<vector<pair<int,int>> > > > srmArea4D(merged_labels.size(),vector<vector<vector<pair<int,int>> > >(merged_labels.size(),vector<vector<pair<int,int>> >(this->rel_op.size(),vector<pair<int,int>>(relOpLevelSize,std::make_pair(0,0)))));

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
			int rel_op_idx = this->relationOpMatrix.at(y).at(x);
			int neighborNum = this->neighborLevelMatrix.at(y).at(x);
			if(rel_op_idx>0) { //ignores "NULL" relations
				try {
					int areaX = itX->second.first;
					srmCount4D.at(y2).at(x2).at(rel_op_idx).at(neighborNum)++;
					//for the x label
					if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itX->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
						srmArea4D.at(y2).at(x2).at(rel_op_idx).at(neighborNum).second += areaX;
						srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itX->first] = 1;
					}
					// for the y label
					if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itY->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
						srmArea4D.at(y2).at(x2).at(rel_op_idx).at(neighborNum).first += areaY;
						srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itY->first] = 1;
					}
				} catch (const std::out_of_range &oor) {
					printf("ShadeShapeRelation::downScaleSrm() out of range!\n");
					printf("relationMatrix.size(): %lu\n",this->relationOpMatrix.size());
					printf("srmArea4D.size(): %lux%lux%lux%lu\n",srmArea4D.size(),srmArea4D.at(0).size(),srmArea4D.at(0).at(0).size(),srmArea4D.at(0).at(0).at(0).size());
					printf("srmCount4D.size(): %lux%lux%lux%lu\n",srmCount4D.size(),srmCount4D.at(0).size(),srmCount4D.at(0).at(0).size(),srmCount4D.at(0).at(0).at(0).size());
					printf("y: %d, x: %d\n",y,x);
					printf("y2: %d, x2: %d\n",y2,x2);
					printf("rel_op_idx: %d\n",rel_op_idx);
					printf("neighborNum: %d\n",neighborNum);
					exit(1);
				}
			}
		}
	}
	auto srm4dPair = std::make_pair(srmCount4D,srmArea4D);
	return srm4dPair;
}*/

void Srm::downScaleSrm() {
	map<String,pair<int,float> > labelMap = this->labels.getMap();
	this->mergedLabels = this->mergeLabels();
	map<String,pair<int,float> > merged_labels = this->mergedLabels.getMap();

	vector<vector<vector<vector<map<String,int>> > > > srmMarkMap(merged_labels.size(),vector<vector<vector<map<String,int>> > >(merged_labels.size(),vector<vector<map<String,int>> >(this->rel_op.size(),vector<map<String,int>>(relOpLevelSize,map<String,int>()))));
	this->dsSrmCount = Func::createVector4D(merged_labels.size(),merged_labels.size(),this->rel_op.size(),this->relOpLevelSize,0);
	this->dsSrmArea.resize(merged_labels.size(),vector<vector<vector<pair<int,int>> > >(merged_labels.size(),vector<vector<pair<int,int>> >(this->rel_op.size(),vector<pair<int,int>>(relOpLevelSize,std::make_pair(0,0)))));

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
			int rel_op_idx = this->relationOpMatrix.at(y).at(x);
			int neighborNum = this->neighborLevelMatrix.at(y).at(x);
			if(rel_op_idx>0) { //ignores "NULL" relations
				try {
					int areaX = itX->second.first;
					this->dsSrmCount.at(y2).at(x2).at(rel_op_idx).at(neighborNum)++;
					//for the x label
					if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itX->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
						this->dsSrmArea.at(y2).at(x2).at(rel_op_idx).at(neighborNum).second += areaX;
						srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itX->first] = 1;
					}
					// for the y label
					if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itY->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
						this->dsSrmArea.at(y2).at(x2).at(rel_op_idx).at(neighborNum).first += areaY;
						srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itY->first] = 1;
					}
				} catch (const std::out_of_range &oor) {
					printf("ShadeShapeRelation::downScaleSrm() out of range!\n");
					printf("relationMatrix.size(): %lu\n",this->relationOpMatrix.size());
					printf("srmArea4D.size(): %lux%lux%lux%lu\n",this->dsSrmArea.size(),this->dsSrmArea.at(0).size(),this->dsSrmArea.at(0).at(0).size(),this->dsSrmArea.at(0).at(0).at(0).size());
					printf("srmCount4D.size(): %lux%lux%lux%lu\n",this->dsSrmCount.size(),this->dsSrmCount.at(0).size(),this->dsSrmCount.at(0).at(0).size(),this->dsSrmCount.at(0).at(0).at(0).size());
					printf("y: %d, x: %d\n",y,x);
					printf("y2: %d, x2: %d\n",y2,x2);
					printf("rel_op_idx: %d\n",rel_op_idx);
					printf("neighborNum: %d\n",neighborNum);
					exit(1);
				}
			}
		}
	}
}

Labels Srm::getLabels() {
	return this->labels;
}

Labels Srm::getMergedLabels() {
	return this->mergedLabels;
}


