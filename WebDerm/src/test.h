/*
 * test.h
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "rgb/rgb.h"
#include "hsl/hsl.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "Math/maths.h"
#include "FileData/filedata.h"
#include "Histogram/histogram.h"
#include "Shades/shades.h"
#include "Color/color.h"

class Test {
public:
	vector<vector<String> > test_shadesOfColor(Mat src);
	Mat test_hslAvgOfColor(Mat src);

	double test_average(Mat window);
};

#endif /* TEST_H_ */
