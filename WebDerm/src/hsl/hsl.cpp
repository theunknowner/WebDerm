/*
 * Hsl.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#include "hsl.h"

double H, S, L;
deque<String> hslColors;
deque< deque<int> > hueThresh;
deque< deque<double> > satThresh;
deque< deque<double> > lumThresh;
deque<int> hueTableNum;

bool Hsl::isThreshImported() {
	return THRESH_IMPORTED;
}

void Hsl::setThreshImported(bool flag) {
	THRESH_IMPORTED = flag;
}

//import main HSL thresholds
bool Hsl::importHslThresholds() {
	String foldername = "Thresholds/";
	String filename = foldername+"hslcolor-thresholds2.csv";
	fstream fsThresh(filename.c_str());
	if(!fsThresh.is_open()) {
		cerr << "Importing HslColors and Thresholds Failed!" << endl;
		return false;
	}
	String temp;
	deque<String> vec;
	deque<int> thresh;
	deque<double> thresh2;
	deque<double> thresh3;
	getline(fsThresh,temp);
	while(getline(fsThresh,temp)) {
		getSubstr(temp,',',vec);
		for(unsigned int i=0; i<vec.size(); i++) {
			if(i==0) hslColors.push_back(vec.at(i));
			if(i>=1 && i<=2) thresh.push_back(atof(vec.at(i).c_str()));
			if(i>=3 && i<=4) thresh2.push_back(atof(vec.at(i).c_str()));
			if(i>=5 && i<=6) thresh3.push_back(atof(vec.at(i).c_str()));
		}
		hueThresh.push_back(thresh);
		satThresh.push_back(thresh2);
		lumThresh.push_back(thresh3);
		vec.clear(); thresh.clear(); thresh2.clear(); thresh3.clear();
	}
	fsThresh.close();
	deque<String>().swap(vec);
	deque<int>().swap(thresh);
	deque<double>().swap(thresh2);
	deque<double>().swap(thresh3);
	String().swap(temp);
	setThreshImported(true);
	return true;
}

//gets the minimum between the rgb value
double Hsl::minRGB(double red, double green, double blue)
	{
		if(red<=green && red<=blue)
		{
			return red;
		}
		if(green<=blue && green<=red)
		{
			return green;
		}
		return blue;
	}

//get the max between tbe rgb value
	double Hsl::maxRGB(double red, double green, double blue)
	{
		if(red>=green && red>=blue)
		{
			return red;
		}
		if(green>=blue && green>=red)
		{
			return green;
		}
		return blue;
	}

//converts rgb values to Hsl values
	double *Hsl::rgb2hsl(double red, double green, double blue)
	{
		double r,g,b;
		double min, max;
		double delta;
		static double HSL[3];
		r = red/255;
		g = green/255;
		b = blue/255;
		min = minRGB(r,g,b);
		max = maxRGB(r,g,b);
		L = (max+min)/2;
		delta = max-min;
		if(delta==0)
		{
			H=0; S=0;
		}
		else
		{
			if(L>0.5)
			{
				S = (max-min)/(2.0-max-min);
			}
			else
			{
				S = (max-min)/(max+min);
			}
			if(max==r)
			{
				H = ((g-b)/delta);
			}
			else if(max==g)
			{
				H = ((b-r)/delta) + 2;
			}
			else
			{
				H = ((r-g)/delta) + 4;
			}
			H *= 60;
			if(H<0) H+=360;
		}
		HSL[0] = round(H); HSL[1] = S; HSL[2] = L;
		return HSL;
	}

	double Hsl::getHue()
	{
		H = round(H);
		return H;
	}

	double Hsl::getSat()
	{
		return S;
	}

	double Hsl::getLum()
	{
		return L;
	}

	double Hsl::calcLum(double red, double green, double blue)
	{
		double r,g,b;
		double min, max;
		double lum;
		r = red/255;
		g = green/255;
		b = blue/255;
		min = minRGB(r,g,b);
		max = maxRGB(r,g,b);
		lum = (max+min)/2;
		return lum;
	}

//return the color of the Hsl values using Hsl colorspace
String Hsl::getHslColor(double hue, double sat, double lum)
{
	String color = "NONE";
	try {
		for(unsigned int i=0; i<hslColors.size(); i++) {
			if(hue>=hueThresh.at(i).at(0) && hue<=hueThresh.at(i).at(1)) {
				if(sat>=satThresh.at(i).at(0) && sat<satThresh.at(i).at(1)) {
					if(lum>=lumThresh.at(i).at(0) && lum<lumThresh.at(i).at(1)) {
						color = hslColors.at(i);
						return color;
					}
				}
			}
		}
	} catch(const std::out_of_range& oor) {
		printf("hsl::getHslColor(); HSL does not exist!\n");
		printf("HSL(%.0f,%.2f,%.2f)\n",hue,sat,lum);
	}
	return color;
}

//return the color of the Hsl values using Hsl colorspace
String Hsl::getHslColor(double hue, double sat, double lum, int &ind, Point pt)
{
	String color = "NONE";
	try {
		for(unsigned int i=0; i<hslColors.size(); i++) {
			if(hue>=hueThresh.at(i).at(0) && hue<=hueThresh.at(i).at(1)) {
				if(sat>=satThresh.at(i).at(0) && sat<satThresh.at(i).at(1)) {
					if(lum>=lumThresh.at(i).at(0) && lum<lumThresh.at(i).at(1)) {
						color = hslColors.at(i);
						ind = i;
						return color;
					}
				}
			}
		}
	} catch(const std::out_of_range& oor) {
		printf("hsl::getHslColor(); HSL does not exist!\n");
		printf("HSL(%.0f,%.2f,%.2f)\n",hue,sat,lum);
	}
	return color;
}

String Hsl::getHslColor(int index) {
	return hslColors.at(index);
}
void Hsl::release_memory()
{
	hslColors.clear();
	hueThresh.clear();
	satThresh.clear();
	lumThresh.clear();
	hslColors.shrink_to_fit();
	hueThresh.shrink_to_fit();
	satThresh.shrink_to_fit();
	lumThresh.shrink_to_fit();

}

//customize calcuation of hue avg
double Hsl::calcHueAvg(deque<int> &vec) {
	double hue=0;
	double total=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		hue = (vec.at(i)+180)%360;
		total += hue;
	}
	total /= vec.size();
	total /= 360;
	return total;
}

inline double hue2rgb(double var1, double var2, double vH) {
	if(vH<0) vH+=1;
	if(vH>1) vH-=1;
	if((6*vH)<1) return (var1+(var2-var1)*6*vH);
	if((2*vH)<1) return var2;
	if((3*vH)<2) return (var1+(var2-var1)*(0.666-vH)*6);
	return var1;
}

/** hue=0-360; sat=0-1; lum=0-1 **/
int *Hsl::hsl2rgb(double hue, double sat, double lum) {
	static double RGB[3];
	static int results[3];
	if(sat==0) {
		RGB[0] = round(lum * 255);
		RGB[1] = round(lum * 255);
		RGB[2] = round(lum * 255);
	}
	else {
		double temp1, temp2;
		if(lum<0.5)
			temp1 = lum*(1+sat);
		else
			temp1 = (lum+sat) - (lum*sat);
		temp2 = (2*lum) - temp1;
		hue /= 360;
		RGB[0] = round(255*hue2rgb(temp2,temp1,(hue+(1./3.))));
		RGB[1] = round(255*hue2rgb(temp2,temp1,hue));
		RGB[2] = round(255*hue2rgb(temp2,temp1,(hue-(1./3.))));
	}
	results[0] = (int)RGB[0];
	results[1] = (int)RGB[1];
	results[2] = (int)RGB[2];
	return results;
}

/** printf HSL/HSP values **/
String Hsl::printHS(double *HSL) {

	HSL[1] = roundDecimal(HSL[1],2);
	HSL[2] = roundDecimal(HSL[2],2);
	char text[50];
	sprintf(text,"(%.0f,%.2f,%.2f)",HSL[0],HSL[1],HSL[2]);
	String str(text);
	return str;
}
