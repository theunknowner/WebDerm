/*
 * mouse.h
 *
 *  Created on: Jul 30, 2014
 *      Author: jason
 */

#ifndef MOUSE_H_
#define MOUSE_H_

class FileData;

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Mouse {

public:
	static void mouseColor(Mat img, String name);
	static void mouseOutputColor(Mat img, FileData &fd);
};

#endif /* MOUSE_H_ */

