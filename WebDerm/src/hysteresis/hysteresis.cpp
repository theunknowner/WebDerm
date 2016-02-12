/*
 * hysteresis.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Intensity/intensity.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Intensity/rules.h"

void hysteresis(FileData &fd) {
	hysteresis(fd.getImage(),fd.ksize,fd.filename,fd);
}

void hysteresis(Mat img, Size size, String name, FileData &fd) {
	blur(img,img,size);
	Rgb rgb;
	Hsl hsl;
	int r,g,b;
	vector<double> HSL;
	double h,s,l;
	String pix;
	String hslStr;
	deque<String> colorWindow;
	deque<String> hslVec;
	for(int i=0; i<img.rows; i++)  {
		for(int j=0; j<img.cols; j++)  {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			HSL = hsl.rgb2hsl(r,g,b);
			h = HSL[0];
			s = ip::roundDecimal(HSL[1],2);
			l = ip::roundDecimal(HSL[2],2);
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.calcColor(r,g,b);
			}
			colorWindow.push_back(pix);
			hslStr = ip::toString(h)+";"+ip::toString(s)+";"+ip::toString(l);
			hslVec.push_back(hslStr);
		}
		fd.windowVec.push_back(colorWindow);
		fd.hslMat.push_back(hslVec);
		colorWindow.clear();
		hslVec.clear();
	}
	Intensity in;
	in.calcMainColorMatrix(fd.getImage(), fd.windowVec, fd.hslMat, fd.filename, fd);
	//rule5(fd);
	cout << "Done!" << endl;
	colorWindow.clear();
	colorWindow.shrink_to_fit();
	hslVec.clear();
	hslVec.shrink_to_fit();
}
