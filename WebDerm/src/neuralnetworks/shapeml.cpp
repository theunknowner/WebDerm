/*
 * Shapeml.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: jason
 */

#include "shapeml.h"

ShapeML::~ShapeML() {
	this->releaseMemory();
}

//function for inputLayerFunction
double ShapeML::inputLayerFunction(double input) {
	return input;
}

const double a=1.71;
const double b=2.0/3.0;
//function for hiddenLayer
double ShapeML::actFunctionLayer1(double input) {
	double result = (2.0*a)/(1.0+exp(-b*input)) - a;
	return result;
}

//function for outputLayerFunction
double ShapeML::outputLayerFunction(double input) {
	double result = (2.0*a)/(1.0+exp(-b*input)) - a;
	return result;
}

//input = outputLayerFunction results, realOutput = label
double ShapeML::computeError(double realOutput, double expectedOutput) {
	double results = (expectedOutput-realOutput);
	return results;
}

double ShapeML::fx0(double input) {
	double result = (-2.0*a*b*exp(-b*input))/pow((1.0+exp(-b*input)),2);
	return result;
}

void ShapeML::init(int inputSize, int outputSize) {
	this->inputPerceptrons = inputSize;
	this->outputPerceptrons = outputSize;
	this->inputLayerPerceptrons.resize(inputSize,0);

	this->hiddenLayerPerceptrons.resize(this->hiddenPerceptrons,0);
	this->hiddenLayerBiasWeights.resize(this->hiddenPerceptrons,1.0);

	this->outputLayerPerceptrons.resize(this->outputPerceptrons,0);
	this->outputLayerBiasWeights.resize(this->outputPerceptrons,1.0);

	this->inputHiddenLayerWeights.resize(this->hiddenLayerPerceptrons.size(),vector<double>(this->inputLayerPerceptrons.size(),0.1));
	this->hiddenOutputLayerWeights.resize(this->outputLayerPerceptrons.size(),vector<double>(this->hiddenLayerPerceptrons.size(),0.17));

	this->hiddenPerceptronErrors.resize(this->hiddenPerceptrons,0);
	this->outputPerceptronErrors.resize(this->outputPerceptrons,0);
}

void ShapeML::resetPerceptrons() {
	this->inputLayerPerceptrons.clear();
	this->inputLayerPerceptrons.resize(this->inputPerceptrons,0);
	this->hiddenLayerPerceptrons.clear();
	this->hiddenLayerPerceptrons.resize(this->hiddenPerceptrons,0);
	this->outputLayerPerceptrons.clear();
	this->outputLayerPerceptrons.resize(this->outputPerceptrons,0);
	this->hiddenPerceptronErrors.clear();
	this->hiddenPerceptronErrors.resize(this->hiddenPerceptrons,0);
	this->outputPerceptronErrors.clear();
	this->outputPerceptronErrors.resize(this->outputPerceptrons,0);
}

void ShapeML::releaseMemory() {
	this->inputLayerPerceptrons.clear();
	this->inputLayerPerceptrons.shrink_to_fit();
	this->hiddenLayerPerceptrons.clear();
	this->hiddenLayerPerceptrons.shrink_to_fit();
	this->outputLayerPerceptrons.clear();
	this->outputLayerPerceptrons.shrink_to_fit();
	this->inputHiddenLayerWeights.clear();
	this->inputHiddenLayerWeights.shrink_to_fit();
	this->hiddenLayerBiasWeights.clear();
	this->hiddenLayerBiasWeights.shrink_to_fit();
	this->hiddenOutputLayerWeights.clear();
	this->hiddenOutputLayerWeights.shrink_to_fit();
	this->hiddenPerceptronErrors.clear();
	this->hiddenPerceptronErrors.shrink_to_fit();
	this->outputLayerBiasWeights.clear();
	this->outputLayerBiasWeights.shrink_to_fit();
	this->outputPerceptronErrors.clear();
	this->outputPerceptronErrors.shrink_to_fit();
}

//imports training data and labels
void ShapeML::importData(String file, vector<vector<double> > &data, vector<vector<double> > &labels) {
	fstream fs(file.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		vector<double> pts;
		vector<double> lbl;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i<400) {
					pts.push_back(atof(vec.at(i).c_str()));
				}
				else {
					lbl.push_back(atof(vec.at(i).c_str()));
				}
			}
			data.push_back(pts);
			labels.push_back(lbl);
			vec.clear();
			pts.clear();
			lbl.clear();
		}
		fs.close();
	}
	else {
		cout << "Import training data failed!" << endl;
	}
}

void ShapeML::importTestData(String file, vector<vector<double> > &testData) {
	fstream fs(file.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		vector<double> pts;
		size_t pos=0;
		int x,y;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				pos = vec.at(i).find(";");
				x = atof(vec.at(i).substr(0,pos).c_str());
				y = atof(vec.at(i).substr(pos+1,vec.at(i).length()).c_str());
				pts.push_back(x);
				pts.push_back(y);
			}
			testData.push_back(pts);
			vec.clear();
			pts.clear();
		}
		fs.close();
	}
	else {
		cout << "Import test data failed!" << endl;
	}
}

void ShapeML::train(vector<vector<double> > trainingData, vector<vector<double> > labels, int iterations) {
	this->init(trainingData.at(0).size(), labels.at(0).size());
	this->max_iterations = iterations;
	for (int iter=0; iter<this->max_iterations; iter++) {
		for(unsigned int dataNum=0; dataNum<trainingData.size(); dataNum++) {
			for(int hidNum=0; hidNum<this->hiddenPerceptrons; hidNum++) {
				for(int inputNum=0; inputNum<this->inputPerceptrons; inputNum++) {
					this->inputLayerPerceptrons.at(inputNum) = this->inputLayerFunction(trainingData.at(dataNum).at(inputNum));
					double results = this->inputLayerPerceptrons.at(inputNum) * this->inputHiddenLayerWeights.at(hidNum).at(inputNum);
					this->hiddenLayerPerceptrons.at(hidNum) += results;
				}
				this->hiddenLayerPerceptrons.at(hidNum) = this->actFunctionLayer1(this->hiddenLayerPerceptrons.at(hidNum)+this->hiddenLayerBiasWeights.at(hidNum));
			}
			//printf("%d: %f\n",dataNum,this->hiddenLayerPerceptrons.at(0));
			for(int outNum=0; outNum<this->outputPerceptrons; outNum++) {
				for(int hidNum=0; hidNum<this->hiddenPerceptrons; hidNum++) {
					double results = this->hiddenLayerPerceptrons.at(hidNum) * this->hiddenOutputLayerWeights.at(outNum).at(hidNum);
					this->outputLayerPerceptrons.at(outNum) += results;
				}
				double results = this->outputLayerFunction(this->outputLayerBiasWeights.at(outNum) + this->outputLayerPerceptrons.at(outNum));
				this->outputPerceptronErrors.at(outNum) = this->computeError(results,labels.at(dataNum).at(outNum)) * fx0(this->outputLayerBiasWeights.at(outNum)*this->outputLayerPerceptrons.at(outNum)) * -1;
				this->outputLayerPerceptrons.at(outNum) = results;
			}
			//printf("%d: %f,%f\n",dataNum,this->outputPerceptronErrors.at(0),this->outputPerceptronErrors.at(1));
			//printf("%f,%f\n",this->outputLayerPerceptrons.at(0),outputLayerPerceptrons.at(1));
			//compute error for hiddenLayerPerceptron
			for(int i=0; i<this->hiddenPerceptrons; i++) {
				for(int j=0; j<this->outputPerceptrons; j++) {
					this->hiddenPerceptronErrors.at(i) += (this->outputPerceptronErrors.at(j) * this->hiddenOutputLayerWeights.at(j).at(i));
				}
				this->hiddenPerceptronErrors.at(i) = this->hiddenLayerPerceptrons.at(i)*(1-this->hiddenLayerPerceptrons.at(i))*this->hiddenPerceptronErrors.at(i);
				//printf("%d: %f\n",dataNum,hiddenPerceptronErrors.at(i));
			}
			//back propagation for weights between hidden and output layer
			for(int i=0; i<this->hiddenPerceptrons; i++) {
				for(int j=0; j<this->outputPerceptrons; j++) {
					this->hiddenOutputLayerWeights.at(j).at(i) += (this->learningRate * this->outputPerceptronErrors.at(j) * this->hiddenLayerPerceptrons.at(i));
				}
			}
			//back propagation for weights between input and hidden layer
			for(int i=0; i<this->hiddenPerceptrons; i++) {
				for(unsigned int j=0; j<this->inputLayerPerceptrons.size(); j++) {
					this->inputHiddenLayerWeights.at(i).at(j) += learningRate * this->hiddenPerceptronErrors.at(i) * this->inputLayerPerceptrons.at(j);
				}
				this->hiddenLayerBiasWeights.at(i) += (learningRate * this->hiddenPerceptronErrors.at(i));
			}
			this->resetPerceptrons();
		}
	}
}

void ShapeML::predict(vector<vector<double> > &testData, vector<vector<double> > &results) {
	this->resetPerceptrons();
	for(unsigned int dataNum=0; dataNum<testData.size(); dataNum++) {
		for(int hidNum=0; hidNum<this->hiddenPerceptrons; hidNum++) {
			for(int inputNum=0; inputNum<this->inputPerceptrons; inputNum++) {
				this->inputLayerPerceptrons.at(inputNum) = this->inputLayerFunction(testData.at(dataNum).at(inputNum));
				double results = this->inputLayerPerceptrons.at(inputNum) * this->inputHiddenLayerWeights.at(hidNum).at(inputNum);
				this->hiddenLayerPerceptrons.at(hidNum) += results;
			}
			this->hiddenLayerPerceptrons.at(hidNum) = this->actFunctionLayer1(this->hiddenLayerPerceptrons.at(hidNum)+this->hiddenLayerBiasWeights.at(hidNum));
		}
		for(int outNum=0; outNum<this->outputPerceptrons; outNum++) {
			for(int hidNum=0; hidNum<this->hiddenPerceptrons; hidNum++) {
				double results = this->hiddenLayerPerceptrons.at(hidNum) * this->hiddenOutputLayerWeights.at(outNum).at(hidNum);
				this->outputLayerPerceptrons.at(outNum) += results;
			}
			this->outputLayerPerceptrons.at(outNum) = this->outputLayerFunction(this->outputLayerBiasWeights.at(outNum) * this->outputLayerPerceptrons.at(outNum));
			results.at(dataNum).at(outNum) = this->outputLayerPerceptrons.at(outNum);
		}
		//printf("%d: %f,%f\n",dataNum,this->outputLayerPerceptrons.at(0),this->outputLayerPerceptrons.at(1));
		this->resetPerceptrons();
	}
}
void ShapeML::saveData() {
	FILE *fp, *fp2, *fp3, *fp4;
	fp = fopen("/home/jason/Desktop/workspace/InputHiddenLayerWeights.csv","w");
	fp2 = fopen("/home/jason/Desktop/workspace/HiddenLayerBiasWeights.csv","w");
	fp3 = fopen("/home/jason/Desktop/workspace/HiddenOutputLayerWeights.csv","w");
	fp4 = fopen("/home/jason/Desktop/workspace/OutputLayerBiasWeights.csv","w");
	for(unsigned int i=0; i<this->inputHiddenLayerWeights.size(); i++) {
		for(unsigned int j=0; j<this->inputHiddenLayerWeights.at(i).size(); j++) {
			fprintf(fp,"%f,",this->inputHiddenLayerWeights.at(i).at(j));
		}
		fprintf(fp,"\n");
	}
	for(unsigned int i=0; i<this->hiddenLayerBiasWeights.size(); i++) {
		fprintf(fp2,"%f,",this->hiddenLayerBiasWeights.at(i));
	}
	for(unsigned int i=0; i<this->hiddenOutputLayerWeights.size(); i++) {
		for(unsigned int j=0; j<this->hiddenOutputLayerWeights.at(i).size(); j++) {
			fprintf(fp3,"%f,",this->hiddenOutputLayerWeights.at(i).at(j));
		}
		fprintf(fp3,"\n");
	}
	for(unsigned int i=0; i<this->outputLayerBiasWeights.size(); i++) {
		fprintf(fp4,"%f,",this->outputLayerBiasWeights.at(i));
	}
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}

void ShapeML::printVectorInfo() {
	printf("inputLayerPerceptrons: %lu\n",this->inputLayerPerceptrons.size());
	printf("hiddenLayerPerceptrons: %lu\n",this->hiddenLayerPerceptrons.size());
	printf("outputLayerPerceptrons: %lu\n",this->outputLayerPerceptrons.size());
	printf("inputHiddenLayerWeights: %lu,%lu\n",this->inputHiddenLayerWeights.size(),this->inputHiddenLayerWeights.at(0).size());
	printf("hiddenLayerBiasWeights: %lu\n",this->hiddenLayerBiasWeights.size());
	printf("hiddenOutputLayerWeights: %lu,%lu\n",this->hiddenOutputLayerWeights.size(),this->hiddenOutputLayerWeights.at(0).size());
	printf("outputLayerBiasWeights: %lu\n",this->outputLayerBiasWeights.size());
	printf("hiddenPerceptronErrors: %lu\n",this->hiddenPerceptronErrors.size());
	printf("outputPerceptronErrors: %lu\n",this->outputPerceptronErrors.size());
	fflush(stdout);
}
