/*
 * nn.h
 *
 *  Created on: Jan 13, 2014
 *      Author: jason
 */

#ifndef NN_H_INCLUDED
#define NN_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"

bool getData(String filename);
void importTrainingData(ifstream &data);
void trainNeuralNetwork();
void runNeuralNetworkTraining(String filename);
void getNormAbsDist(int red, int green, int blue, double * normDistVals, double * absDistVals);
String matchColorNN(double * normDist, double * absDist);
String runNeuralNetworkSystem(int red, int green, int blue);
void createDataFile(String inFileRGB, String outFileData);

#endif
