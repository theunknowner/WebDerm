/*
 * hysteresis.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hysteresis.h"

void hysteresis(FileData &fd) {
	hysteresis(fd.matImage,fd.matSize,fd.filename,fd);
}

//hysteresis moving 1 col/row at a time
	void hysteresis(Mat img, Size size, String name, FileData &fd)
	{
		rgb rgb;
		hsl hsl;
		Color colorObj;
		int dimension = size.width*size.height;
		int matchingScans = dimension/2;
		int windowFlags[dimension];
		deque<String> pixelColorWindow;
		deque<String> colorWindow;
		deque<int> index;
		deque<double> hueWindow;
		deque<double> satWindow;
		deque<double> lumWindow;
		deque<String> hslVec;
		int mainColorIndex[mainColors.size()];
		double mainColorLevels[mainColors.size()];
		double mainColorLevelAvg[mainColors.size()];
		String pix;
		int ind=0;
		double dist=0;
		double grayLevel=0;
		int b=0,g=0,r=0;
		double* HSL;
		int hue=0;
		double hslAvg[3]={0};
		int row=0, col=0;
		int count=0; //stores the amount of pixels hit
		fill_n(windowFlags,dimension,0);
		fill_n(mainColorIndex,mainColors.size(),0);
		fill_n(mainColorLevelAvg,mainColors.size(),0);
		while(row<=(img.rows-size.height))
		{
			while(col<=(img.cols-size.width))
			{
				if(pixelColorWindow.size()==0)
				{
					for(int x=col; x<(col+size.width); x++)
					{
						for(int y=row; y<(row+size.height); y++)
						{
							b = img.at<Vec3b>(y,x)[0];
							g = img.at<Vec3b>(y,x)[1];
							r = img.at<Vec3b>(y,x)[2];
							pix = rgb.checkBlack(r,g,b);
							if(pix=="OTHER") {
								pix = rgb.calcColor(r,g,b,dist,ind);
								//if(dist>10)
									//fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
							}
							HSL = hsl.rgb2hsl(r,g,b);
							HSL[1] = roundDecimal(HSL[1],2);
							HSL[2] = roundDecimal(HSL[2],2);
							hueWindow.push_back(HSL[0]);
							satWindow.push_back(HSL[1]);
							lumWindow.push_back(HSL[2]);
							pixelColorWindow.push_back(pix);
						}
					}
				}
				else
				{
					for(int y=row; y<(row+size.height); y++)
					{
						b = img.at<Vec3b>(y,col+(size.width-1))[0];
						g = img.at<Vec3b>(y,col+(size.width-1))[1];
						r = img.at<Vec3b>(y,col+(size.width-1))[2];
						pix = rgb.checkBlack(r,g,b);
						if(pix=="OTHER") {
							pix = rgb.calcColor(r,g,b,dist,ind);
							//if(dist>10)
								//fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
						}
						HSL = hsl.rgb2hsl(r,g,b);
						HSL[1] = roundDecimal(HSL[1],2);
						HSL[2] = roundDecimal(HSL[2],2);
						hueWindow.pop_front();
						satWindow.pop_front();
						lumWindow.pop_front();
						hueWindow.push_back(HSL[0]);
						satWindow.push_back(HSL[1]);
						lumWindow.push_back(HSL[2]);
						pixelColorWindow.pop_front();
						pixelColorWindow.push_back(pix);
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
					double h = hslAvg[0];
					double s = hslAvg[1];
					double l = hslAvg[2];
					String hslStr = toString(h)+";"+toString(s)+";"+toString(l);
					hslVec.push_back(hslStr);
					pix = colorObj.fixColors(pix,r,g,b);
					if(pix!="Zero")
						pix = toString(grayLevel) + pix;
					colorWindow.push_back(pix);
				}
				else
				{
					colorWindow.push_back("NZ");
				}
				grayLevel=0;
				hslAvg[0] = 0;
				hslAvg[1] = 0;
				hslAvg[2] = 0;
				count=0;
				fill_n(windowFlags,dimension,0);
				fill_n(mainColorIndex,mainColors.size(),0);
				fill_n(mainColorLevelAvg,mainColors.size(),0);
				index.clear();
				/* remove window clears when using FIFO
				hueWindow.clear();
				satWindow.clear();
				lumWindow.clear();
				pixelColorWindow.clear();*/
				col++;
			}//end while col
			fd.hslMat.push_back(hslVec);
			fd.windowVec.push_back(colorWindow);
			colorWindow.clear();
			pixelColorWindow.clear();
			hueWindow.clear();
			satWindow.clear();
			lumWindow.clear();
			hslVec.clear();

			col=0; row++;
		}//end while row
		Intensity in;
		in.writeMainColorMatrix(fd);
		deque<String>().swap(pixelColorWindow);
		deque<String>().swap(colorWindow);
		deque<int>().swap(index);
		deque<String>().swap(hslVec);
		deque<double>().swap(hueWindow);
		deque<double>().swap(satWindow);
		deque<double>().swap(lumWindow);
	}
