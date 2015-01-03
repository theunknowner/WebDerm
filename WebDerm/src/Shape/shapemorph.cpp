/*
 * shapemorph.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: jason
 */

#include "shapemorph.h"

Mat ShapeMorph::prepareImage(Mat src) {
	Mat dst(src.rows,src.cols,src.type());
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			dst.at<uchar>(i,j) = 255 - src.at<uchar>(i,j);
		}
	}
	return dst;
}

void increaseLC(Mat &src) {
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			int lum = src.at<uchar>(i,j);
			lum *= 2;
			if(lum>255) lum=255;
			src.at<uchar>(i,j) = lum;
		}
	}
}

Mat ShapeMorph::findShapes(Mat src) {
	Size size(3,3);
	Mat openImg,closeImg,gradImg, dst, dImg, eImg;
	Point anchor(0,size.height-1);
	eImg = this->erosion(src,size);
	dImg =this->dilation(src,size);
	//this->erosion2(dImg,eImg,size,Point(size.width-1,0));
	gradImg = dImg - eImg;
	dst = this->kmeansCluster(gradImg);
	//dst = this->uniqueLumPercentile(gradImg,0.35);
	//dst = this->contrast1(gradImg);
	//Mat element = getStructuringElement(MORPH_RECT,Size(7,7));
	//morphologyEx(dst,dst,MORPH_CLOSE,element);
	//dImg = this->hysteresisDilation(_src,size,anchor);
	//eImg = this->hysteresisErosion(gradImg,size,anchor);
	//dst = dImg - eImg;
	//Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	//morphologyEx(dst,dst,MORPH_CLOSE,element,anchor);
	//namedWindow("gradImg",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//namedWindow("closeImg",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//namedWindow("hystImg",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//imshow("gradImg",gradImg);
	//imshow("closeImg",dst);
	//imshow("hystImg",eImg);
	//imwrite("shapemorph.png",dst);
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

void ShapeMorph::erosion2(Mat src, Mat &dst, Size size, Point anchor) {
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
	Mat element = getStructuringElement(MORPH_RECT,size,anchor);
	erode(src,dst,element,anchor);
}

Mat ShapeMorph::uniqueLumPercentile(Mat src, double percentile) {
	Mat dst = Mat::zeros(src.rows,src.cols,src.type());
	int lum=0;
	deque<int> lumsFlag(256,0);
	deque<int> dataVec;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			lum = src.at<uchar>(i,j);
			if(lum>0) {
				dataVec.push_back(lum);
				lumsFlag.at(lum)++;
			}
		}
	}
	deque<int> lums;
	for(unsigned int i=0; i<lumsFlag.size(); i++) {
		if(lumsFlag.at(i)!=0) {
			lums.push_back(i);
		}
	}
	writeSeq2File(lumsFlag,"data2");
	int n = percentile * lums.size();
	int thresh = lums.at(n);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			lum = src.at<uchar>(i,j);
			if(lum>thresh)
				dst.at<uchar>(i,j) = lum;
		}
	}
	return dst;
}

Mat ShapeMorph::extractShape(Mat src, Size size, Point anchor) {
	double dist=5.0;
	deque<deque<Point> > shapeVec;
	deque<Point> ptVec;
	Mat ptMap = Mat::zeros(src.rows,src.cols,CV_8U);
	Mat window;
	int right=0,down=0,left=0,up=0;
	if(anchor==Point(-1,-1)) anchor = Point(floor(size.width/2),floor(size.height/2));
	anchor = Point(0,1);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			if(ptMap.at<uchar>(i,j)==0) {
				int x = i;
				int y = j;
				window = src(Rect(x,y,size.width,size.height));
				if(countNonZero(window.row(anchor.y))>=size.width) {
					right=1;
					while(right==1) {
						//if()
						for(int k=x; k<(x+size.width); k++) {
							if(ptMap.at<uchar>(y+anchor.y,k)==0)
								ptVec.push_back(Point(k,y+anchor.y));
						}
						if(countNonZero(window.col(size.width-1))<=round(size.height/2)) {
							down=1;
						}
						if(down==1) {
							//do something
						}
						x++;
						window = src(Rect(x,y,size.width,size.height));
					}
				}
			}
		}
	}
}

Mat ShapeMorph::hysteresisDilation(Mat src, Size size, Point anchor) {
	double h=0.5, scale=1.5;
	Mat window = Mat::zeros(src.rows, src.cols, src.type());
	//Mat results = Mat::zeros(src.rows,src.cols, src.type());
	Mat results = src.clone();
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	if(anchor==Point(-1,-1)) anchor = Point(floor(size.width/2),floor(size.height/2));
	int lum0=-1,lum45=-1,lum90=-1;
	Point pt0, pt45, pt90;
	while(row<=max_rows) {
		while(col<=max_cols) {
			Mat window = src(Rect(col,row,size.width,size.height));
			lum = fn.getMax(window);
			pt0 = Point(col+anchor.x-1,row+anchor.y);
			pt45 = Point(col+anchor.x-1,row+anchor.y-1);
			pt90 = Point(col+anchor.x,row+anchor.y-1);
			if(pt0.y>=0 && pt0.x>=0) {
				lum0 = results.at<uchar>(pt0.y,pt0.x);
			}
			if(pt45.y>=0 && pt45.x>=0) {
				lum45 = results.at<uchar>(pt45.y,pt45.x);
			}
			if(pt90.y>=0 && pt90.x>=0) {
				lum90 = results.at<uchar>(pt90.y,pt90.x);
			}
			if(lum0>=0 && lum45>=0 && lum90>=0) {
				double avg = ((lum0+lum45+lum90)/3.0);
				double newLum = h*lum*scale+(1.0-h)*avg;
				lum = round(newLum);
				if(lum>255) lum=255;
				if(lum<0) lum=0;
			}
			results.at<uchar>(row+anchor.y,col) = lum;
			col++;
			lum0=-1; lum45=-1; lum90=-1;
		}
		col=0;
		row++;
	}
	return results;
}

Mat ShapeMorph::hysteresisErosion(Mat src, Size size, Point anchor) {
	double h=0.5, scale=1.5;
	Mat window = Mat::zeros(size.height, size.width, CV_8U);
	Mat results = src.clone();
	Functions fn;
	int row=0, col=0, lum=0;
	int max_rows = src.rows - size.height;
	int max_cols = src.cols - size.width;
	if(anchor==Point(-1,-1)) anchor = Point(floor(size.width/2),floor(size.height/2));
	Point arrAnchors[4] = {Point(0,size.height-1),Point(size.width-1,0),
			Point(size.width-1,size.height-1),Point(0,0)};
	while(row<=max_rows) {
		while(col<=max_cols) {
			for(int n=0; n<4; n++) {
				anchor = arrAnchors[n];
				int beginX = col - anchor.x;
				int beginY = row - anchor.y;
				for(int i=0; i<size.height; i++) {
					for(int j=0; j<size.width; j++) {
						int x = j+beginX;
						int y = i+beginY;
						if(x>=0 && y>=0 && x<src.cols && y<src.rows)
							window.at<uchar>(i,j) = src.at<uchar>(y,x);
						else {
							if(x<0) x = 0;
							if(x>=src.cols) x = src.cols-1;
							if(y<0) y=0;
							if(y>=src.rows) y = src.rows-1;
							window.at<uchar>(i,j) = src.at<uchar>(y,x);
						}
					}
				}
				lum = fn.getMin(window);
				results.at<uchar>(row,col) += lum;
			}
			col++;
		}
		col=0;
		row++;
	}
	return results;
}

Mat ShapeMorph::contrast1(Mat src) {
	Mat result = Mat::zeros(src.rows, src.cols, CV_8U);
	const double H = 0.72;
	const double enterDemarcThresh = 5;
	const double exitDemarcThresh = -2;
	Point enterDemarcPos(-1,-1);
	Point exitDemarcPos(-1,-1);
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
					}
					if((slope<=exitDemarcThresh && tempSlope<0)) {
						if(enterDemarcPos.x==-1 && enterDemarcPos.y==-1)
							enterDemarcPos = minSlopePt;
						exitDemarcPos = Point(step,pt.y);
						demarcFlag=1;
						break;
					}
				}//end while loop
				if(exitDemarcPos==Point(-1,-1) && demarcFlag==1)
					exitDemarcPos = Point(step,pt.y);
				//printf() for debugging

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
								result.at<uchar>(i,k) = 255;
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
	return result;
}

Mat ShapeMorph::contrast2(Mat src) {
	Mat result = Mat::zeros(src.rows, src.cols, CV_8U);
	const double H = 0.28;
	const double enterDemarcThresh = 5;
	const double exitDemarcThresh = -2;
	Point enterDemarcPos(-1,-1);
	Point exitDemarcPos(-1,-1);
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
					}
					if((slope<=exitDemarcThresh && tempSlope<0)) {
						if(enterDemarcPos.x==-1 && enterDemarcPos.y==-1)
							enterDemarcPos = minSlopePt;
						exitDemarcPos = Point(step,pt.y);
						demarcFlag=1;
						break;
					}
				}//end while loop
				if(exitDemarcPos==Point(-1,-1) && demarcFlag==1)
					exitDemarcPos = Point(step,pt.y);
				//printf() for debugging

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
								result.at<uchar>(i,k) = 255;
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
	return result;
}

Mat ShapeMorph::kmeansCluster(Mat src) {
	deque<int> dataVec;
	deque<int> lumFlag(256,0);
	deque<int> lumVec;
	deque<Point> ptVec;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			int lum = src.at<uchar>(i,j);
			if(lum>0) {
				dataVec.push_back(lum);
				lumFlag.at(lum)++;
				ptVec.push_back(Point(j,i));
			}
		}
	}
	for(unsigned int i=0; i<lumFlag.size(); i++) {
		if(lumFlag.at(i)>0) {
			lumVec.push_back(i);
		}
	}

	Mat samples(dataVec.size(), 1, CV_32F);
	for(unsigned int i=0; i<dataVec.size(); i++) {
		samples.at<float>(i,0) = dataVec.at(i);
	}
	int clusterCount = round(sqrt(lumVec.size()/2));
	Mat labels;
	int attempts = 5;
	Mat centers;
	cout << "clusters: " << clusterCount << endl;
	cout << "input size: " << dataVec.size() << endl;
	cout << "flag size: " << lumVec.size() << endl;
	int compact = kmeans(samples,clusterCount,labels,TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	cout << "compactness: " << compact << endl;
	deque<int> centerCount(clusterCount,0);
	deque<deque<int> > ranges(clusterCount,deque<int>(2,0));
	for(unsigned int i=0; i<ranges.size(); i++) {
		for(unsigned int j=0; j<ranges.at(i).size(); j++) {
			if(j==0) {
				ranges.at(i).at(j) = 256;
			}
			else if(j==1) {
				ranges.at(i).at(j) = 0;
			}
		}
	}
	Mat origPos;
	jaysort(centers,origPos);
	for(int i=0; i<labels.rows; i++) {
		//output.push_back(labels.at<int>(i,0));
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
		printf("%f - %d - Min: %d, Max: %d\n",centers.at<float>(i,0),centerCount.at(i),ranges.at(i).at(0),ranges.at(i).at(1));
	}
	Mat result = Mat::zeros(src.rows, src.cols, src.type());
	Mat clusterImg = Mat::zeros(src.rows,src.cols,CV_8UC3);
	Vec3b red(0,0,255);
	Vec3b blue(255,0,0);
	Vec3b green(0,255,0);
	Vec3b yellow(0,255,255);
	Vec3b purple(255,0,255);
	Vec3b teal(255,255,0);
	for(unsigned int i=0; i<ptVec.size(); i++) {
		int idx = labels.at<int>(i,0);
		if(idx==(centers.rows-1) || idx==(centers.rows-2)) {
			result.at<uchar>(ptVec.at(i).y,ptVec.at(i).x) = 255;
		}
	}
	//imshow("img",clusterImg);
	//waitKey(0);
	return result;
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

Mat ShapeMorph::elementaryDilation(Mat src) {
	Mat result = src.clone();
	for(int i=0; i<result.rows; i++) {
		for(int j=0; j<result.cols; j++) {
			if(j==0) {
				result.at<uchar>(i,j+1) = max(result.at<uchar>(i,j),result.at<uchar>(i,j+1));
			}
			else if(j==(src.cols-1)) {
				result.at<uchar>(i,j-1) = max(result.at<uchar>(i,j),result.at<uchar>(i,j-1));
			}
			else {
				result.at<uchar>(i,j-1) = max(result.at<uchar>(i,j),result.at<uchar>(i,j-1));
				result.at<uchar>(i,j+1) = max(result.at<uchar>(i,j),result.at<uchar>(i,j+1));
			}
		}
	}
	return result;
}