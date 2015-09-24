/*
 * functions.h
 *
 *  Created on: Nov 19, 2013
 *      Author: jason
 */
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

template< class T, size_t N >
std::size_t length(const T (&)[N] )
{
  return N;
};
#define stringify(name) #name

String toString(int val);
String toString(unsigned int val);
String toString(double val);
String toString(bool val);
double roundDecimal(double num, int places);
String decimal2hex(int num);
void imfill(Mat &img);
void imgshow(Mat src, int flag=0, String name="");
void getSubstr(String str, char delimiter, deque<String> &vec);
void getSubstr(String str, char delimiter, vector<String> &vec);
String getFileName(String filename,String end="");
String getFolderName(String path);

void init_2D_Deque(deque< deque<String> > &vec, int rows, int cols, String val="0");
void init_2D_Deque(deque< deque<double> > &vec, int rows, int cols, double val=0);
void init_2D_Deque(deque< deque<int> > &vec, int rows, int cols, int val=0);

void init_3D_Deque(deque< deque< deque<String> > > &vec, int rows, int cols, int dep, String val="0");
void init_3D_Deque(deque< deque< deque<double> > > &vec, int rows, int cols, int dep, double val=0);
void init_3D_Deque(deque< deque< deque<int> > > &vec, int rows, int cols, int dep, int val=0);

deque<deque<deque<deque<double> > > > createDeque4D(int dim1, int dim2, int dim3, int dim4, double val=0);
deque<deque<deque<deque<int> > > > createDeque4D(int dim1, int dim2, int dim3, int dim4, int val=0);

map<float,int> frequency(vector<float> vec);
map<int,int> frequency(vector<int> vec);
float majority(vector<float> vec);
int majority(vector<int> vec);

namespace Func {
	String addDigitsForLabel(int num, String digit, int length=1);
	vector<vector<vector<vector<int> > > > createVector4D(int dim1, int dim2, int dim3, int dim4, int val);
	void getSubstr(String str, char beginDelimit, char endDelimit, vector<String> &vec);
	int largest(Mat input, int rank=1);
}

class Functions {
public:
	double getDelimitedValuesFromString(String inputString, char delimiter, int occurrence);
	void getDelimitedValuesFromString(String inputString, char delimiter, deque<double> &vec);
	double calcSlope(Point pt, Point origin);
	Mat cropImage(Mat input);
	Mat scaleDownImage(Mat input, Size size);
	Mat imageROI(Mat input, Point begin, Point end);
	Mat rotateImage(const Mat& source, double angle);
	Mat shiftImage(Mat input, int x, int y, int keepBkGnd=0);
	Mat fixBinaryImage(Mat input);
	Mat fillEdges2(Mat img);
	Mat fillEdges3(Mat img);
	Mat kMeansClustering(Mat &src);

	int countGreaterEqual(double argNum, ...);
	int countGreater(double argNum, ...);
	int countLesser(double argNum, ...);
	int countLesserEqual(double argNum, ...);
	int countEqual(double argNum, ...);
	int countEqual(String argNum, ...);
	int countEqual(int argNum, ...);

	double getMin(deque<double> &vec, int &minIndex);
	double getMax(deque<double> &vec, int &maxIndex);
	int getMin(Mat &src);
	int getMax(Mat &src);

	void removeOutliersFromVec(deque<int> &vec, double thresh);
	void removeOutliersFromVec(deque<double> &vec, double thresh);
};

#endif
