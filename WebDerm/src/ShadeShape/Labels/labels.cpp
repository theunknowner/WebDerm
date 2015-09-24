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
					this->labelShapeShiftMap[label] = islandVec.at(i).at(j).at(k).isShapeShifted();
					this->labelShapeNumMap[label] = islandVec.at(i).at(j).at(k).shape();
					this->labelPrevShapeNumMap[label] = islandVec.at(i).at(j).at(k).prevShape();
					this->labelShadeLevelMap[label] = j;
				} else {
					cout << "Labels::create(): map<String,pair<int,float> > labelMap duplicate keys!!!" << endl;
					cout << label << endl;
					exit(1);
				}
			}
		}
	}
}

map<String,pair<int,float> >& Labels::getMap() {
	return this->labelMap;
}

map<String,int>& Labels::getShadeLevelMap() {
	return this->labelShadeLevelMap;
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
	return 0;
}

int Labels::area(int num) {
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		int i = distance(this->labelMap.begin(),it);
		if(i==num) {
			return it->second.first;
		}
	}
	return 0;
}

int& Labels::totalArea() {
	return this->labelTotalArea;
}

float Labels::relativeArea(String label) {
	auto it = this->labelMap.find(label);
	if(it!=this->labelMap.end()) {
		return it->second.second;
	}
	return 0.0;
}

float Labels::relativeArea(int num) {
	for(auto it=this->labelMap.begin(); it!=this->labelMap.end(); it++) {
		int i = distance(this->labelMap.begin(),it);
		if(i==num) {
			return it->second.second;
		}
	}
	return 0.0;
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

int Labels::getIndex(String label) {
	auto it = this->labelMap.find(label);
	if(it!=this->labelMap.end()) {
		int idx = distance(labelMap.begin(),it);
		return idx;
	}
	return -1;
}

String Labels::getShape(String label) {
	for(unsigned int i=0; i<ShapeMatch::shapeNames.size(); i++) {
		if(label.find(this->shapeName(i))!=string::npos) {
			return this->shapeName(i);
		}
	}
	return "";
}

String Labels::getShade(String label) {
	size_t pos = label.find("_") + 1;
	String shade = label.substr(pos,label.length()-pos);
	pos = shade.find("_") + 1;
	shade = shade.substr(pos,shade.length()-pos);
	pos = shade.find("_");
	return shade.substr(0,pos);
}

int Labels::getShadeLevel(String label) {
	if(this->labelShadeLevelMap.find(label)!=this->labelShadeLevelMap.end()) {
		return this->labelShadeLevelMap.at(label);
	}

	return -1;
}

void Labels::printCompareLabels(Labels &labels1, Labels &labels2, int markShifted) {
	auto labelMap1 = labels1.getMap();
	auto labelMap2 = labels2.getMap();
	for(auto it1=labelMap1.begin(), it2=labelMap2.begin(); it1!=labelMap1.end(), it2!=labelMap2.end(); it1++, it2++) {
		int i = distance(labelMap1.begin(),it1);
		bool isShifted = labels1.isShapeShifted(it1->first);
		if(markShifted==0 || !isShifted)
			printf("%d) %s: %d | %s: %d\n",i,it1->first.c_str(),it1->second.first,it2->first.c_str(),it2->second.first);
		else
			printf("%d) *%s: %d | %s: %d\n",i,it1->first.c_str(),it1->second.first,it2->first.c_str(),it2->second.first);
	}
}

bool Labels::isShapeShifted(String label) {
	if(this->labelShapeShiftMap.find(label)!=this->labelShapeShiftMap.end()) {
		return this->labelShapeShiftMap.at(label);
	}

	return false;
}

int Labels::getShapeNum(String label) {
	if(this->labelShapeNumMap.find(label)!=this->labelShapeNumMap.end()) {
		return this->labelShapeNumMap.at(label);
	}

	return -1;
}

int Labels::getPrevShapeNum(String label) {
	if(this->labelPrevShapeNumMap.find(label)!=this->labelPrevShapeNumMap.end()) {
		return this->labelPrevShapeNumMap.at(label);
	}

	return -1;
}

void Labels::writeCompareLabels(Labels &labels1, Labels &labels2, int markShifted) {
	String file = labels1.name() + "_" + labels2.name() + "_tr1_max_match_labels.txt";
	FILE * fp;
	fp = fopen(file.c_str(),"w");
	auto labelMap1 = labels1.getMap();
	auto labelMap2 = labels2.getMap();
	for(auto it1=labelMap1.begin(), it2=labelMap2.begin(); it1!=labelMap1.end(), it2!=labelMap2.end(); it1++, it2++) {
		int i = distance(labelMap1.begin(),it1);
		bool isShifted = labels1.isShapeShifted(it1->first);
		if(markShifted==0 || !isShifted)
			fprintf(fp,"%d) %s: %d | %s: %d\n",i,it1->first.c_str(),it1->second.first,it2->first.c_str(),it2->second.first);
		else
			fprintf(fp,"%d) *%s: %d | %s: %d\n",i,it1->first.c_str(),it1->second.first,it2->first.c_str(),it2->second.first);
	}
	fclose(fp);
}
