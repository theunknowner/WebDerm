/*
 * labels.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

Labels::Labels() {}

Labels::Labels(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name) {
	this->create(islandVec,totalArea,name);
}

void Labels::create(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name) {
	this->labelName= name;
	String label;
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				String shape = this->shapeName(i);
				label = toString(i)+"_"+shape+"_s"+toString(j)+"_";
				label += Func::addDigitsForLabel(k,"0",3);
				islandVec.at(i).at(j).at(k).labelName() = label;
				if(this->labelMap.find(label)==this->labelMap.end()) {
					int area = islandVec.at(i).at(j).at(k).area();
					float relArea = area / totalArea;
					this->labelMap[label] = std::make_pair(area,relArea);
					this->_areaLabels[label] = area;
					this->_relAreaLabels[label] = relArea;
				} else {
					cout << "Labels::create(): map<String,pair<int,float> > labelMap duplicate keys!!!" << endl;
					cout << label << endl;
					exit(1);
				}
			}
		}
	}
}

map<String,pair<int,float> >& Labels::getLabels() {
	return this->labelMap;
}

map<String,int>& Labels::areaLabels() {
	if(this->_areaLabels.size()!=this->labelMap.size()) {
		for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
			this->_areaLabels[it->first] = it->second.first;
		}
	}
	return this->_areaLabels;
}

map<String,float>& Labels::relAreaLabels() {
	if(this->_relAreaLabels.size()!=this->labelMap.size()) {
		for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
			this->_relAreaLabels[it->first] = it->second.first;
		}
	}
	return this->_relAreaLabels;
}

int Labels::area(String label) {
	auto it = this->labelMap.find(label);
	if(it!=this->labelMap.end()) {
		return it->second.first;
	}
	return -1;
}

int Labels::area(int num) {
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		int i = distance(this->labelMap.begin(),it);
		if(i==num) {
			return it->second.first;
		}
	}
	return -1;
}

int Labels::totalArea() {
	int total=0;
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		total += it->second.first;
	}
	return total;
}

float Labels::relativeArea(String label) {
	auto it = this->labelMap.find(label);
	if(it!=this->labelMap.end()) {
		return it->second.second;
	}
	return -1.0;
}

float Labels::relativeArea(int num) {
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		int i = distance(this->labelMap.begin(),it);
		if(i==num) {
			return it->second.second;
		}
	}
	return -1.0;
}

float Labels::totalRelArea() {
	float total=0;
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		total += it->second.second;
	}
	return total;
}

int Labels::size() {
	return this->labelMap.size();
}

String Labels::at(int num) {
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		int i = distance(this->labelMap.begin(),it);
		if(i==num) {
			return it->first;
		}
	}
	return "";
}

String& Labels::name() {
	return this->labelName;
}

//! prints the labels and their areas;
void Labels::printLabels() {
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		int index = distance(labelMap.begin(),it);
		printf("%d) ",index);
		printf("%s: %d, %f\n",it->first.c_str(), it->second.first, it->second.second);
	}
}
