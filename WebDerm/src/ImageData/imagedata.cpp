/*
 * imagedata.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#include "imagedata.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"

/************************* PRIVATE FUNCTIONS *****************************/

//! determines pixel color and levels
String ImageData::determinePixelColor(int &r, int &g, int &b) {
	Rgb rgb;
	String pix = rgb.checkBlack(r,g,b);
	if(pix=="OTHER") {
		pix = rgb.calcColor(r,g,b);
	}
	return pix;
}

/*************************** PUBLIC FUNCTIONS *************************/

//! extracts pixel info from image
void ImageData::extract(Mat image, String name) {
	this->imgName = name;
	this->matImage = image.clone();
	this->dataVec.resize(image.rows,vector<String>(image.cols,""));
	this->hslVec.resize(image.rows,vector<String>(image.cols,""));
	int r,g,b;
	vector<double> HSL;
	String pix, hslStr;
	Hsl hsl;
	for(int i=0; i<image.rows; i++) {
		for(int j=0; j<image.cols; j++) {
			r = image.at<Vec3b>(i,j)[2];
			g = image.at<Vec3b>(i,j)[1];
			b = image.at<Vec3b>(i,j)[0];
			pix = this->determinePixelColor(r,g,b);
			this->dataVec.at(i).at(j) = pix;

			HSL = hsl.rgb2hsl(r,g,b);
			HSL[1] = roundDecimal(HSL[1],2);
			HSL[2] = roundDecimal(HSL[2],2);
			hslStr = toString(HSL[0])+";"+toString(HSL[1])+";"+toString(HSL[2]);
			this->hslVec.at(i).at(j) = hslStr;
		}
	}
}

String ImageData::name() {
	return this->imgName;
}

//! returns file path of image
String ImageData::path() {
	return this->file_path;
}

//! return's size of image
Size ImageData::image_size() {
	return this->imgSize;
}

//! returns mat type image
Mat ImageData::image() {
	return this->matImage;
}

//! returns the image's data for each pixel in 2D string
vector<vector<String> > ImageData::data_matrix() {
	return this->dataVec;
}

vector<vector<String> > ImageData::hsl_matrix() {
	return this->hslVec;
}

//! returns PixelData object at (row,col)
PixelData ImageData::at(int row, int col) {
	return this->pixelVec.at(row).at(col);
}
