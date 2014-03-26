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

void getSubstr(String str, char delimiter, vector<String> &vec);
void getCoordinates(Mat &img, int red, int green, int blue);
String getImageName(String filename);
void mouseEvent(int evt, int x, int y, int flags, void* param);
void onMouseHysteresis(int evt, int x, int y, int flags, void* param);
void onMouseHysteresis2(int evt, int x, int y, int flags, void* param);
void onMouseCheckColor(int event, int x, int y, int flags, void* param);

#endif /* FUNCTIONS2_H_ */
