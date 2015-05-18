/*
 * Skin.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef SKIN_H_
#define SKIN_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

namespace Skin {
	void getSkin(Mat &img, Mat &mask);
	void getSkinUsingThresh(Mat &img, Mat &mask);
	void getSkinUsingCorrelation(Mat &img, Mat &mask);
	Mat getSkinUsingHist(Mat &img);
};

#endif /* SKIN_H_ */
