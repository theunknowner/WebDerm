/*
 * getshapeusingcolor.cpp
 *
 *  Created on: May 6, 2015
 *      Author: jason
 */

#include "shapecolor.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/KneeCurve/kneecurve.h"
#include "/home/jason/git/WebDerm/WebDerm/src/CIE/cie.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/xyz.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/cielab.h"

//! Expressive Power of Hue
double epoh(double &sat, double &lum) {
	double result;
	if(lum<=50) {
		result = (sat/100.) * (lum/50.);
	}
	else {
		result = (sat/100.) * (100-lum)/50;
	}
	return result;
}

//using Minkowski distance
Mat epohTheHue(Mat &hMat, Mat &sMat, Mat &lMat) {
	Mat hc(hMat.size(),CV_32F, Scalar(0));
	for(int i=0; i<hMat.rows; i++) {
		for(int j=0; j<hMat.cols; j++) {
			double hue = hMat.at<float>(i,j);
			double sat = sMat.at<float>(i,j);
			double lum = lMat.at<float>(i,j);
			hue *= epoh(sat,lum);
			hc.at<float>(i,j) = roundDecimal(hue,2);
		}
	}
	return hc;
}

//using delta Emax
void maxLocalRanges(Mat mat1, Mat mat2, Mat mat3, Mat hc, Mat noiseMap, double &maxRange, double shift) {
	Cie cie;
	vector<double> maxRangeVec;
	vector<float> LAB1(3,0);
	vector<float> LAB2(3,0);
	Mat altitude(mat1.size(),CV_32F,Scalar(0));
	Mat distMat(mat1.size(),CV_32F,Scalar(0));
	double HC1=0, HC2=0;
	int scanSize = 20;
	int moveSize = 10;
	for(int row=0; row<mat1.rows; row++) {
		float sum=0;
		for(int col=0; col<mat1.cols; col++) {
			if(noiseMap.at<uchar>(row,col)>0) {
				LAB2[0] = mat1.at<float>(row,col);
				LAB2[1] = mat2.at<float>(row,col);
				LAB2[2] = mat3.at<float>(row,col);
				HC2 = hc.at<float>(row,col);
				if(col==0) {
					LAB1[0] = mat1.at<float>(row,col);
					LAB1[1] = mat2.at<float>(row,col);
					LAB1[2] = mat3.at<float>(row,col);
					HC1 = hc.at<float>(row,col);
				}
				else {
					LAB1[0] = mat1.at<float>(row,col-1);
					LAB1[1] = mat2.at<float>(row,col-1);
					LAB1[2] = mat3.at<float>(row,col-1);
					HC1 = hc.at<float>(row,col-1);
				}
				double dist = cie.deltaE76(LAB1,LAB2);
				double direction = HC2 - HC1;
				distMat.at<float>(row,col) = dist;
				if(direction<0)
					sum += (-dist);
				else
					sum += dist;

				altitude.at<float>(row,col) = sum;
			}
		}
	}
	double minMaxDist[2];
	Point minMaxPt[2];
	for(int row=0; row<altitude.rows; row++) {
		for(int col=0; col<altitude.cols; col+=moveSize) {
			vector<float> distVec;
			vector<Point> minMaxPtVec;
			for(int x = col; x<col+scanSize; x++) {
				if(x<0 || x>=altitude.cols) break;
				if(x>=0 && x<altitude.cols) {
					float dist = altitude.at<float>(row,x);
					distVec.push_back(dist);
					minMaxPtVec.push_back(Point(x,row));
				}
			}
			minMaxLoc(distVec,&minMaxDist[0],&minMaxDist[1],&minMaxPt[0],&minMaxPt[1]);
			double maxRange = roundDecimal(minMaxDist[1] - minMaxDist[0],4);
			maxRangeVec.push_back(maxRange);
		}
	}
	sort(maxRangeVec.begin(),maxRangeVec.end());
	//maxRangeVec.erase(remove(maxRangeVec.begin(),maxRangeVec.end(),0),maxRangeVec.end());
	KneeCurve kc;
	kc.removeOutliers(maxRangeVec,0.025);
	int kneePt = kc.kneeCurvePoint(maxRangeVec);
	float percent = roundDecimal((float)kneePt/maxRangeVec.size(),2);
	printf("KneePt: %d\n",kneePt);
	printf("Percent: %f\n",percent);
	if(percent<=0.05)
		kneePt = 0.25 * maxRangeVec.size();
	if(percent>=0.8999999)
		kneePt = 0.75 * maxRangeVec.size();
	//kneePt *= shift;
	maxRange = maxRangeVec.at(kneePt);
	printf("KneePt: %d\n",kneePt);
	printf("Thresh: %f\n",maxRange);
	//writeSeq2File(freq,"freq");
	//writeSeq2File(distMat,"float","distMat");
	//writeSeq2File(altitude,"float","altitude");
	//writeSeq2File(maxRangeVec,"maxrangevec");
}

//get shape using color with deltaE
Mat ShapeColor::getShapeUsingColor(Mat src, Mat noise, double shift) {
	Functions fn;
	Xyz xyz;
	CieLab lab;
	Cie cie;
	Hsl hsl;
	vector<float> XYZ, LAB;
	vector<double> HSL;
	Mat Lvec(src.size(),CV_32F,Scalar(0));
	Mat avec(src.size(),CV_32F,Scalar(0));
	Mat bvec(src.size(),CV_32F,Scalar(0));

	Mat hvec(src.size(),CV_32F,Scalar(0));
	Mat svec(src.size(),CV_32F,Scalar(0));
	Mat lvec(src.size(),CV_32F,Scalar(0));
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			Vec3b RGB = src.at<Vec3b>(i,j);
			XYZ = xyz.rgb2xyz(RGB[2],RGB[1],RGB[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			HSL = hsl.rgb2hsl(RGB[2],RGB[1],RGB[0]);
			Lvec.at<float>(i,j) = LAB.at(0);
			avec.at<float>(i,j) = LAB.at(1);
			bvec.at<float>(i,j) = LAB.at(2);

			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	double maxRange=0;
	maxLocalRanges(Lvec,avec,bvec,hc,noise,maxRange,shift);

	vector<float> LAB_0(3,0);
	vector<float> LabEntry(3,0);
	double HC, HC_0, distFromEntry=0;
	Point LabEntryPt;
	Mat map(Lvec.size(),CV_8U,Scalar(0));
	int _row=0; int _col=0, maxRow=Lvec.rows, maxCol=Lvec.cols;
	int localScanSize = 20;
	const double enterThresh = 0.65 * maxRange;
	const double exitCumulativeThresh = 0.7*enterThresh;
	int enterFlag=0, upDownTheMtn=0;

	for(int row=_row; row<maxRow; row++) {
		Point enterExitPt(-1,-1); //reset after every column
		for(int col=_col; col<maxCol; col++) {
			LAB[0] = Lvec.at<float>(row,col);
			LAB[1] = avec.at<float>(row,col);
			LAB[2] = bvec.at<float>(row,col);
			HC = hc.at<float>(row,col);
			double maxDiff0=0, direction=0;
			Point maxPt0=Point(col,row);
			int j=col-1;
			int x = j;
			int y = row-1;
			int endY = row-localScanSize;
			for(int i=y; i>=endY; i--) {
				if(x<0 && j<0 && i<0) break;
				if(x>=0 && x!= enterExitPt.x && noise.at<uchar>(row,x)!=0) {
					LAB_0[0] = Lvec.at<float>(row,x);
					LAB_0[1] = avec.at<float>(row,x);
					LAB_0[2] = bvec.at<float>(row,x);
					HC_0 = hc.at<float>(row,x);
					double dist = cie.deltaE76(LAB,LAB_0);
					if(dist>=maxDiff0) {
						maxDiff0 = dist;
						maxPt0 = Point(x,row);
						direction = HC - HC_0;
					}
				}
				if(x!= enterExitPt.x)
					--x;
			}// end for(int i)
			if(enterFlag==0) {
				if(fn.countGreaterEqual(2,maxDiff0,enterThresh)>=1) {
					if(direction>0)
						upDownTheMtn = 1;
					else if(direction<0)
						upDownTheMtn = -1;

					enterFlag = 1;
					LabEntry[0] = Lvec.at<float>(maxPt0);
					LabEntry[1] = avec.at<float>(maxPt0);
					LabEntry[2] = bvec.at<float>(maxPt0);
					LabEntryPt = maxPt0;
					enterExitPt = Point(col-1,row);
					map.at<uchar>(row,col) = 255;
				}
			}
			else if(enterFlag==1) {
				if(upDownTheMtn==-1) {
					distFromEntry = cie.deltaE76(LAB,LabEntry);
					if(fn.countGreaterEqual(2,(maxDiff0),enterThresh)>=1 && distFromEntry<=exitCumulativeThresh) {
						if(direction>0) {
							enterFlag = 0;
							upDownTheMtn = 0;
							map.at<uchar>(row,col) = 150;
							enterExitPt = Point(col-1,row);
						}
					}
				}
				else if(upDownTheMtn==1) {
					distFromEntry = cie.deltaE76(LAB,LabEntry);
					if(fn.countGreaterEqual(2,(maxDiff0),enterThresh)>=1 && distFromEntry<=exitCumulativeThresh) {
						if(direction<0) {
							enterFlag = 0;
							upDownTheMtn = 0;
							map.at<uchar>(row,col) = 150;
							enterExitPt = Point(col-1,row);
						}
					}
				}//end if(upTheMtn)
				if(enterFlag)
					map.at<uchar>(row,col) = 255;
			}//end if(enterFlag==true)
			if(col==src.cols-1 && enterFlag==1) {
				LAB[0] = Lvec.at<float>(enterExitPt); //enterExitPt or LabEntryPt
				LAB[1] = avec.at<float>(enterExitPt);
				LAB[2] = bvec.at<float>(enterExitPt);
				HC = hc.at<float>(enterExitPt);
				HC_0 = hc.at<float>(LabEntryPt);
				direction = HC - HC_0;
				upDownTheMtn = 0;
				if(direction>0) upDownTheMtn = 1;
				else if(direction<0) upDownTheMtn = -1;
				double min=1000;
				Point minPt(enterExitPt);
				for(int k=enterExitPt.x+2; k<src.cols; k++) {
					map.at<uchar>(row,k) = 0;
					LAB_0[0] = Lvec.at<float>(row,k);
					LAB_0[1] = avec.at<float>(row,k);
					LAB_0[2] = bvec.at<float>(row,k);
					HC_0 = hc.at<float>(row,k);
					double dist = cie.deltaE76(LAB,LAB_0);
					direction = HC_0 - HC;
					if(upDownTheMtn==1) {
						if(direction<0) {
							if(dist<=min) {
								min = dist;
								minPt = Point(k,row);
							}
						}
					}
					if(upDownTheMtn==-1) {
						if(direction>0) {
							if(dist<=min) {
								min = dist;
								minPt = Point(k,row);
							}
						}
					}
				}
				for(int k=enterExitPt.x+2; k<minPt.x; k++) {
					map.at<uchar>(row,k) = 255;
				}
				enterFlag = 0;
				upDownTheMtn = 0;
				map.at<uchar>(minPt) = 100;
				enterExitPt = Point(minPt.x-1,minPt.y);
			}
			if(this->debugLevel>0)
				if(col==45 && row==42) {
					String mtn = upDownTheMtn==1 ? "Up" : "N/A";
					mtn = upDownTheMtn==-1 ? "Down" : mtn;
					printf("HSL(%f,%f,%f)%f\n",hvec.at<float>(row,col),svec.at<float>(row,col),lvec.at<float>(row,col),HC);
					printf("LAB(%f,%f,%f)\n",Lvec.at<float>(row,col),avec.at<float>(row,col),bvec.at<float>(row,col));
					printf("LabE(%f,%f,%f)\n",LabEntry[0],LabEntry[1],LabEntry[2]);
					printf("HSL0(%f,%f,%f)%f\n",hvec.at<float>(maxPt0),svec.at<float>(maxPt0),lvec.at<float>(maxPt0),HC_0);
					printf("LAB0(%f,%f,%f)\n",Lvec.at<float>(maxPt0),avec.at<float>(maxPt0),bvec.at<float>(maxPt0));
					printf("EnterThresh: %f, MaxDiff0: %f\n",enterThresh, maxDiff0);
					printf("DistFromEntryThresh: %f, DistFromEntry: %f\n",exitCumulativeThresh,distFromEntry);
					printf("MaxPt0: %d,%d\n",maxPt0.x,maxPt0.y);
					printf("LabEntryPt: %d,%d\n",LabEntryPt.x,LabEntryPt.y);
					printf("EnterFlag: %d\n",enterFlag);
					//printf("Hsl_Ranges: [%f,%f,%f]\n",hRange,sRange,lRange);
					//printf("UnitThresh: [%f,%f,%f]\n",unitThresh[0],unitThresh[1],unitThresh[2]);
					printf("Mtn: %s\n",mtn.c_str());
				}
		} //end for(col)
		enterFlag=false;
		upDownTheMtn = 0;
	}//end for(row)
	return map;
}
