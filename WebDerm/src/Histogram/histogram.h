
#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class Histogram {
public:
    Mat calcHistogram(Mat src);
	void equalizeHistogram(Mat src, Mat &dst);
	void hist2SpreadSheet(Mat &src, String name);
	void outputHistogram(Mat &src, String name);
	void lightEqualizer(Mat src, Mat &dst);
};

#endif /* HISTOGRAM_H_ */
