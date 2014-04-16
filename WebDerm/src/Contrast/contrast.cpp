/*
 * contrast.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#include "contrast.h"

double contrast::calcColorfulness(double contrastHue, double colorLevel) {
	double colorfn=0;
	colorfn = colorLevel * contrastHue;
	return colorfn;
}

double contrast::calcContrast(double hue1, double hue2, String color1, String color2) {
	rgb rgb;
	double grayHue = -1.1;
	double colorLevel1=0, colorLevel2=0;
	double cHue1, cHue2;
	double colorfn1=0, colorfn2=0;
	int count1=0, count2=0;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		cHue1 = hue1;
		cHue2 = hue2;
		if(color1.find(mainColors.at(i))!=string::npos) {
			colorLevel1 = rgb.getColorLevel(color1,mainColors.at(i));
			if(color1.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue1 = grayHue;
			}
			if(count1<2)
				colorfn1 += calcColorfulness(cHue1, colorLevel1);

			++count1;
		}
		if(color2.find(mainColors.at(i))!=string::npos) {
			colorLevel2 = rgb.getColorLevel(color2,mainColors.at(i));
			if(color2.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue2 = grayHue;
			}
			if(count2<2)
				colorfn2 += calcColorfulness(cHue2, colorLevel2);

			++count2;
		}
	}
	//printf("Colorfn1: %f\n",colorfn1);
	//printf("Colorfn2: %f\n",colorfn2);
	return colorfn2-colorfn1;
}

void contrast::calcContrastFromMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec,String name) {
	Color c;
	int count1,count2;
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
			count1 = c.countColors(color1);
			count2 = c.countColors(color2);
			if((count1+count2)==5) {
				if(count1==3)
					color1=windowVec.at(i).at(j-1);
				else
					color2=windowVec.at(i).at(j+2);
			}
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

double contrast::calcColorfulness2(double hue, String color) {
	rgb rgb;
	double grayHue = -1.1;
	double colorLevel1=0;
	double cHue1;
	double colorfn1=0;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		cHue1 = hue;
		if(color.find(mainColors.at(i))!=string::npos) {
			colorLevel1 = rgb.getColorLevel(color,mainColors.at(i));
			if(color.find("Gray")!=string::npos && mainColors.at(i)=="Gray")  {
				cHue1 = grayHue;
			}
			colorfn1 += calcColorfulness(cHue1, colorLevel1);
		}
	}
	return colorfn1;
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

void contrast::calcColorfulnessMatrix(vector< vector<String> > &windowVec, vector< vector<double> > &hueVec,String name) {
	Color c;
	String file;
	int count1;
	double colorfn=0;
	String color1;
	double hue1;
	vector<double> vec;
	vector< vector<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-1); j++) {
			color1 = windowVec.at(i).at(j);
			hue1 = hueVec.at(i).at(j);
			count1 = c.countColors(color1);
			if(count1==3) {
				color1=windowVec.at(i).at(j-1);
				windowVec.at(i).at(j) = color1;
			}

			colorfn = calcColorfulness2(hue1,color1);
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
