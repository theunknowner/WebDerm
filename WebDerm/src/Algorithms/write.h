/*
 * write.h
 *
 *  Created on: May 7, 2014
 *      Author: jason
 */

#ifndef WRITE_H_
#define WRITE_H_


#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

void writeSeq2File(deque< deque<String> > &vec, String name);
void writeSeq2File(deque< deque<int> > &vec, String name);
void writeSeq2File(deque< deque<double> > &vec, String name);
void writeSeq2File(deque<double> &vec, String name);
void writeSeq2File(deque< deque<long double> > &vec, String name);
void writeSeq2File(deque<Point> &ptVec, String name);
bool doesFileExist(String filename);

#endif /* WRITE_H_ */
