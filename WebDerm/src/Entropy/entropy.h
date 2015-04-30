/*
 * Entropy.h
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#ifndef ENTROPY_H_
#define ENTROPY_H_

class FileData;

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Entropy {
private:
	bool debugMode=false;
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
	double fn_compareY(double y1, double y2, double weight);
	double fn_compareS(double s1, double s2, double weight);
	double fn_compareV(double v1, double v2, double weight);
	double fn_compareT(deque<double> t1, deque<double> t2, double weight);

	deque< deque<double> > totalPopulation; //Y
	deque< deque<double> > populationDensity; //S
	deque< deque<double> > densityVariation; //V
	deque<double> shapeMetric; //T

public:
	Size entSize;
	void setDebugMode(bool mode);
	bool isDebugModeOn();

	//entropy.cpp
	void eyeFn(FileData &fd, Size ksize, Mat map=Mat(),String targetColor="",String targetShade="");
	Mat showEyeFnSquares(Mat img, Size ksize, String targetColor, String targetShade);
	void writeEntropyFile(String filename, FileData &fd);
	void shapeFn(FileData &fd);
	void shapeFn2(FileData &fd);

	//entcompare.cpp
	bool importEntropyThresholds();
	void releaseMemory();
	bool loadEntropyFiles(String filepath, deque<deque<double> > &dataMat, deque<String> &colorNameVec);
	double compareYSV(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec);
	double compareT(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec);
	double compareEntropy2(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec);

	//entropyrun.cpp
	void runAllEntropy();
	void runEntropy();
	void runCompareEntropy(String targetName);
	void runCompareEntropy2(String targetName);
	void runCompareEntropyList(String fileList, String folder);
	void runCompareEntropyList2(String fileList, String folder);
	void runCompareAllEntropy(String folder);
	void runCompareAllEntropy2(String folder);

	//demo_entropy.cpp
	void demo_runCompareEntropy();
	void demo_showShape(String name);

	//testing
	double test_compareEntropy2a(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<String> &colorNameVec);
	void test_runCompareEntropy2a(String targetName);
	void test_runAllCompareEntropy2a(String folder);

};

#endif /* ENTROPY_H_ */
