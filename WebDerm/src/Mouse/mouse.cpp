/*
 * mouse.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: jason
 */

#include "mouse.h"

String filename;

void onMouseCheckColor(int event, int x, int y, int flags, void* param) {
	Rgb rgb;
	Hsl hsl;
	Mat *img = (Mat*) param;
	Mat img2 = (*img).clone();
	Mat img3 = img3.zeros(Size(300,150),CV_8UC3);
	String color;
	double * HSL;
	int h, s,l;
	char coordText[25];
	char rgbText[25];
	char hslText[25];
	char c[25];
	char gl[25];
	char cl[25];
	char rl[25];
	double graylevel=0, colorlevel=0;
	int relLum=0;
	if(img2.type()==0) {
		relLum = img2.at<uchar>(y,x);
		sprintf(coordText, "(%d,%d)", x, y);
		sprintf(rl,"RelLum: %d",relLum);
		putText(img3, coordText, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, rl, Point(5,30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		imshow("Info",img3);
	}
	else {
		Vec3b rgbVec = img2.at<Vec3b>(y,x);
		HSL = hsl.rgb2hsl(rgbVec[2],rgbVec[1],rgbVec[0]);
		h = (int)HSL[0];
		s = roundDecimal(HSL[1],2) * 100;
		l = roundDecimal(HSL[2],2) * 100;
		color = rgb.checkBlack(rgbVec[2],rgbVec[1],rgbVec[0]);
		if(color=="OTHER")
			color = rgb.calcColor(rgbVec[2],rgbVec[1],rgbVec[0]);
		graylevel = rgb.calcGrayLevel(rgbVec[2],rgbVec[1],rgbVec[0]);
		colorlevel = rgb.calcColorLevel(rgbVec[2],rgbVec[1],rgbVec[0]);
		relLum = rgb.calcPerceivedBrightness(rgbVec[2],rgbVec[1],rgbVec[0]);

		sprintf(coordText, "(%d,%d)", x, y);
		sprintf(rgbText, "RGB(%d,%d,%d)",rgbVec[2],rgbVec[1],rgbVec[0]);
		sprintf(hslText,"HSL(%d,%d,%d)",h,s,l);
		sprintf(c, "%s", color.c_str());
		sprintf(gl,"GL: %.0f",graylevel);
		sprintf(cl,"CL: %0.0f",colorlevel);
		sprintf(rl,"RelLum: %d(%.2f)",relLum,relLum/255.);
		/*
		putText(img3, coordText, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, rgbText, Point(5,30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, hslText, Point(5,45), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, c, Point(5,60), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, gl, Point(5,75), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, cl, Point(5,90), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		putText(img3, rl, Point(5,105), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
		 */
		char text[100];
		sprintf(text,"%s | %s | %s | %s | %s",coordText,rgbText,hslText,c,rl);
		displayStatusBar(filename,text,0);
		//imshow("Info",img3);
	}
}

void Mouse::mouseColor(Mat img, String name) {
	filename = name + "_MouseColor";
	namedWindow(filename, CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
	cvSetMouseCallback(filename.c_str(), onMouseCheckColor, &img);
	imshow(filename, img);
	waitKey(0);
}

deque< deque<String> > shadeColor;
String filename2;

void onMouseCheckOutputColor(int event, int x, int y, int flags, void* param) {
	Mat *img = (Mat*) param;
	Mat img2 = (*img).clone();
	Mat img3 = img3.zeros(Size(300,150),CV_8UC3);
	String color = shadeColor.at(y).at(x);
	char coordText[25];
	char rgbText[25];
	char c[50];
	Vec3b rgbVec = img2.at<Vec3b>(y,x);
	sprintf(coordText, "(%d,%d)", x, y);
	sprintf(rgbText, "RGB(%d,%d,%d)",rgbVec[2],rgbVec[1],rgbVec[0]);
	sprintf(c, "%s", color.c_str());
	char text[100];
	sprintf(text,"%s | %s | %s",coordText,rgbText,c);
	displayStatusBar(filename2,text,0);
}

void Mouse::mouseOutputColor(Mat img, FileData &fd) {
	shadeColor = fd.colorVec;
	filename2 = fd.filename + "_OutputColor";
	namedWindow(filename2, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	cvSetMouseCallback(filename2.c_str(), onMouseCheckOutputColor, &img);
	imshow(filename2, img);
	waitKey(0);
}
