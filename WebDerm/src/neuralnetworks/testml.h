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
	vector<String> shapeNames = {"Comp_Disc","Incomp_Disc","Comp_Donut","Incomp_Donut",
			"Default","Strip","Blotch","Excavated"};
	static bool THRESH_IMPORTED;
public:
	static String PARAM_PATH;
	TestML();
	Mat getData();
	Mat getLabels();
	Mat layers;
	void setLayerParams(Mat layers);
	Mat prepareImage(Mat sample, Size size=Size(0,0));
	Mat prepareMatSamples(vector<Mat> sampleVec);
	void importSamples(String folder, vector<Mat> &samples, Size size=Size(0,0));
	void importLabels(String folder, vector<Mat> &labels);
	void convertImagesToData(String folder,Mat outputLabels, Size size=Size(0,0));
	Mat convertToBinary(Mat input, int min=0, int max=255, int newMin=0, int newMax=255);

	Mat runANN(String param, vector<Mat> sampleVec);

	void importTrainingData(String samplePath, String labelPath, Size size=Size(0,0));

	String getShapeName(int num);
	int numOfShapes();
};

#endif /* TESTML_H_ */
