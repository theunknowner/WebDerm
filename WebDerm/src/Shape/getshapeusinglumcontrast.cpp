/*
 * lum_contrast.cpp
 *
 *  Created on: May 6, 2015
 *      Author: jason
 */

#include "shapecolor.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/kneecurve.h"

//! gets the local max ranges and return the knee as threshold
double local_max_range(Mat &input, Mat &noiseMap, float &shift) {
	int MAX_RANGE=0; // THEE THRESHOLD
	vector<int> maxRangeVec;
	int scanSize=20;
	int lum1=0;
	double minMaxDist[2];
	Point minMaxPt[2];
	/* gets the max local ranges of the luminances */
	for(int row=0; row<input.rows; row++) {
		for(int col=0; col<input.cols; col++) {
			vector<int> distVec;
			for(int j=col; j<col+scanSize; j++) {
				if(noiseMap.at<uchar>(row,j)>0) {
					if(j<0 || j>=input.cols) break;
					if(j>=0 && j<input.cols) {
						lum1 = input.at<uchar>(row,j);
						distVec.push_back(lum1);
					}
				}
			}
			minMaxLoc(distVec,&minMaxDist[0],&minMaxDist[1],&minMaxPt[0],&minMaxPt[1]);
			int _maxRange = minMaxDist[1] - minMaxDist[0];
			maxRangeVec.push_back(_maxRange);
		}
	}
	sort(maxRangeVec.begin(),maxRangeVec.end());
	KneeCurve kc;
	int kneePt = kc.kneeCurvePoint(maxRangeVec);
	kneePt *= shift;
	MAX_RANGE = maxRangeVec.at(kneePt);

	return MAX_RANGE;
}

//! 255=enter, 150=exit
Mat ShapeColor::getShapeUsingLumContrast(Mat input, Mat noiseMap, float shift) {
	const double enterThresh = local_max_range(input,noiseMap,shift);
	const double exitCumulativeThresh = 0.7*enterThresh;
	Functions fn;
	Mat map(input.size(),CV_8U,Scalar(0));
	int lum1=0, lum0=0;
	int localScanSize = 20;
	int enterFlag=0, upDownTheMtn=0;
	for(int row=0; row<input.rows; row++) {
		int lumEntry=0;
		Point lumEntryPt(-1,-1);
		int distFromEntry=0;
		Point enterExitPt(-1,-1); //reset after every column
		for(int col=0; col<input.cols; col++) {
			lum1 = input.at<uchar>(row,col);
			double maxDiff0=0, direction=0;
			Point maxPt0=Point(col,row);
			int j=col-1;
			int x = j;
			int y = row-1;
			int endY = row-localScanSize;
			for(int i=y; i>=endY; i--) {
				if(x<0 && j<0 && i<0) break;
				if(x>=0 && x!= enterExitPt.x && noiseMap.at<uchar>(row,x)!=0) {
					lum0 = input.at<uchar>(row,x);
					int dist = abs(lum1 - lum0);
					if(dist>=maxDiff0) {
						maxDiff0 = dist;
						maxPt0 = Point(x,row);
						direction = lum1 - lum0;
					}
				}
				if(x!= enterExitPt.x)
					--x;
			}// end for(int i)

			/* ENTERING */
			if(enterFlag==0) {
				if(fn.countGreaterEqual(2,maxDiff0,enterThresh)>=1 && direction>0) {
					if(direction>0)
						upDownTheMtn = 1;
					else if(direction<0)
						upDownTheMtn = -1;

					enterFlag = 1;
					lumEntry = input.at<uchar>(maxPt0);
					lumEntryPt = maxPt0;
					enterExitPt = Point(col-1,row);
					map.at<uchar>(row,col) = 255;

					/** DEBUG INFO **/
					if(col==74 && row==65) {
						printf("/** DEBUG INFO **/\n");
						printf("(%d,%d)\n",col,row);
						printf("LumEntry: %d\n",lumEntry);
						printf("maxPt0(%d,%d)\n",maxPt0.x,maxPt0.y);
						printf("enterExitPt(%d,%d)\n",enterExitPt.x,enterExitPt.y);
						printf("/** END DEBUG INFO **/\n");
					}
				}
			}
			/* EXITING */
			else if(enterFlag==1) {
				if(upDownTheMtn==-1) {
					distFromEntry = abs(lum1 - lumEntry);
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
					distFromEntry = abs(lum1 - lumEntry);
					if(fn.countGreaterEqual(2,maxDiff0,enterThresh)>=1 && distFromEntry<=exitCumulativeThresh) {
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
			if(col==input.cols-1 && enterFlag==1) {
				lum1 = input.at<uchar>(enterExitPt); //enterExitPt or LumEntryPt
				lum0 = input.at<uchar>(lumEntryPt);
				direction = lum1 - lum0;
				upDownTheMtn = 0;
				if(direction>0) upDownTheMtn = 1;
				else if(direction<0) upDownTheMtn = -1;
				double min=1000;
				Point minPt(enterExitPt);
				for(int k=enterExitPt.x+2; k<input.cols; k++) {
					map.at<uchar>(row,k) = 0;
					lum0 = input.at<uchar>(row,k);
					int dist = abs(lum1 - lum0);
					direction = lum0 - lum1;
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
				if(col==104 && row==65) {
					String mtn = upDownTheMtn==1 ? "Up" : "N/A";
					mtn = upDownTheMtn==-1 ? "Down" : mtn;
					printf("/** DEBUG INFO - LEVEL %d**/\n",debugLevel);
					printf("Lum1: %d\n",input.at<uchar>(row,col));
					printf("LumEntry: %d\n",lumEntry);
					printf("Lum0: %d\n",input.at<uchar>(maxPt0));
					printf("EnterThresh: %f, MaxDiff0: %f\n",enterThresh, maxDiff0);
					printf("DistFromEntryThresh: %f, DistFromEntry: %d\n",exitCumulativeThresh,distFromEntry);
					printf("MaxPt0(%d,%d)\n",maxPt0.x,maxPt0.y);
					printf("LumEntryPt(%d,%d)\n",lumEntryPt.x,lumEntryPt.y);
					printf("EnterExitPt(%d,%d)\n",enterExitPt.x,enterExitPt.y);
					printf("EnterFlag: %d\n",enterFlag);
					//printf("Hsl_Ranges: [%f,%f,%f]\n",hRange,sRange,lRange);
					//printf("UnitThresh: [%f,%f,%f]\n",unitThresh[0],unitThresh[1],unitThresh[2]);
					printf("Mtn: %s\n",mtn.c_str());
					printf("/** END DEBUG INFO - LEVEL %d**/\n",debugLevel);
				}
		} //end for(col)
		enterFlag=false;
		upDownTheMtn = 0;
	}//end for(row)
	return map;
}
