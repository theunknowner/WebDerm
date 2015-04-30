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
void hysteresis2(Mat img, Size size, String name, FileData &fd);
void hysteresis3(Mat img, Size size, String name, FileData &fd);

#endif /* HYSTERESIS_H_ */
