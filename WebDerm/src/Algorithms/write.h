/*
 * write.h
 *
 *  Created on: May 7, 2014
 *      Author: jason
 */

#ifndef WRITE_H_
#define WRITE_H_


#include "/home/jason/git/WebDerm/WebDerm/src/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/global.h"

void writeSeq2File(vector< vector<String> > &vec, String name);
void writeSeq2File(vector< vector<String> > &vec, String pathname,String name);
void writeSeq2File(vector< vector<int> > &vec, String name);
void writeSeq2File(vector< vector<double> > &vec, String name);
void writeSeq2File(vector<double> &vec, String name);
void writeSeq2File(vector< vector<long double> > &vec, String name);
void writeSeq2File(vector<Point> &ptVec, String name);

#endif /* WRITE_H_ */
