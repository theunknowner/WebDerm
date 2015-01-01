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
	int inputNeurons;
	int outputNeurons;
	int max_iterations;
	const int hiddenNeurons = 20;
	const double eta = 0.1; //learning rate
	const double alpha = 0.1; //momentum
	vector<double> iLayerNeuronWeight;
	vector< vector<double> > ihLayerNeuronWeight;
	vector< vector<double> >hoLayerNeuronWeight;
	vector<double> iLayerNeuronBiasWeight;
	vector<double> oLayerNeuronBiasWeights;
	vector<double> hNeuronBiasWeight;
	vector<double> iLayerNeuron;
	vector<double> hLayerNeurons;
	vector<double> oLayerNeurons;
	vector<double> oNeuronGrad;
	vector<double> hNeuronGrad;
	vector<double> hLayerNeuronTotal;
	vector<double> oLayerNeuronTotal;


	double fx0(double input, double a, double b);
	double fx(double input, double a, double b);
	double mse(vector<double> actualOutput, vector<double> targetOutput);
	double totalError(vector<double> actualOutput, vector<double> targetOutput);
	void resetPerceptrons();
	void init(int inputSize, int outputSize);
	void init_weights();
	void releaseMemory();

public:
	~ShapeML();
	int train(vector<vector<double> > data, vector<vector<double> > labels, int iterations=1);
	void importData(String file, vector<vector<double> > &trainingData, vector<vector<double> > &labels);
	void predict(vector<vector<double> > &data, vector<vector<double> > &results);
	void normalizeData(vector<vector<double> > &data);
	void saveData();
	void loadData(String file);
	void cvtImageToCSV(Mat &img, String file);
	void printVectorInfo();
};

#endif /* SHAPEML_H_ */
