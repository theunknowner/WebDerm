/*
 * test.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#include "test.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "rgb/rgb.h"
#include "hsl/hsl.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "Math/maths.h"
#include "FileData/filedata.h"
#include "Histogram/histogram.h"
#include "Shades/shades.h"
#include "Color/color.h"

vector<vector<String> > Test::test_shadesOfColor(Mat src) {
	vector<vector<String> > shadeMat(src.rows,vector<String>(src.cols,""));
	Shades sh;
	if(src.type()!=CV_8U)
		cvtColor(src,src,CV_BGR2GRAY);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double intensity = src.at<uchar>(i,j);
			String shade = sh.calcShade(intensity);
			shade = sh.combineShades(shade);
			shadeMat.at(i).at(j) = shade;
		}
	}
	return shadeMat;
}

Mat Test::test_hslAvgOfColor(Mat src) {
	Color c;
	Hsl hsl;
	Rgb rgb;
	Shades sh;
	String color, shade;
	int index=0, shadeIndex=0;
	vector<double> HSL;
	vector<vector<double> >hAvg(Rgb::allColors.size(),vector<double>(Shades::g_Shades2.size(),0));
	vector<vector<double> >sAvg(Rgb::allColors.size(),vector<double>(Shades::g_Shades2.size(),0));
	vector<vector<double> >lAvg(Rgb::allColors.size(),vector<double>(Shades::g_Shades2.size(),0));
	vector<vector<int> > colorCount(Rgb::allColors.size(),vector<int>(Shades::g_Shades2.size(),0));
	vector<vector<String> > colorNames(src.rows,vector<String>(src.cols,""));
	vector<vector<String> > shadeNames(src.rows,vector<String>(src.cols,""));
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			try {
				Vec3b RGB = src.at<Vec3b>(i,j);
				HSL = hsl.rgb2hsl(RGB[2],RGB[1],RGB[0]);
				color = hsl.getHslColor(HSL[0],HSL[1],HSL[2]);
				index = rgb.getColorIndex(color);
				double intensity = rgb.calcPerceivedBrightness(RGB[2],RGB[1],RGB[0]);
				shade = sh.calcShade(intensity);
				shade = sh.combineShades(shade);
				int shadeIndex = sh.getShadeIndex2(shade);
				HSL[0] = HSL[0] - floor(HSL[0]/180.0) * 360.;
				hAvg.at(index).at(shadeIndex) += HSL[0];
				sAvg.at(index).at(shadeIndex) += HSL[1];
				lAvg.at(index).at(shadeIndex) += HSL[2];
				colorCount.at(index).at(shadeIndex)++;
				colorNames.at(i).at(j) = color;
				shadeNames.at(i).at(j) = shade;
			} catch (const std::out_of_range &oor) {
				printf("hvgAvgOColor() out of range!\n");
				printf("Point: %d,%d\n",i,j);
				printf("Index: %d\n",index);
				printf("Color: %s\n",color.c_str());
				printf("HSL.size(): %lu\n",HSL.size());
				printf("hAvg.size(): %lu\n",hAvg.size());
				printf("sAvg.size(): %lu\n",sAvg.size());
				printf("lAvg.size(): %lu\n",lAvg.size());
				printf("colorCount.size(): %lu\n", colorCount.size());
				exit(1);
			}
		}
	}
	for(unsigned int i=0; i<hAvg.size(); i++) {
		for(unsigned int j=0; j<hAvg.at(i).size(); j++) {
			if(colorCount.at(i).at(j)>0) {
				hAvg.at(i).at(j) /= colorCount.at(i).at(j);
				sAvg.at(i).at(j) /= colorCount.at(i).at(j);
				lAvg.at(i).at(j) /= colorCount.at(i).at(j);
				//printf("%s%s: %d\n",Shades::g_Shades2.at(j).c_str(),Rgb::allColors.at(i).c_str(),colorCount.at(i).at(j));
			}
		}
	}
	Mat result(src.size(),CV_8UC3,Scalar(0,0,0));
	vector<String> colorSE;
	vector<String> shadeSE;
	unsigned int structElemSize = 3;
	int sides = floor(structElemSize/2.0);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			vector<vector<int> > shadeColorCount(Rgb::allColors.size(),vector<int>(Shades::g_Shades2.size(),0));
			for(int k=j-sides; k<=j+sides; k++) {
				if(k>=0 && k<src.cols) {
					color = colorNames.at(i).at(k);
					shade = shadeNames.at(i).at(k);
					colorSE.push_back(color);
					shadeSE.push_back(shade);
				}
			}
			for(unsigned int k=0; k<colorSE.size(); k++) {
				try {
					int indColor = rgb.getColorIndex(colorSE.at(k));
					int indShade = sh.getShadeIndex2(shadeSE.at(k));
					shadeColorCount.at(indColor).at(indShade)++;
					if(shadeColorCount.at(indColor).at(indShade)>=round(structElemSize/2.0)) {
						color =	Rgb::allColors.at(indColor);
						shade = sh.getShade2(indShade);
						break;
					}
				} catch (const std::out_of_range &oor) {
					printf("Out of range #1!\n");
					exit(1);
				}
			}
			try {
				index = rgb.getColorIndex(color);
				shadeIndex = sh.getShadeIndex2(shade);
				double h = round(hAvg.at(index).at(shadeIndex));
				double s = roundDecimal(sAvg.at(index).at(shadeIndex),2);
				double l = roundDecimal(lAvg.at(index).at(shadeIndex),2);
				if(h<0) h = 360.0 + h;
				vector<int> RGB = hsl.hsl2rgb(h,s,l);
				result.at<Vec3b>(i,j)[2] = RGB.at(0);
				result.at<Vec3b>(i,j)[1] = RGB.at(1);
				result.at<Vec3b>(i,j)[0] = RGB.at(2);
				colorNames.at(i).at(j) = color;
				shadeNames.at(i).at(j) = shade;
			} catch (const std::out_of_range &oor) {
				printf("Out of Range #2!\n");
				printf("Point: %d,%d\n",j,i);
				printf("Color_Index: %d\n",index);
				printf("Shade_Index: %d\n",shadeIndex);
				printf("Color: %s\n",color.c_str());
				exit(1);
			}
			colorSE.clear();
			shadeSE.clear();
		}
	}
	return result;
}
