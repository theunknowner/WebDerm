/*
 * pixeldata.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#include "pixeldata.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"

/******************** PRIVATE FUNCTIONS ***********************/

//! determines pixel color and levels
String PixelData::determinePixelColor(int &r, int &g, int &b) {
	Rgb rgb;
	String pixColor = rgb.checkBlack(r,g,b);
	if(pixColor=="OTHER") {
		pixColor = rgb.calcColor(r,g,b);
	}
	return pixColor;
}

/********************* PUBLIC FUNCTIONS *************************/

PixelData::PixelData(){};

PixelData::PixelData(Vec3b pix) {
	int r = pix[2];
	int g = pix[1];
	int b = pix[0];
	this->pixColor = this->determinePixelColor(r,g,b);
	this->rgbVec[0] = r;
	this->rgbVec[1] = g;
	this->rgbVec[2] = b;

	Hsl hsl;
	vector<double> HSL = hsl.rgb2hsl(r,g,b);
	this->hslVec[0] = HSL[0];
	this->hslVec[1] = roundDecimal(HSL[1],2);
	this->hslVec[2] = roundDecimal(HSL[2],2);

	Rgb rgb;
	this->grayLevel = rgb.calcGrayLevel(r,g,b);
	this->grayLumLevel = rgb.calcGrayLumLevel(r,g,b);
	this->colorLevel = rgb.calcColorLevel(r,g,b);

}
//! returns value of pixel's gray luminance level
//! value after gray
int PixelData::gray_lum_level() {
	return this->grayLumLevel;
}

//! returns value of pixel's gray level
//! value before gray
int PixelData::gray_level() {
	return this->grayLevel;
}

//! returns value of pixel's color level
int PixelData::color_level() {
	return this->colorLevel;
}

//! returns name of pixel's color
String PixelData::color() {
	return this->pixColor;
}

String& PixelData::shade() {
	return this->pixShade;
}

Vec3b PixelData::rgb() {
	return this->rgbVec;
}

Vec3f PixelData::hsl() {
	return this->hslVec;
}
