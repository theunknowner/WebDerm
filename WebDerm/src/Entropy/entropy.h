/*
 * Entropy.h
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#ifndef ENTROPY_H_
#define ENTROPY_H_

#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Graph/graph.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/jaysort.h"

class FileData;
class Entropy {

public:
	deque< deque<double> > outputEntropy(FileData &fd, Size ksize);
	deque< deque<double> > outputCombinedEntropy(FileData &fd, Size ksize);
	deque<int> calcShadeShift(deque< deque<double> > &vec1, deque< deque<double> > &vec2);
	void outputShiftedEntropy(FileData &fd1, FileData &fd2, Size ksize, deque<int> &colorShiftAmt);

	deque< deque<double> > outputSigmoid(FileData &fd, Size ksize, double a, double b, double p);
	deque< deque<double> > outputCombinedSigmoid(FileData &fd, Size ksize, double a, double b, double p);

	void eyeFn(FileData &fd, Size ksize, String targetColor="",String targetShade="");
	Mat showEyeFnSquares(Mat img, Size ksize, String targetColor, String targetShade);

	bool importEntropyThresholds();
	void releaseMemory();
	bool loadEntropyFiles(String filepath, deque<deque<double> > &dataMat);
	double compareEntropy(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<deque<double> > &matchVec);
	double compareEntropy2(deque<deque<double> > vec1, deque<deque<double> > vec2);

	void runAllEntropy();
	void runEntropy();
	void runCompareEntropy();
	void runCompareEntropyAll();
	void runCompareEntropy2();
};

#endif /* ENTROPY_H_ */
