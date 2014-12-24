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
const double b=.1;
//activation function
double ShapeML::fx(double input) {
	double result = (2.0*a)/(1.0+exp(-b*input)) - a;
	return result;
}

//derivative of fx()
double ShapeML::fx0(double input) {
	double result = (-2.0*a*b*exp(-b*input))/pow((1.0+exp(-b*input)),2);
	return result;
}

double sigmFn(double input) {
	double A=1.0, B=1.0;
	double result = B * (1-exp(-A*input))/(1+exp(-A*input));
	return result;
}

//input = outputLayerFunction results, realOutput = label
double ShapeML::computeError(double actualOutput, double targetOutput) {
	double results = (actualOutput-targetOutput);
	return results;
}

//total error of the whole network
double ShapeML::totalError(vector<double> actualOutput, vector<double> idealOutput) {
	double sum = 0;
	for(unsigned int i=0; i<actualOutput.size(); i++) {
		sum += pow(fabs(idealOutput.at(i) - actualOutput.at(i)),2);
	}
	sum = sqrt(sum);
	return sum;
}

void ShapeML::init(int inputSize, int outputSize) {
	this->inputPerceptrons = inputSize;
	this->outputPerceptrons = outputSize;

	this->iLayerPerceptrons.resize(this->inputPerceptrons,0);
	this->hLayerPerceptrons.resize(this->hiddenPerceptrons,0);
	this->oLayerPerceptrons.resize(this->outputPerceptrons,0);

	this->iLayerWeights.resize(this->inputPerceptrons,0);
	this->ihLayerWeights.resize(this->inputPerceptrons,vector<double>(this->hiddenPerceptrons,0));
	this->hoLayerWeights.resize(this->hiddenPerceptrons,vector<double>(this->outputPerceptrons,0));

	this->iLayerBiasWeights.resize(this->inputPerceptrons,0);
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
		//randWt = rng.uniform(0.0,1.0);
		this->iLayerWeights.at(i) = 1.0;
		//randWt = rng.uniform(-5,0);
		this->iLayerBiasWeights.at(i) = 0.0;
	}

	for(int j=0; j<this->hiddenPerceptrons; j++) {
		for(int i=0; i<this->inputPerceptrons; i++) {
			randWt = rng.uniform(0.0,1.0);
			this->ihLayerWeights.at(i).at(j) = randWt;
		}
		randWt = rng.uniform(-5,0);
		this->hLayerBiasWeights.at(j) = randWt;
	}

	for(int k=0; k<this->outputPerceptrons; k++) {
		for(int j=0; j<this->hiddenPerceptrons; j++) {
			randWt = rng.uniform(0.0,1.0);
			this->hoLayerWeights.at(j).at(k) = randWt;
		}
		randWt = rng.uniform(-5, 0);
		this->oLayerBiasWeights.at(k) = randWt;
	}
}

void ShapeML::resetPerceptrons() {
	this->iLayerPerceptrons.clear();
	this->iLayerPerceptrons.resize(this->inputPerceptrons,0);
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
	double B = 0.015;
	double result = 1.0/(1.0+exp(-B * input));
	return result;
}

double symSigFn(double input) {
	double B = 0.06;
	double result = (1-exp(-B * input))/(1+exp(-B * input));
	return result;
}

int ShapeML::train(vector<vector<double> > data, vector<vector<double> > labels, int iterations) {
	this->init(data.at(0).size(), labels.at(0).size());
	this->init_weights();
	this->max_iterations = iterations;
	int iter = 0;
	double eps = 1.0, epsThresh = 0.01;
	while(iter<this->max_iterations) {
		for(unsigned int dataNum=0; dataNum<data.size(); dataNum++) {
			//calculate input layer
			for(int i=0; i<this->inputPerceptrons; i++) {
				double input = data.at(dataNum).at(i) * this->iLayerWeights.at(i);
				this->iLayerPerceptrons.at(i) = input + this->iLayerBiasWeights.at(i);
			}
			//calculate input to hidden layer total
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				for(int i=0; i<this->inputPerceptrons; i++) {
					double input = this->iLayerPerceptrons.at(i);
					this->ihLayerTotal.at(j) += (input * this->ihLayerWeights.at(i).at(j));
				}
				this->ihLayerTotal.at(j) += this->hLayerBiasWeights.at(j);
				this->hLayerPerceptrons.at(j) = this->fx(this->ihLayerTotal.at(j));
				//printf("hNode: %d, hOutput: %f\n",j,this->hLayerPerceptrons.at(j));
			}
			//calculate hidden to output layer total
			for(int k=0; k<this->outputPerceptrons; k++) {
				for(int j=0; j<this->hiddenPerceptrons; j++) {
					double input = this->hLayerPerceptrons.at(j);
					this->hoLayerTotal.at(k) += (input * this->hoLayerWeights.at(j).at(k));
					/*
					if(iter==4 && dataNum==0 && k==0) {
						cout << this->hLayerPerceptrons.at(j) << endl;
					}*/
				}
				this->hoLayerTotal.at(k) += this->oLayerBiasWeights.at(k);
				this->oLayerPerceptrons.at(k) = this->fx(this->hoLayerTotal.at(k));
				/*if(dataNum==0) {
					printf("O: %d, Sum: %f\n",k,this->hoLayerTotal.at(k));
				}
				if(dataNum==0) {
					printf("Iter: %d, O#: %d, Output: %f\n",iter,k,this->oLayerPerceptrons.at(k));
				}*/
			}

			//calculate error of output layer
			for(int k=0; k<this->outputPerceptrons; k++) {
				//eps = this->totalError(this->oLayerPerceptrons,labels.at(dataNum));
				eps = this->computeError(this->oLayerPerceptrons.at(k),labels.at(dataNum).at(k));
				this->oPerceptronErrors.at(k) = eps; * this->fx0(this->hoLayerTotal.at(k));
			}
			//printf info
			double o1 = this->oLayerPerceptrons.at(0);
			double o2 = this->oLayerPerceptrons.at(1);
			double lbl1 = labels.at(dataNum).at(0);
			double lbl2 = labels.at(dataNum).at(1);
			double val1 = o1-lbl1;
			double val2 = lbl2 -o2;
			printf("Iter: %d, Sample: %d | E1: (%f- %f)= %f | E2: (%f- %f)= %f | ES: %f\n",iter+1,dataNum+1,lbl1,o1,val1,lbl2,o2,val2,eps);
			//printf("Iter: %d, Sample: %d, Eps: %f\n",iter, dataNum, eps);

			//calculate error of hidden layer
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				double error=0;
				for(int k=0; k<this->outputPerceptrons; k++) {
					error += (this->oPerceptronErrors.at(k) * this->hoLayerWeights.at(j).at(k));
				}
				this->hPerceptronErrors.at(j) = error * this->fx0(this->ihLayerTotal.at(j));
			}
			//back propagation to update weights between input and hidden layer
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				for(int i=0; i<this->inputPerceptrons; i++) {
					double delta = this->eta * this->hPerceptronErrors.at(j) * data.at(dataNum).at(i);
					this->ihLayerWeights.at(i).at(j) += delta + (alpha * this->ihPrevWeightDelta.at(i).at(j));
					this->ihPrevWeightDelta.at(i).at(j) = delta + (alpha * this->ihPrevWeightDelta.at(i).at(j));
				}
				double delta = this->eta * this->hPerceptronErrors.at(j) * 1.0;
				this->hLayerBiasWeights.at(j) += delta + (alpha * this->hPrevBiasDelta.at(j));
				this->hPrevBiasDelta.at(j) = delta + (alpha * this->hPrevBiasDelta.at(j));
			}
			//back propagation for weights between hidden and output layer
			for(int k=0; k<this->outputPerceptrons; k++) {
				for(int j=0; j<this->hiddenPerceptrons; j++) {
					double delta = this->eta * this->oPerceptronErrors.at(k) * this->hLayerPerceptrons.at(j);
					this->hoLayerWeights.at(j).at(k) += delta + (alpha * this->hoPrevWeightDelta.at(j).at(k));
					this->hoPrevWeightDelta.at(j).at(k) = delta + (alpha * this->hoPrevWeightDelta.at(j).at(k));
				}
				double delta = this->eta * this->oPerceptronErrors.at(k) * 1.0;
				this->oLayerBiasWeights.at(k) += delta + (alpha * this->oPrevBiasDelta.at(k));
				this->oPrevBiasDelta.at(k) = delta + (alpha * this->oPrevBiasDelta.at(k));
			}
			this->resetPerceptrons();
		}
		iter++;
	}
	return iter;
}

void ShapeML::predict(vector<vector<double> > &data, vector<vector<double> > &results) {
	this->resetPerceptrons();
	for(unsigned int dataNum=0; dataNum<data.size(); dataNum++) {
		//calculate input layer
		for(int i=0; i<this->inputPerceptrons; i++) {
			double input = data.at(dataNum).at(i) * this->iLayerWeights.at(i);
			this->iLayerPerceptrons.at(i) = input + this->iLayerBiasWeights.at(i);
		}
		//calculate input to hidden layer total
		for(int j=0; j<this->hiddenPerceptrons; j++) {
			for(int i=0; i<this->inputPerceptrons; i++) {
				double input = this->iLayerPerceptrons.at(i);
				this->ihLayerTotal.at(j) += (input * this->ihLayerWeights.at(i).at(j));
			}
			this->ihLayerTotal.at(j) += this->hLayerBiasWeights.at(j) * b;
			this->hLayerPerceptrons.at(j) = this->fx(this->ihLayerTotal.at(j));
		}
		//calculate hidden to output layer total
		for(int k=0; k<this->outputPerceptrons; k++) {
			for(int j=0; j<this->hiddenPerceptrons; j++) {
				double input = this->hLayerPerceptrons.at(j);
				this->hoLayerTotal.at(k) += (input * this->hoLayerWeights.at(j).at(k));
			}
			this->hoLayerTotal.at(k) += this->oLayerBiasWeights.at(k) * b;
			this->oLayerPerceptrons.at(k) = this->fx(this->hoLayerTotal.at(k));

			results.at(dataNum).at(k) = this->oLayerPerceptrons.at(k);
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
