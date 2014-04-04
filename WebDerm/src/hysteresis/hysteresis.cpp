/*
 * hysteresis.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hysteresis.h"

void writeSeq2File(vector< vector<String> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"[%s],", vec.at(i).at(j).c_str());
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(vector< vector<String> > &vec, String pathname, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%s,", vec.at(i).at(j).c_str());
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(vector< vector<int> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%d,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(vector< vector<double> > &vec, String pathname, String name)
{
	String filename = pathname + name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%f,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

int calcHueAvg(deque<int> &vec) {
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

//hysteresis moving 1 col/row at a time
	void hysteresis(Mat img, Size size, String name)
	{
		rgb rgb;
		hsl hsl;
		Color colorObj;
		double matchingScans = (size.width*size.height)/2;
		deque<String> pixelColorWindow;
		vector<String> colorWindow;
		vector< vector<String> > windowVec;
		vector<int> index;
		//deque<int>	hueVals;
		//vector<int> hueAvg;
		//vector< vector<int> > hueVec;
		int mainColorIndex[mainColors.size()];
		double mainColorLevels[mainColors.size()];
		double mainColorLevelAvg[mainColors.size()];
		String pix;
		int ind=0;
		double dist=0;
		int b,g,r;
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
							//hsl.rgb2hsl(r,g,b);
							//hueVals.push_back(hsl.getHue());
							if(pix=="OTHER")
							{
								pix = rgb.calcColor(r,g,b);
								if(pix=="OTHER") {
									pix = rgb.pushColor(r,g,b,dist,ind);
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
						//hsl.rgb2hsl(r,g,b);
						//hueVals.pop_front();
						//hueVals.push_back(hsl.getHue());
						if(pix=="OTHER")
						{
							pix = rgb.calcColor(r,g,b);
							if(pix=="OTHER") {
								pix = rgb.pushColor(r,g,b,dist,ind);
							}
						}
						pixelColorWindow.pop_front();
						pixelColorWindow.push_back(pix);
					}
				}
				//hueAvg.push_back(calcHueAvg(hueVals));
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
					mainColorLevelAvg[index.at(i)] /= (size.width*size.height);
				}
				if(index.size()!=0)
				{
					pix.clear();
					for(unsigned int i=0; i<index.size(); i++)
					{
						pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
					}
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
			//hueVec.push_back(hueAvg);
			windowVec.push_back(colorWindow);
			//hueAvg.clear();
			//hueVals.clear();
			colorWindow.clear();
			pixelColorWindow.clear();
			col=0; ++row;
		}//end while row
		writeSeq2File(windowVec,name);
		deque<String>().swap(pixelColorWindow);
		vector<String>().swap(colorWindow);
		vector< vector<String> >().swap(windowVec);
		vector<int>().swap(index);
	}

	void hysteresis1x1(Mat img, String name) {
		rgb rgb;
		int r,g,b;
		String pix;
		vector<String> colorWindow;
		vector< vector<String> > windowVec;
		double dist=0;
		int ind=0;
		for(int row=0; row<img.rows; row++) {
			for(int col=0; col<img.cols; col++) {
				r = img.at<Vec3b>(row,col)[2];
				g = img.at<Vec3b>(row,col)[1];
				b = img.at<Vec3b>(row,col)[0];
				pix = rgb.checkBlack(r,g,b);
				if(pix=="OTHER") {
					pix = rgb.calcColor(r,g,b);
					if(pix=="OTHER") {
						pix = rgb.pushColor(r,g,b,dist,ind);
					}
				}
				colorWindow.push_back(pix);
			}
			windowVec.push_back(colorWindow);
			colorWindow.clear();
		}
		writeSeq2File(windowVec,name);
		vector<String>().swap(colorWindow);
		vector< vector<String> >().swap(windowVec);
	}
