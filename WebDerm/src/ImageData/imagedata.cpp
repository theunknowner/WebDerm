/*
 * imagedata.cpp
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#include "imagedata.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "pixeldata.h"

/************************* PRIVATE FUNCTIONS *****************************/

void ImageData::storePixelData(PixelData pd, int row, int col) {
	this->pixelVec.at(row).at(col) = pd;
}

/*************************** PUBLIC FUNCTIONS *************************/

//! extracts pixel info from image
void ImageData::extract(Mat image, String name) {
	this->imgName = name;
	this->matImage = image.clone();
	this->imgSize = image.size();
	this->imgRows = image.rows;
	this->imgCols = image.cols;
	this->pixelVec.resize(image.rows,vector<PixelData>(image.cols,PixelData()));
	this->dataVec.resize(image.rows,vector<String>(image.cols,""));
	this->hslVec.resize(image.rows,vector<String>(image.cols,""));
	double h,s,l;
	String pix, hslStr;
	for(int i=0; i<image.rows; i++) {
		for(int j=0; j<image.cols; j++) {
			PixelData pixData(image.at<Vec3b>(i,j));
			this->storePixelData(pixData,i,j);
			this->dataVec.at(i).at(j) = pixData.color();

			h = pixData.hsl()[0];
			s = pixData.hsl()[1];
			l = pixData.hsl()[2];
			hslStr = toString(h)+";"+toString(s)+";"+toString(l);
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
Size ImageData::size() {
	return this->imgSize;
}

int ImageData::rows() {
	return this->imgRows;
}

int ImageData::cols() {
	return this->imgCols;
}

//! returns mat type image
Mat ImageData::image() {
	return this->matImage;
}

//! returns the image's data for each pixel in 2D string
vector<vector<String> > ImageData::data_matrix() {
	return this->dataVec;
}

//! returns the image's hsl data for each pixel in 2S string
vector<vector<String> > ImageData::hsl_matrix() {
	return this->hslVec;
}

//! returns PixelData object pixel (row,col)
PixelData ImageData::pixel(int row, int col) {
	return this->pixelVec.at(row).at(col);
}
