/*
 * printstream.cpp
 *
 *  Created on: Feb 19, 2016
 *      Author: jason
 */

#include "printstream.h"
#include "../functions.h"

PrintStream::PrintStream() {
	this->psName = "";
	this->strPrintStream = "";
}

PrintStream::PrintStream(String name) {
	this->psName = name;
	this->strPrintStream = "";
}

String PrintStream::name() {
	return this->psName;
}

String PrintStream::getPrintStream() {
	return this->strPrintStream;
}

//! Stores string format to the stream be printed later.
//! Argument input same as printf formatting...
//! i.e. ("%s,%s,%d\n",str1,str2,int1)
void PrintStream::storeStringFormat(String format, ...) {
	char buffer[100];
	va_list vl;
	va_start(vl,format);
	vsprintf(buffer,format.c_str(),vl);
	va_end(vl);
	this->strPrintStream += string(buffer);
}

void PrintStream::writePrintStream(String filename) {
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	fprintf(fp,"%s",this->strPrintStream.c_str());
	fclose(fp);
}

void PrintStream::clear() {
	this->strPrintStream.clear();
	this->strPrintStream = "";
}
