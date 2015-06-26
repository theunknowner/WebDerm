/*
 * jaysort.cpp
 *
 *  Created on: Nov 19, 2014
 *      Author: jason
 */

#include "jaysort.h"

void jaysort(deque<double> &vec, deque<int> &origPos) {
	deque<double> sortedVec;
	origPos.clear();
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
					if(val>=sortedVec.at(j) && val<=sortedVec.at(j+1)) {
						sortedVec.insert(sortedVec.begin()+j+1,val);
						origPos.insert(origPos.begin()+j+1,i);
						break;
					}
					else if(val<=sortedVec.at(j)) {
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

void jaysort(deque<deque<double> > &vec, deque<int> &origPos, deque<int> sortKey) {
	for(unsigned int k=0; k<sortKey.size(); k++) {
		deque<deque<double> > sortedVec(vec.size(),deque<double>(0,0));
		deque<int> pos;
		int key = sortKey.at(k);
		for(unsigned int i=0; i<vec.at(key).size(); i++) {
			double val=vec.at(key).at(i);
			if(sortedVec.at(key).size()==0) {
				for(unsigned int m=0; m<vec.size(); m++) {
					double val2 = vec.at(m).at(i);
					sortedVec.at(m).push_back(val2);
				}
				pos.push_back(i);
			}
			else {
				for(unsigned int j=0; j<sortedVec.at(key).size(); j++) {
					if((sortedVec.at(key).size()-j)<=1) {
						if(val>=sortedVec.at(key).at(j)) {
							for(unsigned int m=0; m<vec.size(); m++) {
								double val2 = vec.at(m).at(i);
								sortedVec.at(m).push_back(val2);
							}
							pos.push_back(i);
							break;
						}
						else {
							for(unsigned int m=0; m<vec.size(); m++) {
								double val2 = vec.at(m).at(i);
								sortedVec.at(m).push_front(val2);
							}
							pos.push_front(i);
							break;
						}
					}
					else {
						if(val>=sortedVec.at(key).at(j) && val<=sortedVec.at(key).at(j+1)) {
							for(unsigned int m=0; m<vec.size(); m++) {
								double val2 = vec.at(m).at(i);
								sortedVec.at(m).insert(sortedVec.at(m).begin()+j+1,val2);
							}
							pos.insert(pos.begin()+j+1,i);
							break;
						}
						else if(val<=sortedVec.at(key).at(j)) {
							for(unsigned int m=0; m<vec.size(); m++) {
								double val2 = vec.at(m).at(i);
								sortedVec.at(m).push_front(val2);
							}
							pos.push_front(i);
							break;
						}
					}
				}
			}
		}
		vec = sortedVec;
		if(origPos.size()==0)
			origPos = pos;
		pos.clear();
		sortedVec.clear();
		sortedVec.shrink_to_fit();
	}
}

void jaysort(vector<double> &vec, vector<int> &origPos) {
	vector<double> sortedVec;
	origPos.clear();
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
						sortedVec.insert(sortedVec.begin(),val);
						origPos.insert(origPos.begin(),i);
						break;
					}
				}
				else {
					if(val>=sortedVec.at(j) && val<=sortedVec.at(j+1)) {
						sortedVec.insert(sortedVec.begin()+j+1,val);
						origPos.insert(origPos.begin()+j+1,i);
						break;
					}
					else if(val<=sortedVec.at(j)) {
						sortedVec.insert(sortedVec.begin(),val);
						origPos.insert(origPos.begin(),i);
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

void jaysort(vector<int> &vec, vector<int> &origPos) {
	vector<int> sortedVec;
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
						sortedVec.insert(sortedVec.begin(),val);
						origPos.insert(origPos.begin(),i);
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
						sortedVec.insert(sortedVec.begin(),val);
						origPos.insert(origPos.begin(),i);
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
