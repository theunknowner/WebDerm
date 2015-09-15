/*
 * jaysort.cpp
 *
 *  Created on: Nov 19, 2014
 *      Author: jason
 */

#include "jaysort.h"

//! sorts in ascending order
void jaysort(deque<double> &vec, deque<int> &origPos) {
	deque<deque<double> > vec2d(vec.size(),deque<double>(0,0));
	for(unsigned int i=0; i<vec.size(); i++) {
		vec2d.at(i).push_back(vec.at(i));
		vec2d.at(i).push_back(i);
	}
	std::sort(vec2d.begin(), vec2d.end(), [](const std::deque< double >& a, const std::deque< double >& b){ return a[0] < b[0]; } );
	deque<double> sortedVec;
	for(unsigned int i=0; i<vec2d.size(); i++) {
		sortedVec.push_back(vec2d.at(i).at(0));
		origPos.push_back(vec2d.at(i).at(1));
	}
	vec = sortedVec;
}

//! sorts in ascending order
void jaysort(deque<int> &vec, deque<int> &origPos) {
	deque<deque<int> > vec2d(vec.size(),deque<int>(0,0));
	for(unsigned int i=0; i<vec.size(); i++) {
		vec2d.at(i).push_back(vec.at(i));
		vec2d.at(i).push_back(i);
	}
	std::sort(vec2d.begin(), vec2d.end(), [](const std::deque< int >& a, const std::deque< int >& b){ return a[0] < b[0]; } );
	deque<int> sortedVec;
	for(unsigned int i=0; i<vec2d.size(); i++) {
		sortedVec.push_back(vec2d.at(i).at(0));
		origPos.push_back(vec2d.at(i).at(1));
	}
	vec = sortedVec;
}

//! sorts in ascending order
void jaysort(Mat &src, Mat &pos) {
	vector<double> sortedVec;
	vector<int> origPos;
	double val=0;
	vector<vector<double> > vec2d(src.rows,vector<double>(0,0));
	for(unsigned int i=0; i<src.rows; i++) {
		vec2d.at(i).push_back(src.at<float>(i,0));
		vec2d.at(i).push_back(i);
	}
	std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< double >& a, const std::vector< double >& b){ return a[0] < b[0]; } );
	for(unsigned int i=0; i<vec2d.size(); i++) {
		sortedVec.push_back(vec2d.at(i).at(0));
		origPos.push_back(vec2d.at(i).at(1));
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

//! sorts in ascending order
void jaysort(vector<double> &vec, vector<int> &origPos) {
	vector<vector<double> > vec2d(vec.size(),vector<double>(0,0));
	for(unsigned int i=0; i<vec.size(); i++) {
		vec2d.at(i).push_back(vec.at(i));
		vec2d.at(i).push_back(i);
	}
	std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< double >& a, const std::vector< double >& b){ return a[0] > b[0]; } );
	vector<double> sortedVec;
	for(unsigned int i=0; i<vec2d.size(); i++) {
		sortedVec.push_back(vec2d.at(i).at(0));
		origPos.push_back(vec2d.at(i).at(1));
	}
	vec = sortedVec;
}

//! flag=0 : ascending; flag=1 : descending
void jaysort(vector<int> &vec, vector<int> &origPos, int flag) {
	vector<vector<int> > vec2d(vec.size(),vector<int>(0,0));
	for(unsigned int i=0; i<vec.size(); i++) {
		vec2d.at(i).push_back(vec.at(i));
		vec2d.at(i).push_back(i);
	}
	if(flag==0)
		std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< int >& a, const std::vector< int >& b){ return a[0] < b[0]; } );
	if(flag==1)
		std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< int >& a, const std::vector< int >& b){ return a[0] > b[0]; } );

	vector<int> sortedVec;
	for(unsigned int i=0; i<vec2d.size(); i++) {
		sortedVec.push_back(vec2d.at(i).at(0));
		origPos.push_back(vec2d.at(i).at(1));
	}
	vec = sortedVec;
}

//! sorts in descending order
void jaysort(vector<float> &vec, vector<int> &origPos) {
	vector<vector<float> > vec2d(vec.size(),vector<float>(0,0));
	for(unsigned int i=0; i<vec.size(); i++) {
		vec2d.at(i).push_back(vec.at(i));
		vec2d.at(i).push_back(i);
	}
	std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< float >& a, const std::vector< float >& b){ return a[0] > b[0]; } );
	vector<float> sortedVec;
	for(unsigned int i=0; i<vec2d.size(); i++) {
		sortedVec.push_back(vec2d.at(i).at(0));
		origPos.push_back(vec2d.at(i).at(1));
	}
	vec = sortedVec;
}

//! flag=0 : ascending; flag=1 : descending
void jaysort(vector<vector<float> > &vec, vector<int> &origPos, int flag) {
	vector<vector<float> > vec2d(vec.size(),vector<float>(0,0));
	for(unsigned int i=0; i<vec.size(); i++) {
		vec2d.at(i).push_back(vec.at(i).at(0));
		vec2d.at(i).push_back(vec.at(i).at(1));
		vec2d.at(i).push_back(vec.at(i).at(2));
		vec2d.at(i).push_back(i);
	}
	if(flag==0)
		std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< float >& a, const std::vector< float >& b){ return a[0] < b[0]; } );
	if(flag==1)
		std::sort(vec2d.begin(), vec2d.end(), [](const std::vector< float >& a, const std::vector< float >& b){ return a[0] > b[0]; } );

	vector<vector<float> > sortedVec(vec2d.size(),vector<float>(0,0));
	for(unsigned int i=0; i<vec2d.size(); i++) {
		for(unsigned int j=0; j<vec2d.at(i).size()-1; j++) {
			sortedVec.at(i).push_back(vec2d.at(i).at(j));
		}
		origPos.push_back(vec2d.at(i).back());
	}
	vec = sortedVec;
}
