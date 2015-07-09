/*
 * readFromCsv.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#include "read.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

namespace Read {

vector<vector<String> > readFromCsv(String filename) {
	fstream fs(filename);
	if(!fs.is_open()) {
		cout << "Cannot open file to read!!!" << endl;
		exit(1);
	}

	String temp;
	vector<String> vec;
	vector<vector<String> > vec2d;
	while(getline(fs,temp)) {
		getSubstr(temp,',',vec);
		vec2d.push_back(vec);
	}
	fs.close();

	return vec2d;
}

}
