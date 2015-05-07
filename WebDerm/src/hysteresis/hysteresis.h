/*
 * hysteresis.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef HYSTERESIS_H_INCLUDED
#define HYSTERESIS_H_INCLUDED

class FileData;

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

void hysteresis(FileData &fd);
void hysteresis(Mat img, Size size, String name, FileData &fd);

#endif /* HYSTERESIS_H_ */
