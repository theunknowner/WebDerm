/*
 * shapecolor.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "shapecolor.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/KneeCurve/kneecurve.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"

void ShapeColor::setDebugLevel(int level) {
	this->debugLevel = level;
}

//use in conjunction with setDebugLevel;
void ShapeColor::setDebugColRow(int col, int row) {
	this->dbgCol = col;
	this->dbgRow = row;
}

//removes lines that run across the image
Mat ShapeColor::removeRunningLines(Mat input, Size size) {
	Mat dst = input.clone();
	const int thresh=11; // length of line
	const int thickness = 5; // thickness of line
	Size winSize = size;
	int countStreak=0;
	Mat window;
	Size prevSize;
	Point startPt(0,0), endPt(0,0);
	int rowDecr=0, rowIncr=0;
	if(this->debugLevel==2)
		namedWindow("img",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	for(int row=0; row<=(dst.rows-winSize.height); row++) {
		startPt.y = row;
		for(int col=0; col<=(dst.cols-winSize.width); col++) {
			window = dst(Rect(col,startPt.y,winSize.width,winSize.height));
			if(countNonZero(window)==window.total()) {
				/****DEBUG OUTPUT INFO ****/
				if(this->debugLevel==2) {
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
								if(this->debugLevel==2) {
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
												if(this->debugLevel==2) {
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
								if(this->debugLevel==2) {
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
								if(this->debugLevel==2) {
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
								if(this->debugLevel==2) {
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
								if(this->debugLevel==2) {
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
							if(this->debugLevel==2) {
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
			if(this->debugLevel==2) {
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

//filter LC using knee of curve
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
	float percent = roundDecimal((float)bestIdx/yVec1.size(),2);

	if(percent<=0.05)
		bestIdx = 0.25 * yVec1.size();
	if(percent>=0.8999999)
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
