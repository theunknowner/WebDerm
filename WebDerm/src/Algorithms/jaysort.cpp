/*
 * jaysort.cpp
 *
 *  Created on: Nov 19, 2014
 *      Author: jason
 */

#include "jaysort.h"

void jaysort(deque<double> &vec, deque<int> &origPos) {
	deque<double> sortedVec;
	double val=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		val=vec.at(i);
		if(sortedVec.size()==0) {
			sortedVec.push_back(val);
			origPos.push_back(i);
		}
		else {
			for(unsigned int j=0; j<sortedVec.size(); j++) {
				if((sortedVec.size()-j)<=1) {
					if(val>=sortedVec.at(j)) {
						sortedVec.push_back(val);
						origPos.push_back(i);
						break;
					}
					else {
						sortedVec.push_front(val);
						origPos.push_front(i);
						break;
					}
				}
				else {
					if(val>=sortedVec.at(j) && val<sortedVec.at(j+1)) {
						sortedVec.insert(sortedVec.begin()+j+1,val);
						origPos.insert(origPos.begin()+j+1,i);
						break;
					}
					if(val<=sortedVec.at(j)) {
						sortedVec.push_front(val);
						origPos.push_front(i);
						break;
					}
				}
			}
		}
	}
	vec = sortedVec;
	sortedVec.clear();
	sortedVec.shrink_to_fit();
}

void jaysort(deque<int> &vec, deque<int> &origPos) {
	deque<int> sortedVec;
	int val=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		val=vec.at(i);
		if(sortedVec.size()==0) {
			sortedVec.push_back(val);
			origPos.push_back(i);
		}
		else {
			for(unsigned int j=0; j<sortedVec.size(); j++) {
				if((sortedVec.size()-j)<=1) {
					if(val>=sortedVec.at(j)) {
						sortedVec.push_back(val);
						origPos.push_back(i);
						break;
					}
					else {
						sortedVec.push_front(val);
						origPos.push_front(i);
						break;
					}
				}
				else {
					if(val>=sortedVec.at(j) && val<sortedVec.at(j+1)) {
						sortedVec.insert(sortedVec.begin()+j+1,val);
						origPos.insert(origPos.begin()+j+1,i);
						break;
					}
					if(val<=sortedVec.at(j)) {
						sortedVec.push_front(val);
						origPos.push_front(i);
						break;
					}
				}
			}
		}
	}
	vec = sortedVec;
	sortedVec.clear();
	sortedVec.shrink_to_fit();
}

void jaysort(Mat &src, Mat &pos) {
	deque<double> sortedVec;
	deque<int> origPos;
	double val=0;
	for(int i=0; i<src.rows; i++) {
		val = src.at<float>(i,0);
		if(sortedVec.size()==0) {
			sortedVec.push_back(val);
			origPos.push_back(i);
		}
		else {
			for(unsigned int j=0; j<sortedVec.size(); j++) {
				if((sortedVec.size()-j)<=1) {
					if(val>=sortedVec.at(j)) {
						sortedVec.push_back(val);
						origPos.push_back(i);
						break;
					}
					else {
						sortedVec.push_front(val);
						origPos.push_front(i);
						break;
					}
				}
				else {
					if(val>=sortedVec.at(j) && val<sortedVec.at(j+1)) {
						sortedVec.insert(sortedVec.begin()+j+1,val);
						origPos.insert(origPos.begin()+j+1,i);
						break;
					}
					if(val<=sortedVec.at(j)) {
						sortedVec.push_front(val);
						origPos.push_front(i);
						break;
					}
				}
			}
		}
	}
	Mat sortedMat(sortedVec.size(), 1, src.type());
	Mat sortedPos(origPos.size(),1,CV_32S);
	for(unsigned int i=0; i<sortedVec.size(); i++) {
		sortedMat.at<float>(i,0) = sortedVec.at(i);
		sortedPos.at<int>(i,0) = origPos.at(i);
	}
	src = sortedMat;
	pos = sortedPos;
}
