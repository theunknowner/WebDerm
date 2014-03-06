/*
 * run.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef RUN_H_INCLUDED
#define RUN_H_INCLUDED

#include "functions.h"
#include "rgb/rgb.h"
#include "hsl/hsl.h"
#include "hysteresis/hysteresis.h"
#include "skin/Skin.h"
#include "scripts/scripts.h"

void runCheckRGBColor();
void runCheckList();
Mat runResizeImage(String foldername, String filename, Size size,int write);
Mat runResizeImage(String filename, Size size,int write);
void runHysteresis();
void runCheckSequence(Mat &img, int row, int col, Size size);
void runGetSkin();
void getMouseInfo(Mat &img);
void runMouseHysteresis();
void runMouseHysteresis2();
void runOutputColorFreq();

#endif /* RUN_H_ */
