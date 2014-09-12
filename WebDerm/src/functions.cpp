/*
 * functions.cpp
 *
 *  Created on: Jan 17, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

String toString(int val)
{
	ostringstream stringStream;
	stringStream << val;
	return stringStream.str();
}

String toString(double val)
{
	ostringstream stringStream;
	stringStream << val;
	return stringStream.str();
}

String toString(bool val) {
	if(val==true) return "true";
	else return "false";
}

/** can also fix floating point precision that is not being shown on the
 * binary level */
double roundDecimal(double num, int places)
{
	double val=0;
	val = num * pow(10,places);
	val = round(val);
	val /= pow(10,places);
	return val;
}

String decimal2hex(int num) {
   stringstream ss;
   ss << std::hex << num;
   String str(ss.str());
   if(str.length()==1) str = "0"+str;
   return str;
}

void init_2D_Deque(deque< deque<String> > &vec, int cols, int rows, String val) {
	deque<String> tempVec;
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			tempVec.push_back(val);
		}
		vec.push_back(tempVec);
		tempVec.clear();
	}
	tempVec.clear();
	deque<String>().swap(tempVec);
}


void init_2D_Deque(deque< deque<double> > &vec, int cols, int rows, double val) {
	deque<double> tempVec;
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			tempVec.push_back(val);
		}
		vec.push_back(tempVec);
		tempVec.clear();
	}
	tempVec.clear();
	deque<double>().swap(tempVec);
}

void init_2D_Deque(deque< deque<int> > &vec, int cols, int rows, int val) {
	deque<int> tempVec;
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			tempVec.push_back(val);
		}
		vec.push_back(tempVec);
		tempVec.clear();
	}
	tempVec.clear();
	deque<int>().swap(tempVec);
}


/* return value up to Nth occurrence = first,second,third... of delimiter */
double Functions::getDelimitedValuesFromString(String inputString, char delimiter, int occurrence) {
	double result=0;
	String substr;
	unsigned int pos=0;
	int count=0;
	for(unsigned int i=0; i<inputString.length(); i++) {
		if(inputString[i]==delimiter) {
			substr = inputString.substr(pos,i-pos);
			pos = i+1;
			count++;
			result = atof(substr.c_str());
		}
		if(i==inputString.length()-1) {
			substr = inputString.substr(pos,(i-pos)+1);
			count++;
			result = atof(substr.c_str());
		}
		if(count==occurrence) break;
	}
	return result;
}

/* return all values separated by delimiter */
void Functions::getDelimitedValuesFromString(String inputString, char delimiter, deque<double> &vec) {
	String substr;
	unsigned int pos=0;
	for(unsigned int i=0; i<inputString.length(); i++) {
		if(inputString[i]==delimiter) {
			substr = inputString.substr(pos,i-pos);
			pos = i+1;
			vec.push_back(atof(substr.c_str()));
		}
		if(i==inputString.length()-1) {
			substr = inputString.substr(pos,(i-pos)+1);
			vec.push_back(atof(substr.c_str()));
		}
	}
}

double Functions::calcSlope(Point pt, Point origin)
{
	double slopeY, slopeX;
	slopeY = (pt.y - origin.y);
	slopeX = (pt.x - origin.x);
	if(slopeX==0)
		return slopeY;
	else
		return slopeY/slopeX;
}

Mat Functions::cropImage(Mat input)
{
	Mat img1, img2;
	int ptX=0, ptY=0;
	int roiWidth=0, roiHeight=0;

	for(int i=0;i<input.cols;i++)
	{
		if(countNonZero(input.col(i))>0)
		{
			ptX = i;
			roiWidth = input.cols-ptX;
			break;
		}
	}
	for(int j=0;j<input.rows;j++)
	{
		if(countNonZero(input.row(j))>0)
		{
			ptY= j;
			roiHeight = input.rows-ptY;
			break;
		}
	}
	img1 = input(Rect(ptX,ptY,roiWidth,roiHeight));

	for(int i=img1.cols-1;i>0;i--)
	{
		if(countNonZero(img1.col(i))>0)
		{
			roiWidth = i;
			break;
		}
	}
	for(int j=img1.rows-1;j>0;j--)
	{
		if(countNonZero(img1.row(j))>0)
		{
			roiHeight = j;
			break;
		}
	}
	img2 = img1(Rect(0,0,roiWidth,roiHeight));
	return img2;
}

Mat Functions::imageROI(Mat input, Point begin, Point end) {
	Mat dst;
	dst = input(Rect(begin,end));
	return dst;
}

Mat Functions::rotateImage(const Mat& source, double angle)
{
	Point2f src_center(source.cols/2.0F, source.rows/2.0F);
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	Mat dst;
	warpAffine(source, dst, rot_mat, source.size());
	return dst;
}

Mat Functions::fillEdges2(Mat img)
{
	Mat img2 = img.clone();
	Mat kernel[3] = Mat::zeros(1, img.cols, CV_8U);
	Point origin[3];
	Point pt[2];
	double originSlope[2];
	double avgOriginSlope=0;
	double thresh = 0.05;
	int flag=0, change=0;
	int j=0;
	int y[3];
	int i=0;
	double dist=0;
	double maxSlope=0;

	for(int row=1; row<img.rows-33; row+=16)
	{
		y[0] = row;
		y[1] = row+16;
		y[2] = row+32;
		kernel[0] = img2.row(row);
		kernel[1] = img2.row(row+16);
		kernel[2] = img2.row(row+32);
		for(int r=0; r<3; r++)
		{
			for(int c=0; c<kernel[r].cols; c++)
			{
				if((int)kernel[r].at<uchar>(0,c) == 0)
				{
					flag=0;
					if(flag!=change)
						change = flag;
				}
				if((int)kernel[r].at<uchar>(0,c) == 255)
				{
					flag=1;
					if(flag!=change)
					{
						change = flag;
						if(j<3)
						{
							origin[j] = Point(c,y[j]);
							j++;
							if(j==3)
							{
								originSlope[0] = calcSlope(origin[1], origin[0]);
								originSlope[1] = calcSlope(origin[2], origin[0]);
								avgOriginSlope = (originSlope[0]+originSlope[1])/2;
							}
							break;
						}
						else
						{
							if(i<3)
							{
								origin[i] = Point(c,y[i]);
								i++;
								if(i==3)
								{
									originSlope[0] = calcSlope(origin[1], origin[0]);
									originSlope[1] = calcSlope(origin[2], origin[0]);
									if( abs(originSlope[0]-avgOriginSlope) > abs(originSlope[1]-avgOriginSlope) )
									{
										maxSlope = abs(originSlope[0]-avgOriginSlope);
									}
									else
									{
										maxSlope = abs(originSlope[1]-avgOriginSlope);
									}
									dist = maxSlope / (abs(avgOriginSlope) + 0.00001);
									if(dist<=thresh)
									{
										avgOriginSlope = (originSlope[0]+originSlope[1])/2;
										i=0;
									}
									else
									{
										pt[0] = origin[1];
										pt[1] = origin[2];
										if(avgOriginSlope==0)
										{
											origin[1].x = origin[0].x;
											origin[2].x = origin[0].x;
										}
										else
										{
											origin[1].x = ((origin[1].y - origin[0].y) / (avgOriginSlope)) + origin[0].x;
											origin[2].x = ((origin[2].y - origin[0].y) / (avgOriginSlope)) + origin[0].x;
										}
										for(int a=1; a<=2; a++)
										{
											for(int col=origin[a].x; col<pt[a-1].x; col++)
											{
												img2.at<uchar>(origin[a].y, col) = 255;
											}
										}
										i=0;
									}
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	return img2;
}

Mat Functions::fillEdges3(Mat img)
{
	Mat img2 = img.clone();
	Mat kernel[5] = Mat::zeros(1, img.cols, CV_8U);
	Point origin[5];
	Point originPt;
	Point pt;
	int flag=0, change=0;
	int y[5];
	int j=0;
	int i=0;
	int devThresh=3;

	for(int row=1; row<img2.rows-5; row+=4)
	{
		y[0] = row;
		y[1] = row+1;
		y[2] = row+2;
		y[3] = row+3;
		y[4] = row+4;
		kernel[0] = img2.row(y[0]);
		kernel[1] = img2.row(y[1]);
		kernel[2] = img2.row(y[2]);
		kernel[3] = img2.row(y[3]);
		kernel[4] = img2.row(y[4]);
		for(int r=0; r<5; r++)
		{
			for(int c=0; c<kernel[r].cols; c++)
			{
				if( (int)kernel[r].at<uchar>(0,c) == 0)
				{
					flag = 0;
					if(flag!=change)
					{
						change=flag;

					}
				}
				if((int)kernel[r].at<uchar>(0,c) == 255)
				{
					flag=1;
					if(flag!=change)
					{
						change=flag;
						if(j<5)
						{
							origin[r] = Point(c, y[r]);
							originPt = origin[0];
							j++;
							if(j<5) break;
						}
						if(j>=5)
						{
							if(i<5)
							{
								origin[r] = Point(c, y[r]);
								i++;
								if(i==5)
								{
									for(int a=1; a<5; a++)
									{
										if(abs(origin[a].x-origin[0].x) >= devThresh || abs(origin[a].x-originPt.x) >= devThresh)
										{
											img2.at<uchar>(y[a],origin[0].x) = 255;
											for(int col=origin[0].x; col<origin[a].x; col++)
											{
												img2.at<uchar>(y[a],col) = 255;
											}
										}
									}
									originPt = origin[0];
								}
								break;
							}
						}
					}
				}
			}
		}
		i=0;
	}
	return img2;
}

Mat Functions::kMeansClustering(Mat &src) {
	Mat samples(src.rows * src.cols, 3, CV_32F);
	for( int y = 0; y < src.rows; y++ )
		for( int x = 0; x < src.cols; x++ )
			for( int z = 0; z < 3; z++)
				samples.at<float>(y + x*src.rows, z) = src.at<Vec3b>(y,x)[z];

	int clusterCount = 15;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers );


	Mat new_image( src.size(), src.type() );
	for( int y = 0; y < src.rows; y++ )
		for( int x = 0; x < src.cols; x++ ) {
			int cluster_idx = labels.at<int>(y + x*src.rows,0);
			new_image.at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
			new_image.at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
			new_image.at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
		}
	return new_image;
}

//! returns number of variables greater than or equal to last input
//! every input has to be in double form
int Functions::countGreaterEqual(double argNum, ...) {
	int arg = (int)argNum;
	double arr[arg-1];
	double lastInput = 0;
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,double);
		}
		else
			lastInput = va_arg(vl,double);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]>=lastInput) {
			++count;
		}
	}
	return count;
}

//! returns number of variables greater than last input
//! every input has to be in double form
int Functions::countGreater(double argNum, ...) {
	int arg = (int)argNum;
	double arr[arg-1];
	double lastInput = 0;
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,double);
		}
		else
			lastInput = va_arg(vl,double);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]>lastInput) {
			++count;
		}
	}
	return count;
}

//! returns number of variables greater than last input
//! every input has to be in double form
int Functions::countLesser(double argNum, ...) {
	int arg = (int)argNum;
	double arr[arg-1];
	double lastInput = 0;
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,double);
		}
		else
			lastInput = va_arg(vl,double);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]<lastInput) {
			++count;
		}
	}
	return count;
}

//! returns number of variables greater than last input
//! every input has to be in double form
int Functions::countLesserEqual(double argNum, ...) {
	int arg = (int)argNum;
	double arr[arg-1];
	double lastInput = 0;
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,double);
		}
		else
			lastInput = va_arg(vl,double);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]<=lastInput) {
			++count;
		}
	}
	return count;
}

//! returns number of variables greater than last input
//! every input has to be in double form
int Functions::countEqual(double argNum, ...) {
	int arg = (int)argNum;
	double arr[arg-1];
	double lastInput = 0;
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,double);
		}
		else
			lastInput = va_arg(vl,double);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]==lastInput) {
			++count;
		}
	}
	return count;
}

int Functions::countEqual(String argNum, ...) {
	int arg = atoi(argNum.c_str());
	String arr[arg-1];
	String lastInput = "";
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,char *);
		}
		else
			lastInput = va_arg(vl,char *);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]==lastInput) {
			++count;
		}
	}
	return count;
}

int Functions::countEqual(int argNum, ...) {
	int arg = argNum;
	int arr[arg-1];
	int lastInput = 0;
	va_list vl;
	va_start(vl,argNum);
	for(int i=0; i<arg; i++) {
		if(i<(arg-1)) {
			arr[i] = va_arg(vl,int);
		}
		else
			lastInput = va_arg(vl,int);
	}
	va_end(vl);
	int count=0;
	for(int i=0; i<(arg-1); i++) {
		if(arr[i]==lastInput) {
			++count;
		}
	}
	return count;
}


double Functions::getMin(deque<double> &vec, int &minIndex) {
	double lowest=-1;
	minIndex=-1;
	try {
		lowest = vec.at(0);
		minIndex = 0;
		for(unsigned int i=0; i<vec.size(); i++) {
			if(lowest>vec.at(i)) {
				lowest = vec.at(i);
				minIndex = i;
			}
		}
	}
	catch(const std::out_of_range &oor) {
		//printf("Functions::getMin(deque<double> vec, minIndex) out of range!\n");
		return -1;
	}
	return lowest;
}

double Functions::getMax(deque<double> &vec, int &maxIndex) {
	double largest=-1;
	maxIndex=-1;
	try {
		largest = vec.at(0);
		maxIndex = 0;
		for(unsigned int i=0; i<vec.size(); i++) {
			if(largest<vec.at(i)) {
				largest = vec.at(i);
				maxIndex = i;
			}
		}
	}
	catch(const std::out_of_range &oor) {
		//printf("Functions::getMax(deque<double> vec, maxIndex) out of range!\n");
		return -1;
	}
	return largest;
}
