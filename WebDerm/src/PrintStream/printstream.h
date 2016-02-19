/*
 * printstream.h
 *
 *  Created on: Feb 19, 2016
 *      Author: jason
 */

#ifndef PRINTSTREAM_H_
#define PRINTSTREAM_H_

#include "../global.h"


/*
 * Holds large amount of string format data to be printed out
 * later on when specified by a key.
 */
class PrintStream {
private:
	// map to hold the string streams referenced by a key
	String strPrintStream;
	String psName;

public:
	PrintStream();
	PrintStream(String name);
	String name();
	String getPrintStream();
	void storeStringFormat(String data, ...);
	void writePrintStream(String filename);
	void clear();
};

#endif /* PRINTSTREAM_H_ */
