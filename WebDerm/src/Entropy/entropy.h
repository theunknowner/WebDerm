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
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"


class FileData;
class Entropy {
private:
	double Y_HIGH;	//upper Y range boundary
	double Y_LOW;	//lower Y range boudnary
	double S_HIGH;
	double S_LOW;
	double V_HIGH;
	double V_LOW;
	double Y_THRESH;	//% threshold Y must meet for special conditions
	double S_THRESH;
	double V_THRESH;
	double Y_DIST;	//distance threshold Y must meet for special conditions
	double S_DIST;
	double V_DIST;
	double Y_PERCEPTION;	//threshold in which the eyes starts noticing color
	double S_PERCEPTION;
	double Y_LARGE_THRESH;
	double distPass;
	double Y1, Y2;
	deque<double> colorWeights; //holds the weights for color impact - My algo
	deque<double> colorWeights2; //Dr. Dube's algo
	void resetThreshVals();
	double compareY(double y1, double y2, double weight);
	double compareS(double s1, double s2, double weight);
	double compareV(double v1, double v2, double weight);
	double compareT(deque<double> t1, deque<double> t2, double weight);

	deque< deque<double> > totalPopulation; //Y
	deque< deque<double> > populationDensity; //S
	deque< deque<double> > densityVariation; //V
	deque<double> shapeMetric; //T

public:
	Size entSize;

	//entropy.cpp
	void eyeFn(FileData &fd, Size ksize, String targetColor="",String targetShade="");
	Mat showEyeFnSquares(Mat img, Size ksize, String targetColor, String targetShade);
	void writeEntropyFile(String filename, FileData &fd);
	void shapeFn(FileData &fd);
	void shapeFn2(FileData &fd);

	//entcompare.cpp
	bool importEntropyThresholds();
	void releaseMemory();
	bool loadEntropyFiles(String filepath, deque<deque<double> > &dataMat, deque<String> &colorNameVec);
	double compareEntropy(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec);
	double compareEntropy2(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec);

	//entropyrun.cpp
	void runAllEntropy();
	void runEntropy();
	void runCompareEntropy(String targetName);
	void runCompareEntropy2(String targetName);
	void runCompareEntropyList(String fileList, String folder);
	void runCompareEntropyList2(String fileList, String folder);

	//demo_entropy.cpp
	void demo_runCompareEntropy();
};

#endif /* ENTROPY_H_ */
