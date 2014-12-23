/*
 * Shapeml.h
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#ifndef SHAPEML_H_
#define SHAPEML_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/stdafx.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class ShapeML {
private:
	int inputPerceptrons;
	int outputPerceptrons;
	int max_iterations;
	const int hiddenPerceptrons = 20;
	const double learningRate = 1.0;
	vector< vector<double> > inputHiddenLayerWeights;
	vector< vector<double> >hiddenOutputLayerWeights;
	vector<double> outputLayerBiasWeights;
	vector<double> hiddenLayerBiasWeights;
	vector<double> hiddenLayerPerceptrons;
	vector<double> inputLayerPerceptrons;
	vector<double> outputLayerPerceptrons;
	vector<double> outputPerceptronErrors;
	vector<double> hiddenPerceptronErrors;

	double fx0(double input);
	double actFunctionLayer1(double input);
	double inputLayerFunction(double input);
	double outputLayerFunction(double input);
	double computeError(double realOutput, double expectedOutput);
	void resetPerceptrons();
	void init(int inputSize, int outputSize);
	void releaseMemory();

public:
	~ShapeML();
	void train(vector<vector<double> > trainingData, vector<vector<double> > labels, int iterations=1);
	void importData(String file, vector<vector<double> > &trainingData, vector<vector<double> > &labels);
	void importTestData(String file, vector<vector<double> > &testData);
	void predict(vector<vector<double> > &testData, vector<vector<double> > &results);
	void saveData();
	void loadData(String file);
	void printVectorInfo();
};

#endif /* SHAPEML_H_ */
