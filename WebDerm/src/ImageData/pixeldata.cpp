/*
 * pixeldata.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#include "pixeldata.h"

//! returns value of pixel's gray luminance level
int PixelData::gray_lum_level() {
	return this->grayLumLevel;
}

//! returns value of pixel's gray level
int PixelData::gray_level() {
	return this->grayLevel;
}

//! returns value of pixel's color level
int PixelData::color_level() {
	return this->colorLevel;
}

//! returns name of pixel's color
String PixelData::color() {
	return this->color();
}

int PixelData::shade() {
	return this->pixShade;
}

String PixelData::shade_name() {
	return this->pixShadeName;
}
