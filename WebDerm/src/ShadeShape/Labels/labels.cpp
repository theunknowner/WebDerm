/*
 * labels.cpp
 *
 *  Created on: Jul 30, 2015
 *      Author: jason
 */

#include "labels.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "../StatSign/statsign.h"

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
	StatSign statSign;
	for(unsigned int i=0; i<islandVec.size(); i++) {
		for(unsigned int j=0; j<islandVec.at(i).size(); j++) {
			for(unsigned int k=0; k<islandVec.at(i).at(j).size(); k++) {
				Islands &isl = islandVec.at(i).at(j).at(k);
				String shape = this->shapeName(i);
				label = toString(i)+"_"+shape+"_s"+toString(j)+"_";
				label += Func::addDigitsForLabel(k,"0",3);
				isl.labelName() = label;

				if(this->labelMap.find(label)==this->labelMap.end()) {
					int area = islandVec.at(i).at(j).at(k).area();
					float relArea = area / totalArea;
					this->labelMap[label] = std::make_pair(area,relArea);
					this->labelShapeShiftMap[label] = islandVec.at(i).at(j).at(k).isShapeShifted();
					this->labelShapeNumMap[label] = islandVec.at(i).at(j).at(k).shape();
					this->labelPrevShapeNumMap[label] = islandVec.at(i).at(j).at(k).prevShape();
					this->labelShadeLevelMap[label] = j;

					//> to calculate the statistical signature of each label
					if(label.find("Excavated")!=string::npos) {
						vector<float> statSignVec = statSign.create(isl.nn_image(),relArea);
						this->labelStatSignMap[label] = statSignVec;
					}

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

map<String,int>& Labels::getShapeMap() {
	return this->labelShapeNumMap;
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

void Labels::setShadeLevel(String label, int level) {
	this->labelShadeLevelMap[label] = level;
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

//! returns the statistical signature of each label/urn/feature.
//! The total number of balls is stored in statSign[0].
vector<float> Labels::getStatSign(String label) {
	if(this->labelStatSignMap.find(label)!=this->labelStatSignMap.end()) {
		return this->labelStatSignMap.at(label);
	}
	return vector<float>();
}

map<String,vector<float>>& Labels::getStatSignMap() {
	return this->labelStatSignMap;
}

void Labels::printCompareLabels(Labels &labels1, Labels &labels2, int markShifted) {
	auto labelMap1 = labels1.getMap();
	auto labelMap2 = labels2.getMap();
	for(auto it1=labelMap1.begin(), it2=labelMap2.begin(); it1!=labelMap1.end(), it2!=labelMap2.end(); it1++, it2++) {
		int i = distance(labelMap1.begin(),it1);
		bool isShifted = labels1.isShapeShifted(it1->first);
		if(markShifted==0 || !isShifted)
			printf("%d) %s: %d(%f) | %s: %d(%f)\n",i,it1->first.c_str(),it1->second.first,it1->second.second,it2->first.c_str(),it2->second.first,it2->second.second);
		else
			printf("%d) *%s: %d(%f) | %s: %d(%f)\n",i,it1->first.c_str(),it1->second.first,it1->second.second,it2->first.c_str(),it2->second.first,it2->second.second);
	}
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
			fprintf(fp,"%d) %s: %d(%f) | %s: %d(%f)\n",i,it1->first.c_str(),it1->second.first,it1->second.second,it2->first.c_str(),it2->second.first,it2->second.second);
		else {
			int prevShape = labels1.getPrevShapeNum(it1->first);
			fprintf(fp,"%d) %d->%s: %d(%f) | %s: %d(%f)\n",i,prevShape,it1->first.c_str(),it1->second.first,it1->second.second,it2->first.c_str(),it2->second.first,it2->second.second);
		}
	}
	fclose(fp);
}

void Labels::printCompareStatSign(Labels &labels1, Labels &labels2, String label) {
	vector<float> statSignVec1 = labels1.getStatSign(label);
	vector<float> statSignVec2 = labels2.getStatSign(label);
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		float porp1 = (float)statSignVec1.at(i) / statSignVec1.at(0);
		float porp2 = (float)statSignVec2.at(i) / statSignVec2.at(0);
		try {
			printf("L%d: %0.2f(%f) | L%d: %0.2f(%f)\n",i,statSignVec1.at(i),porp1,i,statSignVec2.at(i),porp2);
		} catch(const std::out_of_range &oor) {
			printf("statSignVec1.size(): %lu\n",statSignVec1.size());
			printf("statSignVec2.size(): %lu\n",statSignVec2.size());
			printf("i: %d\n",i);
			exit(1);
		}
	}
	printf("Total: %0.2f | Total: %0.2f\n",statSignVec1.at(0), statSignVec2.at(0));
}

void Labels::writeCompareStatSign(Labels &labels1, Labels &labels2, String label, String fileType) {
	vector<float> statSignVec1 = labels1.getStatSign(label);
	vector<float> statSignVec2 = labels2.getStatSign(label);
	assert(statSignVec1.size()>0 && statSignVec2.size()>0);
	assert(statSignVec1.size()==statSignVec2.size());
	String file = labels1.name() + "_" + labels2.name() + "_" + label + "_stat_sign_compare." + fileType;
	FILE * fp;
	fp = fopen(file.c_str(),"w");
	for(unsigned int i=1; i<statSignVec1.size(); i++) {
		float porp1 = (float)statSignVec1.at(i) / statSignVec1.at(0);
		float porp2 = (float)statSignVec2.at(i) / statSignVec2.at(0);
		if(fileType=="txt") {
			fprintf(fp,"L%d: %d(%f) | L%d: %d(%f)\n",i,statSignVec1.at(i),porp1,i,statSignVec2.at(i),porp2);
		} else if(fileType=="csv") {
			fprintf(fp,"L%d,%0.2f,%f,L%d,%0.2f,%f\n",i,statSignVec1.at(i),porp1,i,statSignVec2.at(i),porp2);
		}
	}
	if(fileType=="txt") {
		fprintf(fp,"Total: %0.2f | Total: %0.2f\n",statSignVec1.at(0), statSignVec2.at(0));
	} else if(fileType=="csv") {
		fprintf(fp,"Total,%0.2f,Total,%0.2f\n",statSignVec1.at(0), statSignVec2.at(0));
	}
}
