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
}

void MyExceptions::writeErrorToFile(cv::Exception &ex) {
	FILE * fp;
	String file = this->filename + "_error_log.txt";
	fp = fopen(file.c_str(),"w");
	Timer time;
	fprintf(fp,"%s",time.getCurrentTime().c_str());
	fprintf(fp,"Error: (%d) %s\n",ex.code,ex.err.c_str());
	fprintf(fp,"File: %s\n",ex.file.c_str());
	fprintf(fp,"Line: %d\n",ex.line);
	fprintf(fp,"Function: %s\n",ex.func.c_str());
	fprintf(fp,"Message: %s\n",ex.msg.c_str());
	fclose(fp);
}

void MyExceptions::writeErrorToFile(std::exception &ex) {
	FILE * fp;
	String file = this->filename + "_error_log.txt";
	fp = fopen(file.c_str(),"w");
	Timer time;
	fprintf(fp,"%s",time.getCurrentTime().c_str());
	fprintf(fp,"Message: %s\n",ex.what());
	fclose(fp);
}

void MyExceptions::name(String filename) {
	this->filename = filename;
}
