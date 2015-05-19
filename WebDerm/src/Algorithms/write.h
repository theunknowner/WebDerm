/*
 * write.h
 *
 *  Created on: May 7, 2014
 *      Author: jason
 */

#ifndef WRITE_H_
#define WRITE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

namespace Write {
void writeSeq2File(deque< deque<String> > &vec, String name);
void writeSeq2File(deque< deque<int> > &vec, String name);
void writeSeq2File(deque< deque<double> > &vec, String name);
void writeSeq2File(deque<double> &vec, String name);
void writeSeq2File(vector<vector<float> > &vec, String name);
void writeSeq2File(vector<double> &vec, String name);
void writeSeq2File(vector<float> &vec, String name);
void writeSeq2File(vector<int> &vec, String name);
void writeSeq2File(deque<int> &vec, String name);
void writeSeq2File(deque< deque<long double> > &vec, String name);
void writeSeq2File(deque<Point> &ptVec, String name);
void writeSeq2File(deque<Point2f> &ptVec, String name);
void writeSeq2File(Mat src, String type, String name, bool writeIndex=false);
bool doesFileExist(String filename);
}

#endif /* WRITE_H_ */
