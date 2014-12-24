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
	const double eta = 0.1; //learning rate
	const double alpha = 0.1; //momentum
	vector<double> iLayerWeights;
	vector< vector<double> > ihLayerWeights;
	vector< vector<double> >hoLayerWeights;
	vector< vector<double> > ihPrevWeightDelta;
	vector< vector<double> > hoPrevWeightDelta;
	vector<double> hPrevBiasDelta;
	vector<double> oPrevBiasDelta;
	vector<double> iLayerBiasWeights;
	vector<double> oLayerBiasWeights;
	vector<double> hLayerBiasWeights;
	vector<double> iLayerPerceptrons;
	vector<double> hLayerPerceptrons;
	vector<double> oLayerPerceptrons;
	vector<double> oPerceptronErrors;
	vector<double> hPerceptronErrors;
	vector<double> ihLayerTotal;
	vector<double> hoLayerTotal;


	double fx0(double input);
	double fx(double input);
	double computeError(double actualOutput, double idealOutput);
	double totalError(vector<double> actualOutput, vector<double> idealOutput);
	void resetPerceptrons();
	void init(int inputSize, int outputSize);
	void init_weights();
	void releaseMemory();

public:
	~ShapeML();
	int train(vector<vector<double> > trainingData, vector<vector<double> > labels, int iterations=1);
	void importData(String file, vector<vector<double> > &trainingData, vector<vector<double> > &labels);
	void predict(vector<vector<double> > &testData, vector<vector<double> > &results);
	void normalizeData(vector<vector<double> > &data);
	void saveData();
	void loadData(String file);
	void printVectorInfo();
};

#endif /* SHAPEML_H_ */
