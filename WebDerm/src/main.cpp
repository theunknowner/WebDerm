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
#include "Color/color.h"
#include "Algorithms/write.h"

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

	Rgb rgb;
	Hsl hsl;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "tinea_corporis4";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	ShapeMorph sm;
	ShapeColor sc;
	Size size(5,5);
	//blur(img,img,size);
	cvtColor(img,imgGray,CV_BGR2GRAY);
	int row=79;
	String str = "_row[" + toString(row) + "]";
	writeSeq2File(imgGray.row(row),"uchar",name+str);
	String folder = "/home/jason/git/WebDerm/WebDerm/";
	String py_file = "python /home/jason/Desktop/workspace/Pyth/poly_arg.py ";
	String py_arg = folder+name+str+".csv ";
	String out_arg = folder+name+str+"_polyfit.csv";
	py_file += py_arg + out_arg;
	system(py_file.c_str());
/*
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	//Mat map = sc.getShapeUsingColor(img,mapOfNonNoise);
	//Mat maskEmax = sc.removeRunningLines(map,Size(3,1));
	//Mat maskLC;
	//src.copyTo(maskLC,mapOfNonNoise);
	//maskLC = sc.filterKneePt(maskLC);
	//sc.setDebugLevel(1);
	Mat maskLC2 = sc.getShapeUsingLumContrast(src,mapOfNonNoise);
	//src.copyTo(maskLC,maskLC2);
	//img2 = maskLC * 255;
	img2 = sm.densityConnector(maskLC2,0.9999);
	//deque<Mat> islands = sm.liquidFeatureExtraction(img2,0.0,1);
	img3 = sm.haloTransform(img2);
	img3.convertTo(img3,CV_8U);
	img4 = (img3 - 5) * 255;
	img.copyTo(img5,img4);
	//img5 = sc.applyDiscreteShade(img4);
	//imgshow(src);
	//imgshow(map);
	imgshow(maskLC2);
	imgshow(img2);
	imgshow(img3);
	imgshow(img4);
	imgshow(img5);
	//imwrite(name+".png",img5);

/*
	CreateTrainingData ctd;
	//Mat src1 = imread("/home/jason/Desktop/Programs/Training_Samples/tinea_corporis4-Point(35,0).png",0);
	//Mat src2 = imread("/home/jason/Desktop/Programs/Training_Samples/tinea_corporis11-Point(35,0).png",0);
	//Mat stitch = ctd.stitchData(src1,src2);
	//imwrite("test16.png",stitch);
	//String folder = "/home/jason/Desktop/Programs/Training_Samples/";
	//String file = "/home/jason/Desktop/Programs/Training_Samples/custom15-Point(35,35).png";

	//vector<String> list;
	//ctd.makeFalsePairs(folder, file, 450, list);

	img5 = ctd.maxDimensionCrop(img4);
	img5 = runResizeImage(img5,Size(70,70));
	img5 = sc.applyDiscreteShade(img5);
	ctd.mouseSelectSegment(img5,Size(35,35),name);
/**/
	/*
	TestML ml;
	String path1 = "/home/jason/git/Samples/Samples/Training/samples_path.csv";
	String path2 = "/home/jason/git/Samples/Samples/Training/labels_path.csv";
	ml.importTrainingData(path1,path2,Size(20,20));
	Mat data = ml.getData();
	Mat labels = ml.getLabels();
	/*
	TestML ml;
	String samplesPath = "/home/webderm/Files/Run/Positive_Pairs/";
	String samplesPath2 = "/home/webderm/Files/Run/Negative_Pairs/";
	String labelPath = "/home/webderm/Files/Run/Positive-Training-Labels.csv";
	String labelPath2 = "/home/webderm/Files/Run/Negative-Training-Labels.csv";
	deque<deque<String> > dataMat1;
	deque<deque<String> > dataMat2;
	FileData fd;
	fd.loadFileMatrix(labelPath,dataMat1);
	fd.loadFileMatrix(labelPath2,dataMat2);
	Mat positiveLabels(dataMat1.size(),1,CV_32F,Scalar(0));
	Mat negativeLabels(dataMat2.size(),1,CV_32F,Scalar(0));
	for(unsigned int i=0; i<dataMat1.size(); i++) {
		float val = atof(dataMat1.at(i).at(1).c_str());
		positiveLabels.at<float>(i,0) = val;
	}
	for(unsigned int i=0; i<dataMat2.size(); i++) {
		float val = atof(dataMat2.at(i).at(1).c_str());
		negativeLabels.at<float>(i,0) = val;
	}
	ml.convertImagesToData(samplesPath,positiveLabels);
	Mat data1 = ml.getData();
	Mat labels1 = ml.getLabels();
	ml.convertImagesToData(samplesPath2,negativeLabels);
	Mat data2 = ml.getData();
	Mat labels2 = ml.getLabels();

	Mat data, labels;
	vconcat(data1,data2,data);
	vconcat(labels1,labels2,labels);
	//ml.writeData(samplesPath+"data_set.csv",data,labels);
/**/

	/*
	//FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Negative_Pairs/";
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	Mat sample = imread("/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/test74.png",0);
	sample = ml.prepareImage(img3,Size(20,20));
	vector<Mat> sampleVec;

	//for(unsigned int i=0; i<files.size(); i++) {
		//Mat sample = imread(folder + files.at(i),0);

		//////// Translates Images ///////
		//sample = ml.tempFixPrepareImg(sample);

		sampleVec.push_back(sample);
	//}
	Mat results = ml.runANN(param,sampleVec);
	cout << results << endl;
	imgshow(img3);
/*
	int count=0;
	int realTotal=0;
	for(int i=0; i<results.rows; i++) {
		if(files.at(i).find("copy")==string::npos)
			realTotal++;
		if(results.at<float>(i,0)>0) {
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
	String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
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

