/*
 * functions2.h
 *
 *  Created on: Jan 31, 2014
 *      Author: jason
 */

#ifndef FUNCTIONS2_H_INCLUDED
#define FUNCTIONS2_H_INCLUDED

#include "global.h"
#include "functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/test.h"

void getSubstr(String str, char delimiter, deque<String> &vec);
void getCoordinates(Mat &img, int red, int green, int blue);
String getFileName(String filename,String end="");

#endif /* FUNCTIONS2_H_ */
