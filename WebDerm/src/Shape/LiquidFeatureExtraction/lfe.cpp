/*
 * lfe.cpp
 *
 *  Created on: Dec 1, 2015
 *      Author: jason
 */

#include "lfe.h"
#include "../../functions.h"

//! coded for only 2 seed maps so far
//! code needs to be changed for more seed map comparisons
//! checks to see if the seed maps crossover on any Points
bool LiquidFeatureExtraction::doesSeedMapsCrossOver(vector<Mat> seed_map_vec) {
	vector<vector<Point> > point_map_vec(seed_map_vec.size(),vector<Point>(0,Point()));
	if(seed_map_vec.size()>1) {
		for(unsigned int i=0; i<seed_map_vec.size(); i++) {
			if(countNonZero(seed_map_vec.at(i))==0) return true;

			cv::findNonZero(seed_map_vec.at(i),point_map_vec.at(i));
		}
		vector<Point> first_point_map = point_map_vec.at(0);
		vector<Point> second_point_map = point_map_vec.at(1);
		for(unsigned int i=0; i<first_point_map.size(); i++) {
			auto it = std::find(second_point_map.begin(),second_point_map.end(),first_point_map.at(i));
			if(it!=second_point_map.end()) {
				return true;
			}
		}
		return false;
	}
	return true;
}

Mat LiquidFeatureExtraction::localExtraction(Mat src, Point seed, int steps) {

	int countSteps=0;
	Mat seed_map = Mat::zeros(src.rows,src.cols,CV_8U);
	deque<Point> ptVec;
	deque<Point> temp;
	int row=seed.y, col=seed.x;
	if(src.at<uchar>(row,col)>0 && seed_map.at<uchar>(row,col)==0) {
		ptVec.push_back(Point(col,row));
		seed_map.at<uchar>(row,col) = 255;
		temp.push_back(Point(col,row));
		while(temp.size()>0) {
			Point up(temp.at(0).x,temp.at(0).y-1);
			Point left(temp.at(0).x-1,temp.at(0).y);
			Point right(temp.at(0).x+1,temp.at(0).y);
			Point down(temp.at(0).x,temp.at(0).y+1);
			Point downLeft(temp.at(0).x-1,temp.at(0).y+1);
			Point downRight(temp.at(0).x+1,temp.at(0).y+1);
			Point upLeft(temp.at(0).x-1,temp.at(0).y-1);
			Point upRight(temp.at(0).x+1,temp.at(0).y-1);
			if(up.y>=0) {
				if(seed_map.at<uchar>(up)==0 && src.at<uchar>(up)>0) {
					ptVec.push_back(up);
					seed_map.at<uchar>(up)=255;
					temp.push_back(up);
				}
			}
			if(left.x>=0) {
				if(seed_map.at<uchar>(left)==0 && src.at<uchar>(left)>0) {
					ptVec.push_back(left);
					seed_map.at<uchar>(left)=255;
					temp.push_back(left);
				}
			}
			if(right.x<src.cols) {
				if(seed_map.at<uchar>(right)==0 && src.at<uchar>(right)>0) {
					ptVec.push_back(right);
					seed_map.at<uchar>(right)=255;
					temp.push_back(right);
				}
			}
			if(down.y<src.rows) {
				if(seed_map.at<uchar>(down)==0 && src.at<uchar>(down)>0) {
					ptVec.push_back(down);
					seed_map.at<uchar>(down)=255;
					temp.push_back(down);
				}
			}
			if(down.y<src.rows && left.x>=0) {
				if(seed_map.at<uchar>(downLeft)==0 && src.at<uchar>(downLeft)>0) {
					ptVec.push_back(downLeft);
					seed_map.at<uchar>(downLeft)=255;
					temp.push_back(downLeft);
				}
			}
			if(down.y<src.rows && right.x<src.cols) {
				if(seed_map.at<uchar>(downRight)==0 && src.at<uchar>(downRight)>0) {
					ptVec.push_back(downRight);
					seed_map.at<uchar>(downRight)=255;
					temp.push_back(downRight);
				}
			}
			if(up.y>=0 && left.x>=0) {
				if(seed_map.at<uchar>(upLeft)==0 && src.at<uchar>(upLeft)>0) {
					ptVec.push_back(upLeft);
					seed_map.at<uchar>(upLeft)=255;
					temp.push_back(upLeft);
				}
			}
			if(up.y>=0 && right.x<src.cols) {
				if(seed_map.at<uchar>(upRight)==0 && src.at<uchar>(upRight)>0) {
					ptVec.push_back(upRight);
					seed_map.at<uchar>(upRight)=255;
					temp.push_back(upRight);
				}
			}
			temp.pop_front();
			countSteps++;
			if(countSteps==steps) {
				break;
			}
		}
	}
	return seed_map;
}

vector<Mat> LiquidFeatureExtraction::run(Mat src, vector<Point> seed_vec) {
	this->seed_vec = seed_vec;
	this->seed_map_vec.resize(this->seed_vec.size(),Mat());
	const int steps = 50;
	for(unsigned int i=0; i<this->seed_vec.size(); i++) {
		this->seed_map_vec.at(i) = this->localExtraction(src,seed_vec.at(i), steps);
	}
	return this->seed_map_vec;
}
