/*
 * hysteresis.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef HYSTERESIS_H_INCLUDED
#define HYSTERESIS_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/src/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"

void hysteresis(Mat img, Size size, String name);
void hysteresis1x1(Mat img,String name);
void writeSeq2File(vector< vector<String> > &vec, String pathname,String name);
void writeSeq2File(vector< vector<int> > &vec, String name);
void writeSeq2File(vector< vector<double> > &vec, String name);

#endif /* HYSTERESIS_H_ */
