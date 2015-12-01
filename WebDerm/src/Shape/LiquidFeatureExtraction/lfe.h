/*
 * lfe.h
 *
 *  Created on: Dec 1, 2015
 *      Author: jason
 */

#ifndef LFE_H_
#define LFE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class LiquidFeatureExtraction {
private:
	vector<Point> seed_vec;
	vector<map<Point,int> > seed_map_vec;

public:
	void localExtraction(Mat src, vector<Point> seed_vec);
	vector<Mat> run(Mat src, Point seed=Point(0,0), double lcThresh=0, int sort=0, int numOfPtsThresh=10);
};

#endif /* LFE_H_ */
