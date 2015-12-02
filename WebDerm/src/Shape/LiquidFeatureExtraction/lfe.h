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
	vector<Mat> seed_map_vec;

public:
	bool doesSeedMapsCrossOver(vector<Mat> seed_map_vec);
	Mat localExtraction(Mat src, Point seed, int steps=-1);
	vector<Mat> run(Mat src, vector<Point> seed_vec);
};

#endif /* LFE_H_ */
