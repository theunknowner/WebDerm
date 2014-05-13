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
	int count1,count2;
	deque<String> vec1, vec2;
	extractColorFromString(color1,vec1);
	extractColorFromString(color2, vec2);
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
String Color::reassignLevels(String color, int r, int g, int b) {
	rgb rgb;
	deque<String> colorVec;
	String pix;
	double grayLevel=0;
	double colorLevel=0;
	extractColorFromString(color, colorVec);
	grayLevel = rgb.calcGrayLevel2(r,g,b);
	colorLevel = rgb.calcColorLevel2(r,g,b);
	for(unsigned int i=0; i<colorVec.size(); i++) {
		if(colorVec.at(i)=="Gray")
			pix += colorVec.at(i) + toString(grayLevel);
		else
			pix += colorVec.at(i) + toString(colorLevel);
	}
	return pix;
}

String Color::getMainColor(String color) {
	String pix;
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color.find(mainColors.at(i))!=string::npos && mainColors.at(i)!="Gray")
			pix += mainColors.at(i);
	}
	return pix;
}
