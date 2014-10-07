/*
 * mouse.h
 *
 *  Created on: Jul 30, 2014
 *      Author: jason
 */

#ifndef MOUSE_H_
#define MOUSE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"

class FileData;
class Mouse {

public:
	void mouseColor(Mat img, String name);
	void mouseOutputColor(Mat img, FileData &fd);
};

#endif /* MOUSE_H_ */

