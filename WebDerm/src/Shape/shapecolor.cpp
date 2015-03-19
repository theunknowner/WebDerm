/*
 * shapecolor.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "shapecolor.h"

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
	if(this->debugMode) {
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
				/*if(this->debugMode) {
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

Mat ShapeColor::getShapeUsingColor2(Mat src, Mat noise) {
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
	this->maxLocalRanges(Lvec,avec,bvec,hc,noise,maxRange);

	vector<float> LAB_0(3,0);
	vector<float> LabEntry(3,0);
	double HC, HC_0, distFromEntry=0;
	Point LabEntryPt;
	Mat map(Lvec.size(),CV_8U,Scalar(0));
	int _row=0; int _col=0, maxRow=Lvec.rows, maxCol=Lvec.cols;
	int localScanSize = 40;
	const double unitRange = 19.1431; //base on TC5
	const double enterThresh = 10.6;
	const double exitCumulativeThresh = 0.7*enterThresh;
	int enterFlag=0, upTheMtn=0, downTheMtn=0;
	if(this->debugMode) {
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
						upTheMtn = 1;
					else if(direction<0)
						downTheMtn = 1;

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
				if(downTheMtn) {
					distFromEntry = cie.deltaE76(LAB,LabEntry);
					if(fn.countGreaterEqual(2,(maxDiff0),enterThresh)>=1 && distFromEntry<=exitCumulativeThresh) {
						if(direction>0) {
							enterFlag = 0;
							upTheMtn = downTheMtn = 0;
							map.at<uchar>(row,col) = 150;
							enterExitPt = Point(col-1,row);
						}
					}
				}
				else if(upTheMtn) {
					distFromEntry = cie.deltaE76(LAB,LabEntry);
					if(fn.countGreaterEqual(2,(maxDiff0),enterThresh)>=1 && distFromEntry<=exitCumulativeThresh) {
						if(direction<0) {
							enterFlag = 0;
							upTheMtn = downTheMtn = 0;
							map.at<uchar>(row,col) = 150;
							enterExitPt = Point(col-1,row);
						}
					}
				}//end if(upTheMtn)
				if(enterFlag)
					map.at<uchar>(row,col) = 255;
			}//end if(enterFlag==true)
			if(col==16 && row==45) {
				String mtn = upTheMtn==1 ? "Up" : "N/A";
				mtn = downTheMtn==1 ? "Down" : mtn;
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
		upTheMtn = downTheMtn = false;
	}//end for(row)
	return map;
}

//set HSL ranges for function getShapeUsingColors() HSL unitThresh
void ShapeColor::setHslVals(deque<double> hueVals, deque<double> satVals, deque<double> lumVals) {
	this->hueVals = hueVals;
	this->satVals = satVals;
	this->lumVals = lumVals;
}

//! Expressive Power of Hue
double ShapeColor::epoh(double sat, double lum) {
	double l,result;
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

void ShapeColor::maxLocalRanges(Mat mat1, Mat mat2, Mat mat3, Mat hc, Mat noiseMap, double &maxRange) {
	Cie cie;
	vector<float> maxRangeVec;
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
			int j=col-1;
			int x = j;
			int y = row-1;
			int endY = row-40;
			for(int i=y; i>=endY; i--) {
				if(x<0) break;
				if(x>=0) {
					float dist = altitude.at<float>(row,x);
					distVec.push_back(dist);
					minMaxPtVec.push_back(Point(x,row));
				}
				--x;
			}// end for(int i)
			minMaxLoc(distVec,&minMaxDist[0],&minMaxDist[1],&minMaxPt[0],&minMaxPt[1]);
			double maxRange = roundDecimal(minMaxDist[1] - minMaxDist[0],4);
			if(roundDecimal(maxRange,1)==12.3) {
				printf("Min: %f, Max:%f\n",minMaxDist[0],minMaxDist[1]);
				printf("MinPt: (%d,%d), MaxPt: (%d,%d)\n",minMaxPtVec[minMaxPt[0].x].x,minMaxPtVec[minMaxPt[0].x].y,minMaxPtVec[minMaxPt[1].x].x,minMaxPtVec[minMaxPt[1].x].y);
			}
			maxRangeVec.push_back(maxRange);
		}
	}
	sort(maxRangeVec.begin(),maxRangeVec.end());
	vector<vector<float> > freq = frequency(maxRangeVec);
	//writeSeq2File(freq,"freq");
	//writeSeq2File(distMat,"float","distMat");
	//writeSeq2File(altitude,"float","altitude");
	writeSeq2File(maxRangeVec,"maxrangevec");
	float sum=0, total=0;
	for(unsigned int i=1; i<freq.size(); i++) {
		float product = freq.at(i).at(0) * freq.at(i).at(1);
		sum += product;
		total += freq.at(i).at(1);
	}
	sum /= total;
	maxRange = sum;
	cout << maxRange<<endl;
}