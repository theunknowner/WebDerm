/*
 * main.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "rgb/rgb.h"
#include "test.h"
#include "hsl/hsl.h"
#include "Hsv/hsv.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "scripts/scripts.h"
#include "Math/maths.h"
#include "FileData/filedata.h"
#include "Histogram/histogram.h"
#include "Shades/shades.h"
#include "Entropy/entropy.h"
#include "Shape/circle.h"
#include "Shape/shape.h"
#include "neuralnetworks/shapeml.h"
#include "neuralnetworks/testml.h"
#include "Shape/shapemorph.h"
#include "Algorithms/cluster.h"
#include "Algorithms/kneecurve.h"
#include "GridDisplay/griddisplay.h"
#include "Poly/poly.h"
#include "Matlab/matlab.h"
#include "test.h"
#include "Shape/shapecolor.h"
#include "Colorspace/xyz.h"
#include "Colorspace/cielab.h"
#include "Create/createtrainingdata.h"
#include "Draw/draw.h"

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
	String name = "custom15";
	img = imread("/home/jason/Desktop/Programs/Training Data Pairs/"+name+".png");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	ShapeMorph sm;
	ShapeColor sc;
	Size size(5,5);
	//blur(img,img,size);
	cvtColor(img,imgGray,CV_BGR2GRAY);
	/*
	CreateTrainingData ctd;
	String inDir = "/home/jason/Desktop/Programs/Training Samples/Non_Similiar_Pairs/";
	String outDir = "/home/jason/Desktop/Programs/Training Samples/Negative_Pairs/";
	ctd.generateStitchPairs(inDir,outDir);
/**/
/*
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat map = sc.getShapeUsingColor2(img,mapOfNonNoise);
	Mat maskEmax = sc.removeRunningLines(map,Size(3,1));
	Mat maskLC;
	src.copyTo(maskLC,mapOfNonNoise);
	maskLC = sc.filterKneePt(maskLC);
	src.copyTo(maskLC,maskEmax);
	img2 = maskLC * 255;
	img2 = sm.densityConnector(img2,0.9999);
	deque<Mat> islands = sm.liquidFeatureExtraction(img2,0.0,1);
	img3 = sm.haloTransform(islands.at(0));
	img3.convertTo(img3,CV_8U);
	imgGray.copyTo(img4,img3);
	//img5 = sc.applyDiscreteShade(img4);
/*
	imgshow(img);
	imgshow(img2);
	imgshow(img3);
	imgshow(img4);
	imgshow(img5);
	 */
/*
	CreateTrainingData ctd;
	Mat src1 = imread("/home/jason/Desktop/Programs/Training Samples/tinea_corporis4-Point(35,0).png",0);
	Mat src2 = imread("/home/jason/Desktop/Programs/Training Samples/tinea_corporis11-Point(35,0).png",0);
	Mat stitch = ctd.stitchData(src1,src2);
	imwrite("test16.png",stitch);
	//String folder = "/home/jason/Desktop/Programs/Training Samples/";
	//String file = "/home/jason/Desktop/Programs/Training Samples/custom15-Point(35,35).png";

	//vector<String> list;
	//ctd.makeFalsePairs(folder, file, 450, list);
/*
	img5 = ctd.maxDimensionCrop(img4);
	img5 = runResizeImage(img5,Size(70,70));
	img5 = sc.applyDiscreteShade(img5);
	ctd.mouseSelectSegment(img5,Size(35,35),name);
/**/
/*
	TestML ml;
	String samplesPath = "/home/webderm/Files/Run/Positive_Pairs/";
	String samplesPath2 = "/home/webderm/Files/Run/Negative_Pairs/";
	vector<double> myLabels(1,0);
	vector<double> myLabels2(1,0);
	for(unsigned int i=0; i<myLabels.size(); i++) {
		if(i==0) {
			myLabels.at(i)=1;
			myLabels2.at(i)=-1;
		}
		else {
			myLabels.at(i)=-1;
			myLabels2.at(i)=1;
		}
	}
	ml.convertImagesToData(samplesPath,myLabels);
	Mat data1 = ml.getData();
	Mat labels1 = ml.getLabels();
	ml.convertImagesToData(samplesPath2,myLabels2);
	Mat data2 = ml.getData();
	Mat labels2 = ml.getLabels();

	Mat data, labels;
	vconcat(data1,data2,data);
	vconcat(labels1,labels2,labels);
	//ml.writeData(samplesPath+"data_set.csv",data,labels);
/**/
/*
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training Samples/Positive_Pairs/";
	fd.getFilesFromDirectory(folder,files);
	TestML ml;
	String param = "/home/jason/Desktop/Programs/Training Samples/Test Data/param.xml";
	Mat sample = imread("/home/jason/Desktop/Programs/Training Samples/Positive_Pairs/sample_116.png",0);
	vector<Mat> sampleVec;
	//for(unsigned int i=0; i<files.size(); i++) {
		//Mat sample = imread(folder + files.at(i),0);
		Mat translatedImg = ml.tempFixPrepareImg(sample);
		sampleVec.push_back(translatedImg);
	//}
	Mat results = ml.runANN(param,sampleVec);
	cout << results << endl;
/*
	int count=0;
	int realTotal=0;
	for(int i=0; i<results.rows; i++) {
		if(files.at(i).find("copy")==string::npos)
			realTotal++;
		if(results.at<float>(i,0)<0) {
			if(files.at(i).find("copy")==string::npos) {
				count++;
				printf("%s: Results: ", files.at(i).c_str());
				printf("[%f]\n",results.at<float>(i,0));
			}
		}
	}
	cout << count << endl;
	cout << realTotal << endl;
	/**/
	/*
	deque<String> files;
	FileData fd;
	String folder = "/home/jason/git/Samples/Samples/Training/Circles/";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String oldname = folder+files.at(i);
		String newname = folder+"random(0"+toString(i+1)+").png";
		cout << newname << endl;
		rename(oldname.c_str(),newname.c_str());
	}
/**/
/*
	//TestML ml;
	//vector<vector<double> > trainingData;
	//vector<vector<double> > trainingLabels;
	//ml.importCsvData("/home/jason/git/Samples/Samples/training_set.csv",trainingData,trainingLabels);
	//Mat training_set(sampleSize,inputSize,CV_32F);
	//Mat training_labels(sampleSize,outputSize,CV_32F);
	//ml.vecToMat(trainingData,trainingLabels,training_set,training_labels);
	Mat training_set = data;
	Mat training_labels = labels;
	int sampleSize = training_set.rows;
	int inputSize = training_set.cols;
	int outputSize = training_labels.cols;
	int hiddenNodes = 140;
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);

	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 4000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/webderm/Files/Run/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	String name = "psoriasis18";
	String file = "/home/jason/Desktop/workspace/True_Positive_Pairs.csv";
	String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
	Entropy en;
	en.importEntropyThresholds();
	en.setDebugLevel(true);
	//en.runCompareEntropy(name);
	//en.runCompareEntropy2(name);
	en.test_runCompareEntropy2a(name);
	//en.runCompareEntropyList(file,folder);
	//en.runCompareEntropyList2(file,folder);
	//en.runCompareAllEntropy(folder);
	//en.runCompareAllEntropy2(folder);
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
	/*
	namedWindow(fd.filename+"_Squares",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//namedWindow(fd2.filename+"_Squares2",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	imshow(fd.filename+"_Squares",img3);
	//imshow(fd2.filename+"_Squares2",img4);
	waitKey(0);
	 */
	return 0;
}

