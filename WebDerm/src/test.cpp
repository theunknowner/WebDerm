/*
 * test.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/test.h"

String testHysteresis(Mat &img, int row, int col, Size size)
{
	rgb rgb;
	hsl hsl;
	Intensity in;
	Color colorObj;
	int b,g,r;
	int ind=0;
	double dist=0;
	double grayLevel=0;
	double* HSL;
	int hue=0;
	double hslAvg[3]={0};
	deque<double> hueWindow;
	deque<double> satWindow;
	deque<double> lumWindow;
	String hslStr;
	int count=0; //stores the amount of pixels hit
	deque<int> index;
	String pix;
	deque<String> pixelColorWindow;
	int colorIndex[rgbColors.size()];
	int mainColorIndex[mainColors.size()];
	double mainColorLevels[mainColors.size()];
	double mainColorLevelAvg[mainColors.size()];
	int dimension = size.width*size.height;
	int matchingScans = dimension/2;
	int windowFlags[dimension];
	fill_n(windowFlags,dimension,0);
	fill_n(colorIndex,rgbColors.size(),0);
	fill_n(mainColorIndex,mainColors.size(),0);
	fill_n(mainColorLevelAvg,mainColors.size(),0);
	for(int x=(col-1); x<(col+size.width-1); x++)
	{
		for(int y=(row-1); y<(row+size.height-1); y++)
		{
			b = img.at<Vec3b>(y,x)[0];
			g = img.at<Vec3b>(y,x)[1];
			r = img.at<Vec3b>(y,x)[2];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER")
			{
				pix = rgb.calcColor(r,g,b,dist,ind);
			}
			HSL = hsl.rgb2hsl(r,g,b);
			HSL[1] = roundDecimal(HSL[1],2);
			HSL[2] = roundDecimal(HSL[2],2);
			hueWindow.push_back(HSL[0]);
			satWindow.push_back(HSL[1]);
			lumWindow.push_back(HSL[2]);
			pixelColorWindow.push_back(pix);
			cout << pix << img.at<Vec3b>(y,x) << ";" << ind+2 << ";" << endl;
		}
	}
	for(unsigned int i=0; i<pixelColorWindow.size(); i++)
	{
		for(unsigned int j=0; j<mainColors.size(); j++)
		{
			if(colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j))!=0)
			mainColorIndex[j]++;
		}
	}
	for(unsigned int j=0; j<mainColors.size(); j++)
	{
		if(mainColorIndex[j]!=0)
		{
			cout << mainColors.at(j) << ":"<< mainColorIndex[j] << endl;
		}
		if(mainColorIndex[j]>=round(matchingScans))
		{
			index.push_back(j);
		}
	}
	for(unsigned int i=0; i<pixelColorWindow.size(); i++)
	{
		for(unsigned int j=0; j<index.size(); j++)
		{
			mainColorLevels[index.at(j)] = rgb.getColorLevel(pixelColorWindow.at(i),
					mainColors.at(index.at(j)));
			mainColorLevelAvg[index.at(j)] += mainColorLevels[index.at(j)];
			if(pixelColorWindow.at(i).find(mainColors.at(index.at(j)))!=string::npos) {
				windowFlags[i] = 1;
			}
		}
	}
	for(int i=0; i<dimension; i++) {
		if(windowFlags[i]==true) {
			grayLevel += rgb.getGrayLevel1(pixelColorWindow.at(i));
			hue =  hueWindow.at(i) + 180;
			if(hue>=360) hue %= 360;
			hslAvg[0] += hue;
			hslAvg[1] += satWindow.at(i);
			hslAvg[2] += lumWindow.at(i);
			count++;
		}
	}
	for(unsigned int i=0; i <index.size(); i++)
	{
		mainColorLevelAvg[index.at(i)] /= mainColorIndex[index.at(i)];
	}
	double h = 0, s = 0, l = 0;
	if(index.size()!=0)
	{
		pix.clear();
		for(unsigned int i=0; i<index.size(); i++)
		{
			if(mainColors.at(index[i])=="Zero") {
				pix = mainColors.at(index[i]);
				break;
			}
			else if(mainColors.at(index[i])=="Black" || mainColors.at(index[i])=="White") {
				pix = mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
				break;
			}
			else
				pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
		}
		grayLevel = round(grayLevel/count);
		hslAvg[0] = round(hslAvg[0]/count);
		hslAvg[1] = roundDecimal(hslAvg[1]/count,2);
		hslAvg[2] = roundDecimal(hslAvg[2]/count,2);
		hslAvg[0] -= 180;
		if(hslAvg[0]<0) hslAvg[0] += 360;
		h = hslAvg[0];
		s = hslAvg[1];
		l = hslAvg[2];
		pix = colorObj.fixColors(pix,r,g,b);
		if(pix!="Zero")
			pix = toString(grayLevel) + pix;
	}
	else pix = "NOISE";

	hslStr = toString(h)+";"+toString(s)+";"+toString(l);
	cout << "HSL_Avg: " << hslStr << endl;
	//cout << "COLORINT:" << in.calcIntensity(pix) << endl;
	deque<int>().swap(index);
	deque<String>().swap(pixelColorWindow);
	//img.release();
	return pix;
}

//hysteresis with info output to image window using vectors
String testMouseHysteresis(Mat &img, int row, int col, Size size,
						   deque<Vec3b> &vec, deque<String> &colorVec)
{
	rgb rgb;
	Color colorObj;
	int b,g,r;
	deque<int> index;
	String pix;
	deque<String> pixelColorWindow;
	int colorIndex[rgbColors.size()];
	int mainColorIndex[mainColors.size()];
	int matchingScans = (size.width*size.height)/2;
	fill_n(colorIndex,rgbColors.size(),0);
	fill_n(mainColorIndex,mainColors.size(),0);
	for(int x=(col-1); x<(col+size.width-1); x++)
	{
		for(int y=(row-1); y<(row+size.height-1); y++)
		{
			b = img.at<Vec3b>(y,x)[0];
			g = img.at<Vec3b>(y,x)[1];
			r = img.at<Vec3b>(y,x)[2];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER")
			{
				pix = rgb.pushColor(r,g,b);
				pixelColorWindow.push_back(pix);
			}
			else
			{
				pixelColorWindow.push_back(pix);
			}
			vec.push_back(img.at<Vec3b>(y,x));
			colorVec.push_back(pix);
		}
	}
	int count=0;
	for(unsigned int i=0; i<pixelColorWindow.size(); i++)
	{
		for(unsigned int j=0; j<mainColors.size(); j++)
		{
			count = colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j));
			mainColorIndex[j]+=count;
		}
	}
	for(unsigned int j=0; j<mainColors.size(); j++)
	{
		if(mainColorIndex[j]>matchingScans)
		{
			if(mainColorIndex[j]>(matchingScans*size.height))
			{
				index.push_back(j);
				index.push_back(j);
			}
			else
				index.push_back(j);
		}
	}
	if(index.size()!=0)
	{
		pix.clear();
		if(index.size()==1 && mainColors.at(index[0])=="Light")
		{
			pix = "White";
		}
		else if(index.size()==1 && mainColors.at(index[0])=="Dark")
		{
			pix = "Black";
		}
		else
		{
			for(unsigned int i=0; i<index.size(); i++)
			{
				pix += mainColors.at(index[i]);
			}
		}
	}
	else pix = "NOISE";

	deque<int>().swap(index);
	deque<String>().swap(pixelColorWindow);
	//img.release();
	return pix;
}

void testColorIndex(Mat &img, int index)
{
	rgb rgb;
	int r,g,b;
	int ind=0;
	double dist=0;
	int count=0;
	Mat mask = mask.zeros(img.size(), CV_8U);
	Mat img2;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			rgb.pushColor(r,g,b,ind,dist);
			if((ind+2)==index)
			{
				//printf("%d,%d\n", col,row);
				mask.at<uchar>(row,col) = 255;
				++count;
			}
		}
	}
	cout << count << endl;
	img.copyTo(img2,mask);
	imshow("Img", img);
	imshow("Img2",img2);
	waitKey(0);
}

String testColorAtLoc(Mat &img, Point pt, double &h) {
	rgb rgb;
	hsl hsl;
	int r,g,b;
	int ind= -3;
	double dist=0;
	int hue=0;
	String pix;
	r = img.at<Vec3b>(pt.y-1,pt.x-1)[2];
	g = img.at<Vec3b>(pt.y-1,pt.x-1)[1];
	b = img.at<Vec3b>(pt.y-1,pt.x-1)[0];
	pix = rgb.checkBlack(r,g,b);
	hsl.rgb2hsl(r,g,b);
	if(pix=="OTHER") {
		pix = rgb.calcColor2(r,g,b);
		if(pix=="OTHER") {
			pix = rgb.pushColor(r,g,b,ind,dist);
		}
	}
	hue = (hsl.getHue()+180);
	hue %= 360;
	hue /= 360;
	h = hue;
	cout << pix << img.at<Vec3b>(pt.y-1,pt.x-1) << ";" << ind+2 << ";" << hue<< endl;
	return pix;
}
