/*
 * Rgb.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "rgb.h"

deque<String> mainColors;
deque<String> allColors;

int Rgb::getMainColorIndex(String color) {
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color==mainColors.at(i))
			return i;
	}
	return 0;
}

int Rgb::getColorIndex(String color) {
	for(unsigned int i=0; i<allColors.size(); i++) {
		try {
			if(color==allColors.at(i))
				return i;
		} catch (const std::out_of_range &oor) {
			printf("Rgb::getColorIndex() out of range!\n");
			printf("Index: %d\n",i);
			printf("allColors.size(): %lu\n",allColors.size());
			exit(1);
		}
	}
	return -1;
}

bool Rgb::importThresholds() {
	int size=1;
	bool flag[size];
	flag[0] = importColorThresholds();
	for(int i=0; i<size; i++)
		if(flag[i]!=true)
			return false;

	return true;
}
//imports RGB colorspace thresholds
bool Rgb::importColorThresholds()
{
	String folderName = "Thresholds/";
	String filename2 = folderName+"main_colors.csv";
	String filename3 = folderName+"colors.csv";
	fstream fsColors(filename2.c_str());
	fstream fsColors2(filename3.c_str());
	if(fsColors.is_open() && fsColors2.is_open()) {
		String temp;
		deque<String> vec;
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
		fsColors.close(); fsColors2.close();
		deque<String>().swap(vec);
		String().swap(temp);
		return true;
	}
	else
	{
		cout << "Importing Colors and Thresholds Failed!" << endl;
		return false;
	}
}

//calculate the Euclidean Distance betweeen normalized Rgb input and colors(vec).
double Rgb::normEucDist(double red, double green, double blue, deque<double> &vec) {
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

//calculates the Euclidean Distance between absolute Rgb input and colors(vec).
double Rgb::absEucDist(double red, double green, double blue, deque<double> &vec)
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

double Rgb::absEucDist(double red, double green, double blue, deque<int> &vec)
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

void Rgb::outputRGBVals(FILE * fp, int red, int green, int blue, Point coord, double dist, String color, int ind)
{
	fprintf(fp,"%d,%d,%d - (%d,%d) - %f - %s - %d\n", red,green,blue,coord.x,coord.y,dist, color.c_str(), ind+2);
}

//check if dist is greater than thresh
bool Rgb::checkAbsDist(double dist, double thresh)
{
	if(dist>thresh)
	{
		return true;
	}
	return false;
}

//checks if color is black
String Rgb::checkBlack(int r, int g, int b)
{
	if(r==0 && g==0 && b==0)
	{
		return "Zero";
	}
	return "OTHER";
}

double Rgb::calcGrayLevel(int red, int green, int blue)
{
	Hsl hsl;
	double sat=0;
	hsl.rgb2hsl(red,green,blue);
	sat = hsl.getSat();
	sat = roundDecimal(sat,2) * 100;
	sat = 100 - sat;
	return sat;
}

//get level after gray
double Rgb::getGrayLevel2(String color)
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

//get level before gray
double Rgb::getGrayLevel1(String color) {
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


double Rgb::getColorLevel(String color, String mainColor)
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

double Rgb::getColorLevel(String pix) {
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

double Rgb::calcPerceivedBrightness(double red, double green, double blue) {
	double lum;
	lum = (0.299*red) + (0.587*green) + (0.114*blue);
	return round(lum);
}
//outputs image window with color of Rgb value
void Rgb::showPixelColor(int r, int g, int b, Size size)
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
void Rgb::showPixelColorAtLoc(Mat img, int row, int col, Size size)
{
	Mat img2;
	int r,g,b;
	b = img.at<Vec3b>(row-1,col-1)[0];
	g = img.at<Vec3b>(row-1,col-1)[1];
	r = img.at<Vec3b>(row-1,col-1)[2];
	showPixelColor(r,g,b,size);
}

void Rgb::release_memory() {
	mainColors.clear();
	allColors.clear();
	mainColors.shrink_to_fit();
	allColors.shrink_to_fit();
}

double Rgb::calcColorLevel(double red, double green, double blue) {
	double lum = calcPerceivedBrightness(red, green, blue);
	lum /=255;
	lum = roundDecimal(lum,2) * 100;
	lum = 100 - lum;
	return lum;
}

String Rgb::calcColor2(int red, int green, int blue) {
	Hsl hsl;
	Color c;
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
						if(c.countColors(hslColors.at(i))>=2)
							pix = c.reassignLevels(pix,red,green,blue);
						return toString(grayLevel) + pix;
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

String Rgb::calcColor(int red, int green, int blue, double &dist, int &ind) {
	String pix;
	pix = calcColor2(red,green,blue);
	return pix;
}

String Rgb::calcColor(int red, int green, int blue) {
	String pix;
	pix = calcColor2(red,green,blue);
	return pix;
}

double Rgb::calcGrayLumLevel(double red, double green, double blue) {
	Hsl hsl;
	double grayLumLevel=0;
	vector<double> HSL = hsl.rgb2hsl(red,green,blue);
	double sat = roundDecimal(HSL[1],2) * 100;
	double lum = roundDecimal(HSL[2],2) * 100;
	//grayLevel = grayLUT.at(sat).at(lum);
	//grayLevel = ((100-sat)*(100-lum))/100;
	double a=4;
	double temp = a*(1.0-(sat/100));
	grayLumLevel = (100-lum) * temp/(temp+1);

	return round(grayLumLevel);
}

double Rgb::calcColorLumLevel(double red, double green, double blue) {
	Hsl hsl;
	double colorLumLevel=0;
	vector<double> HSL = hsl.rgb2hsl(red,green,blue);
	double sat = roundDecimal(HSL[1],2) * 100;
	double lum = roundDecimal(HSL[2],2) * 100;
	double a=4;
	double temp = a * (1.0-(sat/100));
	colorLumLevel = (100-lum) * 1/(temp+1);
	return round(colorLumLevel);
}


