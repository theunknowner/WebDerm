/*
 * run.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"

void runCheckRGBColor()
{
	rgb rgb;
	rgb.importThresholds();
	String str;
	deque<String> strVec;
	deque<int> rgbVec;
	int ch=0;
	for(;;)
	{
		cout << "Enter RGB: ";
		cin >>	str;
		getSubstr(str,',',strVec);
		for(unsigned int i=0; i<strVec.size(); i++)
		{
			rgbVec.push_back(atoi(strVec.at(i).c_str()));
		}
		cout << "RESULTS: " << rgb.pushColor(rgbVec[0],rgbVec[1],rgbVec[2]) << endl;
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
		deque<String>().swap(strVec);
		deque<int>().swap(rgbVec);
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
		deque<int> rgbVec;
		deque<String> strVec;
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
 			str = rgb.pushColor(rgbVec[0],rgbVec[1],rgbVec[2]);
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
		deque<int>().swap(rgbVec);
		deque<String>().swap(strVec);
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
	deque<String> vec;
	Mat img = imread(file);
	Mat img2;
	if(img.cols>=size.width || img.rows>=size.height)
		{resize(img, img2, size,0,0,INTER_AREA);}
	if(img.cols<size.width || img.rows<size.height)
		{resize(img, img2, size,0,0, INTER_CUBIC);}
	getSubstr(filename,'.',vec);
	if(write==1) imwrite(vec[0]+".png",img2);

	deque<String>().swap(vec);
	return img2;
}

Mat runResizeImage(String filename, Size size,int write)
{
	deque<String> vec;
	Mat img = imread(filename);
	Mat img2;
	if(img.cols>=size.width || img.rows>=size.height)
		{resize(img, img2, size,0,0,INTER_AREA);}
	if(img.cols<size.width || img.rows<size.height)
		{resize(img, img2, size,0,0, INTER_CUBIC);}
	getSubstr(filename,'.',vec);
	if(write==1) imwrite(vec[0]+".png",img2);

	img.release();
	deque<String>().swap(vec);
	return img2;
}

void runHysteresis()
{
	rgb rgb;
	hsl hsl;
	Intensity in;
	String filename;
	String name;
	String input;
	Size size;
	cout << "Enter filename: ";
	cin >> filename;
	cout << "1) Size(1x1)" << endl << "2) Size(2x2)" << endl;
	cin >> input;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	name = getImageName(filename);
	if(input=="1") size = Size(1,1);
	if(input=="2") size = Size(2,2);
	int s = 3;
	bool flag[s];
	flag[0]=rgb.importThresholds();
	flag[1]=hsl.importHslThresholds();
	flag[2]=in.importThresholds();
	for(int i=0; i<s; i++) {
		if(flag[i]==false) {
			flag[0] = false;
			break;
		}
	}
	if(flag[0]==true) {
		if(size.height==1 && size.width==1)
			hysteresis1x1(img2,name);
		else
			hysteresis(img2,size,name);
	}
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
	hsl.release_memory();
	in.release_memory();
}

void runAllHysteresis(String *filenames, int fileSize) {
	rgb rgb;
	hsl hsl;
	Intensity in;
	String name;
	String input;
	Size size(2,2);
	Mat img, img2,img3, mask;
	int s = 3;
	bool flag[s];
	flag[0]=rgb.importThresholds();
	flag[1]=hsl.importHslThresholds();
	flag[2]=in.importThresholds();
	for(int i=0; i<s; i++) {
		if(flag[i]==false) {
			flag[0] = false;
			break;
		}
	}
	if(flag[0]==true) {
		for(int i=0; i<fileSize; i++) {
			img = runResizeImage(filenames[i],Size(700,700),0);
			getSkin(img, mask);
			img.copyTo(img2, mask);
			name = getImageName(filenames[i]);
			hysteresis(img2,size,name);
			img.release(); img2.release(); mask.release();
		}
	}
	rgb.release_memory();
	hsl.release_memory();
	in.release_memory();
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
	hsl hsl;
	hsl.importLsThreshold();
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
	hsl hsl;
	hsl.importLsThreshold();
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
	hsl.release_memory();
}

void outputFreqColor(Mat &img)
{
	rgb rgb;
	int arr[rgbColors.size()];
	fill_n(arr, rgbColors.size(), 0);
	int ind=0;
	double dist=0;
	int r,g,b;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			rgb.pushColor(r,g,b,ind,dist);
			++arr[ind];
		}
	}
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorFreq.txt","w");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		if(arr[i]!=0)
		{
			fprintf(fp,"%d: %s(%d)\n", i+2, rgbColors.at(i).c_str(), arr[i]);
		}
	}
}

void runOutputColorFreq()
{
	rgb rgb;
	hsl hsl;
	String filename;
	String name;
	String input;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	rgb.importThresholds();
	hsl.importLsThreshold();
	outputFreqColor(img2);
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
	hsl.release_memory();
}

void runOutputFarRGB() {
	rgb rgb;
	hsl hsl;
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
	hsl.importLsThreshold();
	//outputFarRGBScript(img2,name);
	img.release(); img2.release(); mask.release();
	rgb.release_memory(); hsl.release_memory();
}
