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
		FILE * fp;
		fp = fopen("farColors.csv","w");
		rgb rgb;
		Color colorObj;
		double matchingScans = (size.width*size.height)/2;
		deque<String> pixelColorWindow;
		deque<String> colorWindow;
		deque< deque<String> > windowVec;
		deque<int> index;
		int mainColorIndex[mainColors.size()];
		double mainColorLevels[mainColors.size()];
		double mainColorLevelAvg[mainColors.size()];
		String pix;
		int ind=0;
		double dist=0;
		int b=0,g=0,r=0;
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
							if(pix=="OTHER")
							{
								pix = rgb.calcColor2(r,g,b);
								if(pix=="OTHER") {
									pix = rgb.pushColor(r,g,b,dist,ind);
									if(dist>4)
										pix = colorObj.reassignLevels(pix,r,g,b);
									if(dist>10)
										fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
								}
							}
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
						if(pix=="OTHER")
						{
							pix = rgb.calcColor2(r,g,b);
							if(pix=="OTHER") {
								pix = rgb.pushColor(r,g,b,dist,ind);
								if(dist>4)
									pix = colorObj.reassignLevels(pix,r,g,b);
								if(dist>10)
									fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
							}
						}
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
					if(colorObj.getMainColor(pix)=="Gray")
						pix = "Grey" + toString(rgb.calcColorLevel2(r,g,b));
					colorWindow.push_back(pix);
				}
				else
				{
					colorWindow.push_back("NZ");
				}
				fill_n(mainColorIndex,mainColors.size(),0);
				fill_n(mainColorLevelAvg,mainColors.size(),0);
				index.clear();
				++col;
			}//end while col
			windowVec.push_back(colorWindow);
			colorWindow.clear();
			pixelColorWindow.clear();
			col=0; ++row;
		}//end while row
		writeSeq2File(windowVec,name);
		//contrast con;
		//con.writeMainColorMatrix(windowVec,name);
		//con.calcContrastFromMatrix(windowVec,hueVec,filename);
		Intensity in;
		//in.writeIntensityMatrix(windowVec,name);
		in.writeMainColorMatrix(windowVec,name);
		deque<String>().swap(pixelColorWindow);
		deque<String>().swap(colorWindow);
		deque< deque<String> >().swap(windowVec);
		deque<int>().swap(index);
	}

	void hysteresis1x1(Mat img, String name) {
		rgb rgb;
		hsl hsl;
		contrast con;
		Color colorObj;
		int r,g,b;
		String pix;
		deque<int> hueVals;
		deque<double> hueAvg;
		deque< deque<double> > hueVec;
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
				hsl.rgb2hsl(r,g,b);
				hueVals.push_back(hsl.getHue());
				if(pix=="OTHER") {
					pix = rgb.calcColor2(r,g,b);
					if(pix=="OTHER") {
						pix = rgb.pushColor(r,g,b,dist,ind);
						if(dist>4)
							pix = colorObj.reassignLevels(pix,r,g,b);
					}
				}
				hueAvg.push_back(hsl.calcHueAvg(hueVals));
				hueVals.clear();
				colorWindow.push_back(pix);
			}
			hueVec.push_back(hueAvg);
			hueAvg.clear();
			windowVec.push_back(colorWindow);
			colorWindow.clear();
		}
		writeSeq2File(windowVec,name);
		//con.writeMainColorMatrix(windowVec,name);
		//String filename = name + "Contrast";
		//con.calcContrastFromMatrix(windowVec,filename);
		Intensity in;
		in.writeIntensityMatrix(windowVec,name);
		in.writeMainColorMatrix(windowVec,name);
		deque<String>().swap(colorWindow);
		deque< deque<String> >().swap(windowVec);
	}
