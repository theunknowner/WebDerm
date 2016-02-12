/*
 * Skin.h
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#ifndef SKIN_H_
#define SKIN_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ImageData/imagedata.h"

class ImageData;
namespace Skin {
	Mat getSkin(Mat &img);
	void getSkinUsingThresh(Mat &img, Mat &mask);
	void getSkinUsingCorrelation(Mat &img, Mat &mask);

	Mat getSkinUsingKmeans(ImageData &id);
};

#endif /* SKIN_H_ */
