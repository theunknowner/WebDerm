/*
 * write.h
 *
 *  Created on: May 7, 2014
 *      Author: jason
 */

#ifndef WRITE_H_
#define WRITE_H_

#include "../global.h"

namespace Write {
void writeToCsv(deque< deque<String> > &vec, String name);
void writeToCsv(deque< deque<int> > &vec, String name);
void writeToCsv(deque< deque<double> > &vec, String name);
void writeToCsv(deque<double> &vec, String name);
void writeToCsv(vector<vector<float> > &vec, String name);
void writeToCsv(vector<double> &vec, String name, String delimit="\n");
void writeToCsv(vector<float> &vec, String name);
void writeToCsv(vector<int> &vec, String name);
void writeToCsv(vector<String> &vec, String name);
void writeToCsv(deque<int> &vec, String name);
void writeToCsv(deque< deque<long double> > &vec, String name);
void writeToCsv(deque<Point> &ptVec, String name);
void writeToCsv(deque<Point2f> &ptVec, String name);
void writeToCsv(Mat src, String type, String name, bool writeIndex=false);
bool doesFileExist(String filename);
}

#endif /* WRITE_H_ */
