/*
 * functions2.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/test.h"

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
String getFileName(String filename)
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
	return name;
}
///////////////MOUSE EVENTS///////////////////////////
void mouseEvent(int evt, int x, int y, int flags, void* param) {
	rgb rgb;
    Mat* RGB = (Mat*) param;
    String color;
    int r,g,b;
    r = (int)(*RGB).at<Vec3b>(y, x)[2];
    g = (int)(*RGB).at<Vec3b>(y, x)[1];
    b = (int)(*RGB).at<Vec3b>(y, x)[0];
    color = rgb.pushColor(r,g,b);
    if (evt == CV_EVENT_LBUTTONDOWN) {
        printf("(%d,%d): %d,%d,%d - %s\n",x, y,r,g,b,color.c_str());
    }
}

//prints hysteresis output to terminal
void onMouseHysteresis(int evt, int x, int y, int flags, void* param) {
    Mat* rgb = (Mat*) param;
    static Mat img;
    static int flag=0;
    if (evt == CV_EVENT_LBUTTONDOWN) {
    	cout << "\033[2J\033[1;1H"; //clears terminal
    	if(flag==0)
    	{
    		img = (*rgb).clone();
    		flag=1;
    	}
    	(*rgb) = img.clone();
    	printf("(%d,%d)\n",x,y);
    	cout << "Result: " << testHysteresis(*rgb, y,x,Size(3,3)) << endl;
    	rectangle(*rgb,Point(x,y),Point(x+2,y+2),Scalar(0,0,255));
       // printf("(%d,%d): %d,%d,%d - %s\n",x, y,r,g,b,color.c_str());
    }
}

//prints hysteresis output to image window
void onMouseHysteresis2(int evt, int x, int y, int flags, void* param) {
	char coordText[10];
	char text[50];
	char arrText[9][50];
	int ptY = 30;
	String color;
	deque<Vec3b> vecRGB;
	deque<String> colorVec;
    Mat* rgb = (Mat*) param;
    Mat img2 = img2.zeros(Size(300,300),(*rgb).type());
    Mat img3 = (*rgb).clone();
    static Mat img;
    static int flag=0;
    sprintf(coordText, "(%d,%d)", x,y);
    if (evt == CV_EVENT_LBUTTONDOWN) {
    	if(flag==0)
    	{
    		img = (*rgb).clone();
    		flag=1;
    	}
    	(*rgb) = img.clone();
    	putText(img2, coordText, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    	color = testMouseHysteresis(*rgb, y,x,Size(3,3),vecRGB, colorVec);
    	sprintf(text, "%s", color.c_str());
    	rectangle(*rgb,Point(x,y),Point(x+2,y+2),Scalar(0,0,255));
    	for(unsigned int i=0; i<vecRGB.size(); i++)
    	{
    		sprintf(arrText[i], "[%d,%d,%d]-%s", vecRGB[i][2],vecRGB[i][1],vecRGB[i][0],colorVec[i].c_str());
    		putText(img2, arrText[i],Point(5,ptY), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    		ptY += 16;
    	}
    	putText(img2, text, Point(5,ptY), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    	imshow("Info", img2);
    }

    putText(img3, coordText, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    imshow("Hysteresis", img3);
}

//prints pixel info on left click in separate image window
void onMouseCheckColor(int event, int x, int y, int flags, void* param)
{
	rgb rgb;
	hsl hsl;
	Mat *img = (Mat*) param;
	Mat img2 = (*img).clone();
	Mat img3 = img3.zeros(Size(300,150),img2.type());
	String color;
	double * HSL;
	int h, s,l;
    char text[100];
    char hslText[100];
    char c[30];
    char gll[50];
    char cl[50];
    char ratioText[50];
    double graylumlevel=0, colorlevel=0, ratio=0;
    Vec3b rgbVec = img2.at<Vec3b>(y,x);
    HSL = hsl.rgb2hsl(rgbVec[2],rgbVec[1],rgbVec[0]);
    h = (int)HSL[0];
    s = roundDecimal(HSL[1],2) * 100;
    l = roundDecimal(HSL[2],2) * 100;
    color = rgb.checkBlack(rgbVec[2],rgbVec[1],rgbVec[0]);
    if(color=="OTHER")
    	color = rgb.calcColor(rgbVec[2],rgbVec[1],rgbVec[0]);
    graylumlevel = rgb.calcGrayLumLevel(rgbVec[2],rgbVec[1],rgbVec[0]);
    colorlevel = rgb.calcColorLevel2(rgbVec[2],rgbVec[1],rgbVec[0]);
    ratio = graylumlevel/colorlevel;

    sprintf(text, "(%d,%d) - [%d,%d,%d]", x, y,rgbVec[2],rgbVec[1],rgbVec[0]);
    sprintf(hslText,"{%d,%d,%d}",h,s,l);
    sprintf(c, "%s", color.c_str());
    sprintf(gll,"GLL: %0.3f",graylumlevel);
    sprintf(cl,"CL: %0.0f",colorlevel);
    sprintf(ratioText,"GLL/CL: %0.3f",ratio);
    putText(img3, text, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, hslText, Point(5,30), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, c, Point(5,45), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, gll, Point(5,60), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, cl, Point(5,75), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    putText(img3, ratioText, Point(5,90), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
    imshow("Info",img3);
}
