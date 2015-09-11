/*
 * my_exceptions.h
 *
 *  Created on: Sep 10, 2015
 *      Author: jason
 */

#ifndef MY_EXCEPTIONS_H_
#define MY_EXCEPTIONS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include <exception>

class MyExceptions : public cv::Exception {
private:
	String filename;
public:
	MyExceptions();
	void writeErrorToFile(cv::Exception &ex);
	void writeErrorToFile(std::exception &ex);
	void name(String filename);
};

#endif /* MY_EXCEPTIONS_H_ */
