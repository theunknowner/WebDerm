/*
 * matlab.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: jason
 */

#include "matlab.h"

vector<double> Matlab::diff(vector<double> vec) {
	vector<double> _vec;
	try{
		for(unsigned int i=1; i<vec.size(); i++) {
			double val = vec.at(i) - vec.at(i-1);
			_vec.push_back(val);
		}
	}
	catch(const std::out_of_range &oor) {
		printf("Matlab::diff(vec) out of range!\n");
		printf("vec.size(): %lu\n",vec.size());
		exit(1);
	}
	return _vec;
}

vector<int> Matlab::diff(vector<int> vec) {
	vector<int> _vec;
	try{
		for(unsigned int i=1; i<vec.size(); i++) {
			int val = vec.at(i) - vec.at(i-1);
			_vec.push_back(val);
		}
	}
	catch(const std::out_of_range &oor) {
		printf("Matlab::diff(vec) out of range!\n");
		printf("vec.size(): %lu\n",vec.size());
		exit(1);
	}
	return _vec;
}

//! return vector of indexes+offset that does not equal "val" parameter
vector<int> Matlab::indexOfNonMatchElem(vector<double> vec, double val, double offset) {
	vector<int> idx;
	for(unsigned int i=0; i<vec.size(); i++) {
		if(vec.at(i)!=val) idx.push_back(i+offset);
	}
	return idx;
}

//! return vector of indexes+offset that does not equal "val" parameter
vector<int> Matlab::indexOfNonMatchElem(vector<int> vec, int val, int offset) {
	vector<int> idx;
	for(unsigned int i=0; i<vec.size(); i++) {
		if(vec.at(i)!=val) idx.push_back(i+offset);
	}
	return idx;
}

//! return vector of indexes+offset that equals "val" parameter
vector<int> Matlab::indexOfMatchElem(vector<double> vec, double val, double offset) {
	vector<int> idx;
	for(unsigned int i=0; i<vec.size(); i++) {
		if(vec.at(i)==val) idx.push_back(i+offset);
	}
	return idx;
}

//! return vector of indexes+offset that equals "val" parameter
vector<int> Matlab::indexOfMatchElem(vector<int> vec, int val, int offset) {
	vector<int> idx;
	for(unsigned int i=0; i<vec.size(); i++) {
		if(vec.at(i)==val) idx.push_back(i+offset);
	}
	return idx;
}
