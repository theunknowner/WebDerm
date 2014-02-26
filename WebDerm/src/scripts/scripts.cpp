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
			pix = "White";
		}
		else if(rgbColors.at(i)=="Black")
		{
			pix = "Black";
		}
		else if(rgbColors.at(i)!="Gray" && grayLevel!=0)
		{
			pix = "Gray" + toString(grayLevel) + rgbColors.at(i) + toString(colorLevel);
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
