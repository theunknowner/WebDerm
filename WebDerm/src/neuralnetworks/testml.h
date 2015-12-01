/*
 * testml.h
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#ifndef TESTML_H_
#define TESTML_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/stdafx.h"

class TestML {
private:
	Mat data;
	Mat labels;
	static vector<String> shapeNames;
	static map<String,int> shapeNamesMap;
	static bool THRESH_IMPORTED;
	static Size img_size;
	static vector<String> shapeNames2;
	static vector<CvANN_MLP *> cvAnnVec;
	static map<int,CvANN_MLP *> cvAnnVec2;
	static bool importParam;

public:
	static String PARAM_PATH;
	TestML(bool import=true);
	bool importThresholds();
	Mat getData();
	Mat getLabels();
	Mat layers;
	void setLayerParams(Mat layers);
	Mat prepareImage(Mat sample, Size size=Size(0,0));
	Mat prepareMatSamples(vector<Mat> sampleVec);
	void importSamples(String folder, vector<Mat> &samples, Size size=Size(0,0));
	void importLabels(String folder, vector<Mat> &labels);

	Mat runANN(String param, vector<Mat> sampleVec);

	void importTrainingData(String samplePath, String labelPath, Size size=Size(0,0));

	String getShapeName(int num);
	int numOfShapes();
	Size getSize();
	int getShapeIndex(String shape);

	Mat runANN2(vector<Mat> sampleVec);
	String getShapeName2(int num);
	int getIndexContainingShape(String shape);

	Mat runANN2b(vector<Mat> sampleVec, int nnShape);

	static void clear();
};

#endif /* TESTML_H_ */
