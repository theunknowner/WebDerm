/*
 * nn.cpp
 *
 *  Created on: Jan 17, 2014
 *      Author: jason
 */

#include "nn.h"

const int SIZE = 8; //size of colors
const int a0=-1;
const int a=1;
const int delta=1;
double biasWeight[SIZE];
double normWeight[SIZE][SIZE];
double absWeight[SIZE][SIZE];
vector<double> normVector;
vector<double> absVector;
vector<double> outputVector;

bool getData(String filename)
	{
		fstream fs(filename.c_str());
		if(fs.is_open())
		{
			return true;
		}
		return false;
	}
	void importTrainingData(ifstream &data)
	{
		String temp, token;
		vector<String> str;
		int line=0;
		size_t pos=0;
		String delimiter = ",";
		while(getline(data,temp))
		{
			while((pos = temp.find(delimiter))!=string::npos)
			{
				token = temp.substr(0,pos);
				temp.erase(0,pos+delimiter.length());
				str.push_back(token);
			}
			str.push_back(temp);
			if(str.size()==4)
			{
				if(line%(SIZE+1)!=0)
				{
					for(unsigned int i=0; i<str.size(); i++)
					{
						if(i==1)
						{
							normVector.push_back(atof(str.at(i).c_str()));
						}
						if(i==2)
						{
							absVector.push_back(atof(str.at(i).c_str()));
						}
						if(i==3)
						{
							outputVector.push_back(atof(str.at(i).c_str()));
						}
					}
				}
				str.clear();
			}
			line++;
		}
	}

	void trainNeuralNetwork()
	{
		FILE * fpWeights;
		fpWeights = fopen("weights.txt", "w");
		String str;
		double normDist[normVector.size()];
		double absDist[absVector.size()];
		double correctOutput[outputVector.size()];
		double totalInput[SIZE];
		double input[SIZE][SIZE];
		double output[SIZE];
		double error[SIZE];
		double gradientError[SIZE];
		fill_n(biasWeight,SIZE,1);
		fill_n(totalInput, SIZE, 0);
		unsigned int x=0;
		unsigned int length;
		for(int i=0; i<SIZE; i++)
		{
			for(int j=0; j<SIZE; j++)
			{
				normWeight[i][j] = 10;
				absWeight[i][j] = 0.01;
			}
		}
		for(unsigned int i=0; i<normVector.size(); i++)
		{
			normDist[i] = normVector.at(i);
			absDist[i] = absVector.at(i);
			correctOutput[i] = outputVector.at(i);
		}
		length = x+SIZE;
		while(x<normVector.size())
		{
			for(unsigned int i=x; i<length; i++)
			{
				for(unsigned int j=x; j<length; j++)
				{
					input[i%SIZE][j%SIZE] = (a0 * biasWeight[i%SIZE]) + (normDist[j] * normWeight[i%SIZE][j%SIZE]) + (absDist[j] * absWeight[i%SIZE][j%SIZE]);
				}
				for(int k=0; k<SIZE; k++)
				{
					totalInput[i%SIZE] +=input[i%SIZE][k];
				}
				output[i%SIZE] = 1/(1+exp(-a*totalInput[i%SIZE]));
				error[i%SIZE] =	correctOutput[i] - output[i%SIZE];
				gradientError[i%SIZE] = error[i%SIZE] * (a * exp(-a*totalInput[i%SIZE]))/pow((1+a*exp(-a*totalInput[i%SIZE])),2);
			}
			for(unsigned int i=x; i<length; i++)
			{
				biasWeight[i%SIZE] += (delta*a0*gradientError[i%SIZE]);
				for(unsigned int j=x;j<length; j++)
				{
					normWeight[i%SIZE][j%SIZE] += (delta*normDist[j]*gradientError[i%SIZE]);
					absWeight[i%SIZE][j%SIZE] += (delta*absDist[j]*gradientError[i%SIZE]);
				}
			}
			for(int i=0; i<SIZE; i++)
			{
				totalInput[i] = 0;
				error[i] = 0;
				gradientError[i] = 0;
			}
			x+=SIZE;
			length = x+SIZE;
			if(length>normVector.size())
			{
				length = normVector.size();
			}
		}
		for(int i=0; i<SIZE; i++)
		{
			fprintf(fpWeights,"BiasWeight - %s: %f\n", rgbColors[i].c_str(),biasWeight[i]);
			for(int j=0; j<SIZE; j++)
			{
				str = rgbColors[i]+rgbColors[j];
				fprintf(fpWeights,"%s\n",str.c_str());
				fprintf(fpWeights, "NormWeight: %f\n", normWeight[i][j]);
				fprintf(fpWeights, "AbsWeight: %f\n", absWeight[i][j]);
			}
		}
	}

	void runNeuralNetworkTraining(String filename)
	{
		ifstream data(filename.c_str());
		if(data.is_open())
		{
		importTrainingData(data);
		trainNeuralNetwork();
		}
		else
		{
			cout << filename << " not found!" << endl;
		}
		data.close();
	}

	void getNormAbsDist(int red, int green, int blue, double * normDistVals, double * absDistVals)
	{
		rgb rgb;
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			normDistVals[i] = rgb.normEucDist(red,green,blue, normMeanThresh.at(i));
			absDistVals[i] = rgb.absEucDist(red,green,blue, absMeanThresh.at(i));
		}
	}

	String matchColorNN(double * normDist, double * absDist)
	{
		double input[SIZE][SIZE];
		double totalInput[SIZE];
		double output[SIZE];
		double largest=0;
		int index=0;
		fill_n(totalInput, SIZE, 0);
		for(int i=0; i<SIZE; i++)
		{
			for(int j=0; j<SIZE; j++)
			{
				input[i][j] = (a0 * biasWeight[i]) + (normDist[j] * normWeight[i][j]) + (absDist[j] * absWeight[i][j]);
			}
			for(int k=0; k<SIZE; k++)
			{
				totalInput[i] += input[i][k];
			}
			output[i] = 1/(1+exp(-a*totalInput[i]));
		}
		largest = output[0];
		for(int i=0; i<SIZE; i++)
		{
			if(output[i]>largest)
			{
				largest = output[i];
				index=i;
			}
		}
		return rgbColors[index];
	}

	String runNeuralNetworkSystem(int red, int green, int blue)
	{
		double normDistVals[normMeanThresh.size()];
		double absDistVals[absMeanThresh.size()];
		String color;
		getNormAbsDist(red, green, blue, normDistVals, absDistVals);
		color = matchColorNN(normDistVals, absDistVals);
		return color;
	}

	void createDataFile(String inFileRGB, String outFileData)
	{
		rgb rgb;
		//fstream outFile(outputFileData.c_str());
		FILE * fp;
		fp = fopen(outFileData.c_str(), "a");
		fstream inFile(inFileRGB.c_str());
		String temp;
		vector<int> val;
		vector<vector<int> > RGB;
		double normDistVals[normMeanThresh.size()];
		double absDistVals[absMeanThresh.size()];
		int rgbVal[3];
		while(getline(inFile,temp,','))
		{
			val.push_back(atoi(temp.c_str()));
			if(val.size()==3)
			{
				//cout << val.at(0) << "," << val.at(1) << "," << val.at(2) << endl;
				RGB.push_back(val);
				val.clear();
			}
		}
		for(unsigned int i=0; i<RGB.size(); i++)
		{
			for(int j=0; j<3; j++)
			{
				//cout << rgb.at(i).at(j) << endl;
				rgbVal[j] = RGB.at(i).at(j);
			}
			temp = rgb.pushColor(rgbVal[0],rgbVal[1],rgbVal[2],0.25);
			//cout << temp << endl;
			fprintf(fp, "%d,%d,%d,%s\n",rgbVal[0],rgbVal[1],rgbVal[2],temp.c_str());
			//outFile << rgbVal[0] << "," << rgbVal[1] << "," << rgbVal[2] << "," << temp << endl;
			for(unsigned int k=0; k<normMeanThresh.size(); k++)
			{
				normDistVals[k] = rgb.normEucDist(rgbVal[0],rgbVal[1],rgbVal[2],normMeanThresh.at(k));
				absDistVals[k] = rgb.absEucDist(rgbVal[0],rgbVal[1],rgbVal[2], absMeanThresh.at(k));
				fprintf(fp, "%s,%f,%f\n",rgbColors.at(k).c_str(),normDistVals[k],absDistVals[k]);
				//outFile << colors.at(k) << "," << normDistVals[k] << "," << absDistVals[k] << endl;
			}
		}
	}
