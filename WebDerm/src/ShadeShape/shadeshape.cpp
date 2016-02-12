/*
 * shadeshape.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#include "shadeshape.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "islands.h"
#include "features.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"
#include "/home/jason/git/WebDerm/WebDerm/src/ImageData/imagedata.h"

using namespace ip;

String winName = "Interactive Islands";
String winName2 = "Extracted";
String winName3 = "40x40";
void onMouseCheckIslands(int event, int x, int y, int flags, void* param) {
	ShadeShape &ss = *((ShadeShape*)param);
	Mat img = ss.image().clone();
	Islands island;
	static TestML ml;
	if  ( event == EVENT_LBUTTONDOWN ){
		island = ss.getIslandWithPoint(Point(x,y));
		if(!island.isEmpty()) {
			char text[100];
			int lum = img.at<uchar>(y,x);
			int area = island.area();
			int shadeNum = ss.getIndexOfShade(island.shade());
			float nnResult = *max_element(island.nn_results().begin<float>(),island.nn_results().end<float>());
			cvtColor(img,img,CV_GRAY2BGR);
			for(auto it=island.coordinates().begin(); it!=island.coordinates().end(); it++) {
				int x = it->second.x;
				int y = it->second.y;
				img.at<Vec3b>(y,x) = Vec3b(0,255,0);
			}
			String shade_shape = island.shape_name() + "_s" + toString(shadeNum);
			sprintf(text,"(%d,%d) | Lum: %d | Area: %d | ShadeShape: %s | NN: %f",x,y,lum,area,shade_shape.c_str(),nnResult);
			cv::displayStatusBar(winName,text);
			char textScore[100];
			char textScore2[20];
			Mat nnResults = island.nn_results();
			sprintf(textScore,"[%.5f, %.5f, %.5f, %.5f, %.5f]",nnResults.at<float>(0,0),nnResults.at<float>(0,1),nnResults.at<float>(0,2),nnResults.at<float>(0,3),nnResults.at<float>(0,4));
			sprintf(textScore2,"[%.5f]",island.nn_score_2());
			namedWindow(winName2, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
			cv::displayStatusBar(winName2,textScore);
			imshow(winName2,island.image());
			namedWindow(winName3, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
			cv::displayStatusBar(winName3,textScore2);
			imshow(winName3,island.nn_image());
		}
	}
	if(event == EVENT_LBUTTONUP) {
		img = ss.image().clone();
	}
	imshow(winName,img);
}

void onMouseCheckSubIslands(int event, int x, int y, int flags, void* param) {
	ShadeShape &ss = *((ShadeShape*)param);
	Mat img = ss.image().clone();
	Islands island;
	static TestML ml;
	bool islandExist = false;
	if  ( event == EVENT_LBUTTONDOWN ){
		island = ss.getIslandWithPoint(Point(x,y));
		if(!island.isEmpty()) {
			char text[100];
			int lum = img.at<uchar>(y,x);
			int area = island.area();
			int shadeNum = ss.getIndexOfShade(island.shade());
			float nnResult = *max_element(island.nn_results().begin<float>(),island.nn_results().end<float>());
			cvtColor(img,img,CV_GRAY2BGR);
			for(auto it=island.coordinates().begin(); it!=island.coordinates().end(); it++) {
				int x = it->second.x;
				int y = it->second.y;
				img.at<Vec3b>(y,x) = Vec3b(0,255,0);
			}
			String shade_shape = island.shape_name() + "_s" + toString(shadeNum);
			sprintf(text,"(%d,%d) | Lum: %d | Area: %d | ShadeShape: %s | NN: %f | %d",x,y,lum,area,shade_shape.c_str(),nnResult,ss.area());
			cv::displayStatusBar(winName,text);
			//char textScore[100];
			//Mat nnResults = island.nn_results();
			//sprintf(textScore,"[%.5f, %.5f, %.5f, %.5f, %.5f]",nnResults.at<float>(0,0),nnResults.at<float>(0,1),nnResults.at<float>(0,2),nnResults.at<float>(0,3),nnResults.at<float>(0,4));
			//namedWindow(winName3, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
			//imshow(winName3,island.image());
			//namedWindow(winName2, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
			//cv::displayStatusBar(winName2,textScore);
			//imshow(winName2,island.nn_image());
			islandExist = true;
		}
	}
	if(event == EVENT_LBUTTONUP) {
		img = ss.image().clone();
	}
	imshow(winName,img);
	if(islandExist) {
		island.showInteractiveSubIslands();
	}
}

/******************** PRIVATE FUNCTIONS **********************/
bool ShadeShape::isOnTheEdge(Mat &src, int x, int y) {
	Size size(3,3);
	int beginCol = x - floor(size.width/2);
	int beginRow = y - floor(size.height/2);
	int endCol = x + floor(size.width/2);
	int endRow = y + floor(size.height/2);
	if(beginCol<0) beginCol = x;
	if(beginRow<0) beginRow = y;
	if(endCol>=src.cols) endCol = x;
	if(endRow>=src.rows) endRow = y;
	Mat rec = src(Rect(beginCol,beginRow,size.width,size.height));
	int leftEdge = countNonZero(rec.col(0));
	int rightEdge = countNonZero(rec.col(rec.cols-1));
	int topEdge = countNonZero(rec.row(0));
	int bottomEdge = countNonZero(rec.row(rec.rows-1));
	if(leftEdge==0) return true;
	if(rightEdge==0) return true;
	if(topEdge==0) return true;
	if(bottomEdge==0) return true;
	return false;
}

bool ShadeShape::isUnitBridged(Mat &src, int x, int y) {
	Size size(3,3);
	Point ptArr[4][2] = {
			{Point(0,0), Point(2,2)},
			{Point(1,0), Point(1,2)},
			{Point(2,0),Point(0,2)},
			{Point(0,1),Point(2,1)}
	};

	int beginCol = x - 1;
	int beginRow = y - 1;
	int endCol = x + 1;
	int endRow = y + 1;
	Mat rec(size,CV_8U,Scalar(0));
	if(beginCol<0) {
		for(int n=0; n<size.height; n++) {
			rec.at<uchar>(n,0) = 1;
		}
		beginCol = x;
	}
	if(endCol>=src.cols) {
		for(int n=0; n<size.height; n++) {
			rec.at<uchar>(n,2) = 1;
		}
		endCol = x;
	}
	if(beginRow<0) {
		for(int m=0; m<size.width; m++) {
			rec.at<uchar>(0,m) = 1;
		}
		beginRow = y;
	}
	if(endRow>=src.rows) {
		for(int m=0; m<size.width; m++) {
			rec.at<uchar>(2,m) = 1;
		}
		endRow = y;
	}
	for(int i=beginRow; i<=endRow; i++) {
		for(int j=beginCol; j<=endCol; j++) {
			rec.at<uchar>(i-beginRow,j-beginCol) = src.at<uchar>(i,j);
		}
	}
	for(int i=0; i<4; i++) {
		int val1 = rec.at<uchar>(ptArr[i][0]);
		int val2 = rec.at<uchar>(ptArr[i][1]);
		if(val1>0 && val2>0) return true;
	}

	return false;
}

//! should only be used if isUnitBridged() returns true
bool ShadeShape::isBridgeWeak(Mat &src, int x, int y) {
	Size size(3,3);
	int beginCol = x - floor(size.width/2);
	int beginRow = y - floor(size.height/2);
	int endCol = x + floor(size.width/2);
	int endRow = y + floor(size.height/2);
	if(beginCol<0) beginCol = x;
	if(beginRow<0) beginRow = y;
	if(endCol>=src.cols) endCol = x;
	if(endRow>=src.rows) endRow = y;
	double thresh = 2.55;
	Mat kernel(size,CV_32F,Scalar(0));
	for(int i=0; i<kernel.rows; i++) {
		for(int j=0; j<kernel.cols; j++) {
			float dist = MyMath::eucDist(Point(j,i),Point(1,1));
			if(dist!=0) {
				float val = 1.0/(dist*2.0);
				kernel.at<float>(i,j) = roundDecimal(val,2);
			}
		}
	}
	float total = 0, sum=0;
	for(int i=beginRow; i<=endRow; i++) {
		for(int j=beginCol; j<endCol; j++) {
			total += kernel.at<float>(i-beginRow,j-beginCol);
			if(src.at<uchar>(i,j)>0) sum+= kernel.at<float>(i-beginRow,j-beginCol);
		}
	}
	if(sum<=0.75*total)
		return true;

	return false;
}

vector<Mat> ShadeShape::extractFeatures(Mat src) {
	ShapeMorph sm;
	vector<Mat> featureVec = sm.liquidFeatureExtraction(src,0,0,0);
	return featureVec;
}

vector<ImageData> ShadeShape::extractFeatures(ImageData &id) {
	ShapeMorph sm;
	vector<ImageData> featureVec = sm.liquidFeatureExtraction(id,0,0,0);
	return featureVec;
}

void ShadeShape::storeFeature(Features feature) {
	this->featureVec.push_back(feature);
}

//! gets the unique shade values of the islands and stores them in a vector
void ShadeShape::getShadesOfFeatures(Mat src, int debugSym) {
	int maxVal = *max_element(src.begin<uchar>(),src.end<uchar>());
	vector<int> shadeVec(maxVal+1,0);
	for(int i=0; i<this->numOfFeatures(); i++) {
		for(int j=0; j<this->feature(i).numOfIslands(); j++) {
			shadeVec.at(this->feature(i).island(j).shade())++;
		}
	}
	for(unsigned int i=0; i<shadeVec.size(); i++) {
		if(shadeVec.at(i)>0) {
			this->shadeVec.push_back(i);
		}
	}
	if(debugSym==1) {
		cout << this->shadeVec.size() << endl;
	}
}

void ShadeShape::removeDuplicatePointsFromIslands() {
	Mat img = this->img;
	for(int i=0; i<this->numOfFeatures(); i++) {
		for(int j=0; j<this->feature(i).numOfIslands(); j++) {
			Islands isl = this->feature(i).island(j);
			map<String,Point> coordMap = isl.coordinates();
			for(auto it=coordMap.begin(); it!=coordMap.end(); it++) {
				int col = it->second.x;
				int row = it->second.y;
				int val = img.at<uchar>(row,col);

				if(isl.shade()!=val) {
					coordMap.erase(it->first);
				}
			}
			for(auto it=coordMap.begin(); it!=coordMap.end(); it++) {
				int col = it->second.x;
				int row = it->second.y;
				int val = img.at<uchar>(row,col);

				if(isl.shade()!=val) {
					coordMap.erase(it->first);
				}
			}
			this->feature(i).island(j).coordinates() = coordMap;
		}
	}
}

void ShadeShape::storeIslandAreas() {
	for(int i=0; i<this->numOfFeatures(); i++) {
		for(int j=0; j<this->feature(i).numOfIslands(); j++) {
			Islands isl = this->feature(i).island(j);
			this->areaVec.push_back(isl.area());
		}
	}
}

/******************** PUBLIC FUNCTIONS *********************/

ShadeShape::ShadeShape() {}

ShadeShape::ShadeShape(Mat src, String name, int debugSym) {
	this->extract(src,name,debugSym);
}

ShadeShape::ShadeShape(ImageData &id, bool disconnectIslands, int debugSym) {
	this->extract(id, disconnectIslands,debugSym);
}
//! extracts the features from the image
void ShadeShape::extract(Mat src, String name, int debugSym) {
	this->ss_name = getFileName(name);
	this->img = src.clone();
	vector<Mat> featureVec = this->extractFeatures(src);
	for(unsigned int i=0; i<featureVec.size(); i++)  {
		Features feature(featureVec.at(i),this->id);
		this->storeFeature(feature);
	}
	this->numOfFeats = this->featureVec.size();
	this->getShadesOfFeatures(src);
	this->storeIslandAreas();
	this->ssArea = countNonZero(src);
	this->ssAreaPostDensityConnector = std::accumulate(this->areaVec.begin(),this->areaVec.end(),0);
	this->removeDuplicatePointsFromIslands();
}

//! extracts the features from the image
void ShadeShape::extract(ImageData &id, bool disconnectIslands, int debugSym) {
	this->id = id;
	this->ss_name = getFileName(id.name());
	this->img = id.image().clone();
	this->ssArea = countNonZero(this->img);
	vector<Mat> featureVec = this->extractFeatures(this->img);
	for(unsigned int i=0; i<featureVec.size(); i++)  {
		Features feature(featureVec.at(i),this->id,disconnectIslands);
		this->storeFeature(feature);
	}
	this->numOfFeats = this->featureVec.size();
	this->getShadesOfFeatures(this->img);
	this->storeIslandAreas();
	this->ssAreaPostDensityConnector = std::accumulate(this->areaVec.begin(),this->areaVec.end(),0);
	this->removeDuplicatePointsFromIslands();

}

//! returns feature of [index]
Features& ShadeShape::feature(int featNum) {
	return this->featureVec.at(featNum);
}

//! returns number of features in image
int ShadeShape::numOfFeatures() {
	return this->numOfFeats;
}

//! returns shade value of [index]
int ShadeShape::shade(int num) {
	return this->shadeVec.at(num);
}

//! returns numbers of shades
int ShadeShape::numOfShades() {
	return this->shadeVec.size();
}

int ShadeShape::getIndexOfShade(int shade) {
	int index = std::find(this->shadeVec.begin(),this->shadeVec.end(),shade) - this->shadeVec.begin();
	return index;
}

int ShadeShape::area() {
	return this->ssArea;
}

//! returns total area of all islands post densityConnector()
int ShadeShape::areaPostDensityConnector() {
	return this->ssAreaPostDensityConnector;
}

Mat& ShadeShape::image() {
	return this->img;
}

String& ShadeShape::name() {
	return this->ss_name;
}

vector<int> ShadeShape::get_shades() {
	return this->shadeVec;
}

void ShadeShape::release() {
	for(int i=0; i<this->numOfFeatures(); i++) {
		this->feature(i).release();
	}
	this->featureVec.clear();
	this->featureVec.shrink_to_fit();
	this->shadeVec.clear();
	this->shadeVec.shrink_to_fit();
	this->img.release();
}

Islands ShadeShape::getIslandWithPoint(Point pt) {
	String coords = toString(pt.x)+","+toString(pt.y);
	for(int i=0; i<this->numOfFeatures(); i++) {
		for(int j=0; j<this->feature(i).numOfIslands(); j++) {
			Islands island = this->feature(i).island(j);
			if(island.coordinates().find(coords)!=island.coordinates().end()) {
				return this->feature(i).island(j);
			}
		}
	}
	return Islands();
}

//! returns the largest area of all the islands
int ShadeShape::getMaxArea() {
	return *max_element(this->areaVec.begin(),this->areaVec.end());
}

void ShadeShape::showInteractiveIslands() {
	winName = this->ss_name;
	namedWindow(winName, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	cv::setMouseCallback(winName,onMouseCheckIslands, this);
	imshow(winName,this->img);
	waitKey(0);
}

void ShadeShape::set_island_shade(int featNum, int islNum, int newShade) {
	Islands &island = this->feature(featNum).island(islNum);
	for(int i=0; i<island.image().rows; i++) {
		for(int j=0; j<island.image().cols; j++) {
			String coords = to_string(j) + "," + to_string(i);
			if(island.coordinates().find(coords)!=island.coordinates().end() && island.image().at<uchar>(i,j)>0) {
				this->img.at<uchar>(i,j) = newShade;
				island.image().at<uchar>(i,j) = newShade;
			} else {
				island.image().at<uchar>(i,j) = 0;
			}
		}
	}
}

ImageData& ShadeShape::getImageData() {
	return this->id;
}

vector<Mat> ShadeShape::isolateConnectedFeatures(Mat src) {
	Size size(3,3);
	vector<Point> ptsVec;

	for(int row=0; row<src.rows; row++) {
		for(int col=0; col<src.cols; col++) {
			if(src.at<uchar>(row,col)>0) {
				bool edged = this->isOnTheEdge(src,col,row);
				bool bridged = this->isUnitBridged(src,col,row);
				if(!edged && bridged) {
					bool weak = this->isBridgeWeak(src,col,row);
					if(weak)
						ptsVec.push_back(Point(col,row));
				}
			}
		}
	}
	Mat results = src.clone();
	for(unsigned int i=0; i<ptsVec.size(); i++) {
		results.at<uchar>(ptsVec.at(i)) = 0;
	}
	imgshow(src);
	imgshow(results);
	ShapeMorph sm;
	vector<Mat> isolatedFeatures = sm.liquidFeatureExtraction(results);
	return isolatedFeatures;
}

void ShadeShape::showInteractiveSubIslands() {
	winName = this->ss_name;
	namedWindow(winName, CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	cv::setMouseCallback(winName,onMouseCheckSubIslands, this);
	imshow(winName,this->img);
	waitKey(0);
}

