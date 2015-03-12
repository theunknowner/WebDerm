/*
 * functions2.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"

//generic getSubstr function
void getSubstr(String str, char delimiter, deque<String> &vec)
{
	String temp;
	int j=0;
	for(unsigned int i=0; i<str.size(); i++)
	{
		if(str[i]==delimiter)
		{
			temp = str.substr(j,(i-j));
			vec.push_back(temp.c_str());
			j=i+1;
		}
		if(i==(str.size()-1))
		{
			temp = str.substr(j,str.size()-j);
			if(temp!="")
				vec.push_back(temp.c_str());
			j=0;
		}
	}
}



//gets coordinated that matches RGB
void getCoordinates(Mat &img, int red, int green, int blue)
{
	int r,g,b;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			b = img.at<Vec3b>(row,col)[0];
			g = img.at<Vec3b>(row,col)[1];
			r = img.at<Vec3b>(row,col)[2];
			if(r==red && g==green && b==blue)
			{
				printf("(%d,%d)\n",col,row);
			}
		}
	}
}

//gets image filename from filepath
String getFileName(String filename, String end)
{
	char delimit1 = '/';
	char delimit2 = '.';
	String name;
	int pos=0;
	int pos2=0;
	for(unsigned int i=0; i<filename.size(); i++)
	{
		if(filename[i]==delimit1)
		{
			pos=i+1;
		}
		if(filename[i]==delimit2)
		{
			pos2=i;
		}
	}
	name = filename.substr(pos,pos2-pos);
	if(end!="") {
		pos = name.find(end);
		name = name.substr(0,pos);
	}
	return name;
}
