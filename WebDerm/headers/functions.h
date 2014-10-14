/*
 * functions.h
 *
 *  Created on: Nov 19, 2013
 *      Author: jason
 */
#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/quicksort.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/median.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/mean.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/stddev.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

template< class T, size_t N >
std::size_t length(const T (&)[N] )
{
  return N;
};
#define stringify(name) #name

String toString(int val);
String toString(double val);
String toString(bool val);
double roundDecimal(double num, int places);
String decimal2hex(int num);
void init_2D_Deque(deque< deque<String> > &vec, int cols, int rows, String val="0");
void init_2D_Deque(deque< deque<double> > &vec, int cols, int rows, double val=0);
void init_2D_Deque(deque< deque<int> > &vec, int cols, int rows, int val=0);

void init_3D_Deque(deque< deque< deque<String> > > &vec, int cols, int rows, int dep, String val="0");
void init_3D_Deque(deque< deque< deque<double> > > &vec, int cols, int rows, int dep, double val=0);
void init_3D_Deque(deque< deque< deque<int> > > &vec, int cols, int rows, int dep, int val=0);

class Functions {
public:
	double getDelimitedValuesFromString(String inputString, char delimiter, int occurrence);
	void getDelimitedValuesFromString(String inputString, char delimiter, deque<double> &vec);
	double calcSlope(Point pt, Point origin);
	Mat cropImage(Mat input);
	Mat imageROI(Mat input, Point begin, Point end);
	Mat rotateImage(const Mat& source, double angle);
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
};

#endif
