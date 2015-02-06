/*
 * shapemorph.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: jason
 */

#include "shapemorph.h"

Mat ShapeMorph::prepareImage(Mat src) {
	int lightToDarkFlag = 0;
	int darkToLightFlag = 0;
	Size size(3,3);
	int boundaryLeft = 15;
	int boundaryRight = src.cols-boundaryLeft;
	double dkThresh = 0.22;
	double darkToLiteThresh = 1.03;
	double liteToDarkThresh = 0.97;
	double totalDK=0, avgDK=0, dkRatio=0, cumulativeDK=0;
	int row=0, col=0;
	int countDK=0;
	Point begin;
	Mat drkMat(src.rows,src.cols,CV_32F,Scalar(0));
	while(row<src.rows) {
		while(col<src.cols) {
			begin=Point(col-floor(size.width/2),row-floor(size.height/2));
			for(int i=begin.y; i<(begin.y+size.height); i++) {
				for(int j=begin.x; j<(begin.x+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						totalDK += src.at<uchar>(i,j);
						countDK++;
					}
				}
			}
			avgDK = totalDK/countDK;
			if(countDK==0) avgDK=0;
			drkMat.at<float>(row,col) = avgDK;
			if(col>=boundaryLeft && col<=boundaryRight) {
				dkRatio = drkMat.at<float>(row,col)/drkMat.at<float>(row,col-1);
				cumulativeDK += dkRatio - 1.0;
				//if(row==0) {
				//	//printf("DK: %d,%d: %.f, %d, %f\n",col,row,totalDK,countDK,avgDK);
				//	printf("DK: %d,%d: %f,%f,%f,%f\n",col,row,drkMat.at<float>(row,col-1),drkMat.at<float>(row,col),dkRatio,cumulativeDK);
				//}
				if(cumulativeDK>=dkThresh || dkRatio>=darkToLiteThresh) {
					darkToLightFlag++;
					break;
				}
				if(cumulativeDK<-dkThresh || dkRatio<=liteToDarkThresh) {
					lightToDarkFlag++;
					break;
				}
			}
			totalDK=0;
			countDK=0;
			col++;
		}
		totalDK=0;
		countDK=0;
		cumulativeDK=0;
		col=0;
		row++;
	}
	Mat result;
	//cout << "Dark2Lite: " << darkToLightFlag << endl;
	//cout << "Lite2Dark: " << lightToDarkFlag << endl;
	if(lightToDarkFlag>darkToLightFlag)
		result = 255 - src;
	else
		result = src.clone();
	return result;
}

//! experimental function/method for filtering using luminance
Mat ShapeMorph::lumFilter(Mat src) {
	Mat _src = src.clone();
	Mat results(src.rows, src.cols, CV_8U, Scalar(0));
	deque<double> lumVec;
	deque<double> lumVec2;
	vector<int> uniqueLumVec(256,0);
	Size size(20,20);
	int row=0, col=0;
	KneeCurve kc;
	while(row<src.rows) {
		while(col<src.cols) {
			for(int i=row; i<(row+size.height); i++) {
				for(int j=col; j<(col+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						double lum = _src.at<uchar>(i,j);
						lumVec.push_back(lum);
					}
				}
			}
			int bestIdx = round(kc.kneeCurvePoint(lumVec) * 0.95);
			double lumThresh = lumVec.at(bestIdx);

			if((col/size.width)==3 && (row/size.height)==4) {
				cout << "OldIdx: " << bestIdx << endl;
				cout << "OldThresh: " << lumThresh << endl;
			}

			double percent = 1.0 - ((double)bestIdx/lumVec.size());
			if(percent<0.15) {
				//bestIdx = round(fnVec.size()*(1.0-percent*3.5));
				bestIdx = round(bestIdx*0.75);
			}
			lumThresh = lumVec.at(bestIdx);
			if((col/size.width)==3 && (row/size.height)==4) {
				cout << "NewIdx:" << bestIdx << endl;
				cout << "NewThresh: " << lumThresh << endl;
				writeSeq2File(lumVec,"data0");
			}
			for(int i=row; i<(row+size.height); i++) {
				for(int j=col; j<(col+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						double lum = _src.at<uchar>(i,j);
						if(lum>=lumThresh) {
							results.at<uchar>(i,j) = _src.at<uchar>(i,j);
							lumVec2.push_back(lum);
						}
					}
				}
			}
			lumVec.clear();
			col+=(size.width);
		}
		col=0;
		row+=(size.height);
	}
/*
	//imgshow(results);
	kc.removeOutliers(lumVec2,0.005);
	for(unsigned int i=0; i<lumVec2.size(); i++) {
		uniqueLumVec.at(lumVec2.at(i)) = 1;
	}
	int uniqueLums = countNonZero(uniqueLumVec);
	double factor = uniqueLums/256.0;
	cout << uniqueLums << endl;
	int bestIdx = kc.kneeCurvePoint(lumVec2);
	double lumThresh = lumVec2.at(bestIdx);
	cout << bestIdx << endl;
	cout << lumThresh << endl;
	double percent = 1.0 - ((double)bestIdx/lumVec2.size());
	if(percent<0.15) {
		//bestIdx = round(fnVec.size()*(1.0-percent*3.5));
		bestIdx = round(bestIdx*factor);
	}
	lumThresh = lumVec2.at(bestIdx);
	cout << "BestIdx: " << bestIdx << endl;
	cout << "LumThresh: " << lumThresh << endl;
	writeSeq2File(lumVec2,"data");
	for(int i=0; i<results.rows; i++) {
		for(int j=0; j<results.cols; j++) {
			double lum = results.at<uchar>(i,j);
			if(lum<lumThresh) {
				results.at<uchar>(i,j) = 0;
			}
		}
	}
*/
	return results;
}

Mat ShapeMorph::findShapes(Mat src) {
	Mat _src = 255 - src;
	Size size(3,3);
	Mat openImg,closeImg,gradImg, dst, dImg, eImg;
	Point anchor(0,size.height-1);
	eImg = this->erosion(src,size);
	dImg =this->dilation(src,size);
	gradImg = dImg - eImg;
	gradImg *= 1.6;
	Mat mask = this->kmeansClusterLC(gradImg);
	_src.copyTo(dst,mask); //mask everything except points that are clustered
	return dst;
}

Mat ShapeMorph::dilation(Mat src, Size size, Point anchor) {
	/*
	Mat dst = Mat::zeros(src.rows,src.cols, CV_8U);
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMax(window);
			dst.at<uchar>(row+size.height-1,col) = lum;
			col++;
		}
		col=0;
		row++;
	}*/
	Mat result;
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	dilate(src,result,element,anchor);
	return result;
}

Mat ShapeMorph::erosion(Mat src, Size size, Point anchor) {
	/*
	Mat results = Mat::zeros(src.rows,src.cols, src.type());
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMin(window);
			results.at<uchar>(row+size.height-1,col) = lum;
			col++;
		}
		col=0;
		row++;
	}
	dst = results;*/
	Mat results;
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	erode(src,results,element,anchor);
	return results;
}

//custom hysteresis-gradient function
Mat ShapeMorph::hysteresisGradient(Mat src) {
	Mat result = Mat::zeros(src.rows, src.cols, CV_8U);
	Mat map(src.rows, src.cols,CV_8U,Scalar(0));
	const double H = 0.28;
	const double enterDemarcThresh = 30;
	const double exitDemarcThresh = -2;
	const double elevationThresh = 20;
	Point enterDemarcPos(-1,-1);
	Point exitDemarcPos(-1,-1);
	double cumulativeSP=0;
	int demarcFlag=0;
	int currentRelLum =0, nextRelLum=0;
	double tempSlope=0, slope=0;
	int step=0;
	double minSlope=100;
	Point minSlopePt, maxSlopePt;
	Point pt;
	Point myPt = Point(0,31); //for debugging use
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			pt = Point(j,i);
			step = pt.x;
			minSlope=100;
			enterDemarcPos = Point(-1,-1);
			exitDemarcPos = Point(-1,-1);
			demarcFlag=0;
			slope=0;
			if(step<(src.cols-1)) {
				while(step<(src.cols-1)) {
					currentRelLum = src.at<uchar>(i,step);
					nextRelLum = src.at<uchar>(i,step+1);
					tempSlope = (nextRelLum - currentRelLum);
					slope = (slope * H) + (tempSlope*(1.0-H));
					if(slope<minSlope) {
						minSlope = slope;
						minSlopePt = Point(step,pt.y);
					}
					//printf() for debugging
					/*
					debug=true;
					if(j==myPt.x && i==myPt.y) {
						printf("(%d,%d) - Curr:%d, Temp:%f, Slope:%f\n",step,pt.y,currentRelLum,tempSlope,slope);
					}
					/**/
					///////////////////////
					step++;
					if(slope>=enterDemarcThresh && demarcFlag==0) {
						enterDemarcPos = Point(step,pt.y);
						demarcFlag=1; //entering Dark shade
						map.at<uchar>(enterDemarcPos) = 1;
					}
					if((slope<=exitDemarcThresh && tempSlope<0)) {
						if(enterDemarcPos.x==-1 && enterDemarcPos.y==-1) {
							enterDemarcPos = minSlopePt;
							map.at<uchar>(enterDemarcPos) = 1;
						}
						exitDemarcPos = Point(step,pt.y);
						demarcFlag=1;
						map.at<uchar>(exitDemarcPos) = 2;
						break;
					}
				}//end while loop
				if(exitDemarcPos==Point(-1,-1) && demarcFlag==1) {
					exitDemarcPos = Point(step,pt.y);
					map.at<uchar>(exitDemarcPos) = 2;
				}
				//printf() for debugging
				/*
				debug=true;
				if(j==myPt.x && i==myPt.y) {
					currentRelLum = src.at<uchar>(pt.y,step);
					//printf("(%d,%d) - Curr:%f, Flag: %d\n",step,fd.pt.y,currentRelLum,demarcFlag);
					printf("(%d,%d) - Curr:%d,",step,pt.y,currentRelLum);
					printf("Flag: %d\n",demarcFlag);
					printf("%d,%d\n",pt.x,pt.y);
					printf("enterDemarcPos - %d,%d\n",enterDemarcPos.x,enterDemarcPos.y);
					printf("exitDemarcPos - %d,%d\n",exitDemarcPos.x,exitDemarcPos.y);
				}
				/**/
				//////////////////////////
				if(enterDemarcPos!=Point(-1,-1)) {
					for(int k=j; k<exitDemarcPos.x; k++) {
						if(k>=enterDemarcPos.x && pt.y>=enterDemarcPos.y) {
							if(k<=exitDemarcPos.x && pt.y<=exitDemarcPos.y) {
								//result.at<uchar>(i,k) = 255;
								result.at<uchar>(i,k) = src.at<uchar>(i,k);
							}
							else {
								enterDemarcPos = Point(-1,-1);
								exitDemarcPos = Point(-1,-1);
								demarcFlag=0;
								slope=0;
							}
						}
					}
				}
				j=step-1; //step-1 because j++ in next iteration
			}// end if color
		}//end for j
	}//end for i
	return map;
}

Mat ShapeMorph::getStructElem(Size size, int shape) {
	Mat result = Mat::zeros(size,CV_8U);
	if(shape==CIRCLE) {
		circle(result,Point((result.cols-1)/2,(result.rows-1)/2),round(size.width/2),Scalar(1),-1);
	}
	else if(shape==RECT) {
		rectangle(result,Point(0,0),Point(result.cols-1,result.rows-1),Scalar(1),-1);
	}
	return result;
}

Mat ShapeMorph::elementaryDilation(Mat origImg, Mat scaleImg) {
	Mat result = scaleImg.clone();
	for(int i=0; i<result.rows; i++) {
		for(int j=0; j<result.cols; j++) {
			if(j==0) {
				int val = max(result.at<uchar>(i,j),result.at<uchar>(i,j+1));
				val = min(val,(int)origImg.at<uchar>(i,j+1));
				result.at<uchar>(i,j+1) = val;
			}
			else if(j==(scaleImg.cols-1)) {
				int val = max(result.at<uchar>(i,j),result.at<uchar>(i,j-1));
				val = min(val,(int)origImg.at<uchar>(i,j-1));
				result.at<uchar>(i,j-1) = val;
			}
			else {
				int val = max(result.at<uchar>(i,j),result.at<uchar>(i,j-1));
				val = min(val,(int)origImg.at<uchar>(i,j-1));
				result.at<uchar>(i,j-1) = val;

				val = max(result.at<uchar>(i,j),result.at<uchar>(i,j+1));;
				val = min(val,(int)origImg.at<uchar>(i,j+1));
				result.at<uchar>(i,j+1) = val;
			}
		}
	}
	return result;
}

//Ands the two images/ Assigns min of two images
Mat ShapeMorph::custAnd(Mat origImg, Mat scaleImg, Mat map) {
	deque<int> ptVec;
	Mat results(origImg.rows,origImg.cols, CV_8U, Scalar(0));
	if(!map.empty()) {
		int flag=0, rmin=0;
		for(int i=0; i<scaleImg.rows; i++) {
			for(int j=0; j<scaleImg.cols; j++) {
				if(scaleImg.at<uchar>(i,j)>0) {
					ptVec.push_back(j);
				}
			}
			for(int j=0; j<scaleImg.cols; j++) {
				if(flag==0) rmin = origImg.at<uchar>(i,j);
				if(map.at<uchar>(i,j)==255) flag=1;
				if(map.at<uchar>(i,j)==0) flag=0;
				for(unsigned int k=0; k<ptVec.size(); k++) {
					if(k<ptVec.size()-1) {
						if(j==ptVec.at(k)) {
							results.at<uchar>(i,j) = min(rmin,(int)origImg.at<uchar>(i,j));
							break;
						}
						else if(j>ptVec.at(k) && j<ptVec.at(k+1)) {
							if(flag==1) {
								results.at<uchar>(i,j) = min(rmin,(int)origImg.at<uchar>(i,j));
							}
							else {
								int val1 = origImg.at<uchar>(i,ptVec.at(k));
								int val2 = origImg.at<uchar>(i,ptVec.at(k+1));
								int val = origImg.at<uchar>(i,j);
								rmin = max(val1,val2);
								results.at<uchar>(i,j) = min(rmin,val);
							}
							break;
						}
						else if(j<ptVec.at(k)) {
							if(flag==1) {
								results.at<uchar>(i,j) = min(rmin,(int)origImg.at<uchar>(i,j));
							}
							else {
								int val1 = origImg.at<uchar>(i,ptVec.at(k));
								int val = origImg.at<uchar>(i,j);
								rmin = val1;
								results.at<uchar>(i,j) = min(val1,val);
							}
							break;
						}
					}
					else {
						if(j>ptVec.at(k)) {
							if(flag==1) {
								results.at<uchar>(i,j) = min(rmin,(int)origImg.at<uchar>(i,j));
							}
							else {
								int val1 = origImg.at<uchar>(i,ptVec.at(k));
								int val = origImg.at<uchar>(i,j);
								rmin=val1;
								results.at<uchar>(i,j) = min(val1,val);
							}
							break;
						}
					}
				}
			}
			ptVec.clear();
		}// end for row i
	}
	else {
		for(int i=0; i<scaleImg.rows; i++) {
			for(int j=0; j<scaleImg.cols; j++) {
				if(scaleImg.at<uchar>(i,j)>0) {
					ptVec.push_back(j);
				}
			}
			for(int j=0; j<scaleImg.cols; j++) {
				for(unsigned int k=0; k<ptVec.size(); k++) {
					if(k<ptVec.size()-1) {
						if(j==ptVec.at(k)) {
							scaleImg.at<uchar>(i,j) = origImg.at<uchar>(i,j);
							break;
						}
						else if(j>ptVec.at(k) && j<ptVec.at(k+1)) {
							int val1 = origImg.at<uchar>(i,ptVec.at(k));
							int val2 = origImg.at<uchar>(i,ptVec.at(k+1));
							int val = origImg.at<uchar>(i,j);
							results.at<uchar>(i,j) = min(max(val1,val2),val);
							break;
						}
						else if(j<ptVec.at(k)) {
							int val1 = origImg.at<uchar>(i,ptVec.at(k));
							int val = origImg.at<uchar>(i,j);
							results.at<uchar>(i,j) = min(val1,val);
							break;
						}
					}
					else {
						if(j>ptVec.at(k)) {
							int val1 = origImg.at<uchar>(i,ptVec.at(k));
							int val = origImg.at<uchar>(i,j);
							results.at<uchar>(i,j) = min(val1,val);
							break;
						}
					}
				}
			}
			ptVec.clear();
		}// end for row i
	}
	return results;
}

Mat ShapeMorph::grayscaleReconstruct(Mat src, Mat scaleImg) {
	Mat _scaleImg = scaleImg.clone();
	Mat tempImg;
	Mat compareMat = Mat::zeros(src.rows,src.cols,CV_8U);
	while(true) {
		tempImg = this->elementaryDilation(src,_scaleImg);
		compareMat = _scaleImg==tempImg;
		if(countNonZero(compareMat)==src.total()) break;
		_scaleImg = tempImg.clone();
	}
	Mat result = src - tempImg;
	return result;
}

Mat ShapeMorph::custGrayscaleRecon(Mat src) {
	Mat scaleImg = src.clone()-1;
	Mat rmins = this->grayscaleReconstruct(src,scaleImg);
	Mat _src = 255 - src; //invert image for LC values
	Mat mask = custAnd(_src,rmins);
	Mat result = this->grayscaleReconstruct(_src,mask);
	return result;
}

Mat ShapeMorph::gsReconUsingRmin1(Mat src) {
	Mat scaleImg = src.clone()-1;
	Mat rmins = this->grayscaleReconstruct(src,scaleImg);
	Mat _src = 255 - src; //invert image for LC values
	Mat mask = custAnd(_src,rmins);
	Mat gsr = this->grayscaleReconstruct(_src,mask);
	Mat map = this->customFn(gsr);
	mask = this->custAnd(_src,rmins,map);
	Mat result = this->grayscaleReconstruct(_src,mask);
	return result;
}

Mat ShapeMorph::gsReconUsingRmin2(Mat src) {
	Mat scaleImg = src.clone()-1;
	Mat rmins = this->grayscaleReconstruct(src,scaleImg);
	Mat _src = 255 - src; //invert image for LC values
	Mat map = this->customFn2(_src);
	Mat mask = custAnd(_src,rmins,map);
	Mat result = this->grayscaleReconstruct(_src,mask);
	return result;
}

vector<Mat> ShapeMorph::liquidFeatureExtraction(Mat src) {
	Mat map(src.rows, src.cols, CV_8U, Scalar(0));
	deque<deque<Point> > numFeatures;
	deque<Point> ptVec;
	deque<Point> temp;
	int row=0, col=0;
	while(row<src.rows) {
		while(col<src.cols) {
			if(src.at<uchar>(row,col)>0 && map.at<uchar>(row,col)==0) {
				ptVec.push_back(Point(col,row));
				map.at<uchar>(row,col) = 255;
				temp.push_back(Point(col,row));
				while(temp.size()>0) {
					Point up(temp.at(0).x,temp.at(0).y-1);
					Point left(temp.at(0).x-1,temp.at(0).y);
					Point right(temp.at(0).x+1,temp.at(0).y);
					Point down(temp.at(0).x,temp.at(0).y+1);
					Point downLeft(temp.at(0).x-1,temp.at(0).y+1);
					Point downRight(temp.at(0).x+1,temp.at(0).y+1);
					if(up.y>=0) {
						if(map.at<uchar>(up)==0 && src.at<uchar>(up)>0) {
							ptVec.push_back(up);
							map.at<uchar>(up)=255;
							temp.push_back(up);
						}
					}
					if(left.x>=0) {
						if(map.at<uchar>(left)==0 && src.at<uchar>(left)>0) {
							ptVec.push_back(left);
							map.at<uchar>(left)=255;
							temp.push_back(left);
						}
					}
					if(right.x<src.cols) {
						if(map.at<uchar>(right)==0 && src.at<uchar>(right)>0) {
							ptVec.push_back(right);
							map.at<uchar>(right)=255;
							temp.push_back(right);
						}
					}
					if(down.y<src.rows) {
						if(map.at<uchar>(down)==0 && src.at<uchar>(down)>0) {
							ptVec.push_back(down);
							map.at<uchar>(down)=255;
							temp.push_back(down);
						}
					}
					if(down.y<src.rows && left.x>=0) {
						if(map.at<uchar>(downLeft)==0 && src.at<uchar>(downLeft)>0) {
							ptVec.push_back(downLeft);
							map.at<uchar>(downLeft)=255;
							temp.push_back(downLeft);
						}
					}
					if(down.y<src.rows && right.x<src.cols) {
						if(map.at<uchar>(downRight)==0 && src.at<uchar>(downRight)>0) {
							ptVec.push_back(downRight);
							map.at<uchar>(downRight)=255;
							temp.push_back(downRight);
						}
					}
					temp.pop_front();
				}
				numFeatures.push_back(ptVec);
				ptVec.clear();
				temp.clear();
			}
			col++;
		}//end while col
		col=0;
		row++;
	}//end while row

	int numOfPtsThresh = 10;
	vector<Mat> featureVec;
	for(unsigned int i=0; i<numFeatures.size(); i++) {
		Mat feature(src.rows, src.cols, CV_8U,Scalar(0));
		if(numFeatures.at(i).size()>=numOfPtsThresh) {
			for(unsigned int j=0; j<numFeatures.at(i).size(); j++) {
				feature.at<uchar>(numFeatures.at(i).at(j)) = src.at<uchar>(numFeatures.at(i).at(j));
			}
			featureVec.push_back(feature);
		}
	}
	return featureVec;
}

vector<Mat> ShapeMorph::filterFeatures(vector<Mat> featureVec) {
	vector<Mat> results;
	vector<double> lcVec;
	double lcThresh = 100.0;
	int pixelCountThresh = 30;
	for(unsigned int n=0; n<featureVec.size(); n++) {
		double avgLC=0;
		int countPix = countNonZero(featureVec.at(n));
		for(int i=0; i<featureVec.at(n).rows; i++) {
			for(int j=0; j<featureVec.at(n).cols; j++) {
				avgLC += featureVec.at(n).at<uchar>(i,j);
			}
		}
		avgLC /= countPix;
		if(countPix>=pixelCountThresh && avgLC>lcThresh) {
			results.push_back(featureVec.at(n));
		}
	}
	return results;
}


//filters noise and connects shape of image
Mat ShapeMorph::filterNoise(Mat src, int radius) {
	int threshCount = 25;
	int threshQuadrant = 18;
	Mat results = src.clone();
	int dist = (radius*2)+1;
	Size size(dist,dist);
	int pixels_top=0, pixels_bottom=0, pixels_left=0, pixels_right=0;
	int pixelsTL=0, pixelsTR=0, pixelsBL=0, pixelsBR=0;
	Point begin;
	Mat window = Mat::zeros(size.height, size.width, CV_8U);
	int row=0, col=0;
	while(row<src.rows) {
		while(col<src.cols) {
			int flagTL=0, flagTR=0, flagBL=0, flagBR=0;
			int flagL=0, flagR=0, flagT=0, flagB=0;
			window = Mat::zeros(size.height, size.width, CV_8U);
			begin = Point(col-floor(size.width/2),row-floor(size.height/2));
			for(int i=0; i<size.height; i++) {
				for(int j=0; j<size.width; j++) {
					if(i+begin.y>=0 && j+begin.x>=0)
						window.at<uchar>(i,j) = src.at<uchar>(i+begin.y,j+begin.x);
				}
			}
			Mat roiTL = window(Rect(0,0,ceil(size.width/2.),ceil(size.height/2.)));
			Mat roiTR = window(Rect(floor(size.width/2.),0,ceil(size.width/2.),ceil(size.height/2.)));
			Mat roiBL = window(Rect(0,floor(size.height/2.),ceil(size.width/2.),ceil(size.height/2.)));
			Mat roiBR = window(Rect(floor(size.width/2.),floor(size.height/2.),ceil(size.width/2.),ceil(size.height/2)));
			pixelsTL = countNonZero(roiTL);
			pixelsTR = countNonZero(roiTR);
			pixelsBL = countNonZero(roiBL);
			pixelsBR = countNonZero(roiBR);
			pixels_top = pixelsTL + pixelsTR;
			pixels_bottom = pixelsBL + pixelsBR;
			pixels_left = pixelsTL + pixelsBL;
			pixels_right = pixelsTR + pixelsBR;
			//int totalPix = pixels_top+pixels_bottom;
			//debug printf info
			/*
			if(col==66 && row==47) {
				printf("TL: %d\n",pixelsTL);
				printf("TR: %d\n",pixelsTR);
				printf("BL: %d\n",pixelsBL);
				printf("BR: %d\n",pixelsBR);
				printf("Left: %d\n",pixels_left);
				printf("Right: %d\n",pixels_right);
				printf("Top: %d\n",pixels_top);
				printf("Bottom: %d\n",pixels_bottom);
				printf("Total: %d\n",totalPix);
				//namedWindow("roi",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
				//imshow("roi",window);
				//waitKey(0);
			}
			 */
			if(pixelsTL>=threshQuadrant) flagTL=1;
			if(pixelsTR>=threshQuadrant) flagTR=1;
			if(pixelsBL>=threshQuadrant) flagBL=1;
			if(pixelsBR>=threshQuadrant) flagBR=1;
			if(pixels_left>=threshCount) flagL=1;
			if(pixels_right>=threshCount) flagR=1;
			if(pixels_top>=threshCount) flagT=1;
			if(pixels_bottom>=threshCount) flagB=1;

			if((flagL+flagR+flagT+flagB<=2)&&(flagTL+flagTR+flagBL+flagBR<=2)) {
				results.at<uchar>(row,col) = 0;
			}
			col++;
		}
		col=0;
		row++;
	}
	//imfill(results);
	return results;
}

//draw a line from point to every point after
Mat ShapeMorph::connectImage(Mat src, Size size, double dist) {
	Mat results = src.clone();
	deque<Point> ptVec;
	int row=0, col=0;
	while(row<src.rows) {
		while(col<src.cols) {
			for(int i=row; i<(row+size.height); i++) {
				for(int j=col; j<(col+size.width); j++) {
					if((i<src.rows && j<src.cols))
						if(results.at<uchar>(i,j)>0)
							ptVec.push_back(Point(j,i));
				}
			}
			for(unsigned int i=0; i<ptVec.size(); i++) {
				for(unsigned int j=i; j<ptVec.size(); j++) {
					if(eucDist(ptVec.at(i),ptVec.at(j))<dist) {
						line(results,ptVec.at(i),ptVec.at(j),Scalar(255));
					}
				}
			}
			ptVec.clear();
			col+=(int)dist;
		}
		col=0;
		row+=(int)dist;
	}
	return results;
}

//returns luminance based heat map using Point density
Mat ShapeMorph::detectHeat(Mat src, Size size) {
	float sensitivity = 0.08; //weight of each point with D distance
	Mat kernel = Mat::zeros(size.height, size.width,CV_32F);
	Point origin(floor(size.width/2),floor(size.height/2));
	for(int i=0; i<kernel.rows; i++) {
		for(int j=0; j<kernel.cols; j++) {
			double dist = eucDist(Point(i,j),origin);
			double weight = 1.0 - (dist*sensitivity);
			kernel.at<float>(i,j) = weight;
		}
	}
	int row=0, col=0;
	Point begin;
	Mat heatMap = Mat::zeros(src.rows,src.cols,CV_32F);
	double total=0;
	while(row<src.rows) {
		while(col<src.cols) {
			begin = Point(col-floor(size.width/2),row-floor(size.height/2));
			for(int i=0; i<kernel.rows; i++) {
				for(int j=0; j<kernel.cols; j++) {
					int x = j+begin.x;
					int y = i+begin.y;
					if(y>=0 && x>=0 && y<src.rows && x<src.cols) {
						if(src.at<uchar>(y,x)>5)
							total += kernel.at<float>(i,j);
					}
				}
			}
			heatMap.at<float>(row,col) = total/kernel.total();
			total=0;
			col++;
		}
		col=0;
		row++;
	}
	Mat results(heatMap.rows, heatMap.cols, CV_8U,Scalar(0));
	for(int i=0; i<heatMap.rows; i++) {
		for(int j=0; j<heatMap.cols; j++) {
			int value = round(heatMap.at<float>(i,j) * 255);
			if(value>25 && src.at<uchar>(i,j)>0)
				results.at<uchar>(i,j) = 255;
		}
	}
	return results;
}

//returns mask using kmeans clustering of LC values
Mat ShapeMorph::kmeansClusterLC(Mat src) {
	static unsigned int defaultSeed = theRNG().state;
	theRNG().state = defaultSeed; //reset RNG seed for kmeans initial centroids

	deque<int> dataVec;
	deque<int> uniqueLum(256,0);
	deque<int> lumVec;
	deque<Point> ptVec;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			int lum = src.at<uchar>(i,j);
			if(lum>0) {
				dataVec.push_back(lum);
				uniqueLum.at(lum)++;
				ptVec.push_back(Point(j,i));
			}
		}
	}
	for(unsigned int i=0; i<uniqueLum.size(); i++) {
		if(uniqueLum.at(i)>0) {
			lumVec.push_back(i);
		}
	}
	Mat samples(dataVec.size(), 1, CV_32F);
	for(unsigned int i=0; i<dataVec.size(); i++) {
		samples.at<float>(i,0) = dataVec.at(i);
	}
	int lumRange = lumVec.at(lumVec.size()-1) - lumVec.at(0);
	int clusterCount = round(sqrt(lumRange/2))<lumVec.size() ? round(sqrt(lumRange/2)) : lumVec.size();
	clusterCount = min(11,clusterCount);
	Mat labels;
	int attempts = 5;
	Mat centers;
	printf("Min Lum: %d, Max Lum: %d, Range: %d\n",lumVec.at(0),lumVec.at(lumVec.size()-1), lumRange);
	cout << "Initial clusters: " << clusterCount << endl;
	cout << "Input size: " << dataVec.size() << endl;
	cout << "Unique Lum size: " << uniqueLum.size() << endl;
	cout << "Unique Lum considered: " << lumVec.size() << endl;
	double compact = kmeans(samples,clusterCount,labels,TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	//cout << "compactness: " << compact << endl;
	deque<int> centerCount(clusterCount,0);
	deque<deque<int> > ranges(clusterCount,deque<int>(2,0));
	for(unsigned int i=0; i<ranges.size(); i++) {
		for(unsigned int j=0; j<ranges.at(i).size(); j++) {
			if(j==0) {
				ranges.at(i).at(j) = uniqueLum.size();
			}
			else if(j==1) {
				ranges.at(i).at(j) = 0;
			}
		}
	}
	Mat origPos;
	jaysort(centers,origPos);
	for(int i=0; i<labels.rows; i++) {
		int idx = labels.at<int>(i,0);
		for(int j=0; j<origPos.rows; j++) {
			if(idx==origPos.at<int>(j,0)) {
				idx = j;
				labels.at<int>(i,0) = idx;
				break;
			}
		}
		centerCount.at(idx)++;
		if(dataVec.at(i)>ranges.at(idx).at(1)) {
			ranges.at(idx).at(1) = dataVec.at(i);
		}
		else if(dataVec.at(i)<ranges.at(idx).at(0)) {
			ranges.at(idx).at(0) = dataVec.at(i);
		}
	}
	for(int i=0; i<centers.rows; i++) {
		printf("%d) %f - %d - Min: %d, Max: %d\n",i,centers.at<float>(i,0),centerCount.at(i),ranges.at(i).at(0),ranges.at(i).at(1));
	}
	Mat result = Mat::zeros(src.rows, src.cols, src.type());
	Mat clusterImg = Mat::zeros(src.rows,src.cols,CV_8UC3);
	int idxThresh = ceil(clusterCount*0.5); //! threshold for cluster filtering
	printf("idxThresh: %d\n",idxThresh);
	for(unsigned int i=0; i<ptVec.size(); i++) {
		int idx = labels.at<int>(i,0);
		if(idx>idxThresh) {
			result.at<uchar>(ptVec.at(i).y,ptVec.at(i).x) = 255;
		}
	}
	return result;
}

Mat ShapeMorph::kmeansClusterPt(Mat src) {
	deque<Point> dataVec;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			int lum = src.at<uchar>(i,j);
			if(lum>0) {
				dataVec.push_back(Point(j,i));
			}
		}
	}
	Mat samples(dataVec.size(), 2, CV_32F);
	for(unsigned int i=0; i<dataVec.size(); i++) {
		samples.at<float>(i,0) = dataVec.at(i).x;
		samples.at<float>(i,1) = dataVec.at(i).y;
	}
	//int clusterCount = round(sqrt(dataVec.size()/2));
	int clusterCount = round(sqrt(src.cols/2));
	//int clusterCount = 4;
	Mat labels;
	int attempts = 1;
	Mat centers;
	int compact = kmeans(samples,clusterCount,labels,TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	cout << "Initial clusters: " << clusterCount << endl;
	cout << "Input size: " << dataVec.size() << endl;
	cout << "compactness: " << compact << endl;
	deque<int> centerCount(clusterCount,0);
	deque<deque<Point> > ranges(clusterCount,deque<Point>(2,Point(-1,-1)));
	for(unsigned int i=0; i<ranges.size(); i++) {
		for(unsigned int j=0; j<ranges.at(i).size(); j++) {
			if(j==0) {
				ranges.at(i).at(j) = Point(src.cols,src.rows);
			}
			else if(j==1) {
				ranges.at(i).at(j) = Point(-1,-1);
			}
		}
	}
	Mat origPos;
	jaysort(centers,origPos);
	for(int i=0; i<labels.rows; i++) {
		int idx = labels.at<int>(i,0);
		for(int j=0; j<origPos.rows; j++) {
			if(idx==origPos.at<int>(j,0)) {
				idx = j;
				labels.at<int>(i,0) = idx;
				break;
			}
		}
		centerCount.at(idx)++;
		/*if(dataVec.at(i)>ranges.at(idx).at(1)) {
			ranges.at(idx).at(1) = dataVec.at(i);
		}
		else if(dataVec.at(i)<ranges.at(idx).at(0)) {
			ranges.at(idx).at(0) = dataVec.at(i);
		}*/
	}

	for(int i=0; i<centers.rows; i++) {
		printf("%f,%f - %d - Min: (%d,%d), Max: (%d,%d)\n",centers.at<float>(i,0),centers.at<float>(i,1),centerCount.at(i),ranges.at(i).at(0).x,ranges.at(i).at(0).y,ranges.at(i).at(1).x,ranges.at(i).at(1).y);
	}
	Mat result = Mat::zeros(src.rows, src.cols, CV_8U);
	Mat clusterImg = Mat::zeros(src.rows,src.cols,CV_8UC3);
	int idxThresh = ceil(clusterCount*0.5); //! threshold for cluster filtering
	RNG rng;
	vector<Vec3b> colorVec;
	for(int i=0; i<centers.rows; i++) {
		Vec3b color(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
		colorVec.push_back(color);
	}
	for(unsigned int i=0; i<dataVec.size(); i++) {
		int idx = labels.at<int>(i,0);
		//if(idx>idxThresh) {
		clusterImg.at<Vec3b>(dataVec.at(i).y,dataVec.at(i).x) = colorVec.at(idx);
		//printf("%d,%d - %d\n",dataVec.at(i).x,dataVec.at(i).y, idx);
		//}
	}
	return clusterImg;
}

//! entry/exit map of edges
Mat ShapeMorph::customFn(Mat src) {
	Mat map(src.rows, src.cols, CV_8U, Scalar(0));
	Size size(5,5);
	const int minLCThresh = 5;
	const double minDNThresh = round(size.area() * 0.15);
	double enterDKThresh = 0.6;
	double enterDNThresh = 3.5;
	double exitDKThresh = 0.6;
	double exitDNThresh = 3.5;
	int entryFlag=0;
	double totalDK=0, totalDN=0;
	double avgDK=0, density=0;
	double dkRatio=0, dnRatio=0;
	double cumulativeDK=0, cumulativeDN=0;
	int row=0, col=0;
	int countDK=0;
	Point begin;
	Mat drkMat(src.rows,src.cols,CV_32F,Scalar(0));
	Mat dnMat(src.rows,src.cols,CV_32F,Scalar(0));
	while(row<src.rows) {
		while(col<src.cols) {
			begin=Point(col-floor(size.width/2),row-floor(size.height/2));
			for(int i=begin.y; i<(begin.y+size.height); i++) {
				for(int j=begin.x; j<(begin.x+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						if(src.at<uchar>(i,j)>minLCThresh) {
							totalDK += src.at<uchar>(i,j);
							countDK++;
						}
						if(src.at<uchar>(i,j)>minLCThresh)
							totalDN++;
					}
				}
			}
			avgDK = totalDK/countDK;
			if(countDK==0) avgDK=0;
			density = totalDN/size.area();
			drkMat.at<float>(row,col) = avgDK;
			dnMat.at<float>(row,col) = density;
			if(col>0 && drkMat.at<float>(row,col-1)>minLCThresh && totalDN>minDNThresh) {
				dkRatio = drkMat.at<float>(row,col)/drkMat.at<float>(row,col-1);
				dnRatio = dnMat.at<float>(row,col)/dnMat.at<float>(row,col-1);
				cumulativeDK += dkRatio - 1.0;
				cumulativeDN += dnRatio - 1.0;
				/*if(row==36) {
					//printf("DK: %d,%d: %.f, %d, %f\n",col,row,totalDK,countDK,avgDK);
					printf("DK: %d,%d: %f,%f,%f,%f,  ",col,row,drkMat.at<float>(row,col-1),drkMat.at<float>(row,col),dkRatio,cumulativeDK);
					printf("DN: %d,%d: %f,%f,%f,%f\n",col,row,dnMat.at<float>(row,col-1),dnMat.at<float>(row,col),dnRatio,cumulativeDN);
				}*/
				if(cumulativeDK>=enterDKThresh || cumulativeDN>=enterDNThresh) {
					entryFlag=1;
				}
				else if(cumulativeDK<=exitDKThresh || cumulativeDN<=exitDNThresh) {
					entryFlag=0;
				}
				if(entryFlag==1)
					map.at<uchar>(row,col) = 255;
			}
			totalDK=0;
			totalDN=0;
			countDK=0;
			col++;
		}
		cumulativeDK=0;
		cumulativeDN=0;
		col=0;
		row++;
	}
	return map;
}

//! returns entry/exit map of edges
Mat ShapeMorph::customFn2(Mat src) {
	Mat map(src.rows, src.cols, CV_8U, Scalar(0));
	Size size(5,5);
	double enterDKThresh = 0.1;
	double exitDKThresh = 0.1;
	double enterDKThresh2 = 1.03;
	double exitDKThresh2 = 1.0;
	int entryFlag=0;
	double totalDK=0;
	double avgDK=0;
	double dkRatio=0;
	double cumulativeDK=0;
	int row=0, col=0;
	int countDK=0;
	Point begin;
	Mat drkMat(src.rows,src.cols,CV_32F,Scalar(0));
	Mat dnMat(src.rows,src.cols,CV_32F,Scalar(0));
	while(row<src.rows) {
		while(col<src.cols) {
			begin=Point(col-floor(size.width/2),row-floor(size.height/2));
			for(int i=begin.y; i<(begin.y+size.height); i++) {
				for(int j=begin.x; j<(begin.x+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						totalDK += src.at<uchar>(i,j);
						countDK++;
					}
				}
			}
			avgDK = totalDK/countDK;
			if(countDK==0) avgDK=0;
			drkMat.at<float>(row,col) = avgDK;
			if(col>0) {
				dkRatio = drkMat.at<float>(row,col)/drkMat.at<float>(row,col-1);
				cumulativeDK += dkRatio - 1.0;
				if(avgDK>=180)
					entryFlag=1;
				if(entryFlag==0 && avgDK<180) {
					if(cumulativeDK>=enterDKThresh)
						entryFlag=1;
					if(dkRatio>=enterDKThresh2) {
						entryFlag=1;
						cumulativeDK=enterDKThresh;
					}
				}
				if(entryFlag==1 && avgDK<180) {
					if(cumulativeDK<exitDKThresh)
						entryFlag=0;
					if(dkRatio<exitDKThresh2) {
						entryFlag=0;
						cumulativeDK=0;
					}
				}
				//if(row==37) {
				//printf("DK: %d,%d: %.f, %d, %f\n",col,row,totalDK,countDK,avgDK);
				//	printf("DK: %d,%d: %f,%f,%f,%f,  ",col,row,drkMat.at<float>(row,col-1),drkMat.at<float>(row,col),dkRatio,cumulativeDK);
				//	printf("Flag: %d\n",entryFlag);
				//}
				if(entryFlag==1)
					map.at<uchar>(row,col) = 255;
			}
			totalDK=0;
			countDK=0;
			col++;
		}
		cumulativeDK=0;
		col=0;
		row++;
	}
	return map;
}

Mat ShapeMorph::densityDetection(Mat src) {
	//Mat map = src.clone();
	Mat map(src.rows,src.cols,CV_8U,Scalar(0));
	Size size(5,5);
	const double C=1.0;
	const double alpha=1.0;
	const double beta=0.0;
	int row=0, col=0;
	deque<double> fnVec;
	double density=0, countDk=0, avgDk=0;
	double absDiscernThresh=5.0;
	while(row<src.rows) {
		while(col<src.cols) {
			for(int i=row; i<(row+size.height); i++) {
				for(int j=col; j<(col+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						int lc = src.at<uchar>(i,j);
						if(lc>absDiscernThresh) {
							density++;
							avgDk += lc;
							countDk++;
						}
					}
				}
			}
			density /= size.area();
			avgDk /= countDk;
			if(countDk==0) avgDk = 0;
			double fx = C * pow(density,alpha) * pow(avgDk,beta);
			if(fx>0)
				fnVec.push_back(fx);
			avgDk=0;
			density=0;
			countDk=0;
			//col+=size.width;
			col++;
		}
		col=0;
		//row+=size.height;
		row++;
	}

	writeSeq2File(fnVec,"fnVec");
	//calculate knee of curve for fx for filtering
	KneeCurve kc;
	int bestIdx = kc.kneeCurvePoint(fnVec);
	//bestIdx = round(bestIdx*1.05);
	cout << "BestIdx: " << bestIdx << endl;
	//fx threshold filtering
	double fxThresh = fnVec.at(bestIdx);
	double percent = 1.0 - ((double)bestIdx/fnVec.size());
	//if(percent<0.15) {
		//bestIdx = round(fnVec.size()*(1.0-percent*3.5));
	//	bestIdx = round(bestIdx*0.75);
	//}
	fxThresh = fnVec.at(bestIdx);
	cout << "New BestIdx: " << bestIdx << endl;
	cout << "FxThresh: " << fxThresh << endl;
	row=0; col=0;
	/*
	while(row<src.rows) {
		while(col<src.cols) {
			for(int i=row; i<(row+size.height); i++) {
				for(int j=col; j<(col+size.width); j++) {
					if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
						int lc = src.at<uchar>(i,j);
						if(lc>absDiscernThresh) {
							density++;
							avgDk += lc;
							countDk++;
						}
					}
				}
			}
			density /= size.area();
			avgDk /= countDk;
			if(countDk==0) avgDk = 0;
			double fx = C * pow(density,alpha) * pow(avgDk,beta);
			if(fx>fxThresh) {
				for(int i=row; i<(row+size.height); i++) {
					for(int j=col; j<(col+size.width); j++) {
						if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
							if(src.at<uchar>(i,j)>absDiscernThresh)
								map.at<uchar>(i,j)=255;
						}
					}
				}
			}
			avgDk=0;
			density=0;
			countDk=0;
			//col+=size.width;
			col++;
		}
		col=0;
		//row+=size.height;
		row++;
	}
	*/
	//imgshow(map);

	//connect nearest neighbors
	double q = 0.99999;
	double b = fxThresh;
	double a = pow(-log(1.0-q)/(3.14159 * b),0.5);
	a -= 0.5;
	cout << a << endl;
	Mat result(src.rows,src.cols,CV_8U,Scalar(0));
	row=0; col=0;
	Size square(ceil(a),ceil(a));
	a = round(a);
	while(row<src.rows) {
		while(col<src.cols) {
			int lc = src.at<uchar>(row,col);
			if(lc>0) {
				Point begin = Point(col-square.width,row-square.height);
				Point end = Point(col+square.width,row+square.height);
				for(int i=begin.y; i<end.y; i++) {
					for(int j=begin.x; j<end.x; j++) {
						if(j>=0 && i>=0 && j<src.cols && i<src.rows) {
							lc = src.at<uchar>(i,j);
							//double dist = eucDist(Point(j,i),Point(col,row));
							double dist = abs(j-col) + abs(i-row);
							if(dist<=a && lc>absDiscernThresh) {
								if(Point(j,i)!=Point(col,row)) {
									line(result,Point(j,i),Point(col,row),Scalar(255),1);
								}
							}
						}
					}
				}
			}
			col++;
		}
		col=0;
		row++;
	}
	return result;
}

vector<Mat> ShapeMorph::runShapeMorphTest(deque<String> &nameVec, deque<int> &labels) {
	fstream fs("/home/jason/Desktop/workspace/Test_Images.csv");
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	vector<Mat> samplesVec;
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==0)
					nameVec.push_back(vec.at(i));
				if(i==1)
					labels.push_back(atoi(vec.at(i).c_str()));
			}
			vec.clear();
		}
		for(unsigned int i=0; i<nameVec.size(); i++) {
			String file = folder + nameVec.at(i) + ".jpg";
			Mat img = runResizeImage(file,Size(140,140),0);
			Size size(3,3);
			img = runColorNormalization(img);
			cvtColor(img,img,CV_BGR2GRAY);
			Mat img2 = this->prepareImage(img);
			Mat result2 = this->gsReconUsingRmin2(img2);
			Mat img3 = this->densityDetection(result2);
			vector<Mat> featureVec = this->liquidFeatureExtraction(img3);
			int largest=0, idx=0;
			for(unsigned int i=0; i<featureVec.size(); i++) {
				if(countNonZero(featureVec.at(i))>largest) {
					largest = countNonZero(featureVec.at(i));
					idx = i;
				}
			}
			Mat img5 = this->dilation(featureVec.at(idx),Size(5,5));
			samplesVec.push_back(img5);
		}
	}
	else {
		cout << "runShapeMorphTest() can't open file!" << endl;
	}
	return samplesVec;
}
