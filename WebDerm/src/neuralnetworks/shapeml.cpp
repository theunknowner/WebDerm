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

//const double a=1.0;
//const double b=.1;
//activation function
double ShapeML::fx(double input, double a, double b) {
	double result = a * tanh(b*input);
	//double result = (2*a)/(1+exp(-b*input));
	return result;
}

//derivative of fx()
double ShapeML::fx0(double input, double a, double b) {
	//double result = (-4.0*a*b*exp(-2*b*input))/pow(1.0+exp(-2*b*input),2);
	double result = 1.14393 * (1 - pow(tanh(b*input),2));
	//double result = (2*a*b*exp(-b*input))/pow(1+exp(-b*input),2);
	return result;
}

double symSigmFn(double input) {
	double A=1.0, B=1.0;
	double result = B * (1-exp(-A*input))/(1+exp(-A*input));
	return result;
}

double sigmFn(double input) {
	double result = 1.0/(1.0 - exp(-input));
	return result;
}

//input = outputLayerFunction results, realOutput = label
double ShapeML::computeError(double actualOutput, double targetOutput) {
	double results = (actualOutput-targetOutput);
	return results;
}

//total error of the whole network
double ShapeML::totalError(vector<double> actualOutput, vector<double> targetOutput) {
	double sum = 0;
	for(unsigned int i=0; i<actualOutput.size(); i++) {
		sum += pow(fabs(targetOutput.at(i) - actualOutput.at(i)),2);
	}
	sum = sqrt(sum);
	return sum;
}

void ShapeML::init(int inputSize, int outputSize) {
	this->inputNeurons = inputSize;
	this->outputNeurons = outputSize;

	this->iLayerNeuron.resize(this->inputNeurons,0);
	this->hLayerNeurons.resize(this->hiddenNeurons,0);
	this->oLayerPerceptrons.resize(this->outputNeurons,0);

	this->iLayerNeuronWeight.resize(this->inputNeurons,0);
	this->ihLayerNeuronWeight.resize(this->inputNeurons,vector<double>(this->hiddenNeurons,0));
	this->hoLayerNeuronWeight.resize(this->hiddenNeurons,vector<double>(this->outputNeurons,0));

	this->iLayerNeuronBiasWeight.resize(this->inputNeurons,0);
	this->hNeuronBiasWeight.resize(this->hiddenNeurons,0);
	this->oLayerNeuronBiasWeights.resize(this->outputNeurons,0);

	this->hNeuronGrad.resize(this->hiddenNeurons,0);
	this->oNeuronGrad.resize(this->outputNeurons,0);

	this->hLayerNeuronTotal.resize(this->hiddenNeurons,0);
	this->oLayerNeuronTotal.resize(this->outputNeurons,0);

	this->ihPrevWeightDelta.resize(this->inputNeurons,vector<double>(this->hiddenNeurons,0));
	this->hPrevBiasDelta.resize(this->hiddenNeurons,0);
	this->hoPrevWeightDelta.resize(this->hiddenNeurons,vector<double>(this->outputNeurons,0));
	this->oPrevBiasDelta.resize(this->outputNeurons,0);
}

void ShapeML::init_weights() {
	RNG rng(time(0));
	double randWt=0;
	for(int i=0; i<this->inputNeurons; i++) {
		//randWt = rng.uniform(0.0,1.0);
		this->iLayerNeuronWeight.at(i) = 1.0;
		//randWt = rng.uniform(-5,0);
		this->iLayerNeuronBiasWeight.at(i) = 0.0;
	}

	for(int j=0; j<this->hiddenNeurons; j++) {
		for(int i=0; i<this->inputNeurons; i++) {
			randWt = rng.uniform(-1.0,1.0);
			this->ihLayerNeuronWeight.at(i).at(j) = randWt;
		}
		randWt = rng.uniform(2.5,5.0);
		this->hNeuronBiasWeight.at(j) = randWt;
	}

	for(int k=0; k<this->outputNeurons; k++) {
		for(int j=0; j<this->hiddenNeurons; j++) {
			randWt = rng.uniform(-1.0,1.0);
			this->hoLayerNeuronWeight.at(j).at(k) = randWt;
		}
		randWt = rng.uniform(2.5, 5.0);
		this->oLayerNeuronBiasWeights.at(k) = randWt;
	}
}

void ShapeML::resetPerceptrons() {
	this->iLayerNeuron.clear();
	this->iLayerNeuron.resize(this->inputNeurons,0);
	this->hLayerNeurons.clear();
	this->hLayerNeurons.resize(this->hiddenNeurons,0);
	this->oLayerPerceptrons.clear();
	this->oLayerPerceptrons.resize(this->outputNeurons,0);
	this->hNeuronGrad.clear();
	this->hNeuronGrad.resize(this->hiddenNeurons,0);
	this->oNeuronGrad.clear();
	this->oNeuronGrad.resize(this->outputNeurons,0);
	this->hLayerNeuronTotal.clear();
	this->hLayerNeuronTotal.resize(this->hiddenNeurons,0);
	this->oLayerNeuronTotal.clear();
	this->oLayerNeuronTotal.resize(this->outputNeurons,0);
}

void ShapeML::releaseMemory() {
	this->hLayerNeurons.clear();
	this->hLayerNeurons.shrink_to_fit();
	this->oLayerPerceptrons.clear();
	this->oLayerPerceptrons.shrink_to_fit();
	this->ihLayerNeuronWeight.clear();
	this->ihLayerNeuronWeight.shrink_to_fit();
	this->hNeuronBiasWeight.clear();
	this->hNeuronBiasWeight.shrink_to_fit();
	this->hoLayerNeuronWeight.clear();
	this->hoLayerNeuronWeight.shrink_to_fit();
	this->hNeuronGrad.clear();
	this->hNeuronGrad.shrink_to_fit();
	this->oLayerNeuronBiasWeights.clear();
	this->oLayerNeuronBiasWeights.shrink_to_fit();
	this->oNeuronGrad.clear();
	this->oNeuronGrad.shrink_to_fit();
	this->hLayerNeuronTotal.clear();
	this->hLayerNeuronTotal.shrink_to_fit();
	this->oLayerNeuronTotal.clear();
	this->oLayerNeuronTotal.shrink_to_fit();
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
	double B = 1.0;
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
	double eps = 1.0;
	double a=1.7159, b=2./3.;
	//FILE * fp, *fp2;
	//String filename,filename2;
	int flag=0, iNode=-1, jNode=-1;
	double hTotalUnbias[20] = {0};
	double sampleTotal[20] = {0};
	while(iter<this->max_iterations) {
		for(unsigned int dataNum=0; dataNum<data.size(); dataNum++) {
			//filename = "/home/jason/Desktop/workspace/ih-weight-debug"+toString(iter+1)+"-"+toString(dataNum+1)+".csv";
			//filename2 = "/home/jason/Desktop/workspace/ho-weight-debug"+toString(iter+1)+"-"+toString(dataNum+1)+".csv";
			//fp=fopen(filename.c_str(),"w");
			//fp2=fopen(filename2.c_str(),"w");
			//calculate input layer
			for(int i=0; i<this->inputNeurons; i++) {
				double input = data.at(dataNum).at(i) * this->iLayerNeuronWeight.at(i);
				this->iLayerNeuron.at(i) = input + this->iLayerNeuronBiasWeight.at(i);
			}
			//calculate input to hidden layer total
			for(int j=0; j<this->hiddenNeurons; j++) {
				for(int i=0; i<this->inputNeurons; i++) {
					sampleTotal[j] += this->iLayerNeuron.at(i);
					double input = (this->iLayerNeuron.at(i) * this->ihLayerNeuronWeight.at(i).at(j));
					this->hLayerNeuronTotal.at(j) += input;
					//if(dataNum==0 && j==0)
					//printf("Input: %d, Val: %f, Sum: %f\n",i,input,this->hLayerNeuronTotal.at(j));
				}
				hTotalUnbias[j] = this->hLayerNeuronTotal.at(j);
				this->hLayerNeuronTotal.at(j) += this->hNeuronBiasWeight.at(j);
				this->hLayerNeurons.at(j) = this->fx(this->hLayerNeuronTotal.at(j),a,b);
				//if(dataNum==0) {
				//printf("Iter: %d, Sample: %d, hNode: %d, Input: %f, hOutput: %f\n",iter,dataNum,j,this->hLayerNeuronTotal.at(j),this->hLayerNeurons.at(j));
				//printf("Iter: %d, Sample: %d, hNode: %d, hOutput: %f\n",iter,dataNum,j,this->hLayerNeuronTotal.at(j));
				//}
			}
			//calculate hidden to output layer total
			for(int k=0; k<this->outputNeurons; k++) {
				for(int j=0; j<this->hiddenNeurons; j++) {
					double input = this->hLayerNeurons.at(j);
					this->oLayerNeuronTotal.at(k) += (input * this->hoLayerNeuronWeight.at(j).at(k));
					/*
					if(iter==4 && dataNum==0 && k==0) {
						cout << this->hLayerNeurons.at(j) << endl;
					}*/
				}
				this->oLayerNeuronTotal.at(k) += this->oLayerNeuronBiasWeights.at(k);
				//this->oLayerPerceptrons.at(k) = this->fx(this->oLayerNeuronTotal.at(k),a,b);
				this->oLayerPerceptrons.at(k) = this->fx(this->oLayerNeuronTotal.at(k),a,b);
				//if(dataNum==0) {
				//	printf("Iter: %d, Sample: %d, oNode: %d, Input: %f, oOutput: %f\n",iter,dataNum,k,this->oLayerNeuronTotal.at(k),this->oLayerPerceptrons.at(k));
				//}
			}

			//calculate error of output layer
			for(int k=0; k<this->outputNeurons; k++) {
				//eps = this->totalError(this->oLayerPerceptrons,labels.at(dataNum));
				//eps = this->oLayerPerceptrons.at(k) - labels.at(dataNum).at(k);
				eps = this->oLayerPerceptrons.at(k)*(1-this->oLayerPerceptrons.at(k))*(labels.at(dataNum).at(k)-this->oLayerPerceptrons.at(k));
				//double deriv = (1. - this->oLayerPerceptrons.at(k)) + (1.+this->oLayerPerceptrons.at(k));
				//this->oNeuronGrad.at(k) = deriv * (labels.at(dataNum).at(k)-this->oLayerPerceptrons.at(k));
				//this->oNeuronGrad.at(k) = eps * this->fx0(this->oLayerNeuronTotal.at(k),a,b);
				this->oNeuronGrad.at(k) = eps;
				//if(dataNum==0) {
				//	printf("Deriv = (1-%f)+(1+%f)\n",this->oLayerPerceptrons.at(k),this->oLayerPerceptrons.at(k));
				//	printf("Error = (%f-%f)\n",labels.at(dataNum).at(k),this->oLayerPerceptrons.at(k));
				//	printf("Iter: %d, Sample: %d, oNode: %d, Deriv: %f, oOutput: %f\n",iter,dataNum,k,deriv,this->oNeuronGrad.at(k));
				//}
			}
			//printf info
			double o1 = this->oLayerPerceptrons.at(0);
			double o2 = this->oLayerPerceptrons.at(1);
			double lbl1 = labels.at(dataNum).at(0);
			double lbl2 = labels.at(dataNum).at(1);
			double val1 = o1-lbl1;
			double val2 = o2-lbl2;
			printf("Iter: %d, Sample: %d | E1: (%f- %f)= %f | E2: (%f- %f)= %f\n",iter+1,dataNum+1,o1,lbl1,val1,o2,lbl2,val2);
			//printf("Iter: %d, Sample: %d | E1: (%f- %f)= %f, Input: %f\n",iter+1,dataNum+1,o1,lbl1,val1,this->oLayerNeuronTotal.at(0));
			//printf("Iter: %d, Sample: %d, Eps: %f\n",iter, dataNum, eps);

			//calculate error of hidden layer
			double gSum[20] = {0};
			double hfx0[20] = {0};
			for(int j=0; j<this->hiddenNeurons; j++) {
				double sum=0;
				//double deriv = (1. - this->hLayerNeurons.at(j)) * this->hLayerNeurons.at(j);
				for(int k=0; k<this->outputNeurons; k++) {
					sum += (this->oNeuronGrad.at(k) * this->hoLayerNeuronWeight.at(j).at(k));
				}
				gSum[j] = sum;
				hfx0[j] = this->fx0(this->hLayerNeuronTotal.at(j),a,b);
				//this->hNeuronGrad.at(j) = sum * this->fx0(this->hLayerNeuronTotal.at(j),a,b);
				this->hNeuronGrad.at(j) = this->
				//this->hNeuronGrad.at(j) = deriv * sum;
				//if(dataNum==0) {
				//	printf("Iter: %d, Sample: %d, oNode: %d, Deriv: %f, hGrad: %f\n",iter,dataNum,j,deriv,this->hNeuronGrad.at(j));
				//}
			}

			//back propagation to update weights between input and hidden layer
			for(int j=0; j<this->hiddenNeurons; j++) {
				for(int i=0; i<this->inputNeurons; i++) {
					double delta = (alpha * this->ihPrevWeightDelta.at(i).at(j))+(this->eta * this->hNeuronGrad.at(j) * data.at(dataNum).at(i));
					//fprintf(fp,"%f,",this->ihLayerNeuronWeight.at(i).at(j));
					this->ihLayerNeuronWeight.at(i).at(j) += delta;
					/*if(this->ihLayerNeuronWeight.at(i).at(j)<0 && flag==0 && iNode==-1) {
						double prevWt  = this->ihLayerNeuronWeight.at(i).at(j) - delta;
						printf("Iter: %d, Sample:%d,ihNode: (%d,%d), Delta: %f, hGrad: %f, Wt: %f\n",iter+1,dataNum+1,i+1,j+1,delta,this->hNeuronGrad.at(j),this->ihLayerNeuronWeight.at(i).at(j));
						printf("Alpha: %f, Eta: %f, PrevWt: %f, PrevDelta: %f, Xi: %f\n",alpha,eta,prevWt,this->ihPrevWeightDelta.at(i).at(j),data.at(dataNum).at(i));
						printf("hSum: %f, hfx0: %f, hTotal: %f, hBias: %f, hTotalUnbias: %f\n",gSum[j],hfx0[j],this->hLayerNeuronTotal.at(j),this->hNeuronBiasWeight.at(j),hTotalUnbias[j]);
						printf("SampleTotal: %f\n", sampleTotal[j]);
						printf("oGrad1: %f, hoWeights: %f\n",this->oNeuronGrad.at(0),this->hoLayerNeuronWeight.at(j).at(0));
						printf("oGrad2: %f, hoWeights: %f\n",this->oNeuronGrad.at(1),this->hoLayerNeuronWeight.at(j).at(1));
						fill_n(sampleTotal,20,0);
						printf("------------------------------------------------------------------------------------------------------\n");
						flag++;
						iNode = i; jNode=j;
					}
					else if(this->ihLayerNeuronWeight.at(i).at(j)<0 && flag!=10 && i==iNode && j==jNode) {
						double prevWt  = this->ihLayerNeuronWeight.at(i).at(j) - delta;
						printf("Iter: %d, Sample:%d, ihNode: (%d,%d), Delta: %f, hGrad: %f, Wt: %f\n",iter+1,dataNum+1,i+1,j+1,delta,this->hNeuronGrad.at(j),this->ihLayerNeuronWeight.at(i).at(j));
						printf("Alpha: %f, Eta: %f, PrevWt: %f, PrevDelta: %f, Xi: %f\n",alpha,eta,prevWt,this->ihPrevWeightDelta.at(i).at(j),data.at(dataNum).at(i));
						printf("hSum: %f, hfx0: %f, hTotal: %f, hBias: %f, hTotalUnbias: %f\n",gSum[j],hfx0[j],this->hLayerNeuronTotal.at(j),this->hNeuronBiasWeight.at(j), hTotalUnbias[j]);
						printf("SampleTotal: %f\n", sampleTotal[j]);
						printf("oGrad1: %f, hoWeights: %f\n",this->oNeuronGrad.at(0),this->hoLayerNeuronWeight.at(j).at(0));
						printf("oGrad2: %f, hoWeights: %f\n",this->oNeuronGrad.at(1),this->hoLayerNeuronWeight.at(j).at(1));
						fill_n(sampleTotal,20,0);
						printf("------------------------------------------------------------------------------------------------------\n");
						flag++;
					}*/
					this->ihPrevWeightDelta.at(i).at(j) = delta;
				}
				//fprintf(fp,"\n");
				//printf("Iter: %d, Sample: %d, hNode: %d, hGrad: %f, hOutput: %f\n",iter+1,dataNum+1,j+1,this->hNeuronGrad.at(j),this->hLayerNeurons.at(j));
				double delta = (alpha * this->hPrevBiasDelta.at(j)) + (this->eta * this->hNeuronGrad.at(j));
				this->hNeuronBiasWeight.at(j) += delta;
				this->hPrevBiasDelta.at(j) = delta;
			}
			//back propagation for weights between hidden and output layer
			for(int k=0; k<this->outputNeurons; k++) {
				for(int j=0; j<this->hiddenNeurons; j++) {
					double delta = (alpha * this->hoPrevWeightDelta.at(j).at(k))+(this->eta * this->oNeuronGrad.at(k) * this->hLayerNeurons.at(j));
					double val = (this->eta * this->oNeuronGrad.at(k) * this->hLayerNeurons.at(j));
					//fprintf(fp2,"%f,",this->hoLayerNeuronWeight.at(j).at(k));
					this->hoLayerNeuronWeight.at(j).at(k) += delta;
					this->hoPrevWeightDelta.at(j).at(k) = delta;
					//if(k==1)
					//printf("Iter: %d, Sample: %d, hNode: %d, Delta: %f, Val: %f, oGrad: %f, hOutput: %f, E1: (%f- %f)= %f\n",iter+1,dataNum+1,j+1,delta,val,this->oNeuronGrad.at(k),this->hLayerNeurons.at(j),o2,lbl2,val2);
				}
				//fprintf(fp2,"\n");
				double delta = (alpha * this->oPrevBiasDelta.at(k))+(this->eta * this->oNeuronGrad.at(k));
				this->oLayerNeuronBiasWeights.at(k) += delta;
				this->oPrevBiasDelta.at(k) = delta;
			}
			this->resetPerceptrons();
			//fclose(fp);
			//fclose(fp2);
		}
		iter++;
	}
	return iter;
}

void ShapeML::predict(vector<vector<double> > &data, vector<vector<double> > &results) {
	this->resetPerceptrons();
	for(unsigned int dataNum=0; dataNum<data.size(); dataNum++) {
		//calculate input layer
		for(int i=0; i<this->inputNeurons; i++) {
			double input = data.at(dataNum).at(i) * this->iLayerNeuronWeight.at(i);
			this->iLayerNeuron.at(i) = input + this->iLayerNeuronBiasWeight.at(i);
		}
		//calculate input to hidden layer total
		for(int j=0; j<this->hiddenNeurons; j++) {
			for(int i=0; i<this->inputNeurons; i++) {
				double input = (this->iLayerNeuron.at(i) * this->ihLayerNeuronWeight.at(i).at(j));
				this->hLayerNeuronTotal.at(j) += input;
			}
			this->hLayerNeuronTotal.at(j) += this->hNeuronBiasWeight.at(j);
			this->hLayerNeurons.at(j) = sigmFn(this->hLayerNeuronTotal.at(j));
		}
		//calculate hidden to output layer total
		for(int k=0; k<this->outputNeurons; k++) {
			for(int j=0; j<this->hiddenNeurons; j++) {
				double input = this->hLayerNeurons.at(j);
				this->oLayerNeuronTotal.at(k) += (input * this->hoLayerNeuronWeight.at(j).at(k));
			}
			this->oLayerNeuronTotal.at(k) += this->oLayerNeuronBiasWeights.at(k);
			this->oLayerPerceptrons.at(k) = tanh(this->oLayerNeuronTotal.at(k));
		}
		results.at(dataNum) = this->oLayerPerceptrons;
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
	for(unsigned int i=0; i<this->ihLayerNeuronWeight.size(); i++) {
		for(unsigned int j=0; j<this->ihLayerNeuronWeight.at(i).size(); j++) {
			fprintf(fp,"%f,",this->ihLayerNeuronWeight.at(i).at(j));
		}
		fprintf(fp,"\n");
	}
	for(unsigned int i=0; i<this->hNeuronBiasWeight.size(); i++) {
		fprintf(fp2,"%f,",this->hNeuronBiasWeight.at(i));
	}
	for(unsigned int i=0; i<this->hoLayerNeuronWeight.size(); i++) {
		for(unsigned int j=0; j<this->hoLayerNeuronWeight.at(i).size(); j++) {
			fprintf(fp3,"%f,",this->hoLayerNeuronWeight.at(i).at(j));
		}
		fprintf(fp3,"\n");
	}
	for(unsigned int i=0; i<this->oLayerNeuronBiasWeights.size(); i++) {
		fprintf(fp4,"%f,",this->oLayerNeuronBiasWeights.at(i));
	}
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
}

void ShapeML::cvtImageToCSV(Mat &img, String file) {
	Mat img2, dst;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	GaussianBlur(img,img2,Size(3,3),0,0);
	Canny(img2,dst,100,200,3);
	findContours(dst,contours,hierarchy,CV_RETR_LIST,CV_CHAIN_APPROX_NONE,Point(0, 0));
}

void ShapeML::printVectorInfo() {
	printf("hiddenLayerPerceptrons: %lu\n",this->hLayerNeurons.size());
	printf("outputLayerPerceptrons: %lu\n",this->oLayerPerceptrons.size());
	printf("inputHiddenLayerWeights: %lu,%lu\n",this->ihLayerNeuronWeight.size(),this->ihLayerNeuronWeight.at(0).size());
	printf("hiddenLayerBiasWeights: %lu\n",this->hNeuronBiasWeight.size());
	printf("hiddenOutputLayerWeights: %lu,%lu\n",this->hoLayerNeuronWeight.size(),this->hoLayerNeuronWeight.at(0).size());
	printf("outputLayerBiasWeights: %lu\n",this->oLayerNeuronBiasWeights.size());
	printf("hiddenPerceptronErrors: %lu\n",this->hNeuronGrad.size());
	printf("outputPerceptronErrors: %lu\n",this->oNeuronGrad.size());
	fflush(stdout);
}
