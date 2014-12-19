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
	double hiddenLayer1(double input);
	double inputLayer(double input);
	double outputLayer(double input);
	deque<double> inputHiddenLayerWeights;
	deque<double> hiddenOutputLayerWeights;
	deque<double> outputLayerBiasWeights;
	deque<double> hiddenLayerBiasWeights;
public:
	deque<int> labels;
	deque<deque<Point> > trainingData;
	void trainNeuralNetwork(deque<deque<Point> > trainingData, deque<int> labels);
	void importTrainingData(String file, deque<deque<Point> > &trainingData, deque<int> &labels);
};

#endif /* SHAPEML_H_ */
