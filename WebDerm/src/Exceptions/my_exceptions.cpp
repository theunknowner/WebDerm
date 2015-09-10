/*
 * my_exceptions.cpp
 *
 *  Created on: Sep 10, 2015
 *      Author: jason
 */

#include "my_exceptions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Timer/timer.h"

MyExceptions::MyExceptions() {
	this->filename = "";
};

void MyExceptions::writeErrorToFile() {
	FILE * fp;
	String file = this->filename + "_error_log.txt";
	fp = fopen(file.c_str(),"w");
	Timer time;
	fprintf(fp,"%s",time.getCurrentTime().c_str());
	fprintf(fp,"%s\n",this->what());
	fclose(fp);
}

void MyExceptions::name(String filename) {
	this->filename = filename;
}
