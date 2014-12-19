/*
 * Shapeml.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#include "shapeml.h"

double ShapeML::inputLayer(double input) {
	return input;
}

double ShapeML::hiddenLayer1(double input) {
	double result = 1/(1+exp(-input));
	return result;
}

double ShapeML::outputLayer(double input) {
	double result = 1/(1+exp(-input));
	return result;
}

//imports training data and labels and setups the network all at once
void ShapeML::importTrainingData(String file, deque<deque<Point> > &trainingData, deque<int> &labels) {
	fstream fs(file.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		deque<Point> pts;
		size_t pos=0;
		int x,y;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i<vec.size()-1) {
					pos = vec.at(i).find(";");
					x = atoi(vec.at(i).substr(0,pos).c_str());
					y = atoi(vec.at(i).substr(pos+1,vec.at(i).length()).c_str());
					pts.push_back(Point(x,y));
				}
				else {
					labels.push_back(atoi(vec.at(i).c_str()));
				}
			}
			trainingData.push_back(pts);
			vec.clear();
			pts.clear();
		}
		fs.close();
		this->inputHiddenLayerWeights.resize(this->labels.size(),0.1);
		this->hiddenOutputLayerWeights.resize(this->labels.size(),0.17);
		this->outputLayerBiasWeights.resize(this->labels.size(),1.0);
		this->hiddenLayerBiasWeights.resize(this->labels.size(),1.0);
	}
	else {
		cout << "Import training data failed!" << endl;
	}
}

void ShapeML::trainNeuralNetwork(deque<deque<Point> > trainingData, deque<int> labels) {
	for(unsigned int i=0; i<this->trainingData.size(); i++) {
		for(unsigned int j=0; j<this->trainingData.at(i).size(); j++) {

		}
	}
}
