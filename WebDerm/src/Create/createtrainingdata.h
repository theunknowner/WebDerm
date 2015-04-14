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
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"

class CreateTrainingData {
public:
	Mat segmentImage(Mat input, Size size);
	Mat maxDimensionCrop(Mat input);
	void mouseSelectSegment(Mat input, Size size, String name);
	void makeFalsePairs(String directory, String file, int pairNum, vector<String> excludeList);
	void makeTruePairs(String directory, String file, int pairNum, vector<String> includeList);

	Mat stitchData(Mat input1, Mat input2);
	void generateStitchPairs(String inDir, String outDir);

};

#endif /* CREATETRAININGDATA_H_ */
