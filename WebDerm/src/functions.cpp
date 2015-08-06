/*
 * functions.cpp
 *
 *  Created on: Jan 17, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/quicksort.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/median.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/mean.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/stddev.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

String toString(int val)
{
	ostringstream stringStream;
	stringStream << val;
	return stringStream.str();
}

String toString(unsigned int val)
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

void imfill(Mat &img)
{
	vector<vector<Point> > contours; 	//holds all the contour points
	vector<Vec4i> hierarchy;
	findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	Mat drawing = Mat::zeros( img.size(), CV_8U );
	drawContours( drawing, contours, -1, Scalar(255), CV_FILLED, 8, hierarchy, 1, Point(0,0) );
	img.release();
	img = drawing.clone();
	drawing.release();
}

//flag=0: keep window, flag=1: destroy window
void imgshow(Mat src, int flag, String name) {
	static int num=1;
	if(name=="") {
		name = "img" + toString(num);
		num++;
	}
	namedWindow(name,CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	imshow(name,src);
	waitKey(0);
	if(flag==1)
		destroyAllWindows();
}

//generic getSubstr function
void getSubstr(String str, char delimiter, deque<String> &vec)
{
	if(vec.size()>0) vec.clear();

	String temp;
	int j=0;
	for(unsigned int i=0; i<str.size(); i++)
	{
		if(str[i]==delimiter && i>j)
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

//generic getSubstr function
void getSubstr(String str, char delimiter, vector<String> &vec)
{
	if(vec.size()>0) vec.clear();

	String temp;
	int j=0;
	for(unsigned int i=0; i<str.size(); i++)
	{
		if(str[i]==delimiter && i>j)
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

//gets image filename from filepath
String getFileName(String filename, String end)
{
	char delimit1 = '/';
	char delimit2 = '.';
	String name = filename;
	int pos=0;
	int pos2=0;
	if(filename.find(delimit2)!=string::npos) {
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
	}

	return name;
}

String getFolderName(String path) {
	char delimit1 = '/';
	int pos=0;
	if(path.back()=='/') {
		path = path.substr(0,path.length()-1);
	}
	for(int i=path.size(); i>=0; i--) {
		if(path[i]==delimit1) {
			pos=i+1;
			break;
		}
	}
	String name = path.substr(pos,path.length()-pos);

	return name;
}

void init_2D_Deque(deque< deque<String> > &vec, int rows, int cols, String val) {
	vec.resize(rows,deque<String>(cols,val));
}


void init_2D_Deque(deque< deque<double> > &vec, int rows, int cols, double val) {
	vec.resize(rows,deque<double>(cols,val));
}

void init_2D_Deque(deque< deque<int> > &vec, int rows, int cols, int val) {
	vec.resize(rows,deque<int>(cols,val));
}

void init_3D_Deque(deque< deque< deque<String> > > &vec, int rows, int cols, int dep, String val) {
	deque<String> tempVec;
	deque< deque<String> > tempVec2;
	vec.clear();
	deque< deque< deque<String> > >().swap(vec);
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			for(int k=0; k<dep; k++)  {
				tempVec.push_back(val);
			}
			tempVec2.push_back(tempVec);
			tempVec.clear();
		}
		vec.push_back(tempVec2);
		tempVec2.clear();
	}
	tempVec.clear();
	tempVec2.clear();
	deque<String>().swap(tempVec);
	deque< deque<String> >().swap(tempVec2);
}

void init_3D_Deque(deque< deque< deque<double> > > &vec, int rows, int cols, int dep, double val) {
	deque<double> tempVec;
	deque< deque<double> > tempVec2;
	vec.clear();
	deque< deque< deque<double> > >().swap(vec);
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			for(int k=0; k<dep; k++)  {
				tempVec.push_back(val);
			}
			tempVec2.push_back(tempVec);
			tempVec.clear();
		}
		vec.push_back(tempVec2);
		tempVec2.clear();
	}
	tempVec.clear();
	tempVec2.clear();
	deque<double>().swap(tempVec);
	deque< deque<double> >().swap(tempVec2);
}

void init_3D_Deque(deque< deque< deque<int> > > &vec, int rows, int cols, int dep, int val) {
	deque<int> tempVec;
	deque< deque<int> > tempVec2;
	vec.clear();
	deque< deque< deque<int> > >().swap(vec);
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			for(int k=0; k<dep; k++)  {
				tempVec.push_back(val);
			}
			tempVec2.push_back(tempVec);
			tempVec.clear();
		}
		vec.push_back(tempVec2);
		tempVec2.clear();
	}
	tempVec.clear();
	tempVec2.clear();
	deque<int>().swap(tempVec);
	deque< deque<int> >().swap(tempVec2);
}

deque<deque<deque<deque<double> > > > createDeque4D(int dim1, int dim2, int dim3, int dim4, double val) {
	deque<deque<deque<deque<double> > > > deq(dim1,deque<deque<deque<double> > >(dim2,deque<deque<double> >(dim3,deque<double>(dim4,val))));
	return deq;
}

deque<deque<deque<deque<int> > > > createDeque4D(int dim1, int dim2, int dim3, int dim4, int val) {
	deque<deque<deque<deque<int> > > > deq(dim1,deque<deque<deque<int> > >(dim2,deque<deque<int> >(dim3,deque<int>(dim4,val))));
	return deq;
}

//returns a 2D vector with 1st col='value' & 2nd col='frequency'
vector<vector<float> > frequency(vector<float> vec) {
	std::map<float,int> freq;
	for(unsigned int i=0; i<vec.size(); i++) {
		if(freq.find(vec.at(i))==freq.end())
			freq[vec.at(i)] = 1;
		else
			freq[vec.at(i)]++;
	}
	std::map<float,int>::iterator it;
	vector<float> vals(2,0);
	vector<vector<float> > freqVec;
	for(it=freq.begin(); it!=freq.end(); it++) {
		vals.at(0) = it->first;
		vals.at(1) = (float)it->second;
		freqVec.push_back(vals);
	}
	return freqVec;
}

float majority(vector<float> vec) {
	vector<vector<float> > freq = frequency(vec);
	float max=0.0;
	int index=0;
	for(auto i=0; i<freq.size(); i++) {
		float num = freq.at(i).at(1);
		if(num>max) {
			max = num;
			index = i;
		}
	}
	return freq.at(index).at(0);
}

namespace Func {

//! adds specified digit to front of num
//! default [length]=1 returns just the num
String addDigitsForLabel(int num, String digit, int length) {
	String label = toString(num);
	length -= label.length();
	for(int i=0; i<length; i++) {
		label = digit+label;
	}
	return label;
}

vector<vector<vector<vector<int> > > > createVector4D(int dim1, int dim2, int dim3, int dim4, int val) {
	vector<vector<vector<vector<int> > > > vec(dim1,vector<vector<vector<int> > >(dim2,vector<vector<int> >(dim3,vector<int>(dim4,val))));
	return vec;
}

//! returns every string that is between beginDelimit and endDelimit
void getSubstr(String str, char beginDelimit, char endDelimit, vector<String> &vec) {
	if(vec.size()>0) vec.clear();

	String temp;
	int pos1=-1;
	int pos2=-1;
	for(unsigned int i=0; i<str.length(); i++) {
		if(str.at(i)==beginDelimit) pos1=i;
		if(str.at(i)==endDelimit) pos2=i;
		if(pos1>=0 && pos2>=0) {
			pos1++;
			temp = str.substr(pos1,pos2-pos1);
			vec.push_back(temp);
			pos1=-1;
			pos2=-1;
		}
	}
}

} //end namespace

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
	for(int i=img1.cols-1;i>=0;i--)
	{
		if(countNonZero(img1.col(i))>0)
		{
			roiWidth = i+1;
			break;
		}
	}
	for(int j=img1.rows-1;j>=0;j--)
	{
		if(countNonZero(img1.row(j))>0)
		{
			roiHeight = j+1;
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

//! rotates image counter-clockwise
Mat Functions::rotateImage(const Mat& source, double angle)
{
	Point2f src_center(source.cols/2.0F, source.rows/2.0F);
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	Mat dst;
	warpAffine(source, dst, rot_mat, source.size());
	return dst;
}

//! shift image in a xy-axis direction
//! +y shifts up, -y shift down, +x shifts right, -x shift left
Mat Functions::shiftImage(Mat input, int x, int y, int keepBkGnd) {
	Mat result = Mat::zeros(input.size(),input.type());
	Mat temp;
	if(keepBkGnd==1 && input.type()==CV_8U) {
		vector<float> freq;
		int val = input.at<uchar>(0,0);
		freq.push_back((float)val);
		val = input.at<uchar>(0,input.rows-1);
		freq.push_back((float)val);
		val = input.at<uchar>(input.cols-1,0);
		freq.push_back((float)val);
		val = input.at<uchar>(input.cols-1,input.rows-1);
		freq.push_back((float)val);
		float most = majority(freq);
		result = (int)most;
	}

	if(x==0 && y==0) return input;

	//shift down
	if(y<0 && x==0) {
		int dY = abs(y);
		temp = input(Rect(0,0,input.cols,input.rows-dY));
		temp.copyTo(result(Rect(0,dY,temp.cols,temp.rows)));
	}
	//shift up
	else if(y>0 && x==0) {
		int dY = abs(y);
		temp = input(Rect(0,dY,input.cols,input.rows-dY));
		temp.copyTo(result(Rect(0,0,temp.cols,temp.rows)));
	}
	//shift left
	else if(x<0 && y==0) {
		int dX = abs(x);
		temp = input(Rect(dX,0,input.cols-dX,input.rows));
		temp.copyTo(result(Rect(0,0,temp.cols,temp.rows)));
	}
	//shift right
	else if(x>0 && y==0) {
		int dX = abs(x);
		temp = input(Rect(0,0,input.cols-dX,input.rows));
		temp.copyTo(result(Rect(dX,0,temp.cols,temp.rows)));
	}
	//shift upright
	else if(y>0 && x>0) {
		int dX = abs(x);
		int dY = abs(y);
		temp = input(Rect(0,dY,input.cols-dX,input.rows-dY));
		temp.copyTo(result(Rect(dX,0,temp.cols,temp.rows)));
	}
	//shift downleft
	else if(y<0 && x<0) {
		int dX = abs(x);
		int dY = abs(y);
		temp = input(Rect(dX,0,input.cols-dX,input.rows-dY));
		temp.copyTo(result(Rect(0,dY,temp.cols,temp.rows)));
	}
	//shift upleft
	else if(y>0 && x<0) {
		int dX = abs(x);
		int dY = abs(y);
		temp = input(Rect(dX,dY,input.cols-dX,input.rows-dY));
		temp.copyTo(result(Rect(0,0,temp.cols,temp.rows)));
	}
	//shift downright
	else if(y<0 && x>0) {
		int dX = abs(x);
		int dY = abs(y);
		temp = input(Rect(0,0,input.cols-dX,input.rows-dY));
		temp.copyTo(result(Rect(dX,dY,temp.cols,temp.rows)));
	}
	return result;
}

Mat Functions::fixBinaryImage(Mat input) {
	Mat output = input.clone();
	for(int i=0; i<output.rows; i++) {
		for(int j=0; j<output.cols; j++) {
			if(output.at<uchar>(i,j)>40)
				output.at<uchar>(i,j) = 255;
			else
				output.at<uchar>(i,j) = 0;
		}
	}
	return output;
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

//return smallest value && position to minIndex
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

int Functions::getMin(Mat &src) {
	int lowest = -1;
	lowest = src.at<uchar>(0,0);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			if(lowest>src.at<uchar>(i,j))
				lowest = src.at<uchar>(i,j);
		}
	}
	return lowest;
}

//returns largest value && position to maxIndex
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

int Functions::getMax(Mat &src) {
	int largest=-1;
	largest = src.at<uchar>(0,0);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			if(largest<src.at<uchar>(i,j))
				largest = src.at<uchar>(i,j);
		}
	}
	return largest;
}

void Functions::removeOutliersFromVec(deque<int> &vec, double thresh) {
	quicksort(vec,0,vec.size()-1);
	double amt = vec.size() * thresh;
	amt = round(amt);
	for(unsigned int i=0; i<amt; i++) {
		vec.pop_back();
		vec.pop_front();
	}
}

void Functions::removeOutliersFromVec(deque<double> &vec, double thresh) {
	quicksort(vec,0,vec.size()-1);
	double amt = vec.size() * thresh;
	amt = round(amt);
	for(unsigned int i=0; i<amt; i++) {
		vec.pop_back();
		vec.pop_front();
	}
}
