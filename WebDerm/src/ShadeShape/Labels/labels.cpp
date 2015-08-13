/*
 * labels.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

/************************* PRIVATE FUNCTIONS ****************************/

void Labels::calcTotalArea() {
	this->labelTotalArea = 0;
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		this->labelTotalArea += it->second.first;
	}
}

/************************* PUBLIC FUNCTIONS *****************************/
Labels::Labels() {}

Labels::Labels(vector<vector<vector<Islands> > > &islandVec, float totalArea, String name) {
	this->create(islandVec,totalArea,name);
	this->calcTotalArea();
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

void Labels::setLabels(map<String,pair<int,float> > labels) {
	this->labelMap = labels;
	this->calcTotalArea();
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

int& Labels::totalArea() {
	return this->labelTotalArea;
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

size_t Labels::size() {
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

void Labels::printCompareLabels(Labels &labels1, Labels &labels2) {
	auto labelMap1 = labels1.getLabels();
	auto labelMap2 = labels2.getLabels();
	for(auto it1=labelMap1.begin(), it2=labelMap2.begin(); it1!=labelMap1.end(), it2!=labelMap2.end(); it1++, it2++) {
		printf("%s: %d | %s: %d\n",it1->first.c_str(),it1->second.first,it2->first.c_str(),it2->second.first);
	}
}
