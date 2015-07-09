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
	String determinePixelColor(int &r, int &g, int &b);
protected:
	vector<vector<PixelData> > pixelVec;
	String file_path;
	vector<vector<String> > dataVec;
	vector<vector<String> > hslVec;
	Size imgSize;
	Mat matImage;
	String imgName;
public:
	void extract(Mat image, String name);
	String name();
	String path();
	Size image_size();
	Mat image();
	vector<vector<String> > data_matrix();
	vector<vector<String> > hsl_matrix();
	PixelData at(int row, int col);
};

#endif /* IMAGEDATA_H_ */
