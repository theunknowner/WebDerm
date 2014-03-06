/*
 * rgb.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "rgb.h"

//imports RGB colorspace thresholds
bool rgb::importThresholds()
{
	String folderName = path+"Thresholds/";
	String filename = folderName+"color-thresholds.csv";
	String filename2 = folderName+"colors.csv";
	fstream fsThresh(filename.c_str());
	fstream fsColors(filename2.c_str());
	if(fsThresh.is_open() && fsColors.is_open())
	{
		String temp;
		vector<String> vec;
		vector<double> thresh;
		vector<double> thresh2;
		double absValue, normValue;
		while(getline(fsColors,temp))
		{
			mainColors.push_back(temp);
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
				if(j>=1 && j<=6)
				{
					thresh.push_back(atof(vec.at(j).c_str()));
				}
				if(j>=7 && j<=12)
				{
					thresh2.push_back(atof(vec.at(j).c_str()));
				}
			}
			absThresh.push_back(thresh);
			normThresh.push_back(thresh2);
			vec.clear(); thresh.clear(); thresh2.clear();
		}
		for(unsigned int i=0; i<absThresh.size(); i++)
		{
			for(unsigned int j=0; j<6; j+=2)
			{
				absValue = (absThresh.at(i).at(j) + absThresh.at(i).at(j+1))/2;
				normValue = (normThresh.at(i).at(j) + normThresh.at(i).at(j+1))/2;
				thresh.push_back(absValue);
				thresh2.push_back(normValue);
			}
			absMeanThresh.push_back(thresh);
			normMeanThresh.push_back(thresh2);
			thresh.clear(); thresh2.clear();
		}
		fsColors.close(); fsThresh.close();
		vector<String>().swap(vec);
		vector<double>().swap(thresh);
		vector<double>().swap(thresh2);
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
double rgb::normEucDist(double red, double green, double blue, vector<double> &vec)
	{
		double result=0;
		double color[3] = {red,green,blue};
		double normVals[3];
		for(int i=0; i<3; i++)
		{
			normVals[i] = color[i]/(color[0]+color[1]+color[2]);
			normVals[i] -= vec.at(i);
			normVals[i] *= normVals[i];
			result += normVals[i];
		}
		result = sqrt(result);
		return result;
	}

//calculates the Euclidean Distance between absolute rgb input and colors(vec).
double rgb::absEucDist(double red, double green, double blue, vector<double> &vec)
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

//checks if color is black
String rgb::checkBlack(int r, int g, int b)
	{
		if(r<16 && g<16 && b<16)
		{
			return "Black0";
		}
		return "OTHER";
	}

//pushes color using Euclidean Distance
String rgb::pushColor(int red, int green, int blue, double e)
	{
		double smallest=0;
		double val=0;
		unsigned int index=0;
		double normDistVals[normMeanThresh.size()];
		double absDistVals[absMeanThresh.size()];
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			normDistVals[i] = normEucDist(red,green,blue, normMeanThresh.at(i));
			absDistVals[i] = absEucDist(red,green,blue, absMeanThresh.at(i));
			//cout << colors[i] << ": " << setw(10) << "\t" << normDistVals[i] << " | " << absDistVals[i] << " | " << absDistVals[i]*pow(normDistVals[i],e) << endl;
		}
		smallest = absDistVals[0] * pow(normDistVals[0],e);
		index = 0;
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			val = absDistVals[i]*pow(normDistVals[i],e);
			if(val<smallest)
			{
				smallest = val;
				index = i;
			}
		}
		return rgbColors[index];
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
			if(colorIndex[i]>greatest)
			{
				greatest = colorIndex[i];
				index[0] = i;
			}
		}
		return rgbColors[index[0]];
	}

double rgb::pushColor2(int red, int green, int blue, int &ind)
	{
		double smallest=0;
		double val=0;
		unsigned int index=0;
		int colorIndex[rgbColors.size()];
		fill_n(colorIndex,rgbColors.size(),0);
		double absDistVals[absMeanThresh.size()];
		for(unsigned int i=0; i<normMeanThresh.size(); i++)
		{
			absDistVals[i] = absEucDist(red,green,blue, absMeanThresh.at(i));
		}
		smallest = absDistVals[0];
		index = 0;
		for(unsigned int i=0; i<absMeanThresh.size(); i++)
		{
			val = absDistVals[i];
			if(val<smallest)
			{
				smallest = val;
				index = i;
			}
		}
		ind = index;
		return smallest;
	}

String rgb::pushColor(int red, int green, int blue, int &ind)
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
			//cout << rgbColors[i] << ": " << setw(10) << "\t" << normDistVals[i] << " | " << absDistVals[i] << " | " << absDistVals[i]*pow(normDistVals[i]) << endl;
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
		return rgbColors[index2];
	}

String rgb::getModifier(int red, int green, int blue)
{
	hsl hsl;
	double lum=0;
	lum = hsl.calcLum(red,green,blue) * 100;
	for(unsigned int i=0; i<lumThresh.size(); i++)
	{
		if(lum>=lumThresh.at(i).at(0) && lum<=lumThresh.at(i).at(1))
		{
			return hslColors.at(i);
		}
	}
	return "";
}

int rgb::calcGrayLevel(int red, int green, int blue)
{
	hsl hsl;
	double sat=0;
	hsl.rgb2hsl(red,green,blue);
	sat = hsl.getSat();
	sat = roundDecimal(sat,2);
	for(unsigned int i=0; i<satThresh.size(); i++)
	{
		if(sat>satThresh.at(i).at(0) && sat<=satThresh.at(i).at(1))
		{
			return i;
		}
	}
	return 0;
}

int rgb::calcColorLevel(double red, double green, double blue)
{
	hsl hsl;
	double lum=0;
	//hsl.rgb2hsl(red,green,blue);
	lum = hsl.calcLum(red,green,blue);
	lum = roundDecimal(lum,2);
	for(unsigned int i=0; i<lumThresh.size(); i++)
	{
		if(lum>lumThresh.at(i).at(1) && lum<=lumThresh.at(i).at(2))
		{
			return lumThresh.at(i).at(0);
		}
	}
	return 0;
}

int rgb::getGrayLevel(String color)
{
	size_t pos=0;
	int level=0;
	String lvl;
	pos = color.find("Gray");
	if(pos!=string::npos)
	{
		lvl = color.substr(pos+4,1);
		level = atoi(lvl.c_str());
	}
	return level;
}

int rgb::getColorLevel(String color, String mainColor)
{
	int level=0;
	size_t pos=0;
	String str;
	pos = color.find(mainColor);
	if(pos!=string::npos)
	{
		str = color.substr(pos+mainColor.size(),color.size()-(pos+mainColor.size()));
	}
	level = atoi(str.c_str());
	return level;
}

void rgb::getLevels(String color, int * level)
{
	vector<size_t> posVec;
	vector<String> colors;
	int count=0;
	size_t pos=0;
	String lvl;
	pos = color.find("Gray");
	if(pos!=string::npos)
	{
		lvl = color.substr(pos+4,color.size()-(pos+4));
		level[0] = atoi(lvl.c_str());
	}
	else
	{
		level[0] = 0;
	}
	for(unsigned int i=0; i<mainColors.size(); i++)
	{
		pos = color.find(mainColors.at(i));
		if(pos!=string::npos && mainColors.at(i)!="Gray")
		{
			++count;
			posVec.push_back(pos);
			colors.push_back(mainColors.at(i));
		}
	}
	if(count==1)
	{
		lvl = color.substr(posVec.at(0)+colors.at(0).size(),1);
		level[1] = 0;
		level[2] = atoi(lvl.c_str());
	}
	if(count==2)
	{
		lvl = color.substr(posVec.at(0)+colors.at(0).size(),
				colors.at(0).size()-(posVec.at(0)+colors.at(0).size()));
		level[1] = atoi(lvl.c_str());
		lvl = color.substr(posVec.at(1)+colors.at(1).size(),
				colors.at(0).size()-(posVec.at(0)+colors.at(0).size()));
		level[2] = atoi(lvl.c_str());
	}
	vector<size_t>().swap(posVec);
	vector<String>().swap(colors);
}

double rgb::calcIntensity(double red, double green, double blue)
{
	double result = 0.33 * red + 0.33 * green + 0.33 * blue;
	result = round(result);
	return result;
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

void rgb::release_memory()
{
	vector< vector<double> >().swap(normMeanThresh);
	vector< vector<double> >().swap(absMeanThresh);
	vector< vector<double> >().swap(normThresh);
	vector< vector<double> >().swap(absThresh);
	vector<String>().swap(rgbColors);
	vector<String>().swap(mainColors);
}

bool rgb::isBrownViolet(int r, int g, int b)
{
	if(r>=160&&r<=171&&g>=117&&g<=133&&b>=117&&b<=133)
	{
		return true;
	}
	return false;

}
