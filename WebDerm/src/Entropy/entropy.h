/*
 * Entropy.h
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#ifndef ENTROPY_H_
#define ENTROPY_H_

#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"

class FileData;
class Entropy {

public:
	deque< deque<double> > outputEntropy(FileData &fd, Size ksize);
	void importEntropyFiles(String path1, String path2,String name);
	deque<int> calcShadeShift(deque< deque<double> > &vec1, deque< deque<double> > &vec2);
	void outputShiftedEntropy(FileData &fd1, FileData &fd2, Size ksize, deque<int> &colorShiftAmt);
};

#endif /* ENTROPY_H_ */
