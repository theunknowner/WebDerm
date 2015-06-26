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
#include "Shape/shape.h"
#include "neuralnetworks/testml.h"
#include "Shape/shapemorph.h"
#include "Algorithms/cluster.h"
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
#include "Shape/ShadeShape/shadeshape.h"
#include "Mouse/mouse.h"
#include "Shape/ShapeMatch/shapematch.h"

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
	//Scripts::script28a();
	//Scripts::script27();
	Scripts::script30();
	/*theRNG().state = INIT_SEED_VALUE;
	ShadeShape ss1 = Scripts::script31("tinea_corporis8a");
	theRNG().state = INIT_SEED_VALUE;
	ShadeShape ss2 = Scripts::script31("tinea_corporis8b");
	ShapeMatch smatch;
	smatch.match(ss1,ss2);*/
/*
	Rgb rgb;
	Hsl hsl;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "lph4";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	ShapeMorph sm;
	ShapeColor sc;
	Size numOfFeatures(5,5);
	//blur(img,img,numOfFeatures);
	//cvtColor(img,imgGray,CV_BGR2GRAY);

/*
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	String out = "/home/jason/Desktop/Programs/Discrete/";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.numOfFeatures(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));
		//blur(img,img,numOfFeatures);

		cvtColor(img,imgGray,CV_BGR2GRAY);
		Mat src = sm.prepareImage(imgGray);
		Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
		/*Mat maskE = sc.getShapeUsingColor(img,mapOfNonNoise);
		Mat nnConnect = sm.densityConnector(maskE,0.9999);
		Mat transform = sm.haloTransform(nnConnect);
		transform.convertTo(transform,CV_8U);
		maskE = (transform - 5) * 255;
*/
/*
		Mat lcFilterMat = sc.filterKneePt(src);
		Mat lcFilterNoNoise;
		lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
		Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
		Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
		lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
		lcHaloTrans = (lcHaloTrans - 5) * 255;
		img.copyTo(img2,lcHaloTrans);
		//img.copyTo(img3,maskE);
		//img2.copyTo(img3,img2);
		//imgshow(img2);
		//imgshow(img3,1,name);
		img3 = sc.applyDiscreteShade(imgGray);
		img3.copyTo(img4,lcHaloTrans);
		imwrite(out+name+".png",img2);
		imwrite(out+name+"_discrete.png",img4);
		img2.release();
		img4.release();
	}
/*
	//sc.setDebugLevel(1);
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	//Mat maskE = sc.getShapeUsingColor(img,mapOfNonNoise);
	//img.copyTo(img2,maskE);
	//imgshow(img2);
/*
	//maskE = sc.removeRunningLines(maskE,Size(3,1));
	Mat nnConnect2 = sm.densityConnector(maskE,0.9999);
	Mat transform2 = sm.haloTransform(nnConnect2,2);
	transform2.convertTo(transform2,CV_8U);
	maskE = (transform2 - 5) * 255;
	img.copyTo(img5,maskE);
*/
/*
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	img.copyTo(img2,lcHaloTrans);
	//img5.copyTo(img3,img5);
	//img2.copyTo(img3,img2);
	imgshow(lcFilterMat);
	imgshow(src);
	imgshow(img2);
	//imgshow(img2);
	//imgshow(img3);

	img5 = sc.applyDiscreteShade(imgGray);
	imgshow(img5);
*/
	//Scripts::script25();
	//Scripts::script_checkAllTestData();
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
	String param = "/home/jason/git/Samples/Samples/param.xml";
	Mat sample = imread("/home/jason/Desktop/workspace/TestNN/random_test4.png",0);
	sample *= 255;
	imgshow(sample);
	sample = ml.prepareImage(sample,Size(20,20));
	imgshow(sample);
	vector<Mat> sampleVec;

	sampleVec.push_back(sample);
	Mat results = ml.runANN(param,sampleVec);
	cout << results << endl;
/**/
	//Scripts::script_createTrainingLabels();
	//Scripts::script_createAllTrainingLabels();
/*
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

