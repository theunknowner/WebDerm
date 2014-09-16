/*
 * rgb.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "rgb.h"


deque< deque<double> > normMeanThresh;
deque< deque<double> > absMeanThresh;
deque<String> rgbColors;
deque<String> mainColors;
deque<String> allColors;

int rgb::getMainColorIndex(String color) {
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color==mainColors.at(i))
			return i;
	}
	return 0;
}

int rgb::getColorIndex(String color) {
	for(unsigned int i=0; i<allColors.size(); i++) {
		if(color==allColors.at(i))
			return i;
	}
	return -1;
}

bool rgb::importThresholds() {
	int size=1;
	bool flag[size];
	flag[0] = importColorThresholds();
	for(int i=0; i<size; i++)
		if(flag[i]!=true)
			return false;

	return true;
}
//imports RGB colorspace thresholds
bool rgb::importColorThresholds()
{
	String folderName = path+"Thresholds/";
	String filename = folderName+"color-thresholds.csv";
	String filename2 = folderName+"main_colors.csv";
	String filename3 = folderName+"colors.csv";
	fstream fsThresh(filename.c_str());
	fstream fsColors(filename2.c_str());
	fstream fsColors2(filename3.c_str());
	if(fsThresh.is_open() && fsColors.is_open())
	{
		String temp;
		deque<String> vec;
		deque<double> thresh;
		deque<double> thresh2;
		while(getline(fsColors,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==0) mainColors.push_back(vec.at(i));
			}
			vec.clear();
		}
		while(getline(fsColors2,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==0) allColors.push_back(vec.at(i));
			}
			vec.clear();
		}
		getline(fsThresh,temp);
		while(getline(fsThresh,temp))
		{
			getSubstr(temp,',',vec);
			for(unsigned int j=0; j<vec.size(); j++)
			{
				if(j==0)
				{
					rgbColors.push_back(vec.at(j));
				}
				if(j>=1 && j<=3)
				{
					thresh.push_back(atof(vec.at(j).c_str()));
				}
				if(j>=4 && j<=6)
				{
					thresh2.push_back(atof(vec.at(j).c_str()));
				}
			}
			absMeanThresh.push_back(thresh);
			normMeanThresh.push_back(thresh2);
			vec.clear(); thresh.clear(); thresh2.clear();
		}
		fsColors.close(); fsThresh.close();
		deque<String>().swap(vec);
		deque<double>().swap(thresh);
		deque<double>().swap(thresh2);
		String().swap(temp);
		return true;
	}
	else
	{
		cout << "Importing Colors and Thresholds Failed!" << endl;
		return false;
	}
}

//calculate the Euclidean Distance betweeen normalized rgb input and colors(vec).
double rgb::normEucDist(double red, double green, double blue, deque<double> &vec) {
		double result=0;
		double color[3] = {red,green,blue};
		double normVals[3];
		for(int i=0; i<3; i++) {
			normVals[i] = color[i]/(color[0]+color[1]+color[2]);
			normVals[i] -= vec.at(i);
			normVals[i] *= normVals[i];
			result += normVals[i];
		}
		result = sqrt(result);
		return result;
	}

//calculates the Euclidean Distance between absolute rgb input and colors(vec).
double rgb::absEucDist(double red, double green, double blue, deque<double> &vec)
	{
		double result=0;
		double color[3] = {red,green,blue};
		for(int i=0; i<3; i++)
		{
			color[i] -= vec.at(i);
			color[i] *= color[i];
			result += color[i];
		}
		result = sqrt(result);
		return result;
	}

double rgb::absEucDist(double red, double green, double blue, deque<int> &vec)
	{
		double result=0;
		double color[3] = {red,green,blue};
		for(int i=0; i<3; i++)
		{
			color[i] -= vec.at(i);
			color[i] *= color[i];
			result += color[i];
		}
		result = sqrt(result);
		return result;
	}

void rgb::outputRGBVals(FILE * fp, int red, int green, int blue, Point coord, double dist, String color, int ind)
{
	fprintf(fp,"%d,%d,%d - (%d,%d) - %f - %s - %d\n", red,green,blue,coord.x,coord.y,dist, color.c_str(), ind+2);
}

//check if dist is greater than thresh
bool rgb::checkAbsDist(double dist, double thresh)
{
	if(dist>thresh)
	{
		return true;
	}
	return false;
}

//checks if color is black
String rgb::checkBlack(int r, int g, int b)
	{
		if(r==0 && g==0 && b==0)
		{
			return "Zero";
		}
		return "OTHER";
	}

//pushes color using Euclidean Distance with 7 different schemes
String rgb::pushColor(int red, int green, int blue)
	{
		const int length=7;
		double smallest[length]={0};
		double val[length]={0};
		unsigned int index[length]={0};
		int colorIndex[rgbColors.size()];
		fill_n(colorIndex,rgbColors.size(),0);
		double normDistVals[normMeanThresh.size()];
		double absDistVals[absMeanThresh.size()];
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			normDistVals[i] = normEucDist(red,green,blue, normMeanThresh.at(i));
			absDistVals[i] = absEucDist(red,green,blue, absMeanThresh.at(i));
			//cout << rgbColors[i] << ": " << setw(10) << "\t" << normDistVals[i] << " | " << absDistVals[i] << " | " << absDistVals[i]*pow(normDistVals[i],e) << endl;
		}
		smallest[0] = normDistVals[0];
		smallest[1] = absDistVals[0];
		smallest[2] = absDistVals[0] * pow(normDistVals[0],2);
		smallest[3] = absDistVals[0] * normDistVals[0];
		smallest[4] = absDistVals[0] * pow(normDistVals[0],0.25);
		smallest[5] = absDistVals[0] * pow(normDistVals[0],0.45);
		smallest[6] = absDistVals[0] * pow(normDistVals[0],0.65);
		fill_n(index,length,0);
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			val[0] = normDistVals[i];
			val[1] = absDistVals[i];
			val[2] = absDistVals[i] * pow(normDistVals[i],2);
			val[3] = absDistVals[i] * normDistVals[i];
			val[4] = absDistVals[i] * pow(normDistVals[i],0.25);
			val[5] = absDistVals[i] * pow(normDistVals[i],0.45);
			val[6] = absDistVals[i] * pow(normDistVals[i],0.65);
			if(val[1]<=2) {
				return rgbColors[i];
			}
			for(int j=0; j<length; j++)
			{
				if(val[j]<smallest[j])
				{
					smallest[j] = val[j];
					index[j] = i;
				}
			}
		}
		for(int i=0; i<length; i++)
		{
			for(unsigned int j=0; j<rgbColors.size(); j++)
			{
				if(rgbColors.at(index[i])==rgbColors.at(j))
				{
					++colorIndex[j];
				}
			}
		}
		int greatest=0;
		for(unsigned int i=0; i<rgbColors.size(); i++)
		{
			if(colorIndex[i]>=greatest)
			{
				greatest = colorIndex[i];
				index[0] = i;
			}
		}
		return rgbColors[index[0]];
	}

String rgb::pushColor(int red, int green, int blue, double &dist, int &ind)
	{
		const int length=7;
		double smallest[length]={0};
		double val[length]={0};
		unsigned int index[length]={0};
		int index2=0;
		int colorIndex[rgbColors.size()];
		fill_n(colorIndex,rgbColors.size(),0);
		double normDistVals[normMeanThresh.size()];
		double absDistVals[absMeanThresh.size()];
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			normDistVals[i] = normEucDist(red,green,blue, normMeanThresh.at(i));
			absDistVals[i] = absEucDist(red,green,blue, absMeanThresh.at(i));
			//cout << rgbColors[i] << ": " << setw(10) << "\t" << normDistVals[i] << " | " << absDistVals[i] << " | " << absDistVals[i]*(normDistVals[i]) << endl;
		}
		smallest[0] = normDistVals[0];
		smallest[1] = absDistVals[0];
		smallest[2] = absDistVals[0] * pow(normDistVals[0],2);
		smallest[3] = absDistVals[0] * normDistVals[0];
		smallest[4] = absDistVals[0] * pow(normDistVals[0],0.25);
		smallest[5] = absDistVals[0] * pow(normDistVals[0],0.45);
		smallest[6] = absDistVals[0] * pow(normDistVals[0],0.65);
		fill_n(index,length,0);
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			val[0] = normDistVals[i];
			val[1] = absDistVals[i];
			val[2] = absDistVals[i] * pow(normDistVals[i],2);
			val[3] = absDistVals[i] * normDistVals[i];
			val[4] = absDistVals[i] * pow(normDistVals[i],0.25);
			val[5] = absDistVals[i] * pow(normDistVals[i],0.45);
			val[6] = absDistVals[i] * pow(normDistVals[i],0.65);

			if(val[1]<=2) {
				dist = val[1];
				ind = i;
				return rgbColors[i];
			}
			for(int j=0; j<length; j++)
			{
				if(val[j]<smallest[j])
				{
					smallest[j] = val[j];
					index[j] = i;
				}
			}
		}
		for(int i=0; i<length; i++)
		{
			for(unsigned int j=0; j<rgbColors.size(); j++)
			{
				if(index[i]==j)
				{
					++colorIndex[j];
				}
			}
		}
		int greatest=0;
		for(unsigned int i=0; i<rgbColors.size(); i++)
		{
			if(colorIndex[i]>greatest)
			{
				greatest = colorIndex[i];
				index2 = i;
			}
		}
		dist = absDistVals[index2];
		ind = index2;
		return rgbColors[index2];
	}

String rgb::pushColor(int red, int green, int blue, int &ind, double &dist)
	{
		const int length=7;
		double smallest[length]={0};
		double val[length]={0};
		unsigned int index[length]={0};
		int index2=0;
		int colorIndex[rgbColors.size()];
		fill_n(colorIndex,rgbColors.size(),0);
		double normDistVals[normMeanThresh.size()];
		double absDistVals[absMeanThresh.size()];
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			normDistVals[i] = normEucDist(red,green,blue, normMeanThresh.at(i));
			absDistVals[i] = absEucDist(red,green,blue, absMeanThresh.at(i));
			//cout << rgbColors[i] << ": " << setw(10) << "\t" << normDistVals[i] << " | " << absDistVals[i] << " | " << absDistVals[i]*(normDistVals[i]) << endl;
		}
		smallest[0] = normDistVals[0];
		smallest[1] = absDistVals[0];
		smallest[2] = absDistVals[0] * pow(normDistVals[0],2);
		smallest[3] = absDistVals[0] * normDistVals[0];
		smallest[4] = absDistVals[0] * pow(normDistVals[0],0.25);
		smallest[5] = absDistVals[0] * pow(normDistVals[0],0.45);
		smallest[6] = absDistVals[0] * pow(normDistVals[0],0.65);
		fill_n(index,length,0);
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			val[0] = normDistVals[i];
			val[1] = absDistVals[i];
			val[2] = absDistVals[i] * pow(normDistVals[i],2);
			val[3] = absDistVals[i] * normDistVals[i];
			val[4] = absDistVals[i] * pow(normDistVals[i],0.25);
			val[5] = absDistVals[i] * pow(normDistVals[i],0.45);
			val[6] = absDistVals[i] * pow(normDistVals[i],0.65);
			if(val[1]<=2) {
				ind = i;
				cout << absDistVals[ind] << endl;
				return rgbColors[i];
			}
			for(int j=0; j<length; j++)
			{
				if(val[j]<smallest[j])
				{
					smallest[j] = val[j];
					index[j] = i;
				}
			}
		}
		for(int i=0; i<length; i++)
		{
			for(unsigned int j=0; j<rgbColors.size(); j++)
			{
				if(index[i]==j)
				{
					++colorIndex[j];
				}
			}
		}
		int greatest=0;
		for(unsigned int i=0; i<rgbColors.size(); i++)
		{
			if(colorIndex[i]>greatest)
			{
				greatest = colorIndex[i];
				index2 = i;
			}
		}
		ind = index2;
		dist = absDistVals[ind];
		cout << absDistVals[ind] << endl;
		return rgbColors[index2];
	}

double rgb::calcGrayLevel(int red, int green, int blue)
{
	hsl hsl;
	double sat=0;
	hsl.rgb2hsl(red,green,blue);
	sat = hsl.getSat();
	sat = roundDecimal(sat,2) * 100;
	sat = 100 - sat;
	return sat;
}

double rgb::getGrayLevel2(String color)
{
	double level=0;
	size_t pos=0;
	String str, mainColor="Gray";
	pos = color.find(mainColor);
	if(pos!=string::npos) {
		str = color.substr(pos+mainColor.size(),color.size()-(pos+mainColor.size()));
		level = atof(str.c_str());
	}
	if(color.find("Grey")!=string::npos || color.find("Black")!=string::npos)
		level = 100;

	return level;
}

double rgb::getGrayLevel1(String color) {
	double level=0;
	size_t pos=0;
	String str, mainColor="Gray";
	pos = color.find(mainColor);
	if(pos!=string::npos) {
		str = color.substr(0,pos);
		level = atof(str.c_str());
	}
	else {
		pos = color.find("Grey");
		if(pos!=string::npos) {
			str = color.substr(0,pos);
			level = atof(str.c_str());
		}
		else {
			pos = color.find("Black");
			if(pos!=string::npos) {
				str = color.substr(0,pos);
				level = atof(str.c_str());
			}
		}
	}
	return level;
}


double rgb::getColorLevel(String color, String mainColor)
{
	double level=0;
	size_t pos=0;
	String str;
	pos = color.find(mainColor);
	if(pos!=string::npos)
	{
		str = color.substr(pos+mainColor.size(),color.size()-(pos+mainColor.size()));
	}
	level = atof(str.c_str());
	return level;
}

double rgb::getColorLevel(String pix) {
	double level=0;
	size_t pos=0;
	String str;
	try {
		for(unsigned int i=0; i<mainColors.size(); i++) {
			if(mainColors.at(i)!="Gray") {
				pos = pix.find(mainColors.at(i));
				if(pos!=string::npos)
					str = pix.substr(pos+mainColors.at(i).size(),pix.size()-(pos+mainColors.at(i).size()));
			}
		}
		level = atof(str.c_str());
	}
	catch(const std::out_of_range &oor) {
		printf("rgb::getColorLevel() out of range!\n");
		exit(0);
	}
	return level;
}

double rgb::calcPerceivedBrightness(double red, double green, double blue) {
	double lum;
	lum = (0.299*red) + (0.587*green) + (0.114*blue);
	return round(lum);
}
//outputs image window with color of rgb value
void rgb::showPixelColor(int r, int g, int b, Size size)
	{
		Mat img = img.zeros(Size(1,1),16);
		img.at<Vec3b>(0,0)[0] = b;
		img.at<Vec3b>(0,0)[1] = g;
		img.at<Vec3b>(0,0)[2] = r;
		cout << r << "," << g << "," << b << endl;
		resize(img,img,size,0,0,INTER_CUBIC);
		imshow("Pixel", img);
		waitKey(0);
	}

//outputs image window of color at Location x,y
void rgb::showPixelColorAtLoc(Mat img, int row, int col, Size size)
	{
		Mat img2;
		int r,g,b;
		b = img.at<Vec3b>(row-1,col-1)[0];
		g = img.at<Vec3b>(row-1,col-1)[1];
		r = img.at<Vec3b>(row-1,col-1)[2];
		showPixelColor(r,g,b,size);
	}

void rgb::release_memory() {
	normMeanThresh.clear();
	absMeanThresh.clear();
	rgbColors.clear();
	mainColors.clear();
	deque< deque<double> >().swap(normMeanThresh);
	deque< deque<double> >().swap(absMeanThresh);
	deque<String>().swap(rgbColors);
	deque<String>().swap(mainColors);
}

double rgb::calcColorLevel(double red, double green, double blue) {
	double lum = calcPerceivedBrightness(red, green, blue);
	lum /=255;
	lum = roundDecimal(lum,2) * 100;
	lum = 100 - lum;
	return lum;
}

String rgb::calcColor2(int red, int green, int blue) {
	hsl hsl;
	String pix = "OTHER";
	int hue;
	double lum,sat;
	hsl.rgb2hsl(red,green,blue);
	hue = hsl.getHue();
	lum = roundDecimal(hsl.getLum(),2);
	sat = roundDecimal(hsl.getSat(),2);
	double grayLevel = calcGrayLevel(red,green,blue);
	double colorLevel = calcColorLevel(red,green,blue);
	double grayLumLevel = calcGrayLumLevel(red,green,blue);
	for(unsigned int i=0; i<hueThresh.size(); i++) {
		try {
			if(hue>=hueThresh.at(i).at(0) && hue<=hueThresh.at(i).at(1)) {
				if(sat>=satThresh.at(i).at(0) && sat<satThresh.at(i).at(1)) {
					if(lum>=lumThresh.at(i).at(0) && lum<lumThresh.at(i).at(1)) {
						pix = hslColors.at(i);
						if(grayLevel==0) {
							pix = hslColors.at(i) + toString(colorLevel);
						}
						else {
							if(pix=="Black" || pix=="White")
								pix += toString(colorLevel);
							else if(pix=="Grey")
								pix += toString(colorLevel);
							else
								pix = "Gray" + toString(grayLumLevel) + hslColors.at(i) + toString(colorLevel);
						}
						return pix;
					}
				}
			}
		} catch (const std::out_of_range &oor) {
			printf("rgb::calcColor2() out of range!\n");
			printf("hueThresh.Size: %lu", hueThresh.size());
			exit(1);
		}
	}
	return pix;
}

String rgb::calcColor(int red, int green, int blue, double &dist, int &ind) {
	Color c;
	String pix;
	pix = calcColor2(red,green,blue);
	double grayLevel = calcGrayLevel(red,green,blue);
	if(pix=="OTHER")
		pix = pushColor(red,green,blue,dist,ind);
	pix = c.reassignLevels(pix,red,green,blue);
	pix = toString(grayLevel) + pix;
	return pix;
}

String rgb::calcColor(int red, int green, int blue) {
	Color c;
    String pix;
	double dist=0;
	int ind=0;
	pix = calcColor2(red,green,blue);
	double grayLevel = calcGrayLevel(red,green,blue);
    if(pix=="OTHER")
        pix = pushColor(red,green,blue,dist,ind);
	pix = c.reassignLevels(pix,red,green,blue);
	pix = toString(grayLevel) + pix;
	return pix;
}

double rgb::calcGrayLumLevel(double red, double green, double blue) {
	hsl hsl;
	double grayLumLevel=0;
	double *HSL = hsl.rgb2hsl(red,green,blue);
	double sat = roundDecimal(HSL[1],2) * 100;
	double lum = roundDecimal(HSL[2],2) * 100;
	//grayLevel = grayLUT.at(sat).at(lum);
	//grayLevel = ((100-sat)*(100-lum))/100;
	double a=4;
	double temp = a*(1.0-(sat/100));
	grayLumLevel = (100-lum) * temp/(temp+1);

	return round(grayLumLevel);
}

double rgb::calcColorLumLevel(double red, double green, double blue) {
	hsl hsl;
	double colorLumLevel=0;
	double* HSL = hsl.rgb2hsl(red,green,blue);
	double sat = roundDecimal(HSL[1],2) * 100;
	double lum = roundDecimal(HSL[2],2) * 100;
	double a=4;
	double temp = a * (1.0-(sat/100));
	colorLumLevel = (100-lum) * 1/(temp+1);
	return round(colorLumLevel);
}


