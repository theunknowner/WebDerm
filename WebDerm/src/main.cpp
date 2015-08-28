/*
 * main.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "rgb/rgb.h"
#include "test.h"
#include "hsl/hsl.h"
#include "Hsv/hsv.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "MyScripts/myscripts.h"
#include "Math/maths.h"
#include "FileData/filedata.h"
#include "Histogram/histogram.h"
#include "Shades/shades.h"
#include "Entropy/entropy.h"
#include "Shape/circle.h"
#include "neuralnetworks/testml.h"
#include "Shape/shapemorph.h"
#include "Cluster/cluster.h"
#include "KneeCurve/kneecurve.h"
#include "GridDisplay/griddisplay.h"
#include "Poly/poly.h"
#include "Matlab/matlab.h"
#include "test.h"
#include "Shape/shapecolor.h"
#include "Colorspace/xyz.h"
#include "Colorspace/cielab.h"
#include "Create/createtrainingdata.h"
#include "Draw/draw.h"
#include "Color/color.h"
#include "Algorithms/write.h"
#include "CIE/cie.h"
#include "ShadeShape/shadeshape.h"
#include "Mouse/mouse.h"
#include "ShadeShape/ShadeShapeMatch/shadeshapematch.h"
#include "Algorithms/jaysort.h"
#include "Algorithms/read.h"
#include "ShadeShape/ShadeMatch/shadematch.h"
#include "ImageData/imagedata.h"
#include "ShadeShape/ShadeShapeRelation/shadeshaperelation.h"
#include "ShadeShape/Labels/labels.h"
#include "Timer/timer.h"

int main(int argc,char** argv)
{
	//Entropy en;
	//en.importEntropyThresholds();
	//en.runAllEntropy();
	//en.runEntropy();
	//runRenameFiles();
	//runAllHysteresis();
	//runHysteresis();
	//runMouseColor();
	//runResizeAllImages();
	//Scripts::script3();
	/*
	String name = "melanoma7";
	Scripts::script27(name);
	Scripts::script30(name);
/**/
/*
	ShadeShape ss1 = Scripts::script2("/home/jason/Desktop/workspace/test6.png");
	ShadeShape ss2 = Scripts::script2("/home/jason/Desktop/workspace/test7.png");
	ShadeShapeMatch ssm;
	//ssm.test(ss1);
	//ssm.debug_mode(2);
	//cout << ssm.test_match(ss1,ss2) << endl;
	vector<float> results = ssm.match(ss1,ss2);
	printf("TR1: %f x TR2: %f = %f]\n",results.at(1),results.at(2),results.at(0));
	//ss1.showInteractiveIslands();
	//ssm.test(ss1);
	//Islands island = ss1.getIslandWithPoint(Point(48,68));
	//imwrite("comp_disc.png",island.image());
/**/
/*
	ShadeShape ss1 = Scripts::script31("tinea_corporis13a");
	//ShadeShapeMatch ssm;
	//ssm.test(ss1);
	Islands island = ss1.getIslandWithPoint(Point(46,88));
	imwrite("strip.png",island.image());
	//cout << island.nn_results() << endl;
/**/
/*
	Timer time;
	String folder = "Looks_Like/";
	deque<String> files;
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	ShadeShape ss1 = Scripts::script31(argv[1]);
	vector<vector<float> > resultVec;
	vector<String> nameVec;
	vector<int> origPos;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		if(name!=argv[1]) {
			ShadeShape ss2 = Scripts::script31(name);
			ShadeShapeMatch ssm;
			if(argc>=3)
				ssm.debug_mode(atoi(argv[2]));
			vector<float> results = ssm.match(ss1,ss2);
			nameVec.push_back(name);
			resultVec.push_back(results);
		}
	}
	jaysort(resultVec,origPos,1);
	String output = std::string(argv[1]) + "_matches_sorted.csv";
	FILE * fp;
	fp = fopen(output.c_str(),"w");
	for(unsigned int i=0; i<resultVec.size(); i++) {
		fprintf(fp,"%s,%f,%f,%f\n",nameVec.at(origPos.at(i)).c_str(),resultVec.at(i).at(0),resultVec.at(i).at(1),resultVec.at(i).at(2));
	}
	fclose(fp);
	printf("%s...Done!\n",argv[1]);
	time.end();
	time.printTimer();
	/**/
/*
	Timer time;
	ShadeShape ss1 = Scripts::script31(argv[1]);
	ShadeShape ss2 = Scripts::script31(argv[2]);
	ShadeShapeMatch ssm;
	if(argc==4)
		ssm.debug_mode(atoi(argv[3]));
	//ssm.test_match(ss1,ss2);
	vector<float> results = ssm.match(ss1,ss2);
	printf("TR1: %f x TR2: %f = %f]\n",results.at(1),results.at(2),results.at(0));
	time.end();
	time.printTimer();
	/**/
	//Scripts::script25();
	//Scripts::script_checkAllTestData();
	//Scripts::script_createTestDataList();
	//Scripts::script_checkHitRatioTestData();
	/*
	FileData fd;
	String folder = "/home/jason/git/Samples/Samples/Training/Circles-Disc-Incomplete/";
	fd.fixFileNumberSequence(folder,"(",3);
/*
	deque<String> files;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	String name = "circle_disc_incomp(";
	for(unsigned int i=0; i<files.numOfFeatures(); i++) {
		String filename = folder+files.at(i);
		String newFilename = folder+name+toString(i+1)+").png";
		rename(filename.c_str(),newFilename.c_str());
	}
/**/
	/*
	TestML ml;
	String param = TestML::PARAM_PATH;
	Mat sample = imread("/home/jason/Desktop/workspace/test10.png",0);
	sample *= 255;
	imgshow(sample);
	sample = ml.prepareImage(sample,Size(20,20));
	imgshow(sample);
	vector<Mat> sampleVec;

	sampleVec.push_back(sample);
	Mat results = ml.runANN(param,sampleVec);
	cout << results << endl;
/**/
/*
	Scripts::script_createAllTrainingLabels();
	sleep(3);
	TestML ml;
	String path1 = "/home/jason/git/Samples/Samples/Training/samples_path.csv";
	String path2 = "/home/jason/git/Samples/Samples/Training/labels_path.csv";
	ml.importTrainingData(path1,path2,Size(20,20));
	Mat data = ml.getData();
	Mat labels = ml.getLabels();
	Mat training_set = data;
	Mat training_labels = labels;
	int sampleSize = training_set.rows;
	int inputSize = training_set.cols;
	int outputSize = training_labels.cols;
	int hiddenNodes = 20;
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);

	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 4000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/jason/git/Samples/Samples/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	String name = "vesicles25";
	//String file = "/home/jason/Desktop/workspace/True_Positive_Pairs.csv";
	String folder = "/home/jason/Desktop/Programs/Demo/YSV_Output/";
	Entropy en;
	en.importEntropyThresholds();
	//en.setDebugMode(true);
	//en.runCompareEntropy(name);
	//en.runCompareEntropy2(name);
	//en.test_runCompareEntropy2a(name);
	//en.runCompareEntropyList(file,folder);
	//en.runCompareEntropyList2(file,folder);
	//en.runCompareAllEntropy(folder);
	//en.runCompareAllEntropy2(folder);
	String list = "/home/jason/Desktop/workspace/list.csv";
	fstream fs(list);
	while(getline(fs,list)) {
		String file = folder + list + "-5x5-YSV-Combined10x10.csv";
		en.test_runAllCompareEntropy2a(folder,file);
	}
	//en.test_runAllCompareEntropy2a(folder);

	/**/
	/*
	Entropy en;
	FileData fd;
	fd.filename = "lph15";
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Test_Output/"+fd.filename+"-ShadeColors-5x5.csv",fd.colorVec);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Test_Output/"+fd.filename+"-HSL-5x5.csv",fd.hslMat);
	fd.ksize = Size(5,5);
	Size entSize(10,10);
	String targetColor = "Grey";
	String targetShade = "Low";
	en.eyeFn(fd,entSize,targetColor,targetShade);
	img3 = c.output2ImageTargetColor(fd.colorVec,fd.ksize,fd.filename,targetColor,targetShade);
	img3 = en.showEyeFnSquares(img3,entSize,targetColor,targetShade);
	imgshow(img3);
	//Mouse::mouseOutputColor(img3,fd);
/*
	FileData fd2;
	fd2.filename = "herpes-zoster1";
	fd2.loadFileMatrix("/home/jason/Desktop/Programs/Output2/"+fd2.filename+"_ShadeColors_5x5.csv",fd2.colorVec);
	fd2.loadFileMatrix("/home/jason/Desktop/Programs/Output2/"+fd2.filename+"_HSL_5x5.csv",fd2.hslMat);
	fd2.ksize = fd.ksize;
	String targetColor2 = "Brown";
	String targetShade2 = "Dark";
	en.eyeFn(fd2,entSize,targetColor2);
	//pEnt2 = en.outputCombinedSigmoid(fd2,Size(10,10),a,b,p);
	//pEnt2 = en.outputCombinedEntropy(fd2,entSize);
	img4 = c.output2ImageTargetColor(fd2.colorVec,fd2.ksize,fd2.filename,targetColor2,targetShade2);
	//img4 = en.showEntropySquares(img4,entSize);
	img4 = en.showEyeFnSquares(img4,entSize,targetColor2,targetShade2);
	//Mouse::mouseOutputColor(img4,fd2);
/**/
	return 0;
}

