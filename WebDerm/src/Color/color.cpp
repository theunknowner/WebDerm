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
	if(color1.substr(0,color1.size()-1)==color2.substr(0,color2.size()-1))
	{
		return true;
	}
	return false;
}

void Color::extractColorFromString(String color, vector<String> &vecColor) {
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(containsMainColor(color,mainColors.at(i))) {
			vecColor.push_back(mainColors.at(i));
		}
	}
}

