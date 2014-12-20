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
	const int hiddenPerceptrons = 20;
	const double learningRate = 0.25;
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
	void trainNeuralNetwork(vector<vector<int> > trainingData, vector<vector<double> > labels);
	void importTrainingData(String file, vector<vector<int> > &trainingData, vector<vector<double> > &labels);
	void importTestData(String file, vector<vector<int> > &testData);
	void predict(vector<vector<int> > &testData, vector<vector<double> > &results);
	void saveData();
	void loadData(String file);
	void printVectorInfo();
};

#endif /* SHAPEML_H_ */
