/*
 * histogram.h
 *
 *  Created on: Jul 31, 2014
 *      Author: jason
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class Histogram {
public:
	void calcHistogram(Mat src);
	void equalizeHistogram(Mat src, Mat &dst);
	void hist2SpreadSheet(Mat &src);
};

#endif /* HISTOGRAM_H_ */
