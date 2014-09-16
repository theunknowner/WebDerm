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
	void outputEntropy(FileData &fd, Size ksize);
	void importEntropyFiles(String path1, String path2,String name);
	void compareEntropy(deque< deque<double> > &vec1, deque< deque<double> > &vec2,String name);
};

#endif /* ENTROPY_H_ */
