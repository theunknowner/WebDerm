/*
 * Skin.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef SKIN_H_
#define SKIN_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"

void getSkin(Mat &img, Mat &mask);
void getSkinUsingThresh(Mat &img, Mat &mask);
void getSkinUsingCorrelation(Mat &img, Mat &mask);

#endif /* SKIN_H_ */
