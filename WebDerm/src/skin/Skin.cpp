/*
 * Skin.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "Skin.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ImageData/imagedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Cluster/cluster.h"

using namespace ip;

namespace Skin {
/* Fills in holes in the image */
void getSkinUsingThresh(Mat &img, Mat &mask)
{
	mask = Mat::zeros(img.size(),CV_8U);
	int r,g,b;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			b = img.at<Vec3b>(row,col)[0];
			g = img.at<Vec3b>(row,col)[1];
			r = img.at<Vec3b>(row,col)[2];
			if(r>g && g>b) {
				if((b+g)<(2*r) && ((r-g) > 20 || (r-b) > 20) && r>95 && b>55 && b<200 && g>40 && g<200) {
					mask.at<uchar>(row,col) = 255;
				}
			}
		}
	}
	imfill(mask);
}

void getSkinUsingCorrelation(Mat &img, Mat &mask)
{
	mask = mask.zeros(img.rows,img.cols,CV_8U);
	double thresh = 0.038;
	double vec2[3] = {221,183,160}; //skin values for correlation
	int r,g,b;
	double dist=0;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			b = img.at<Vec3b>(row,col)[0];
			g = img.at<Vec3b>(row,col)[1];
			r = img.at<Vec3b>(row,col)[2];
			double vec1[3] = {(double)r,(double)g,(double)b};
			dist = MyMath::correlationDist(vec1,vec2);
			if(dist<thresh) {
				mask.at<uchar>(row,col) = 255;
			}
		}
	}
	imfill(mask);
}

//! returns mask of where skin is
Mat getSkin(Mat &img)
{
	Mat mask;
	double skinCount=(img.rows*img.cols)*0.2; //20%
	getSkinUsingThresh(img,mask);
	if(countNonZero(mask)<skinCount)
	{
		mask.release();
		getSkinUsingCorrelation(img,mask);
	}

	return mask;
}

Mat getSkinUsingKmeans(ImageData &id) {
	vector<double> hueVec;
	for(int i=0; i<id.rows(); i++) {
		for(int j=0; j<id.cols(); j++) {
			double hue = id.pixel(i,j).hsl()[0];
			hueVec.push_back(hue);
		}
	}
	Cluster clst;
	clst.kmeansCluster(hueVec,3);
	int clstNum=0;
	double maxHue = 0;
	for(int i=0; i<clst.getNumOfClusters(); i++) {
		double cntHue = clst.getCenter(i);
		double hue = cntHue - floor(cntHue/180.) * 360;
		if(hue>maxHue && hue<60.0) {
			maxHue = hue;
			clstNum = i;
		}
	}
	//cout << "clstNum: " << clstNum << endl;
	//cout << "maxHue: " << maxHue << endl;
	double minVal = clst.getMin(clstNum);
	double maxVal = clst.getMax(clstNum);
	Mat result = id.image();
	for(int i=0; i<id.rows(); i++) {
		for(int j=0; j<id.cols(); j++) {
			double hue = id.pixel(i,j).hsl()[0];
			double lum = id.pixel(i,j).hsl()[2];
			if(hue>=minVal && hue<=maxVal && hue<60.0 && lum>0.40)
				result.at<Vec3b>(i,j) = Vec3b(0,0,0);
		}
	}
	//imgshow(result);
	return result;
}

} // end namespace
