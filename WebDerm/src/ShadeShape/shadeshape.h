/*
 * shadeshape.h
 *
 *  Created on: Jun 4, 2015
 *      Author: jason
 */

#ifndef SHADESHAPE_H_
#define SHADESHAPE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
//#include "/home/jason/git/WebDerm/WebDerm/src/ImageData/imagedata.h"
#include "features.h"

class Features;
class Islands;
class ImageData;

class ShadeShape {
private:
	String ss_name;
	vector<Features> featureVec;
	vector<int> shadeVec;
	vector<int> areaVec;
	int numOfFeats;
	int ssArea;
	int ssAreaPostDensityConnector;
	Mat img;
	ImageData id;

	bool isBridgeWeak(Mat &src, int x, int y);
	bool isOnTheEdge(Mat &src, int x, int y);
	bool isUnitBridged(Mat &src, int x, int y);
	vector<Mat> extractFeatures(Mat src);
	vector<ImageData> extractFeatures(ImageData &id);
	void storeFeature(Features feature);
	void getShadesOfFeatures(Mat src);
	void removeDuplicatePointsFromIslands();
	void storeIslandAreas();

public:
	ShadeShape();
	ShadeShape(Mat src, String name="");
	ShadeShape(ImageData &id, bool disconnectIslands=false);
	void extract(Mat src, String name="");
	void extract(ImageData &id, bool disconnectIslands=false);
	Features& feature(int featNum);
	int numOfFeatures();
	int shade(int num);
	int numOfShades();
	int getIndexOfShade(int shade);
	int area();
	int areaPostDensityConnector();
	Mat& image();
	String& name();
	vector<int> get_shades();
	void release();
	Islands getIslandWithPoint(Point pt);
	int getMaxArea();
	void showInteractiveIslands();
	void set_island_shade(int featNum, int islNum, int newShade);
	ImageData& getImageData();

	vector<Mat> isolateConnectedFeatures(Mat src);

	void showInteractiveSubIslands();

};

#endif /* SHADESHAPE_H_ */
