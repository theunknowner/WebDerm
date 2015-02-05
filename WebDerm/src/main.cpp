/*
 * main.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/test.h"
#include "hsl/hsl.h"
#include "Hsv/hsv.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "scripts/scripts.h"
#include "Math/maths.h"
#include "Contrast/contrast.h"
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

int main(int argc,char** argv)
{
	//Entropy en;
	//en.importEntropyThresholds();
	//en.demo_runCompareEntropy();
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
	Mat img, img2,img3, img4, img5;
	img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/urticaria5.jpg",Size(140,140),0);
	//img = runResizeImage("/home/jason/Desktop/Programs/Color Normalized/acne12-2.png",Size(140,140),0);
	//img3 = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/clp4jpg",Size(700,700),0);
	//namedWindow("img",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//namedWindow("img2",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	ShapeMorph sm;
	Size size(3,3);
	img = runColorNormalization(img);
	cvtColor(img,img,CV_BGR2GRAY);

	img2 = sm.prepareImage(img);
	img3 = sm.lumFilter(img2);
	imgshow(img2);
	imgshow(img3);

/*
	GridDisplay gd;
	gd.setImage(img2);
	gd.setGridSize(Size(7,7));
	gd.drawGrid();
	gd.displayGrid();
*/

	img4 = sm.densityDetection(img3);
	vector<Mat> featureVec = sm.liquidFeatureExtraction(img4);
	int countPix=0, idx=0;
	deque<int> countVec;
	deque<int> idxVec;
	for(unsigned int i=0; i<featureVec.size(); i++) {
		countPix = countNonZero(featureVec.at(i));
		idx = i;
		countVec.push_back(countPix);
	}
	jaysort(countVec,idxVec);
	vector<Mat> matVec;
	Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
	while(true) {
		static unsigned int n=1;
		//matVec.push_back(featureVec.at(idxVec.at(idxVec.size()-n)));
		morphologyEx(featureVec.at(idxVec.at(idxVec.size()-n)),img5,MORPH_CLOSE,element);
		//img5 = sm.dilation(featureVec.at(idxVec.at(idxVec.size()-n)),Size(3,3));
		matVec.push_back(img5.clone());
		//imgshow(matVec.at(matVec.size()-1));
		//imwrite("img"+toString(n)+".png",matVec.at(matVec.size()-1));
		n++;
		if(matVec.size()>=5) break;
		if(n>idxVec.size()) break;
	}
	//img5 = sm.dilation(featureVec.at(idx),Size(5,5));
	//vector<Mat> matVec;
	//matVec.push_back(img5);
	//imgshow(result2);
	//imgshow(img3);
	imgshow(img4);
	/**/
	/*
	deque<String> nameVec;
	deque<int> labels;
	vector<Mat> matVec = sm.runShapeMorphTest(nameVec,labels);
/**/
	/*
	deque<deque<String> > vec;
	FileData fd;
	fd.loadFileMatrix("/home/jason/git/WebDerm/WebDerm/data.csv",vec);
	Mat src = fd.stringVec2Mat1D(vec);
	Cluster clst;
	clst.kmeansClusterGeneric(src);

	/*
	FileData fd;
	Entropy en;
	fd.setImage(img);
	en.shapeFn(fd);
/**/
/*
	TestML ml;
	String samplesPath = "/home/jason/git/Samples/Samples/Training/Random/";
	vector<double> labels(5,0);
	for(unsigned int i=0; i<labels.size(); i++) {
		if(i==4) labels.at(i)=1;
		else labels.at(i)=-1;
	}
	ml.convertImagesToData(samplesPath,labels);
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

	TestML ml;
	vector<vector<double> > trainingData;
	vector<vector<double> > trainingLabels;
	ml.importCsvData("/home/jason/git/Samples/Samples/training_set.csv",trainingData,trainingLabels);
	int sampleSize = trainingData.size();
	int inputSize = trainingData.at(0).size();
	int outputSize = trainingLabels.at(0).size();
	int hiddenNodes = 20;
	ml.setLayerParams(inputSize,hiddenNodes,outputSize);
	Mat training_set(sampleSize,ml.inputSize,CV_32F);
	Mat training_labels(sampleSize,ml.outputSize,CV_32F);
	ml.vecToMat(trainingData,trainingLabels,training_set,training_labels);
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);
/*
	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/jason/git/Samples/Samples/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/

	//TestML ml;
	vector<Mat> sampleVec;
	Mat sample;
	for(unsigned int i=0; i<matVec.size(); i++) {
		sample = ml.prepareImage(matVec.at(i));
		sampleVec.push_back(sample);
		sample.release();
	}
	Mat results;
	ml.setLayerParams(400,20,5);
	Mat sample_set = ml.prepareMatSamples(sampleVec);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(sample_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
		//printf("%s: ",nameVec.at(i).c_str());
		//cout << results.row(i) << " - ";
		//cout << labels.at(i) << endl;
	}
	waitKey(0);
	/*
	vector<vector<double> > data;
	vector<vector<double> > labels;
	ml.importCsvData("/home/jason/git/Samples/Samples/test_set.csv",data,labels);
	sampleSize = data.size();
	Mat test_set(sampleSize,inputSize,CV_32F);
	Mat test_labels(sampleSize,outputSize,CV_32F);
	Mat results(sampleSize,outputSize,CV_32F);
	ml.vecToMat(data,labels,test_set,test_labels);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(test_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}
	/**/
	/*
	vector<Mat> samples;
	ml.importSamples("/home/jason/git/Samples/Samples/Test/",samples);
	namedWindow("img",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	for(unsigned int i=0; i<samples.size(); i++) {
		double count=0;
		for(int j=0; j<samples.at(i).rows; j++) {
			count += countNonZero(samples.at(i).row(j));
		}
		cout << count << endl;
		imshow("img",samples.at(i));
		waitKey(0);
	}
/**/
	/*
	FileData fd;
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output2/acne12_5x5.csv",fd.windowVec);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output2/acne12_ShadeColors_5x5.csv",fd.colorVec);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output2/acne12_HSL_5x5.csv",fd.hslMat);
	init_2D_Deque(fd.m_ContrastMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.d_HslMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.hslPtMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.cumHslMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.minMaxHslMat,fd.windowVec.size(), fd.windowVec.at(0).size());
	fd.pt = Point(111,14);
	String newPix = c.getMainColor(fd.windowVec.at(fd.pt.y).at(fd.pt.x));
	String newShade = sh.extractShade(fd.colorVec.at(fd.pt.y).at(fd.pt.x));
	rule3(fd,newPix,newShade);
	printf("HSL(%s)\n",fd.hslMat.at(fd.pt.y).at(fd.pt.x).c_str());
	printf("Color: %s\n",fd.windowVec.at(fd.pt.y).at(fd.pt.x).c_str());
	printf("%s\n",fd.hslPtMat.at(fd.pt.y).at(fd.pt.x).c_str());
	printf("mCon: %s\n",fd.m_ContrastMat.at(fd.pt.y).at(fd.pt.x).c_str());
	printf("dHSL: %s\n",fd.d_HslMat.at(fd.pt.y).at(fd.pt.x).c_str());
	printf("HSLs: %s\n",fd.minMaxHslMat.at(fd.pt.y).at(fd.pt.x).c_str());
	cout << newPix << endl;
	/**/
/*
	String name = "clp4";
	String file = "/home/jason/Desktop/workspace/False_Positive_Pairs.csv";
	String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
	Entropy en;
	en.importEntropyThresholds();
	//en.runCompareEntropy(name);
	//en.runCompareEntropy2(name);
	en.runCompareEntropyList(file,folder);
	//en.runCompareEntropyList2(file,folder);

	/*
	Entropy en;
	FileData fd;
	fd.filename = "herpes13";
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output2/"+fd.filename+"_ShadeColors_5x5.csv",fd.colorVec);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output2/"+fd.filename+"_HSL_5x5.csv",fd.hslMat);
	fd.ksize = Size(5,5);
	Size entSize(10,10);
	String targetColor = "Brown";
	String targetShade = "Dark1";
	en.eyeFn(fd,entSize,targetColor,targetShade);
	img3 = c.output2ImageTargetColor(fd.colorVec,fd.ksize,fd.filename,targetColor,targetShade);
	img3 = en.showEyeFnSquares(img3,entSize,targetColor,targetShade);
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


	Point pt1(7,41);
	Point pt2(9,43);
	//addNewColors(img2, pt1,pt2,"Gray", "Brown");
	//addNewColors(img2, Point(344,274), Point(346,275),"Gray", "Violet");
	//checkColorsFromList(img2,pt1,pt2);
	//generateColorRegionTable(img, pt1,pt2);
	//generateColorRegionTable(img2, Point(422,265), Size(3,3));
/*
	int col = 477;
	int row = 407;
	cout << "Result: " << testHysteresis(img2, row, col, Size(2,2)) << endl;
	//cout << in.calcIntensity("Gray39Brown30");
	//cout << con.calcContrast(0.50,0.555,"Gray90Brown61","Gray88Brown58") << endl;
	//runNeuralNetworkTraining("data.csv");
	//createDataFile("rgb.txt", "NN_Data1.csv");
	//imwrite("img.png", img3);
	//runLPH(img4);
	//imshow("Img1", img3);
	//imshow("Img2", img5);
	//waitKey(0);
/**/
	return 0;
}

