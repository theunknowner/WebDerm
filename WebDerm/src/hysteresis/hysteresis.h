/*
 * hysteresis.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef HYSTERESIS_H_INCLUDED
#define HYSTERESIS_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Intensity/intensity.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"

class FileData;
void hysteresis(FileData &fd);
void hysteresis(Mat img, Size size, String name, FileData &fd);

#endif /* HYSTERESIS_H_ */
