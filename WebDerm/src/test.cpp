/*
 * test.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#include "test.h"
int calcHueAvg2(deque<int> &vec) {
	int hue=0;
	int total=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		hue = vec.at(i);
		if(hue>=0 && hue<=13) {
			hue += 360;
		}
		total += hue;
	}
	total /= vec.size();
	if(total>=360) total -= 360;
	return total;
}
String testHysteresis(Mat &img, int row, int col, Size size)
{
	rgb rgb;
	hsl hsl;
	Color colorObj;
	int b,g,r;
	int ind=0;
	vector<int> index;
	String pix;
	deque<int> hueVals;
	deque<String> pixelColorWindow;
	int colorIndex[rgbColors.size()];
	int mainColorIndex[mainColors.size()];
	double mainColorLevels[mainColors.size()];
	double mainColorLevelAvg[mainColors.size()];
	double matchingScans = (size.width*size.height)/2;
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
			hsl.rgb2hsl(r,g,b);
			hueVals.push_back(hsl.getHue());
			if(pix=="OTHER")
			{
				pix = rgb.calcColor(r,g,b);
				if(pix=="OTHER") {
					pix = rgb.pushColor(r,g,b,ind);
				}
				pixelColorWindow.push_back(pix);
			}
			else
			{
				pixelColorWindow.push_back(pix);
			}
			cout << pix << img.at<Vec3b>(y,x) << "-" << ind+2 << "-" << hsl.getHue()<< endl;
		}
	}
	cout << calcHueAvg2(hueVals) << endl;
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
		}
	}
	for(unsigned int i=0; i <index.size(); i++)
	{
		mainColorLevelAvg[index.at(i)] /= mainColorIndex[index.at(i)];
	}
	if(index.size()!=0)
	{
		pix.clear();
		for(unsigned int i=0; i<index.size(); i++)
		{
			pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
		}
	}
	else pix = "NOISE";

	vector<int>().swap(index);
	deque<String>().swap(pixelColorWindow);
	//img.release();
	return pix;
}

//hysteresis with info output to image window using vectors
String testMouseHysteresis(Mat &img, int row, int col, Size size,
						   vector<Vec3b> &vec, vector<String> &colorVec)
{
	rgb rgb;
	Color colorObj;
	int b,g,r;
	vector<int> index;
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

	vector<int>().swap(index);
	deque<String>().swap(pixelColorWindow);
	//img.release();
	return pix;
}

//get color of pixel at coordinate of mouse location on image
void testMouseColor(Mat img)
{
	namedWindow("image");
	cvSetMouseCallback("image", onMouseCheckColor, &img);
	imshow("image", img);
	waitKey(0);
}

void testSatLum(Mat &img)
{
	hsl hsl;
	int r,g,b,hue;
	double lum=0, sat=0;
	char text[20];
	vector<String> vec;
	vector< vector<String> > windowVec;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			hsl.rgb2hsl(r,g,b);
			hue = hsl.getHue();
			lum = hsl.getLum();
			sat = hsl.getSat();
			lum = roundDecimal(lum,2) * 100;
			sat = roundDecimal(sat,2) * 100;
			sprintf(text,"(%d;%d;%d)", (int)hue,(int)sat,(int)lum);
			vec.push_back(text);
		}
		windowVec.push_back(vec);
		vec.clear();
	}
	writeSeq2File(windowVec,path,"satlum");
	vector< vector<String> >().swap(windowVec);
	vector<String>().swap(vec);
	hsl.release_memory();
}

void testColorIndex(Mat &img, int index)
{
	rgb rgb;
	int r,g,b;
	int ind;
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
			rgb.pushColor(r,g,b,ind);
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
