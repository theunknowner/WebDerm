/*
 * pixeldata.h
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#ifndef PIXELDATA_H_
#define PIXELDATA_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class PixelData {
protected:
	int grayLumLevel;
	int grayLevel;
	int colorLevel;
	String pixColor;
	int pixShade;
	String pixShadeName;
public:
	int gray_lum_level();
	int gray_level();
	int color_level();
	String color();
	int shade();
	String shade_name();
};

#endif /* PIXELDATA_H_ */
