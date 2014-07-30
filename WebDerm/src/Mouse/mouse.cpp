/*
 * mouse.cpp
 *
 *  Created on: Jul 30, 2014
 *      Author: jason
 */

#include "mouse.h"

void onMouseCheckColor(int event, int x, int y, int flags, void* param) {
	rgb rgb;
	hsl hsl;
	Mat *img = (Mat*) param;
	Mat img2 = (*img).clone();
	Mat img3 = img3.zeros(Size(300,150),img2.type());
	String color;
	double * HSL;
	int h, s,l;
    char text[100];
    char hslText[100];
    char c[30];
    char gl[50];
    char cl[50];
    char ratioText[50];
    double graylevel=0, colorlevel=0, ratio=0;
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
    ratio = graylevel/colorlevel;

    sprintf(text, "(%d,%d) - [%d,%d,%d]", x, y,rgbVec[2],rgbVec[1],rgbVec[0]);
    sprintf(hslText,"{%d,%d,%d}",h,s,l);
    sprintf(c, "%s", color.c_str());
    sprintf(gl,"GL: %.0f",graylevel);
    sprintf(cl,"CL: %0.0f",colorlevel);
    sprintf(ratioText,"GLL/CL: %0.3f",ratio);
    putText(img3, text, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, hslText, Point(5,30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, c, Point(5,45), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, gl, Point(5,60), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, cl, Point(5,75), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, ratioText, Point(5,90), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    imshow("Info",img3);
}

void Mouse::mouseColor(Mat img) {
	namedWindow("image");
	cvSetMouseCallback("image", onMouseCheckColor, &img);
	imshow("image", img);
	waitKey(0);
}
