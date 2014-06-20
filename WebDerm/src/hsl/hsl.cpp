/*
 * hsl.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#include "hsl.h"

bool hsl::isThreshImported() {
	return THRESH_IMPORTED;
}

void hsl::setThreshImported(bool flag) {
	THRESH_IMPORTED = flag;
}

//import main HSL thresholds
bool hsl::importHslThresholds() {
	String foldername = path+"Thresholds/";
	String filename = foldername+"hslcolor-thresholds.csv";
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
			if(i>=1 && i<=2) thresh.push_back(atoi(vec.at(i).c_str()));
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

bool hsl::importLsThreshold()
{
	String folderName = path+"Thresholds/";
	String filename = folderName+"lum-thresholds.csv";
	String filename2 = folderName+"sat-thresholds.csv";
	fstream fsThresh(filename.c_str());
	fstream fsThresh2(filename2.c_str());
	if(fsThresh.is_open() && fsThresh2.is_open())
	{
		String temp;
		deque<String> vec;
		deque<double> thresh;
		deque<double> thresh2;
		getline(fsThresh,temp);
		while(getline(fsThresh,temp))
		{
			getSubstr(temp,',',vec); //gets line to string
			for(unsigned int i=0; i<vec.size(); i++)
			{
				thresh.push_back(atof(vec.at(i).c_str()));
			}
			lumLevel.push_back(thresh);
			vec.clear(); thresh.clear();
		}
		getline(fsThresh2,temp);
		while(getline(fsThresh2,temp))
		{
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++)
			{
				thresh2.push_back(atof(vec.at(i).c_str()));
			}
			satLevel.push_back(thresh2);
			vec.clear(); thresh2.clear();
		}
		fsThresh.close(); fsThresh2.close();
		deque<String>().swap(vec);
		deque<double>().swap(thresh);
		deque<double>().swap(thresh2);
		return true;
	}
	else
	{
		cout << "Importing ls-Thresholds Failed!" << endl;
		return false;
	}
}

//gets the minimum between the rgb value
double hsl::minRGB(double red, double green, double blue)
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
	double hsl::maxRGB(double red, double green, double blue)
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

//converts rgb values to hsl values
	double *hsl::rgb2hsl(double red, double green, double blue)
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
		B = max;
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

	double hsl::getHue()
	{
		H = round(H);
		return H;
	}

	double hsl::getSat()
	{
		return S;
	}

	double hsl::getLum()
	{
		return L;
	}

	double hsl::getBrite()
	{
		return B;
	}

	double hsl::calcLum(double red, double green, double blue)
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

	double hsl::calcBrite(double red, double green, double blue)
	{
		double r,g,b;
		double max;
		r = red/255;
		g = green/255;
		b = blue/255;
		max = maxRGB(r,g,b);
		return max;
	}

	double hsl::calcRelLum(double red, double green, double blue)
	{
		double lum;
		lum = (0.2126*red) + (0.7152*green) + (0.0722*blue);
		lum/=255;
		return lum;
	}

//return the color of the rgb values using hsl colorspace
String hsl::getHslColor(double r,double g,double b)
{
	int flag[3];
	deque<int> index;
	String color;
	rgb2hsl(r,g,b);
	for(unsigned int i=0; i<hslColors.size(); i++)
	{
		if((int)H>=hueThresh.at(i).at(0) && (int)H<=hueThresh.at(i).at(1))
		{
			flag[0]=1;
		}
		if(S>=satThresh.at(i).at(0) && S<=satThresh.at(i).at(1))
		{
			flag[1]=1;
		}
		if(L>=lumThresh.at(i).at(0) && L<=lumThresh.at(i).at(1))
		{
			flag[2]=1;
		}
		if(flag[0]==1 && flag[1]==1 && flag[2]==1)
		{
			index.push_back(i);
		}
		fill_n(flag,3,0);
	}
	for(unsigned int i=0; i<index.size(); i++)
	{
		color += hslColors.at(index[i]);
	}
	deque<int>().swap(index);
	return color;
}

void hsl::release_memory()
{
	deque<String>().swap(hslColors);
	deque< deque<int> >().swap(hueThresh);
	deque< deque<double> >().swap(satThresh);
	deque< deque<double> >().swap(lumThresh);
	deque< deque<double> >().swap(satLevel);
	deque< deque<double> >().swap(lumLevel);
}

//customize calcuation of hue avg
double hsl::calcHueAvg(deque<int> &vec) {
	double hue=0;
	double total=0;
	int flag=0, change=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		hue = (vec.at(i)+180)%360;
		//cout << hue << endl;
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

double *hsl::hsl2rgb(double hue, double sat, double lum) {
	static double RGB[3];
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
		RGB[0] = round(255*hue2rgb(temp2,temp1,(hue+0.333)));
		RGB[1] = round(255*hue2rgb(temp2,temp1,hue));
		RGB[2] = round(255*hue2rgb(temp2,temp1,(hue-0.333)));
	}
	return RGB;
}
