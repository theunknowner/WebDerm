/*
 * run.h
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#ifndef RUN_H_INCLUDED
#define RUN_H_INCLUDED

#include "global.h"

namespace Run {

void runBlurImage();
void runResizeImage();
void runResizeAllImages();
Mat runResizeImage(String foldername, String filename, Size size,int write);
Mat runResizeImage(String filename, Size size, int write=0);
Mat runResizeImage(Mat img, Size size);
void runHysteresis();
void runAllHysteresis();
void runGetSkin();
void runAllGetSkin();
void getMouseInfo(Mat &img);
void runMouseColor();
void runMouseOutputColor();
void runRenameFiles();
Mat runColorNormalization(Mat &img);

}

#endif /* RUN_H_ */
