/*
 * run.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#include "run.h"

void runCheckRGBColor()
{
	rgb rgb;
	rgb.importThresholds();
	String str;
	vector<String> strVec;
	vector<int> rgbVec;
	int ch=0;
	double e=0.25;
	for(;;)
	{
		cout << "Enter RGB: ";
		cin >>	str;
		getSubstr(str,',',strVec);
		for(unsigned int i=0; i<strVec.size(); i++)
		{
			rgbVec.push_back(atoi(strVec.at(i).c_str()));
		}
		cout << "RESULTS: " << rgb.pushColor(rgbVec[0],rgbVec[1],rgbVec[2],e) << endl;
		cout << "Press any key to continue...";
		getchar();
		for(;;)
		{
			ch = getchar();
			if(ch!=0)
			{
				cout << "\033[2J\033[1;1H";
				break;
			}
		}
		vector<String>().swap(strVec);
		vector<int>().swap(rgbVec);
	}
}

//function to check RGB list
void runCheckList()
{
	rgb rgb;
	fstream fs("RGB Check List.csv");
	FILE * fp;
	fp = fopen("RGB Output.csv", "w");
	if(fs.is_open())
	{
		rgb.importThresholds();
		String temp, str;
		vector<int> rgbVec;
		vector<String> strVec;
		char delimit = ',';
		bool flag;
		getline(fs,temp);
		while(getline(fs,temp))
		{
			getSubstr(temp,delimit,strVec);
			for(unsigned int i=0; i<strVec.size(); i++)
			{
				if(i<3)
				{
					rgbVec.push_back(atoi(strVec.at(i).c_str()));
				}
			}
 			str = rgb.pushColor(rgbVec[0],rgbVec[1],rgbVec[2],0.25);
 			if(str==strVec.at(3))
 			{
 				flag=true;
 			}
 			else
 			{
 				flag=false;
 			}
			fprintf(fp, "%d,%d,%d,%s,%s,%d\n",rgbVec[0],rgbVec[1],rgbVec[2],strVec.at(3).c_str(),str.c_str(),flag);
			strVec.clear();
			rgbVec.clear();
		}
		vector<int>().swap(rgbVec);
		vector<String>().swap(strVec);
		fs.close();
	}
	else
	{
		cout << "RGB Check List.csv not found!" << endl;
	}
}

Mat runResizeImage(String foldername, String filename, Size size,int write)
{
	String file = foldername+"/"+filename;
	vector<String> vec;
	Mat img = imread(file);
	Mat img2;
	if(img.cols>size.width || img.rows>size.height)
		{resize(img, img2, size,0,0,INTER_AREA);}
	if(img.cols<size.width || img.rows<size.height)
		{resize(img, img2, size,0,0, INTER_CUBIC);}
	getSubstr(filename,'.',vec);
	if(write==1) imwrite(vec[0]+".png",img2);

	vector<String>().swap(vec);
	return img2;
}

Mat runResizeImage(String filename, Size size,int write)
{
	vector<String> vec;
	Mat img = imread(filename);
	Mat img2;
	if(img.cols>size.width || img.rows>size.height)
		{resize(img, img2, size,0,0,INTER_AREA);}
	if(img.cols<size.width || img.rows<size.height)
		{resize(img, img2, size,0,0, INTER_CUBIC);}
	getSubstr(filename,'.',vec);
	if(write==1) imwrite(vec[0]+".png",img2);

	img.release();
	vector<String>().swap(vec);
	return img2;
}

void runHysteresis()
{
	rgb rgb;
	String filename;
	String name;
	String input;
	cout << "Enter filename: ";
	cin >> filename;
	cout << "Do you want to write image? (y/n) ";
	cin >> input;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	name = getImageName(filename);
	if(input=="y") imwrite(name+".png",img2);
	rgb.importThresholds();
	hysteresis(img2,name);
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
}

 void runGetSkin()
 {
	String filename;
	String input;
	cout << "Enter filename: ";
	cin >> filename;
	cout << "Do you want to write image? (y/n)";
	cin >> input;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	if(input=="y")
	{
		String name = getImageName(filename);
		imwrite(name+".png",img2);
	}
	imshow("Img", img2);
	waitKey(0);
	img.release(); img2.release(); mask.release();
 }

 //runs hysteresis with info output to terminal
void runMouseHysteresis()
{
	rgb rgb;
	rgb.importThresholds();
	Mat img2, mask;
	String filename;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2,mask);
	namedWindow("Hysteresis");
	cvSetMouseCallback("Hysteresis", onMouseHysteresis, &img2);
	while (waitKey(1)<0)
	{
	imshow("Hysteresis", img2);
	}
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
}

//runs hysteresis with info output to image window
void runMouseHysteresis2()
{
	rgb rgb;
	rgb.importThresholds();
	Mat img2, mask;
	String filename;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2,mask);
	namedWindow("Hysteresis");
	cvSetMouseCallback("Hysteresis", onMouseHysteresis2, &img2);
	imshow("Hysteresis", img2);
	waitKey(0);
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
}
