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
private:
	String determinePixelColor(int &r, int &g, int &b);

protected:
	Vec3b rgbVec;
	Vec3f hslVec;
	int grayLumLevel;
	int grayLevel;
	int colorLevel;
	String pixColor;
	String pixShade;
public:
	PixelData();
	PixelData(Vec3b pix);
	int gray_lum_level();
	int gray_level();
	int color_level();
	String color();
	String& shade();
	Vec3b rgb();
	Vec3f hsl();
};

#endif /* PIXELDATA_H_ */
