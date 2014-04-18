/*
 * contrast.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#include "contrast.h"

#define PI 3.14159265
#define RAD 180.0/PI

//calculating colorfulness using colorlevel and hue value
double contrast::calcColorfulness(double Hue, double colorLevel) {
	double colorfn=0;
	colorfn = colorLevel * Hue;
	return colorfn;
}

//uses calcColorfulness to calculate colorfulness
double contrast::calcColorfulness2(double hue, String color) {
	rgb rgb;
	double grayHue = 0.5;
	double colorLevel[mainColors.size()];
	double cHue[mainColors.size()];
	double colorfn[mainColors.size()];
	fill_n(colorLevel,mainColors.size(),0);
	fill_n(cHue,mainColors.size(),0);
	fill_n(colorfn,mainColors.size(),0);
	double totalLevel=0;
	double totalColorfn=0;
	vector<int> index;
	vector<double> vec,vec2;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		cHue[i] = hue;
		if(color.find(mainColors.at(i))!=string::npos) {
			colorLevel[i] = rgb.getColorLevel(color,mainColors.at(i));
			index.push_back(i);
			if(mainColors.at(i)!="Gray") {
				totalLevel+=colorLevel[i];
			}
			if(color.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue[i] = grayHue;
			}
			if(color.find("Pink")!=string::npos && mainColors.at(i)=="Pink")  {
				cHue[i] = 176/360.;
			}
			if(color.find("Brown")!=string::npos && mainColors.at(i)=="Brown")  {
				cHue[i] = 207/360.;
			}
			if(color.find("Violet")!=string::npos && mainColors.at(i)=="Violet")  {
				cHue[i] = 140/360.;
			}
		}
	}
	for(unsigned int i=0; i<index.size(); i++) {
		colorfn[index.at(i)] = colorLevel[index.at(i)] * cHue[index.at(i)];
		if(mainColors.at(index.at(i))!="Gray") {
			colorfn[index.at(i)] *= (colorLevel[index.at(i)]/totalLevel);
		}
		totalColorfn += colorfn[index.at(i)];
	}
	return totalColorfn;
}

//calculates contrast between two colors
double contrast::calcContrast(double hue1, double hue2, String color1, String color2) {
	double colorfn1=0, colorfn2=0;
	double contrast=0;
	colorfn1 = calcColorfulness2(hue1,color1);
	colorfn2 = calcColorfulness2(hue2,color2);
	//printf("Colorfn1: %f\n",colorfn1);
	//printf("Colorfn2: %f\n",colorfn2);
	contrast = colorfn2-colorfn1;
	contrast = roundDecimal(contrast,1);
	if(contrast<2.5 && contrast>-2.5) contrast=0;

	return contrast;
}
//calculates contrast angle between two colors
double contrast::getContrastAngle(double hue1, double hue2, String color1, String color2) {
	double colorfn1=0, colorfn2=0;
	double angleContrast = 0;
	colorfn1 = calcColorfulness2(hue1,color1);
	colorfn2 = calcColorfulness2(hue2,color2);
	angleContrast = calcContrastAngle(hue1, hue2, colorfn1, colorfn2);
	return angleContrast;
}
//change between calcContrast and calcContrast2 for diff options
void contrast::calcContrastFromMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec,String name) {
	double contrast=0;
	String color1,color2;
	double hue1,hue2;
	vector<double> vec;
	vector< vector<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-2); j++) {
			color1 = windowVec.at(i).at(j);
			color2 = windowVec.at(i).at(j+1);
			hue1 = hueVec.at(i).at(j);
			hue2 = hueVec.at(i).at(j+1);
			contrast = calcContrast(hue1, hue2, color1, color2);
			vec.push_back(contrast);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	writeSeq2File(vec2,name);
	vector<double>().swap(vec);
	vector< vector<double> >().swap(vec2);
}

void contrast::colorfulnessMatrix1x1(Mat &img, String name) {
	rgb rgb;
	hsl hsl;
	String filename = name + "Colorfn";
	contrast con;
	double colorfn;
	double hue;
	vector<double> clrfn;
	vector< vector<double> > clrfnVec;
	int ind=0;
	double dist=0;
	String pix;
	int r,g,b;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			hsl.rgb2hsl(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.calcColor2(r,g,b);
				if(pix=="OTHER") {
					pix = rgb.pushColor(r,g,b,dist,ind);
				}
			}
			hue = (hsl.getHue()+180)%360;
			hue /=360;
			colorfn = con.calcColorfulness2(hue,pix);
			clrfn.push_back(colorfn);
		}
		clrfnVec.push_back(clrfn);
		clrfn.clear();
	}
	writeSeq2File(clrfnVec,filename);
}
//used for calculating 2x2 colorfulness
void contrast::calcColorfulnessMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec,String name) {
	String file;
	double colorfn=0;
	String color;
	double hue;
	vector<double> vec;
	vector< vector<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-1); j++) {
			color = windowVec.at(i).at(j);
			hue = hueVec.at(i).at(j);
			colorfn = calcColorfulness2(hue,color);
			vec.push_back(colorfn);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	file = name + "Colorfn2x2";
	writeSeq2File(vec2,file);
	vector<double>().swap(vec);
	vector< vector<double> >().swap(vec2);
}

void contrast::colorfulMatrix(Mat img, Size size, String name)
	{
		rgb rgb;
		hsl hsl;
		contrast con;
		Color colorObj;
		double matchingScans = (size.width*size.height)/2;
		deque<String> pixelColorWindow;
		vector<String> colorWindow;
		vector< vector<String> > windowVec;
		vector<int> index;
		deque<int> hueVals;
		vector<double> hueAvg;
		vector< vector<double> > hueVec;
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
							hsl.rgb2hsl(r,g,b);
							hueVals.push_back(hsl.getHue());
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
						hsl.rgb2hsl(r,g,b);
						hueVals.pop_front();
						hueVals.push_back(hsl.getHue());
						if(pix=="OTHER")
						{
							pix = rgb.calcColor2(r,g,b);
							if(pix=="OTHER") {
								pix = rgb.pushColor(r,g,b,dist,ind);
							}
						}
						pixelColorWindow.pop_front();
						pixelColorWindow.push_back(pix);
					}
				}
				hueAvg.push_back(hsl.calcHueAvg(hueVals));
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
			hueVec.push_back(hueAvg);
			windowVec.push_back(colorWindow);
			hueAvg.clear();
			hueVals.clear();
			colorWindow.clear();
			pixelColorWindow.clear();
			col=0; ++row;
		}//end while row
		//writeSeq2File(windowVec,name);
		con.calcColorfulnessMatrix(windowVec,hueVec,name);
		deque<String>().swap(pixelColorWindow);
		vector<String>().swap(colorWindow);
		vector< vector<String> >().swap(windowVec);
		deque<int>().swap(hueVals);
		vector<double>().swap(hueAvg);
		vector< vector<double> >().swap(hueVec);
		vector<int>().swap(index);
	}

double contrast::calcContrastAngle(double hue1, double hue2, double colorfn1, double colorfn2) {
	const double c = 65.0;
	double fColor1 = c * hue1;
	double fColor2 = c * hue2;
	double angle=0;
	double top=0, bottom=0;
	double root = 1/2.5;
	top = abs((fColor1*colorfn2) - (colorfn1*fColor2));
	bottom = sqrt(pow(fColor1,2)+pow(colorfn1,2)) * sqrt(pow(fColor2,2)+pow(colorfn2,2));
	angle = top/bottom;
	angle = pow(angle,root);
	return angle;
}
