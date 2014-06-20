/*
 * run.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef RUN_H_INCLUDED
#define RUN_H_INCLUDED

#include "functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/skin/Skin.h"
#include "/home/jason/git/WebDerm/WebDerm/src/scripts/scripts.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"

void runCheckRGBColor();
void runCheckList();
Mat runResizeImage(String foldername, String filename, Size size,int write);
Mat runResizeImage(String filename, Size size,int write);
void runHysteresis();
void runAllHysteresis(String *filenames, int fileSize);
void runCheckSequence(Mat &img, int row, int col, Size size);
void runGetSkin();
void getMouseInfo(Mat &img);
void runMouseHysteresis();
void runMouseHysteresis2();
void outputFreqColor(Mat &img);
void runOutputColorFreq();
void runOutputFarRGB();

#endif /* RUN_H_ */
