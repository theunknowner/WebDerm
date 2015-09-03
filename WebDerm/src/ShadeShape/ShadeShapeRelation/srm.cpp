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
	this->relationAreaMatrix.clear();
	this->relationAreaMatrix.resize(labels.size(),vector<pair<int,int>>(labels.size(),make_pair(0,0)));
	this->relationTouchCountMatrix.clear();
	this->relationTouchCountMatrix.resize(labels.size(),vector<int>(labels.size(),0));
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

//! return reference to SRM DN touch count between label1 and label2
int& Srm::relationTouchCount(int index1, int index2) {
	return this->relationTouchCountMatrix.at(index1).at(index2);
}

pair<int,int>& Srm::relationArea(int index1, int index2) {
	return this->relationAreaMatrix.at(index1).at(index2);
}

size_t Srm::size() {
	return this->relationOpMatrix.size();
}
//! return max neighbor level
int& Srm::maxNeighborLevel() {
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

void Srm::writeNeighborLevelMatrix(Labels &labels, String name) {
	name += "_los.csv";
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
			int level = this->neighborLevelMatrix.at(i).at(j);
			fprintf(fp,"%d,",level);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void Srm::writeRelationCountMatrix(Labels &labels, String name) {
	name += "_count.csv";
	FILE * fp;
	fp = fopen(name.c_str(),"w");
	map<String,pair<int,float> > labelMap = labels.getMap();
	fprintf(fp,",");
	for(auto it=labelMap.begin(); it!=labelMap.end(); it++) {
		fprintf(fp,"%s,",it->first.c_str());
	}
	fprintf(fp,"\n");
	auto it = labelMap.begin();
	for(unsigned int i=0; i<this->relationCountMatrix.size(); i++) {
		fprintf(fp,"%s,",it->first.c_str());
		it++;
		for(unsigned int j=0; j<this->relationCountMatrix.at(i).size(); j++) {
			int count = this->relationCountMatrix.at(i).at(j);
			fprintf(fp,"%d,",count);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void Srm::downScaleSrm() {
	map<String,pair<int,float> > labelMap = this->labels.getMap();
	this->mergedLabels = this->mergeLabels();
	map<String,pair<int,float> > merged_labels = this->mergedLabels.getMap();

	vector<vector<vector<vector<map<String,int>> > > > srmMarkMap(merged_labels.size(),vector<vector<vector<map<String,int>> > >(merged_labels.size(),vector<vector<map<String,int>> >(this->rel_op.size(),vector<map<String,int>>(relOpLevelSize,map<String,int>()))));
	this->dsSrmCount = Func::createVector4D(merged_labels.size(),merged_labels.size(),this->rel_op.size(),this->relOpLevelSize,0);
	this->dsSrmArea.resize(merged_labels.size(),vector<vector<vector<pair<int,int>> > >(merged_labels.size(),vector<vector<pair<int,int>> >(this->rel_op.size(),vector<pair<int,int>>(relOpLevelSize,std::make_pair(0,0)))));
	this->mergedLabelContainer.resize(merged_labels.size(),vector<vector<vector<pair<vector<String>,vector<String>>> > >(merged_labels.size(),vector<vector<pair<vector<String>,vector<String>>> >(this->rel_op.size(),vector<pair<vector<String>,vector<String>>>(relOpLevelSize,std::make_pair(vector<String>(),vector<String>())))));

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
					String equationKey = itY->first + "<" + this->rel_op.at(rel_op_idx) + ">" + itX->first;
					if(this->equationMap.find(equationKey)==this->equationMap.end()) {
						this->equationMap[equationKey] = std::make_pair(itY->second.first,itX->second.first);
					}
					// for the x label
					if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itX->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
						this->dsSrmArea.at(y2).at(x2).at(rel_op_idx).at(neighborNum).second += areaX;
						srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itX->first] = 1;
						this->mergedLabelContainer.at(y2).at(x2).at(rel_op_idx).at(neighborNum).second.push_back(itX->first);
					}

					// for the y label
					if(srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).find(itY->first)==srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum).end()) {
						this->dsSrmArea.at(y2).at(x2).at(rel_op_idx).at(neighborNum).first += areaY;
						srmMarkMap.at(y2).at(x2).at(rel_op_idx).at(neighborNum)[itY->first] = 1;
						this->mergedLabelContainer.at(y2).at(x2).at(rel_op_idx).at(neighborNum).first.push_back(itY->first);
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

vector<vector<vector<vector<int> > > >& Srm::downscaleSrmCount() {
	return this->dsSrmCount;
}

vector<vector<vector<vector<pair<int,int>> > > >& Srm::downscaleSrmArea() {
	return this->dsSrmArea;
}

int Srm::getIndex(String label) {
	return this->labels.getIndex(label);
}

int Srm::area(String label) {
	return this->labels.area(label);
}
