/*
 * shapecolor.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "shapecolor.h"

void ShapeColor::setDebugLevel(int level) {
	this->debugLevel = level;
}

//use in conjunction with setDebugLevel;
void ShapeColor::setDebugColRow(int col, int row) {
	this->dbgCol = col;
	this->dbgRow = row;
}
Mat ShapeColor::getShapeUsingColor(Mat src) {
	Hsl hsl;
	Functions fn;
	Mat map(src.size(),CV_8U,Scalar(0));
	int _row=0; int _col=0, maxRow=src.rows, maxCol=src.cols;
	int localScanSize = 20;
	int r,g,b;
	vector<double> HSL, LAB_0, HSL_45, HSL_90;
	double gradientDiff0, gradientDiff45, gradientDiff90;
	vector<double> HslEntry;
	const double unitThresh[3] = {6.0,5.0,4.0};
	const double enterThresh=2.0;
	const double exitCumulativeThresh = 1.0;
	const double offset = 2;
	bool enterFlag=false, upTheMtn=false, downTheMtn=false;
	if(this->debugLevel>0) {
		enterFlag = this->enterFlag;
		localScanSize = this->test_localScanSize;
		_row = this->test_row;
		_col = this->test_col;
		maxRow = this->test_row+1;
		maxCol = this->test_col+1;
	}
	for(int row = _row; row<maxRow; row++) {
		for(int col = _col; col<maxCol; col++) {
			r = src.at<Vec3b>(row,col)[2];
			g = src.at<Vec3b>(row,col)[1];
			b = src.at<Vec3b>(row,col)[0];
			HSL = hsl.rgb2hsl(r,g,b);
			HSL[0] = (HSL[0] - floor(HSL[0]/180.) * 360.);
			HSL[1] = round(HSL[1] * 100.0);
			HSL[2] = round(HSL[2] * 100.0);
			double maxDiff0=0, maxDiff45=0, maxDiff90=0;
			Point maxPt0=Point(col,row);
			Point maxPt45=Point(col,row);
			Point maxPt90=Point(col,row);
			int j=col-1;
			int x = j;
			int endY = row-localScanSize;
			for(int i=row-1; i>=endY; i--) {
				if(x<0 && j<0 && i<0) break;
				if(x>=0) {
					r = src.at<Vec3b>(row,x)[2];
					g = src.at<Vec3b>(row,x)[1];
					b = src.at<Vec3b>(row,x)[0];
					LAB_0 = hsl.rgb2hsl(r,g,b);
					LAB_0[0] = (LAB_0[0] - floor(LAB_0[0]/180.) * 360.);
					LAB_0[1] = round(LAB_0[1] * 100.0);
					LAB_0[2] = round(LAB_0[2] * 100.0);
					double hueDiff = (HSL[0]-LAB_0[0])/unitThresh[0];
					double satDiff = (HSL[1]-LAB_0[1])/unitThresh[1];
					double lumDiff = (HSL[2]-LAB_0[2])/unitThresh[2];
					hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
					satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
					lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
					gradientDiff0 = hueDiff+satDiff+lumDiff;
					if(abs(gradientDiff0)>=abs(maxDiff0)) {
						maxDiff0 = gradientDiff0;
						maxPt0 = Point(x,row);
					}
				}
				--x;
				if(j>=0 && i>=0) {
					r = src.at<Vec3b>(i,j)[2];
					g = src.at<Vec3b>(i,j)[1];
					b = src.at<Vec3b>(i,j)[0];
					HSL_45 = hsl.rgb2hsl(r,g,b);
					HSL_45[0] = (HSL_45[0] - floor(HSL_45[0]/180.) * 360.);
					HSL_45[1] = round(HSL_45[1] * 100.0);
					HSL_45[2] = round(HSL_45[2] * 100.0);
					double hueDiff = (HSL[0]-HSL_45[0])/unitThresh[0];
					double satDiff = (HSL[1]-HSL_45[1])/unitThresh[1];
					double lumDiff = (HSL[2]-HSL_45[2])/unitThresh[2];
					hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
					satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
					lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
					gradientDiff45 = hueDiff+satDiff+lumDiff;
					if(abs(gradientDiff45)>=abs(maxDiff45)) {
						maxDiff45 = gradientDiff45;
						maxPt45 = Point(j,i);
					}
				}
				--j;
				if(i>=0) {
					r = src.at<Vec3b>(i,col)[2];
					g = src.at<Vec3b>(i,col)[1];
					b = src.at<Vec3b>(i,col)[0];
					HSL_90 = hsl.rgb2hsl(r,g,b);
					HSL_90[0] = (HSL_90[0] - floor(HSL_90[0]/180.) * 360.);
					HSL_90[1] = round(HSL_90[1] * 100.0);
					HSL_90[2] = round(HSL_90[2] * 100.0);
					double hueDiff = (HSL[0]-HSL_90[0])/unitThresh[0];
					double satDiff = (HSL[1]-HSL_90[1])/unitThresh[1];
					double lumDiff = (HSL[2]-HSL_90[2])/unitThresh[2];
					hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
					satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
					lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
					gradientDiff90 = hueDiff+satDiff+lumDiff;
					if(abs(gradientDiff90)>=abs(maxDiff90)) {
						maxDiff90 = gradientDiff90;
						maxPt90 = Point(col,i);
					}
				}
				/*if(this->debugLevel) {
					printf("MaxDiff0: %f, MaxDiff45: %f, MaxDiff90: %f\n",maxDiff0,maxDiff45,maxDiff90);
					printf("MaxPt0: %d,%d, MaxPt45: %d,%d, MaxPt90: %d,%d\n",maxPt0.x,maxPt0.y,maxPt45.x,maxPt45.y,maxPt90.x,maxPt90.y);
				}*/
			}// end for(int i)
			if(enterFlag==false) {
				if(fn.countGreaterEqual(4,maxDiff0,maxDiff45,maxDiff90,enterThresh)>=2) {
					upTheMtn = true;
				}
				if(fn.countLesserEqual(4,maxDiff0,maxDiff45,maxDiff90,-enterThresh)>=2) {
					downTheMtn = true;
				}
				if(upTheMtn || downTheMtn) {
					enterFlag = true;
					Vec3b RGB = src.at<Vec3b>(row,col-offset);
					HslEntry = hsl.rgb2hsl(RGB[2],RGB[1],RGB[0]);
					HslEntry[0] = (int)(HslEntry[0] - floor(HslEntry[0]/180.) * 360.);
					HslEntry[1] *= 100.0;
					HslEntry[2] *= 100.0;
					map.at<uchar>(row,col-offset) = 255;
				}
			}
			else if(enterFlag==true) {
				if(downTheMtn) {
					if(fn.countGreaterEqual(4,maxDiff0,maxDiff45,maxDiff90,enterThresh)>=2) {
						enterFlag = false;
						upTheMtn = downTheMtn = false;
						HslEntry.clear();
						map.at<uchar>(row,col+offset) = 255;
					}
					else {
						double hueDiff = (HSL[0]-HslEntry[0])/unitThresh[0];
						double satDiff = (HSL[1]-HslEntry[1])/unitThresh[1];
						double lumDiff = (HSL[2]-HslEntry[2])/unitThresh[2];
						hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
						satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
						lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
						gradientDiff0 = hueDiff+satDiff+lumDiff;
						if(gradientDiff0>=-exitCumulativeThresh) {
							enterFlag = false;
							upTheMtn = downTheMtn = false;
							HslEntry.clear();
							map.at<uchar>(row,col+offset) = 255;
						}
					}
				}
				else if(upTheMtn) {
					if(fn.countLesserEqual(4,maxDiff0,maxDiff45,maxDiff90,-enterThresh)>=2) {
						enterFlag = false;
						upTheMtn = downTheMtn = false;
						HslEntry.clear();
						map.at<uchar>(row,col+2) = 255;
					}
					else {
						double hueDiff = (HSL[0]-HslEntry[0])/unitThresh[0];
						double satDiff = (HSL[1]-HslEntry[1])/unitThresh[1];
						double lumDiff = (HSL[2]-HslEntry[2])/unitThresh[2];
						hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
						satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
						lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
						gradientDiff0 = hueDiff+satDiff+lumDiff;
						if(gradientDiff0<=exitCumulativeThresh) {
							enterFlag = false;
							upTheMtn = downTheMtn = false;
							HslEntry.clear();
							map.at<uchar>(row,col+offset) = 255;
						}
					}
				}//end if(upTheMtn)
			}//end if(enterFlag==true)
		} //end for(col)
	}//end for(row)
	return map;
}

Mat ShapeColor::test_getShapeUsingColor(Mat src, int col, int row, int localScanSize, bool enterFlag) {
	this->test_row = row;
	this->test_col = col;
	this->test_localScanSize = localScanSize;
	this->enterFlag = enterFlag;
	//return this->getShapeUsingColor(src);
	return Mat();
}

Mat ShapeColor::getShapeUsingColor2(Mat src, Mat noise, double shift) {
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
	Mat hc = this->epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	double maxRange=0;
	this->maxLocalRanges(Lvec,avec,bvec,hc,noise,maxRange,shift);

	vector<float> LAB_0(3,0);
	vector<float> LabEntry(3,0);
	double HC, HC_0, distFromEntry=0;
	Point LabEntryPt;
	Mat map(Lvec.size(),CV_8U,Scalar(0));
	int _row=0; int _col=0, maxRow=Lvec.rows, maxCol=Lvec.cols;
	int localScanSize = 20;
	const double enterThresh = maxRange;
	const double exitCumulativeThresh = 0.7*enterThresh;
	int enterFlag=0, upDownTheMtn=0;
	if(this->debugLevel>0) {
		enterFlag = this->enterFlag;
		localScanSize = this->test_localScanSize;
		_row = this->test_row;
		_col = this->test_col;
		maxRow = this->test_row+1;
		maxCol = this->test_col+1;
	}
	for(int row= _row; row<maxRow; row++) {
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
				if(col==84 && row==104) {
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

//! Expressive Power of Hue
double ShapeColor::epoh(double sat, double lum) {
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
Mat ShapeColor::epohTheHue(Mat hMat, Mat sMat, Mat lMat) {
	Mat hc(hMat.size(),CV_32F, Scalar(0));
	for(int i=0; i<hMat.rows; i++) {
		for(int j=0; j<hMat.cols; j++) {
			double hue = hMat.at<float>(i,j);
			double sat = sMat.at<float>(i,j);
			double lum = lMat.at<float>(i,j);
			hue *= this->epoh(sat,lum);
			hc.at<float>(i,j) = roundDecimal(hue,2);
		}
	}
	return hc;
}

//using delta Emax
void ShapeColor::maxLocalRanges(Mat mat1, Mat mat2, Mat mat3, Mat hc, Mat noiseMap, double &maxRange, double shift) {
	Cie cie;
	vector<double> maxRangeVec;
	vector<float> LAB1(3,0);
	vector<float> LAB2(3,0);
	Mat altitude(mat1.size(),CV_32F,Scalar(0));
	Mat distMat(mat1.size(),CV_32F,Scalar(0));
	double HC1=0, HC2=0;
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
		for(int col=0; col<altitude.cols; col++) {
			vector<float> distVec;
			vector<Point> minMaxPtVec;
			for(int x = col; x<col+20; x++) {
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
	int kneePt = kc.kneeCurvePoint(maxRangeVec);
	kneePt *= shift;
	maxRange = maxRangeVec.at(kneePt);
	//writeSeq2File(freq,"freq");
	//writeSeq2File(distMat,"float","distMat");
	//writeSeq2File(altitude,"float","altitude");
	//writeSeq2File(maxRangeVec,"maxrangevec");
}

Mat ShapeColor::removeRunningLines(Mat input, Size size) {
	Mat dst = input.clone();
	const int thresh=11;
	const int thickness = 5;
	Size winSize = size;
	int countStreak=0;
	Mat window;
	Size prevSize;
	Point startPt(0,0), endPt(0,0);
	int rowDecr=0, rowIncr=0;
	if(this->debugLevel==1)
		namedWindow("img",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	for(int row=0; row<=(dst.rows-winSize.height); row++) {
		startPt.y = row;
		for(int col=0; col<=(dst.cols-winSize.width); col++) {
			window = dst(Rect(col,startPt.y,winSize.width,winSize.height));
			if(countNonZero(window)==window.total()) {
				/****DEBUG OUTPUT INFO ****/
				if(this->debugLevel==1) {
					Mat test = input.clone();
					rectangle(test,Rect(col,startPt.y,winSize.width,winSize.height),Scalar(150));
					printf("****BEFORE0****\n");
					printf("Point(%d,%d)\n",col,row);
					printf("winSize: %dx%d\n",winSize.width, winSize.height);
					printf("Begin:(%d,%d)\n",col,row-rowDecr);
					printf("Streak: %d\n",countStreak);
					imshow("img",test);
					waitKey(0);
				} //end debug print

				//expand upwards
				if(row>0) {
					while(1) {
						try {
							rowDecr++;
							winSize.height = size.height+rowDecr+rowIncr;
							if(row-rowDecr>=0) {
								window = dst(Rect(col,row-rowDecr,winSize.width,winSize.height));

								/****DEBUG OUTPUT INFO ****/
								if(this->debugLevel==1) {
									Mat test = input.clone();
									rectangle(test,Rect(col,row-rowDecr,winSize.width,winSize.height),Scalar(150));
									printf("****BEFORE1****\n");
									printf("Point(%d,%d)\n",col,row);
									printf("winSize: %dx%d\n",winSize.width, winSize.height);
									printf("Begin:(%d,%d)\n",col,row-rowDecr);
									printf("Streak: %d\n",countStreak);
									imshow("img",test);
									waitKey(0);
								} //end debug output
							}

							if(countNonZero(window)!=window.total() || (row-rowDecr)<0) {
								rowDecr--;
								winSize.height = size.height+rowDecr+rowIncr;
								window = dst(Rect(col,row-rowDecr,winSize.width,winSize.height));

								if(prevSize!=window.size()) {
									if(countStreak>thresh && window.rows<=thickness) {
										for(int i=startPt.y; i<startPt.y+window.rows; i++) {
											for(int j=startPt.x; j<col+window.cols; j++) {
												if(dst.type()==CV_8U)
													dst.at<uchar>(i,j) = 0;
												/****DEBUG OUTPUT INFO ****/
												if(this->debugLevel==1) {
													imshow("dst",dst);
													waitKey(0);
												} // end debug output
											}
										}
									}
									countStreak=0;
								}
								if(countStreak==0)
									startPt = Point(col,row-rowDecr);

								/****DEBUG OUTPUT INFO ****/
								if(this->debugLevel==1) {
									Mat test = input.clone();
									rectangle(test,Rect(col,row-rowDecr,winSize.width,winSize.height),Scalar(150));
									printf("****AFTER1****\n");
									printf("Point(%d,%d)\n",col,row);
									printf("winSize: %dx%d\n",winSize.width, winSize.height);
									printf("Begin:(%d,%d)\n",col,row-rowDecr);
									printf("Streak: %d\n",countStreak);
									imshow("img",test);
									waitKey(0);
								} //end debug output

								break;
							}
						} catch (cv::Exception &e) {
							printf("Catch #1\n");
							printf("%d,%d\n",col,row);
							printf("Row+RowIncr: %d\n",row+rowIncr);
							exit(1);
						}
					} // end while
				} // end if(row>0)

				//expand downwards
				if(row<input.rows-1) {
					while(1) {
						try {
							rowIncr++;
							winSize.height = size.height+rowDecr+rowIncr;
							if(row+rowIncr<dst.rows) {
								window = dst(Rect(col,startPt.y,winSize.width,winSize.height));

								/****DEBUG OUTPUT INFO ****/
								if(this->debugLevel==1) {
									Mat test = input.clone();
									rectangle(test,Rect(col,startPt.y,winSize.width,winSize.height),Scalar(150));
									printf("****BEFORE2****\n");
									printf("Point(%d,%d)\n",col,row);
									printf("winSize: %dx%d\n",winSize.width, winSize.height);
									printf("Begin:(%d,%d)\n",col,row-rowDecr);
									printf("End:(%d,%d)\n",col,row+rowIncr);
									printf("Streak: %d\n",countStreak);
									imshow("img",test);
									waitKey(0);
								} //end debug output
							}

							if(countNonZero(window)!=window.total() || row+rowIncr>=dst.rows) {
								rowIncr--;
								winSize.height = size.height+rowDecr+rowIncr;
								endPt = Point(col,row+rowIncr);
								window = dst(Rect(col,startPt.y,winSize.width,winSize.height));

								/****DEBUG OUTPUT INFO ****/
								if(this->debugLevel==1) {
									Mat test = input.clone();
									rectangle(test,Rect(col,startPt.y,winSize.width,winSize.height),Scalar(150));
									printf("****AFTER2****\n");
									printf("Point(%d,%d)\n",col,row);
									printf("winSize: %dx%d\n",winSize.width, winSize.height);
									printf("Begin:(%d,%d)\n",col,row-rowDecr);
									printf("End:(%d,%d)\n",col,row+rowIncr);
									printf("Streak: %d\n",countStreak);
									imshow("img",test);
									waitKey(0);
								} // end debug output
								break;
							}
						} catch (cv::Exception &e) {
							printf("Catch #2\n");
							printf("%d,%d\n",col,row);
							printf("Row+RowIncr: %d\n",row+rowIncr);
							printf("StartPt:(%d,%d)\n",col,startPt.y);
							printf("EndPt(%d,%d)\n",endPt.x,endPt.y);
							printf("WinSize(%d,%d)\n",winSize.width,winSize.height);
							exit(1);
						}
					} // end while
				} // end if
				if(prevSize==window.size()) countStreak++;
				if(prevSize!=window.size() || col==(dst.cols-winSize.width)) {
					if(countStreak>=thresh && window.rows<=thickness) {
						for(int i=startPt.y; i<startPt.y+window.rows; i++) {
							for(int j=startPt.x; j<col+window.cols; j++) {
								if(dst.type()==CV_8U)
									dst.at<uchar>(i,j) = 0;
								/****DEBUG OUTPUT INFO ****/
								if(this->debugLevel==1) {
									imshow("dst",dst);
									waitKey(0);
								} // end debug output
							}
						}
					}
					if(countStreak>0) {
						winSize = size;
						rowDecr=0;
						rowIncr=0;
						startPt = Point(col,row);
					}
					countStreak=0;
				}
			} // end if
			else {
				if(countStreak>=thresh && window.rows<=thickness) {
					for(int i=startPt.y; i<startPt.y+window.rows; i++) {
						for(int j=startPt.x; j<col+window.cols; j++) {
							if(dst.type()==CV_8U)
								dst.at<uchar>(i,j) = 0;
							if(dst.type()==CV_8UC3)
								dst.at<Vec3b>(i,j) = Vec3b(0,0,0);
							/****DEBUG OUTPUT INFO ****/
							if(this->debugLevel==1) {
								imshow("dst",dst);
								waitKey(0);
							} // end debug output

						}
					}
				}
				winSize=size;
				rowDecr=0;
				rowIncr=0;
				startPt = Point(col,row);
				countStreak=0;
			}

			/****DEBUG OUTPUT INFO ****/
			if(this->debugLevel==1) {
				printf("****FINAL****\n");
				printf("Point(%d,%d)\n",col,row);
				printf("PrevSize: %dx%d, WindowSize: %dx%d\n",prevSize.width,prevSize.height,window.cols,window.rows);
				printf("Streak: %d\n",countStreak);
			} // end debug output
			prevSize = window.size();
		} // end col
	} // end row
	return dst;
}


Mat ShapeColor::filterKneePt(Mat src, double thresh, double shift) {
	KneeCurve kc;
	vector<double> yVec1;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double lum = src.at<uchar>(i,j);
			if(lum>thresh)
				yVec1.push_back(lum);
		}
	}
	int bestIdx = kc.kneeCurvePoint(yVec1);
	float percent = (float)bestIdx/yVec1.size();
	if(percent<0.05 || percent>0.90)
		bestIdx = 0.75 * yVec1.size();
	double threshFilter = yVec1.at(bestIdx);
	Mat dst =src.clone();
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			double lum = src.at<uchar>(i,j);
			if(lum<threshFilter)
				dst.at<uchar>(i,j) = 0;
		}
	}
	return dst;
}

//input is grayscale image
Mat ShapeColor::applyDiscreteShade(Mat input) {
	Mat dst(input.size(),CV_8U,Scalar(0));
	Shades sh;
	vector<String> discreteShade = {"Dark2","Dark3","High","Low","Light","White"};
	vector<int> discreteVals = {10,51,102,153,204,255};
	for(int row=0; row<input.rows; row++) {
		for(int col=0; col<input.cols; col++) {
			int val = input.at<uchar>(row,col);
			if(val>0) {
				String shade = sh.calcShade2(val);
				for(unsigned int i=0; i<discreteShade.size(); i++) {
					if(shade==discreteShade.at(i)) {
						dst.at<uchar>(row,col) = discreteVals.at(i);
						break;
					}
				}
			}
		}
	}
	return dst;
}
