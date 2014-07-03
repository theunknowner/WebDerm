/*
 * hysteresis.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hysteresis.h"



//hysteresis moving 1 col/row at a time
	void hysteresis(Mat img, Size size, String name)
	{
		//FILE * fp;
		//fp = fopen("farColors.csv","w");
		rgb rgb;
		hsl hsl;
		Color colorObj;
		double matchingScans = (size.width*size.height)/2;
		deque<String> pixelColorWindow;
		deque<String> colorWindow;
		deque< deque<String> > windowVec;
		deque<int> index;
		deque<double> hueWindow;
		deque<double> satWindow;
		deque<double> lumWindow;
		deque<double> hueVec;
		deque<double> satVec;
		deque<double> lumVec;
		deque< deque<double> > hueMat;
		deque< deque<double> > satMat;
		deque< deque<double> > lumMat;
		int mainColorIndex[mainColors.size()];
		double mainColorLevels[mainColors.size()];
		double mainColorLevelAvg[mainColors.size()];
		String pix;
		int ind=0;
		double dist=0;
		double grayLevel=0;
		int b=0,g=0,r=0;
		double* HSL;
		int hue;
		double hslAvg[3]={0};
		int row=0, col=0;
		fill_n(mainColorIndex,mainColors.size(),0);
		fill_n(mainColorLevelAvg,mainColors.size(),0);
		while(row<=(img.rows-size.height))
		{
			while(col<=(img.cols-size.width))
			{
				if(col==0)
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
					}
					grayLevel += rgb.getGrayLevel1(pixelColorWindow.at(i));
					hue =  hueWindow.at(i) + 180;
					if(hue>360) hslAvg[0] = 0;
					hue %= 360;
					hslAvg[0] += hue;
					hslAvg[1] += satWindow.at(i);
					hslAvg[2] += lumWindow.at(i);
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
						if(mainColors.at(index[i])=="Black" || mainColors.at(index[i])=="White") {
							pix = mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
							break;
						}
						else
							pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
					}
					pix  = colorObj.fixColors(pix,r,g,b);
					grayLevel = round(grayLevel/(size.width*size.height));
					hslAvg[0] = round(hslAvg[0]/(size.width*size.height));
					hslAvg[1] = round(hslAvg[1]/(size.width*size.height));
					hslAvg[2] = round(hslAvg[2]/(size.width*size.height));
					hslAvg[0] -= 180;
					if(hslAvg[0]<0) hslAvg[0] += 360;
					hueVec.push_back(hslAvg[0]);
					satVec.push_back(hslAvg[1]);
					lumVec.push_back(hslAvg[2]);
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
				fill_n(mainColorIndex,mainColors.size(),0);
				fill_n(mainColorLevelAvg,mainColors.size(),0);
				index.clear();
				++col;
			}//end while col
			hueMat.push_back(hueVec);
			satMat.push_back(satVec);
			lumMat.push_back(lumVec);
			windowVec.push_back(colorWindow);
			colorWindow.clear();
			pixelColorWindow.clear();
			hueWindow.clear();
			satWindow.clear();
			lumWindow.clear();
			col=0; ++row;
		}//end while row
		writeSeq2File(windowVec,name);
		//contrast con;
		//con.writeMainColorMatrix(windowVec,name);
		//con.calcContrastFromMatrix(windowVec,hueVec,filename);
		Intensity in;
		in.writeMainColorMatrix(img, windowVec,name);
		deque<String>().swap(pixelColorWindow);
		deque<String>().swap(colorWindow);
		deque< deque<String> >().swap(windowVec);
		deque<int>().swap(index);
		//fclose(fp);
	}

	void hysteresis1x1(Mat img, String name) {
		rgb rgb;
		hsl hsl;
		Color colorObj;
		int r,g,b;
		String pix;
		deque<String> colorWindow;
		deque< deque<String> > windowVec;
		double dist=0;
		int ind=0;
		for(int row=0; row<img.rows; row++) {
			for(int col=0; col<img.cols; col++) {
				r = img.at<Vec3b>(row,col)[2];
				g = img.at<Vec3b>(row,col)[1];
				b = img.at<Vec3b>(row,col)[0];
				pix = rgb.checkBlack(r,g,b);
				if(pix=="OTHER") {
					pix = rgb.calcColor(r,g,b,dist,ind);
				}
				colorWindow.push_back(pix);
			}
			windowVec.push_back(colorWindow);
			colorWindow.clear();
		}
		writeSeq2File(windowVec,name);
		//con.writeMainColorMatrix(windowVec,name);
		//String filename = name + "Contrast";
		//con.calcContrastFromMatrix(windowVec,filename);
		Intensity in;
		in.writeIntensityMatrix(windowVec,name);
		in.writeMainColorMatrix(img, windowVec,name);
		deque<String>().swap(colorWindow);
		deque< deque<String> >().swap(windowVec);
	}
