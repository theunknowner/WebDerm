/*
 * imagedata.h
 *
 *  Created on: Jul 8, 2015
 *      Author: jason
 */

#ifndef IMAGEDATA_H_
#define IMAGEDATA_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "pixeldata.h"

class PixelData;
class ImageData {
private:
	void storePixelData(PixelData pd, int row, int col);

protected:
	vector<vector<PixelData> > pixelVec;
	String file_path;
	vector<vector<String> > dataVec;
	vector<vector<String> > hslVec;
	Size imgSize;
	int imgRows, imgCols;
	Mat matImage;
	String imgName;

public:
	void extract(Mat image, String name="");
	String name();
	String path();
	Size size();
	int rows();
	int cols();
	Mat image();
	vector<vector<String> > data_matrix();
	vector<vector<String> > hsl_matrix();
	PixelData pixel(int row, int col);
};

#endif /* IMAGEDATA_H_ */