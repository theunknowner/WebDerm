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

const double a=1.0;
const double b=0.01;
//activation function
double ShapeML::activationFunction(double input) {
	double result = (2.0*a)/(1.0+exp(-b*input)) - a;
	return result;
}

//input = outputLayerFunction results, realOutput = label
double ShapeML::computeError(double actualOutput, double idealOutput) {
	double results = (idealOutput-actualOutput);
	return results;
}

//total error of the whole network
double ShapeML::totalError(vector<double> actualOutput, vector<double> idealOutput) {
	double sum = 0;
	for(unsigned int i=0; i<actualOutput.size(); i++) {
		sum += abs(idealOutput.at(i) - actualOutput.at(i));
	}
	return sum;
}

double ShapeML::fx0(double input) {
	double result = (-2.0*a*b*exp(-b*input))/pow((1.0+exp(-b*input)),2);
	return result;
}

void ShapeML::init(int inputSize, int outputSize) {
	this->inputPerceptrons = inputSize;
	this->outputPerceptrons = outputSize;

	this->hLayerPerceptrons.resize(this->hiddenPerceptrons,0);
	this->oLayerPerceptrons.resize(this->outputPerceptrons,0);

	this->ihLayerWeights.resize(this->inputPerceptrons,vector<double>(this->hiddenPerceptrons,0));
	this->hoLayerWeights.resize(this->hiddenPerceptrons,vector<double>(this->outputPerceptrons,0));
	this->hLayerBiasWeights.resize(this->hiddenPerceptrons,0);
	this->oLayerBiasWeights.resize(this->outputPerceptrons,0);

	this->hPerceptronErrors.resize(this->hiddenPerceptrons,0);
	this->oPerceptronErrors.resize(this->outputPerceptrons,0);

	this->ihLayerTotal.resize(this->hiddenPerceptrons,0);
	this->hoLayerTotal.resize(this->outputPerceptrons,0);

	this->ihPrevWeightDelta.resize(this->inputPerceptrons,vector<double>(this->hiddenPerceptrons,0));
	this->hPrevBiasDelta.resize(this->hiddenPerceptrons,0);
	this->hoPrevWeightDelta.resize(this->hiddenPerceptrons,vector<double>(this->outputPerceptrons,0));
	this->oPrevBiasDelta.resize(this->outputPerceptrons,0);
}

void ShapeML::init_weights() {
	RNG rng(time(0));
	double randWt=0;
	for(int i=0; i<this->inputPerceptrons; i++) {
		for(unsigned int j=0; j<this->hiddenPerceptrons; j++) {
			randWt = rng.uniform(0.0,1.0);
			this->ihLayerWeights.at(i).at(j) = randWt;
		}
	}
	for(int j=0; j<this->hiddenPerceptrons; j++) {
		randWt = rng.uniform(-10.0,0.0);
		this->hLayerBiasWeights.at(j) = randWt;
	}
	for(int j=0; j<this->hiddenPerceptrons; j++) {
		for(int k=0; k<this->outputPerceptrons; k++) {
			randWt = rng.uniform(1.0,2.0);
			this->hoLayerWeights.at(j).at(k) = randWt;
		}
	}
	for(int k=0; k<this->outputPerceptrons; k++) {
		randWt = rng.uniform(-10.0, 0.0);
		this->oLayerBiasWeights.at(k) = randWt;
	}
}

void ShapeML::resetPerceptrons() {
	this->hLayerPerceptrons.clear();
	this->hLayerPerceptrons.resize(this->hiddenPerceptrons,0);
	this->oLayerPerceptrons.clear();
	this->oLayerPerceptrons.resize(this->outputPerceptrons,0);
	this->hPerceptronErrors.clear();
	this->hPerceptronErrors.resize(this->hiddenPerceptrons,0);
	this->oPerceptronErrors.clear();
	this->oPerceptronErrors.resize(this->outputPerceptrons,0);
	this->ihLayerTotal.clear();
	this->ihLayerTotal.resize(this->hiddenPerceptrons,0);
	this->hoLayerTotal.clear();
	this->hoLayerTotal.resize(this->outputPerceptrons,0);
}

void ShapeML::releaseMemory() {
	this->hLayerPerceptrons.clear();
	this->hLayerPerceptrons.shrink_to_fit();
	this->oLayerPerceptrons.clear();
	this->oLayerPerceptrons.shrink_to_fit();
	this->ihLayerWeights.clear();
	this->ihLayerWeights.shrink_to_fit();
	this->hLayerBiasWeights.clear();
	this->hLayerBiasWeights.shrink_to_fit();
	this->hoLayerWeights.clear();
	this->hoLayerWeights.shrink_to_fit();
	this->hPerceptronErrors.clear();
	this->hPerceptronErrors.shrink_to_fit();
	this->oLayerBiasWeights.clear();
	this->oLayerBiasWeights.shrink_to_fit();
	this->oPerceptronErrors.clear();
	this->oPerceptronErrors.shrink_to_fit();
	this->ihLayerTotal.clear();
	this->ihLayerTotal.shrink_to_fit();
	this->hoLayerTotal.clear();
	this->hoLayerTotal.shrink_to_fit();
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
					//divide by 255 to normalize between 0 & 1
					pts.push_back(atof(vec.at(i).c_str())/255.0);
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

double sigmoidFn(double input) {
	double result = 1.0/(1.0+exp(-input));
	return result;
}

int ShapeML::train(vector<vector<double> > trainingData, vector<vector<double> > labels, int iterations) {
	this->init(trainingData.at(0).size(), labels.at(0).size());
	this->init_weights();
	this->max_iterations = iterations;
	int iter = 0;
	double eps = 1.0, epsThresh = 0.000001;
	while(iter<this->max_iterations && eps>epsThresh) {
		for(unsigned int dataNum=0; dataNum<trainingData.size(); dataNum++) {
			//calculate input to hidden layer total
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				for(int i=0; i<this->inputPerceptrons; i++) {
					double input = trainingData.at(dataNum).at(i);
					this->ihLayerTotal.at(j) += (input * this->ihLayerWeights.at(i).at(j));
				}
				this->ihLayerTotal.at(j) += this->hLayerBiasWeights.at(j);
				this->hLayerPerceptrons.at(j) = this->activationFunction(this->ihLayerTotal.at(j));
			}
			//calculate hidden to output layer total
			for(int k=0; k<this->outputPerceptrons; k++) {
				for(int j=0; j<this->hiddenPerceptrons; j++) {
					double input = this->hLayerPerceptrons.at(j);
					this->hoLayerTotal.at(k) += (input * this->hoLayerWeights.at(j).at(k));
				}
				this->hoLayerTotal.at(k) += this->oLayerBiasWeights.at(k);
				this->oLayerPerceptrons.at(k) = this->activationFunction(this->hoLayerTotal.at(k));
			}
			//calculate error of output layer
			for(int k=0; k<this->outputPerceptrons; k++) {
				double error = this->computeError(this->oLayerPerceptrons.at(k),labels.at(dataNum).at(k));
				this->oPerceptronErrors.at(k) = 1 * error * this->fx0(this->hoLayerTotal.at(k));
			}
			//printf info
			double o1 = this->oLayerPerceptrons.at(0);
			double o2 = this->oLayerPerceptrons.at(1);
			double lbl1 = labels.at(dataNum).at(0);
			double lbl2 = labels.at(dataNum).at(1);
			double val1 = lbl1 - o1;
			double val2 = lbl2 -o2;
			printf("Iter: %d, Sample: %d | E1: (%f- %f)= %f | E2: (%f- %f)= %f\n",iter,dataNum,lbl1,o1,val1,lbl2,o2,val2);
			eps = this->totalError(this->oLayerPerceptrons,labels.at(dataNum));
			if(eps<epsThresh) {
				return iter;
			}
			//calculate error of hidden layer
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				double error=0;
				for(int k=0; k<this->outputPerceptrons; k++) {
					error += (this->oPerceptronErrors.at(k) * this->hoLayerWeights.at(j).at(k));
				}
				this->hPerceptronErrors.at(j) = 1 * error * this->fx0(this->ihLayerTotal.at(j));
			}
			//back propagation to update weights between input and hidden layer
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				for(int i=0; i<this->inputPerceptrons; i++) {
					double delta = this->eta * this->hPerceptronErrors.at(j) * trainingData.at(dataNum).at(i);
					this->ihLayerWeights.at(i).at(j) += delta + (alpha * this->ihPrevWeightDelta.at(i).at(j));
					this->ihPrevWeightDelta.at(i).at(j) = delta;
				}
				double delta = this->eta * this->hPerceptronErrors.at(j) * 1.0;
				this->hLayerBiasWeights.at(j) += delta + (alpha * this->hPrevBiasDelta.at(j));
				this->hPrevBiasDelta.at(j) = delta;
			}
			//back propagation for weights between hidden and output layer
			for(int k=0; k<this->outputPerceptrons; k++) {
				for(int j=0; j<this->hiddenPerceptrons; j++) {
					double delta = this->eta * this->oPerceptronErrors.at(k) * this->hLayerPerceptrons.at(j);
					this->hoLayerWeights.at(j).at(k) += delta + (alpha * this->hoPrevWeightDelta.at(j).at(k));
					this->hoPrevWeightDelta.at(j).at(k) = delta;
				}
				double delta = this->eta * this->oPerceptronErrors.at(k) * 1.0;
				this->oLayerBiasWeights.at(k) += delta + (alpha * this->oPrevBiasDelta.at(k));
				this->oPrevBiasDelta.at(k) = delta;
			}
			this->resetPerceptrons();
		}
		iter++;
	}
	return iter;
}

void ShapeML::predict(vector<vector<double> > &testData, vector<vector<double> > &results) {
	this->resetPerceptrons();
	for(unsigned int dataNum=0; dataNum<testData.size(); dataNum++) {
		//calculate input to hidden layer total
		for(int j=0; j<this->hiddenPerceptrons; j++) {
			for(int i=0; i<this->inputPerceptrons; i++) {
				double input = testData.at(dataNum).at(i);
				this->ihLayerTotal.at(j) += (input * this->ihLayerWeights.at(i).at(j));
			}
			this->ihLayerTotal.at(j) += this->hLayerBiasWeights.at(j);
			this->hLayerPerceptrons.at(j) = this->activationFunction(this->ihLayerTotal.at(j));
		}
		//calculate hidden to output layer total
		for(int k=0; k<this->outputPerceptrons; k++) {
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				double input = this->hLayerPerceptrons.at(j);
				this->hoLayerTotal.at(k) += (input * this->hoLayerWeights.at(j).at(k));
			}
			this->hoLayerTotal.at(k) += this->oLayerBiasWeights.at(k);
			this->oLayerPerceptrons.at(k) = this->activationFunction(this->hoLayerTotal.at(k));
		}
		this->resetPerceptrons();
	}
}

void ShapeML::normalizeData(vector<vector<double> > &data) {

}
void ShapeML::saveData() {
	FILE *fp, *fp2, *fp3, *fp4;
	fp = fopen("/home/jason/Desktop/workspace/InputHiddenLayerWeights.csv","w");
	fp2 = fopen("/home/jason/Desktop/workspace/HiddenLayerBiasWeights.csv","w");
	fp3 = fopen("/home/jason/Desktop/workspace/HiddenOutputLayerWeights.csv","w");
	fp4 = fopen("/home/jason/Desktop/workspace/OutputLayerBiasWeights.csv","w");
	for(unsigned int i=0; i<this->ihLayerWeights.size(); i++) {
		for(unsigned int j=0; j<this->ihLayerWeights.at(i).size(); j++) {
			fprintf(fp,"%f,",this->ihLayerWeights.at(i).at(j));
		}
		fprintf(fp,"\n");
	}
	for(unsigned int i=0; i<this->hLayerBiasWeights.size(); i++) {
		fprintf(fp2,"%f,",this->hLayerBiasWeights.at(i));
	}
	for(unsigned int i=0; i<this->hoLayerWeights.size(); i++) {
		for(unsigned int j=0; j<this->hoLayerWeights.at(i).size(); j++) {
			fprintf(fp3,"%f,",this->hoLayerWeights.at(i).at(j));
		}
		fprintf(fp3,"\n");
	}
	for(unsigned int i=0; i<this->oLayerBiasWeights.size(); i++) {
		fprintf(fp4,"%f,",this->oLayerBiasWeights.at(i));
	}
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}

void ShapeML::printVectorInfo() {
	printf("hiddenLayerPerceptrons: %lu\n",this->hLayerPerceptrons.size());
	printf("outputLayerPerceptrons: %lu\n",this->oLayerPerceptrons.size());
	printf("inputHiddenLayerWeights: %lu,%lu\n",this->ihLayerWeights.size(),this->ihLayerWeights.at(0).size());
	printf("hiddenLayerBiasWeights: %lu\n",this->hLayerBiasWeights.size());
	printf("hiddenOutputLayerWeights: %lu,%lu\n",this->hoLayerWeights.size(),this->hoLayerWeights.at(0).size());
	printf("outputLayerBiasWeights: %lu\n",this->oLayerBiasWeights.size());
	printf("hiddenPerceptronErrors: %lu\n",this->hPerceptronErrors.size());
	printf("outputPerceptronErrors: %lu\n",this->oPerceptronErrors.size());
	fflush(stdout);
}
