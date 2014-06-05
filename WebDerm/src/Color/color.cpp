/*
 * color.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: jason
 */

#include "color.h"

//check if color1 contains color2
bool Color::containsColor(String color1, String color2)
{
	size_t pos=0;
	pos = color1.find("+");
	if(pos!=string::npos)
	{
		if(color2 == color1.substr(0,pos) || color2 == color1.substr(pos+1,color1.size()-(pos+1)))
		{
			return true;
		}
	}
	return false;
}

//returns the amount of time pixels contain main color
int Color::containsMainColor(String color, String mColor)
{
	int count=0;
	size_t pos=0;
	while((pos=color.find(mColor,pos))!=string::npos)
	{
		++count;
		pos += mColor.length();
	}
	return count;
}

bool Color::isSameColor(String color1, String color2)
{
	deque<String> vec1, vec2;
	extractColorFromString(color1,vec1);
	extractColorFromString(color2, vec2);
	if(vec1.size()!=vec2.size()) return false;
	if(vec1.size()==1 && vec2.size()==1) {
		if(vec1.at(0)==vec2.at(0)) return true;
	}
	if(vec1.size()>=3 || vec2.size()>=3) {
		if(vec1.size()!=vec2.size())
			return false;
	}
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec2.size(); j++) {
			if(vec1.at(i)!="Gray" && vec2.at(j)!="Gray") {
				if(vec1.at(i)==vec2.at(j))
					return true;
			}
		}
	}
	return false;
}

void Color::extractColorFromString(String color, deque<String> &vecColor) {
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(containsMainColor(color,mainColors.at(i))) {
			vecColor.push_back(mainColors.at(i));
		}
	}
}

String Color::extractShade(String pix) {
	Intensity in;
	int shadeCount = in.getShadeCount();
	String shade = "";
	if(pix=="Black") return pix;
	for(int i=0; i<shadeCount; i++) {
		shade = in.getShade(i);
		if(pix.find(shade)!=string::npos)
			break;
	}
	return shade;
}
//returns the amount of different colors in the string
int Color::countColors(String color) {
	int count=0;
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color.find(mainColors.at(i))!=string::npos) {
			++count;
		}
	}
	return count;
}

//gets color and recalculates gray and color levels
String Color::reassignLevels(String pix, int r, int g, int b) {
	rgb rgb;
	deque<String> colorVec;
	String pix2;
	double grayLevel=0;
	double colorLevel=0;
	extractColorFromString(pix, colorVec);
	grayLevel = rgb.calcGrayLevel(r,g,b);
	colorLevel = rgb.calcColorLevel2(r,g,b);
	for(unsigned int i=0; i<colorVec.size(); i++) {
		if(colorVec.at(i)=="Gray")
			pix2 += colorVec.at(i) + toString(grayLevel);
		else
			pix2 += colorVec.at(i) + toString(colorLevel);
	}
	return pix2;
}

String Color::getMainColor(String color) {
	String pix;
	int flag=0;
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color.find(mainColors.at(i))!=string::npos && mainColors.at(i)!="Gray") {
			pix += mainColors.at(i);
			flag=1;
		}
	}
	if(flag==0) {
		pix = "Grey";
	}
	return pix;
}

void Color::output2Image(deque< deque<double> > &input) {
	Mat img = img.zeros(Size(700,700),CV_8U);
	for(unsigned int y=0; y<input.size(); y++) {
		for(unsigned int x=0; x<input.at(y).size(); x++) {
			img.at<uchar>(y,x) = input.at(y).at(x);
		}
	}
	imwrite("2x2imgBw.png",img);
	//imshow("BW",img);
	//waitKey(0);
}

void Color::output2Image2(deque< deque<String> > &input, String name) {
	Mat img = img.zeros(Size(700,700),CV_8U);
	for(unsigned int y=0; y<input.size(); y++) {
		for(unsigned int x=0; x<input.at(y).size(); x++) {
			if(input.at(y).at(x)=="Black")
				img.at<uchar>(y,x) = 0;
			if(input.at(y).at(x)=="Dark")
				img.at<uchar>(y,x) = 60;
			if(input.at(y).at(x)=="High")
				img.at<uchar>(y,x) = 110;
			if(input.at(y).at(x)=="Low")
				img.at<uchar>(y,x) = 155;
			if(input.at(y).at(x)=="Light")
				img.at<uchar>(y,x) = 185;
			if(input.at(y).at(x)=="White")
				img.at<uchar>(y,x) = 255;
		}
	}
	String str = "5shadesBW2x2" + name + ".png";
	imwrite(str,img);
}

void Color::output2Image3(deque< deque<String> > &window, String name) {
	String filename = path+"Thresholds/output-shades.csv";
	fstream fs(filename.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		deque<String> color;
		deque<int>	thresh1;
		deque< deque<int> > values;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==0) color.push_back(vec.at(i));
				if(i>0) thresh1.push_back(atoi(vec.at(i).c_str()));
			}
			values.push_back(thresh1);
			thresh1.clear();
			vec.clear();
		}
		String shade;
		Mat img = img.zeros(Size(700,700),16);
		for(unsigned int i=0; i<window.size(); i++) {
			for(unsigned int j=0; j<window.at(i).size(); j++) {
				shade = extractShade(window.at(i).at(j));
				if(shade=="Black") {
					img.at<Vec3b>(i,j)[2] = 0;
					img.at<Vec3b>(i,j)[1] = 0;
					img.at<Vec3b>(i,j)[0] = 0;
				}
				else if(shade=="White") {
					img.at<Vec3b>(i,j)[2] = 255;
					img.at<Vec3b>(i,j)[1] = 255;
					img.at<Vec3b>(i,j)[0] = 255;
				}
				else {
					for(unsigned int k=0; k<color.size(); k++) {
						if(window.at(i).at(j).find(color.at(k))!=string::npos) {
							img.at<Vec3b>(i,j)[2] = values.at(k).at(0);
							img.at<Vec3b>(i,j)[1] = values.at(k).at(1);
							img.at<Vec3b>(i,j)[0] = values.at(k).at(2);
							break;
						}
					}
				}
			}
		}
		String file = "outputShades2x2" + name + ".png";
		imwrite(file,img);
	}
}
