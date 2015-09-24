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
	static bool THRESH_IMPORTED;
	static Size img_size;
public:
	static String PARAM_PATH;
	TestML(int import=1);
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
};

#endif /* TESTML_H_ */
