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
	img = imread("/home/jason/Desktop/Programs/Looks_Like/lph4.jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	ShapeMorph sm;
	Size size(5,5);
	cvtColor(img,imgGray,CV_BGR2GRAY);
	blur(img,img,size);
	FileData fd;
	Matlab mb;
	Poly poly;
	int r,g,b;
	vector<double> HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b RGB = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(RGB[2],RGB[1],RGB[0]);
			HSL[0] = HSL[0] - floor(HSL[0]/180.) * 360.;
			HSL[1] = round(HSL[1] * 100.);
			HSL[2] = round(HSL[2] * 100.);
			hvec.at<float>(i,j) = HSL.at(0);
			svec.at<float>(i,j) = HSL.at(1);
			lvec.at<float>(i,j) = HSL.at(2);
		}
	}
	Mat hVec2(hvec.size(),CV_32F,Scalar(0));
	Mat sVec2(svec.size(),CV_32F,Scalar(0));
	Mat lVec2(lvec.size(),CV_32F,Scalar(0));
	for(int row=0; row<hvec.rows; row++) {
		writeSeq2File(hvec.row(row),"float","hvec",true);
		writeSeq2File(svec.row(row),"float","svec",true);
		writeSeq2File(lvec.row(row),"float","lvec",true);
		String command = "python /home/jason/git/WebDerm/WebDerm/poly.py";
		system(command.c_str());
		deque< deque<String> > hData;
		deque< deque<String> > sData;
		deque< deque<String> > lData;
		vector<double> hfitval;
		vector<double> sfitval;
		vector<double> lfitval;
		fd.loadFileMatrix("/home/jason/git/WebDerm/WebDerm/hfitvals.csv",hData);
		fd.loadFileMatrix("/home/jason/git/WebDerm/WebDerm/sfitvals.csv",sData);
		fd.loadFileMatrix("/home/jason/git/WebDerm/WebDerm/lfitvals.csv",lData);
		for(unsigned int i=0; i<hData.size(); i++) {
			for(unsigned int j=0; j<hData.at(i).size(); j++) {
				double val = atof(hData.at(i).at(j).c_str());
				hfitval.push_back(val);
				val = atof(sData.at(i).at(j).c_str());
				sfitval.push_back(val);
				val = atof(lData.at(i).at(j).c_str());
				lfitval.push_back(val);
			}
		}
		vector<double> xvec, xmaxH, xminH;
		vector<double> xmaxS, xminS;
		vector<double> xmaxL, xminL;
		vector<int> imaxH, iminH;
		vector<int> imaxS, iminS;
		vector<int> imaxL, iminL;
		for(unsigned int i=0; i<hfitval.size(); i++) {
			xvec.push_back(i);
		}
		poly.findExtremas(hfitval,xmaxH,imaxH,xminH,iminH);
		poly.findExtremas(sfitval,xmaxS,imaxS,xminS,iminS);
		poly.findExtremas(lfitval,xmaxL,imaxL,xminL,iminL);
		vector<double> hfitval2 = hfitval;
		vector<double> sfitval2 = sfitval;
		vector<double> lfitval2 = lfitval;
		for(unsigned int i=0; i<imaxH.size(); i++) {
			hfitval2.at(imaxH.at(i)) = hvec.at<float>(row,imaxH.at(i));
		}
		for(unsigned int i=0; i<iminH.size(); i++) {
			hfitval2.at(iminH.at(i)) = hvec.at<float>(row,iminH.at(i));
		}
		for(unsigned int i=0; i<imaxS.size(); i++) {
			sfitval2.at(imaxS.at(i)) = svec.at<float>(row,imaxS.at(i));
		}
		for(unsigned int i=0; i<iminS.size(); i++) {
			sfitval2.at(iminS.at(i)) = svec.at<float>(row,iminS.at(i));
		}
		for(unsigned int i=0; i<imaxL.size(); i++) {
			lfitval2.at(imaxL.at(i)) = lvec.at<float>(row,imaxL.at(i));
		}
		for(unsigned int i=0; i<iminL.size(); i++) {
			lfitval2.at(iminL.at(i)) = lvec.at<float>(row,iminL.at(i));
		}
		for(int j=0; j<hVec2.cols; j++) {
			hVec2.at<float>(row,j) = hfitval2.at(j);
		}
		for(int j=0; j<sVec2.cols; j++) {
			sVec2.at<float>(row,j) = sfitval2.at(j);
		}
		for(int j=0; j<lVec2.cols; j++) {
			lVec2.at<float>(row,j) = lfitval2.at(j);
		}
		hfitval.clear();
		hfitval.shrink_to_fit();
		hfitval2.clear();
		hfitval2.shrink_to_fit();
		xvec.clear();
		xvec.shrink_to_fit();
	}
	Mat map = sm.getShapeUsingColor2(hVec2,sVec2,lVec2);
	imgshow(map);
	//writeSeq2File(hfitval2,"hfitval2");
	/*writeSeq2File(hvec,"float","hvec");
	writeSeq2File(svec,"float","svec");
	writeSeq2File(lvec,"float","lvec");
	//imwrite("herpes3.png",img);
	//img3 = sm.getShapeUsingColor(img);
	//sm.setDebugMode(true);
	//img3 = sm.test_getShapeUsingColor(img,66,25);
	//imgshow(img3);
	/*
	vector<Mat> matVec = sm.lumFilter1(img);
	vector<Mat> matVec2 = sm.lumFilter2(img);

	imgshow(img);
	imgshow(matVec.at(0));
	imgshow(matVec2.at(0));

	imwrite("img1.png",matVec.at(0));
	imwrite("img2.png",matVec2.at(0));
	TestML ml;
	Mat results = ml.runANN(matVec);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}
	results = ml.runANN(matVec2);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}
	waitKey(0);
	/*
	Mat result1 = sm.origFilter(img);
	Mat result2 = sm.closeFilter(img);
	imgshow(result1);
	imgshow(result2);
/*
	img4 = sm.densityDetection(result1);
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
	//imgshow(img4);
	 */
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
	/*
	TestML ml;
	vector<vector<double> > trainingData;
	vector<vector<double> > trainingLabels;
	ml.importCsvData("/home/jason/git/Samples/Samples/training_set.csv",trainingData,trainingLabels);
	int sampleSize = trainingData.size();
	int inputSize = trainingData.at(0).size();
	int outputSize = trainingLabels.at(0).size();
	int hiddenNodes = 20;
	Mat training_set(sampleSize,inputSize,CV_32F);
	Mat training_labels(sampleSize,outputSize,CV_32F);
	ml.vecToMat(trainingData,trainingLabels,training_set,training_labels);
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);

	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/jason/git/Samples/Samples/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	//TestML ml;
	CvANN_MLP ann;
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
	String name = "lph15";
	String file = "/home/jason/Desktop/workspace/True_Positive_Pairs.csv";
	String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
	Entropy en;
	en.importEntropyThresholds();
	en.setDebugMode(true);
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

