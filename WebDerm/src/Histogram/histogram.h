
#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "../global.h"

class Histogram {
public:
    vector<Mat> calcHistogram(Mat src);
    Mat drawHistogram(vector<Mat> histVec);
	void equalizeHistogram(Mat src, Mat &dst);
	void hslHist2SpreadSheet(Mat &src, String name);
	void outputHistogramLuminance(Mat &src, String name);
	void outputHistogramRelativeLuminance(Mat &src, String name);
	void outputHistogramGrayScale(Mat &src, String name);
	void lightEqualizer(Mat src, Mat &dst);
	float compareHistogram(Mat src1, Mat src2);
};

#endif /* HISTOGRAM_H_ */
