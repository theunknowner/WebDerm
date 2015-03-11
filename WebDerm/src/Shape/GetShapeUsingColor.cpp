/*
 * GetShapeUsingColor.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: jason
 */

#include "shapemorph.h"

Mat ShapeMorph::getShapeUsingColor(Mat src) {
	Hsl hsl;
	Functions fn;
	Mat map(src.size(),CV_8U,Scalar(0));
	int _row=0; int _col=0, maxRow=src.rows, maxCol=src.cols;
	int localScanSize = 20;
	int r,g,b;
	vector<double> HSL, HSL_0, HSL_45, HSL_90;
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
					HSL_0 = hsl.rgb2hsl(r,g,b);
					HSL_0[0] = (HSL_0[0] - floor(HSL_0[0]/180.) * 360.);
					HSL_0[1] = round(HSL_0[1] * 100.0);
					HSL_0[2] = round(HSL_0[2] * 100.0);
					double hueDiff = (HSL[0]-HSL_0[0])/unitThresh[0];
					double satDiff = (HSL[1]-HSL_0[1])/unitThresh[1];
					double lumDiff = (HSL[2]-HSL_0[2])/unitThresh[2];
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

Mat ShapeMorph::test_getShapeUsingColor(Mat src, int col, int row, int localScanSize, bool enterFlag) {
	this->test_row = row;
	this->test_col = col;
	this->test_localScanSize = localScanSize;
	this->enterFlag = enterFlag;
	//return this->getShapeUsingColor(src);
	return Mat();
}

Mat ShapeMorph::getShapeUsingColor2(Mat hMat, Mat sMat, Mat lMat, Mat noise) {
	double HSL[3], HSL_0[3], HSL_45[3], HSL_90[3];
	Functions fn;
	Mat map(hMat.size(),CV_8U,Scalar(0));
	Mat trueEnterExitMap(hMat.size(),CV_8U,Scalar(0));
	int _row=0; int _col=0, maxRow=hMat.rows, maxCol=hMat.cols;
	int localScanSize = 20;
	double gradientDiff0, gradientDiff45, gradientDiff90;
	double gradientDirection0;
	double HslEntry[3];
	double unitThresh[3] = {3.0,12.6,7.2};
	const double unitRanges[3] = {14.0,16.0,11.0};
	const double enterThresh = 2.3;
	const double exitThresh = 1.0;
	const double exitCumulativeThresh = 1.0;
	const double offset = 3;
	bool enterFlag=false, upTheMtn=false, downTheMtn=false;
	if(this->debugMode) {
		enterFlag = this->enterFlag;
		localScanSize = this->test_localScanSize;
		_row = this->test_row;
		_col = this->test_col;
		maxRow = this->test_row+1;
		maxCol = this->test_col+1;
	}
	double hRange = this->hueVals.back() - this->hueVals.front();
	double sRange = this->satVals.back() - this->satVals.front();
	double lRange = this->lumVals.back() - this->lumVals.front();
	//unitThresh[0] *= (hRange/unitRanges[0]);
	//unitThresh[1] *= (sRange/unitRanges[1]);
	//unitThresh[2] *= (lRange/unitRanges[2]);
	deque<double> gradDir;
	deque<double> maxOfMax;
	for(int row= _row; row<maxRow; row++) {
		for(int col=_col; col<maxCol; col++) {
			HSL[0] = hMat.at<float>(row,col);
			HSL[1] = sMat.at<float>(row,col);
			HSL[2] = lMat.at<float>(row,col);
			double maxDiff0=0, maxDiff45=0, maxDiff90=0;
			Point maxPt0=Point(col,row);
			Point maxPt45=Point(col,row);
			Point maxPt90=Point(col,row);
			int j=col-1;
			int x = j;
			int y = row-1;
			int endY = row-localScanSize;
			for(int i=y; i>=endY; i--) {
				if(x<0 && j<0 && i<0) break;
				if(x>=0 && trueEnterExitMap.at<uchar>(row,x)!=150 && noise.at<uchar>(row,x)!=0) {
					HSL_0[0] = hMat.at<float>(row,x);
					HSL_0[1] = sMat.at<float>(row,x);
					HSL_0[2] = lMat.at<float>(row,x);
					double hueDiff = (HSL[0]-HSL_0[0])/unitThresh[0];
					double satDiff = (HSL[1]-HSL_0[1])/unitThresh[1];
					double lumDiff = (HSL[2]-HSL_0[2])/unitThresh[2];
					gradientDiff0 = abs(hueDiff)+abs(satDiff)+abs(lumDiff);
					if(gradientDiff0>=maxDiff0) {
						maxDiff0 = gradientDiff0;
						gradientDirection0 = hueDiff+satDiff+lumDiff;
						maxPt0 = Point(x,row);
					}
				}
				if(trueEnterExitMap.at<uchar>(row,x)!=150)
					--x;
			}// end for(int i)
			if(enterFlag==false) {
				if(fn.countGreaterEqual(2,maxDiff0,enterThresh)>=1) {
					enterFlag=true;
					HslEntry[0] = hMat.at<float>(maxPt0);
					HslEntry[1] = sMat.at<float>(maxPt0);
					HslEntry[2] = lMat.at<float>(maxPt0);
					map.at<uchar>(row,col) = 255;
					trueEnterExitMap.at<uchar>(row,col) = 255;
					if(gradientDirection0>0)
						upTheMtn = true;
					else if(gradientDirection0<0)
						downTheMtn = true;
					else
						upTheMtn = downTheMtn = false;
				}
			}
			/*
			else if(enterFlag==true) {
				if(downTheMtn) {
					double hueDiff = (HSL[0]-HslEntry[0])/unitThresh[0];
					//double satDiff = (HSL[1]-HslEntry[1])/unitThresh[1];
					//double lumDiff = (HSL[2]-HslEntry[2])/unitThresh[2];
					//hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
					//satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
					//lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
					//hueDiff = round(hueDiff);
					gradientDiff0 = hueDiff;
					if(fn.countGreaterEqual(2,maxDiff0,exitThresh)>=1 && gradientDiff0>=-exitCumulativeThresh) {
						enterFlag = false;
						upTheMtn = downTheMtn = false;
						map.at<uchar>(row,col-offset) = 150;
						trueEnterExitMap.at<uchar>(row,col) = 150;
					}
				}
				else if(upTheMtn) {
					double hueDiff = (HSL[0]-HslEntry[0])/unitThresh[0];
					//double satDiff = (HSL[1]-HslEntry[1])/unitThresh[1];
					//double lumDiff = (HSL[2]-HslEntry[2])/unitThresh[2];
					//hueDiff = hueDiff>=0 ? floor(hueDiff) : ceil(hueDiff);
					//satDiff = satDiff>=0 ? floor(satDiff) : ceil(satDiff);
					//lumDiff = lumDiff>=0 ? floor(lumDiff) : ceil(lumDiff);
					gradientDiff0 = hueDiff;
					if(fn.countLesserEqual(2,maxDiff0,-exitThresh)>=1 && gradientDiff0<=exitCumulativeThresh) {
						enterFlag = false;
						upTheMtn = downTheMtn = false;
						map.at<uchar>(row,col-offset) = 150;
						trueEnterExitMap.at<uchar>(row,col) = 150;
					}
				}//end if(upTheMtn)
			}//end if(enterFlag==true)
			*/
			if(col==127 && row==50) {
				String mtn = upTheMtn==true ? "Up" : "N/A";
				mtn = downTheMtn==true ? "Down" : mtn;
				printf("HSL(%f,%f,%f)\n",HSL[0],HSL[1],HSL[2]);
				printf("HslE(%f,%f,%f)\n",HslEntry[0],HslEntry[1],HslEntry[2]);
				printf("HSL0(%f,%f,%f)\n",hMat.at<float>(maxPt0),sMat.at<float>(maxPt0),lMat.at<float>(maxPt0));
				//printf("HSL45(%f,%f,%f)\n",hMat.at<float>(maxPt45),sMat.at<float>(maxPt45),lMat.at<float>(maxPt45));
				//printf("HSL90(%f,%f,%f)\n",hMat.at<float>(maxPt90),sMat.at<float>(maxPt90),lMat.at<float>(maxPt90));
				printf("MaxDiff0: %f, MaxDiff45: %f, MaxDiff90: %f\n",maxDiff0,maxDiff45,maxDiff90);
				printf("MaxPt0: %d,%d, MaxPt45: %d,%d, MaxPt90: %d,%d\n",maxPt0.x,maxPt0.y,maxPt45.x,maxPt45.y,maxPt90.x,maxPt90.y);
				printf("EnterFlag: %d\n",enterFlag);
				printf("GradDirection: %f\n",gradientDirection0);
				printf("MinHue: %f,MaxHue: %f\n",this->hueVals.front(),this->hueVals.back());
				printf("MinSat: %f,MaxSat: %f\n",this->satVals.front(),this->satVals.back());
				printf("MinLum: %f,MaxLum: %f\n",this->lumVals.front(),this->lumVals.back());
				printf("Hsl_Ranges: [%f,%f,%f]\n",hRange,sRange,lRange);
				printf("UnitThresh: [%f,%f,%f]\n",unitThresh[0],unitThresh[1],unitThresh[2]);
				printf("Mtn: %s\n",mtn.c_str());
			}
		} //end for(col)
		enterFlag=false;
		upTheMtn = downTheMtn = false;
	}//end for(row)
	return map;
}

//set HSL ranges for function getShapeUsingColors() HSL unitThresh
void ShapeMorph::setHslVals(deque<double> hueVals, deque<double> satVals, deque<double> lumVals) {
	this->hueVals = hueVals;
	this->satVals = satVals;
	this->lumVals = lumVals;
}
