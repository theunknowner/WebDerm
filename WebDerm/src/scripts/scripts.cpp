/*
 * scripts.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: jason
 */

#include "scripts.h"

void colorThreshNamingScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorThresh2.csv","w");
	hsl hsl;
	rgb rgb;
	double r,g,b;
	String pix;
	int grayLevel=0;
	int colorLevel=0;
	fprintf(fp,"Color,Rmin,Rmax,Gmin,Gmax,Bmin,Bmax,rMin,rMax,gMin,gMax,bMin,bMax\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		grayLevel=rgb.calcGrayLevel(r,g,b);
		colorLevel = rgb.calcColorLevel(r,g,b);
		if(rgbColors.at(i)=="White" || colorLevel==0)
		{
			pix = rgbColors.at(i) + toString(colorLevel);
		}
		else if(rgbColors.at(i)=="Black")
		{
			pix = rgbColors.at(i) + toString(colorLevel);
		}
		else if(rgbColors.at(i)!="Gray" && grayLevel!=0)
		{
			pix = "Gray" + toString(grayLevel) + rgbColors.at(i)+ toString(colorLevel);
		}
		else if(rgbColors.at(i)!="Gray" && grayLevel==0)
		{
			pix = rgbColors.at(i) + toString(colorLevel);
		}
		else
		{
			pix = rgbColors.at(i) + toString(colorLevel);
		}
			fprintf(fp,"%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
					pix.c_str(),absThresh[i][0],absThresh[i][1],absThresh[i][2],absThresh[i][3],
					absThresh[i][4],absThresh[i][5],normThresh[i][0],normThresh[i][1],
					normThresh[i][2],normThresh[i][3],normThresh[i][4],normThresh[i][5]);

	}
}

void colorThreshScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorThresh2.csv","w");
	hsl hsl;
	rgb rgb;
	double r,g,b;
	String pix;
	int grayLevel=0;
	int colorLevel=0;
	double s,l;
	fprintf(fp,"Color,S,Slvl,L,Lvl\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		s = hsl.getSat();
		l = hsl.getLum();
		s = roundDecimal(s,2);
		l = roundDecimal(l,2);
		grayLevel = rgb.calcGrayLevel(r,g,b);
		colorLevel = rgb.calcColorLevel(r,g,b);
		fprintf(fp,"%s,%f,%d,%f,%d\n", rgbColors.at(i).c_str(), s,grayLevel,l,colorLevel);
	}
}

void outputMeanScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/mean.csv", "w");
	double r,g,b;
	fprintf(fp,"Color,R,G,B\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		fprintf(fp,"%s,%f,%f,%f\n",rgbColors.at(i).c_str(),r,g,b);
	}
}

void outputFarRGB(Mat &img)
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/rgb.csv","w");
	rgb rgb;
	int r,g,b;
	int ind=0;
	String pix;
	double min=0;
	fprintf(fp,"R,G,B,Col,Row,Min\n");
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER")
			{
				min = rgb.pushColor2(r,g,b,ind);
				if(min>10)
				{
					fprintf(fp,"%d,%d,%d,%d,%d,%f,%s,%d\n",r,g,b,col,row,min,
							rgbColors.at(ind).c_str(),ind+2);
				}
			}
		}
	}
}

String outputCorrelationRGB(int r, int g, int b)
{
	double vec1[3] = {(double)r,(double)g,(double)b};
	double dist=0;
	int ind=0;
	double min=10;
	String pix;
	for(unsigned int i=0; i<absMeanThresh.size(); i++)
	{
		double vec2[3] = {absMeanThresh.at(i).at(0),absMeanThresh.at(i).at(1),
				absMeanThresh.at(i).at(2)};
		dist = correlationDist(vec1,vec2);
		if(dist!=-1)
		{
			if(dist<min)
			{
				min = dist;
				ind = i;
			}
			pix = rgbColors.at(ind);
		}
	}
	cout << min <<endl;
	cout << ind +2 << endl;
	return pix;
}

void outputFreqColor(Mat &img)
{
	rgb rgb;
	int arr[rgbColors.size()];
	fill_n(arr, rgbColors.size(), 0);
	int ind=0;
	int r,g,b;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			rgb.pushColor(r,g,b,ind);
			++arr[ind];
		}
	}
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorFreq.txt","w");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		if(arr[i]!=0)
		{
			fprintf(fp,"%d: %s(%d)\n", i+2, rgbColors.at(i).c_str(), arr[i]);
		}
	}
}
