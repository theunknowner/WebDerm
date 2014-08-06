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
double roundDecimal(double num, int places);

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
};

#endif
