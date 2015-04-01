/*
 * createtrainingdata.h
 *
 *  Created on: Apr 1, 2015
 *      Author: jason
 */

#ifndef CREATETRAININGDATA_H_
#define CREATETRAININGDATA_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class CreateTrainingData {
public:
	Mat segmentImage(Mat input, Size size);
	Mat maxDimensionCrop(Mat input);
	void mouseSelectSegment(Mat input, Size size, String name);
};

#endif /* CREATETRAININGDATA_H_ */
