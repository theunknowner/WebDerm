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
#include "Exceptions/my_exceptions.h"
#include "ShadeShape/ShadeShapeRelation/srm.h"
#include "ShadeShape/StatSign/statsign.h"

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
	//ShadeShape ss1 = Scripts::script2("/home/jason/Desktop/workspace/Test_Base_NN/test6.png");
	//ShadeShape ss2 = Scripts::script2("/home/jason/Desktop/workspace/Test_Base_NN/test7.png");
	ShadeShape ss1 = Scripts::script31("lph4");
	ShadeShape ss2 = Scripts::script31("lph7");
	ShadeShapeMatch ssm;
	//ssm.test(ss1);
	//ssm.debug_mode(1);
	cout << ssm.test_match(ss1,ss2) << endl;
	//vector<float> results = ssm.match(ss1,ss2);
	//printf("TR1: %f x TR2: %f = %f]\n",results.at(1),results.at(2),results.at(0));
	//ssm.test(ss1);
	//Islands island = ss1.getIslandWithPoint(Point(48,68));
	//imwrite("comp_disc.png",island.image());
/**/

/*
	String file = std::string(argv[1]);
	String name = getFileName(file);
	Mat img = imread(file,0);
	ImageData id(img,name);
	Func::prepareImage(id,Size(140,140));
	ShadeShape ss1;
	ss1.extract(id);
	ss1.showInteractiveIslands();
	TestML::clear();
/**/
	/*
	ShadeShape ss1 = Scripts::script31("melanoma3");
	//ss1.showInteractiveIslands();
	//ss1.writeListOfIslandsWithLowNN();
	//ShadeShapeMatch ssm;
	//ssm.test(ss1);
	Islands island = ss1.getIslandWithPoint(Point(44,61));
	//imwrite(ss1.name()+"_strip.png",island.image());
	//cout << island.nn_results() << endl;
/**/

	MyExceptions ex;
	String name = "";
	try {
		Timer time;
		String folder = "Looks_Like/";
		deque<String> files;
		FileData fd;
		fd.getFilesFromDirectory(folder,files);
		ShadeShape ss1 = Scripts::script31(argv[1]);
		vector<vector<float> > resultVec;
		vector<String> nameVec;
		vector<int> origPos;
		//> STT counter setup <//
		ShapeMatch spm;
		vector<vector<int> > shapeTranslateCount(8,vector<int>(8,0)); //8 shapes
		///////////////////////
		for(unsigned int i=0; i<files.size(); i++) {
			name = folder + files.at(i);
			name = getFileName(name);
			if(name!=argv[1]) {
				ShadeShape ss2 = Scripts::script31(name);
				ShadeShapeMatch ssm;
				if(argc>=3)
					ssm.debug_mode(atoi(argv[2]));
				vector<float> results = ssm.match(ss1,ss2);
				nameVec.push_back(name);
				resultVec.push_back(results);
				ssm.countShapeTranslations(shapeTranslateCount);
			}
		}
		//> STT counter continue <//
		String sttFileName = string(argv[1]) + "_STT_Count.csv";
		FILE * sttCountFile;
		sttCountFile = fopen(sttFileName.c_str(),"w");
		fprintf(sttCountFile,",");
		for(unsigned int i=0; i<8; i++) {
			fprintf(sttCountFile,"%s,",spm.shapeName(i).c_str());
		}
		fprintf(sttCountFile,"\n");
		for(unsigned int i=0; i<shapeTranslateCount.size(); i++) {
			fprintf(sttCountFile,"%s,",spm.shapeName(i).c_str());
			for(unsigned int j=0; j<shapeTranslateCount.at(i).size(); j++) {
				fprintf(sttCountFile,"%d,",shapeTranslateCount.at(i).at(j));
			}
			fprintf(sttCountFile,"\n");
		}
		fclose(sttCountFile);
		//////////////////////////

		jaysort(resultVec,origPos,1);
		String output = std::string(argv[1]) + "_matches_sorted.csv";
		FILE * fp;
		fp = fopen(output.c_str(),"w");
		for(unsigned int i=0; i<resultVec.size(); i++) {
			fprintf(fp,"%s,%f,%f,%f\n",nameVec.at(origPos.at(i)).c_str(),resultVec.at(i).at(0),resultVec.at(i).at(1),resultVec.at(i).at(2));
		}
		printf("%s...Done!\n",argv[1]);
		fclose(fp);
		time.end();
		time.printTimer();
		output = std::string(argv[1]) + "_time_elapsed.txt";
		fp = fopen(output.c_str(),"w"); //reuse fp
		fprintf(fp,"%s\n",time.getTimerString().c_str());
		fprintf(fp,"Begin: %s",time.getStartTime().c_str());
		fprintf(fp,"End: %s",time.getEndTime().c_str());
		fclose(fp);
	} catch(cv::Exception &e) {
		ex.name(std::string(argv[1])+"_"+name);
		ex.writeErrorToFile(e);
	} catch(std::out_of_range &e) {
		ex.name(std::string(argv[1])+"_"+name);
		ex.writeErrorToFile(e);
	}
	/**/
/*
	Timer time;
	ShadeShape ss1 = Scripts::script31(argv[1]);
	ShadeShape ss2 = Scripts::script31(argv[2]);
	ShadeShapeMatch ssm;
	vector<vector<int> > shapeTranslateCount(8,vector<int>(8,0));
	if(argc==4)
		ssm.debug_mode(atoi(argv[3]));
	vector<float> results = ssm.match(ss1,ss2);
	ssm.countShapeTranslations(shapeTranslateCount);
	printf("TR1: %f x TR2: %f = %f]\n",results.at(1),results.at(2),results.at(0));
	time.end();
	time.printTimer();
	ShapeMatch spm;
	FILE * fp;
	fp = fopen("stt-count.csv","w");
	fprintf(fp,",");
	for(unsigned int i=0; i<8; i++) {
		fprintf(fp,"%s,",spm.shapeName(i).c_str());
	}
	fprintf(fp,"\n");
	for(unsigned int i=0; i<shapeTranslateCount.size(); i++) {
		fprintf(fp,"%s,",spm.shapeName(i).c_str());
		for(unsigned int j=0; j<shapeTranslateCount.at(i).size(); j++) {
			fprintf(fp,"%d,",shapeTranslateCount.at(i).at(j));
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
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
	deque<String> files;
	String folder = "/home/jason/git/Samples/Samples/Training/Strip/";
	FileData fd;
	TestML ml;
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		Mat img = imread(name,0);
		name = getFileName(name);
		Mat sample = img * 255;
		sample = ml.prepareImage(sample,Size(40,40));
		imwrite(name+".png",sample);
	}
/**/
/*
	Mat img1 = imread("/home/jason/git/WebDerm/WebDerm/lph7_rei_s2.png",0);
	Mat img2 = imread("/home/jason/git/WebDerm/WebDerm/melanoma8c_rei_s0.png",0);
	StatSign statSign;
	vector<int> statSignVec1 = statSign.create(img1);
	vector<int> statSignVec2 = statSign.create(img2);
	statSign.adjustValues(statSignVec1);
	statSign.adjustValues(statSignVec2);
	statSign.printCompare(statSignVec1,statSignVec2);
	//cout << statSign.dotProduct(statSignVec1,statSignVec2);
	cout << statSign.proportion(statSignVec1,statSignVec2) << endl;
	//statSign.writeCompare("acne_vulg5-melanoma8b.csv",statSignVec1,statSignVec2);
	/**/

	//Scripts::checkAllTestData3();
	//Scripts::checkAllTestData2();
/*
	//ShadeShape ss1 = Scripts::script2("/home/jason/git/NN3-Donut/NN3-Donut/Training/Excavated/excavated2(032).png");
	ShadeShape ss1 = Scripts::script31("melanoma8c");
	//ss1.showInteractiveIslands();
	Islands island = ss1.getIslandWithPoint(Point(41,54));
	imwrite(ss1.name()+"_sample.png",island.nn_image());
	//imwrite(ss1.name()+"_sample.png",island.image());
	/*
	Mat results = island.nn_results();
	cout << results << endl;
	TestML ml;
	//String param = TestML::PARAM_PATH;
	//Mat sample = imread("/home/jason/Desktop/workspace/circle_donut_incomp(020).png",0);
	Mat sample = island.image();
	sample *= 255;
	imgshow(sample);
	sample = ml.prepareImage(sample,Size(40,40));
	imgshow(sample);
	vector<Mat> sampleVec;
/*
	sampleVec.push_back(sample);
	//Mat results = ml.runANN(param,sampleVec);
	Mat results = ml.runANN2(sampleVec);
	//Mat results = ml.runANN2b(sampleVec);
	cout << results << endl;
	/**/
	/*
	Scripts::script_createAllTrainingLabels();
	sleep(3);
	TestML ml;
	//String mainPath = "/home/jason/git/Samples/";
	String path1 = "Samples/Training/samples_path.csv";
	String path2 = "Samples/Training/labels_path.csv";
	String path3 = "Samples/log.txt";
	Timer time;
	time.begin();
	ml.importTrainingData(path1,path2,ml.getSize());
	Mat data = ml.getData();
	Mat labels = ml.getLabels();
	Mat training_set = data;
	Mat training_labels = labels;
	int sampleSize = training_set.rows;
	int inputSize = training_set.cols;
	int outputSize = training_labels.cols;
	int hiddenNodes = 65;
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	printf("Img Size: %dx%d\n",ml.getSize().width,ml.getSize().height);
	printf("Input Size: %d\n",inputSize);
	printf("Hidden Nodes: %d\n",hiddenNodes);
	printf("Output Size: %d\n",outputSize);
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);

	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 4000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);

	time.end();
	FILE * fp;
	fp = fopen(path3.c_str(),"a");
	fprintf(fp,"%s\n",time.getTimerString().c_str());
	fprintf(fp,"%s",time.getEndTime().c_str());
	fprintf(fp,"Img Size: %dx%d\n",ml.getSize().width,ml.getSize().height);
	fprintf(fp,"Input Size: %d\n",inputSize);
	fprintf(fp,"Hidden Nodes: %d\n",hiddenNodes);
	fprintf(fp,"Output Size: %d\n",outputSize);
	fprintf(fp,"Iterations: %d\n",iter);
	fprintf(fp,"-----------------------\n");
	fclose(fp);

	cout << "Iterations: " << iter << endl;
	time.printTimer();
	String outputFile = "Samples/param.xml";
	CvFileStorage* storage = cvOpenFileStorage(outputFile.c_str(), 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	String shape = "Donut";
	Scripts::script_createAllTrainingLabels2(shape);
	sleep(3);
	TestML ml;
	String mainPath = "NN3-"+shape+"-Comp-Incomp/";
	String path1 = mainPath+"Training/samples_path.csv";
	String path2 = mainPath+"Training/labels_path.csv";
	String path3 = mainPath+"log.txt";
	Timer time;
	time.begin();
	ml.importTrainingData(path1,path2,ml.getSize());
	Mat data = ml.getData();
	Mat labels = ml.getLabels().col(0);
	Mat training_set = data;
	Mat training_labels = labels;
	int sampleSize = training_set.rows;
	int inputSize = training_set.cols;
	int outputSize = training_labels.cols;
	int hiddenNodes = 40;
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	printf("Img Size: %dx%d\n",ml.getSize().width,ml.getSize().height);
	printf("Input Size: %d\n",inputSize);
	printf("Hidden Nodes: %d\n",hiddenNodes);
	printf("Output Size: %d\n",outputSize);
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);

	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 4000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);

	time.end();
	FILE * fp;
	fp = fopen(path3.c_str(),"a");
	fprintf(fp,"%s\n",time.getTimerString().c_str());
	fprintf(fp,"%s",time.getEndTime().c_str());
	fprintf(fp,"Img Size: %dx%d\n",ml.getSize().width,ml.getSize().height);
	fprintf(fp,"Input Size: %d\n",inputSize);
	fprintf(fp,"Hidden Nodes: %d\n",hiddenNodes);
	fprintf(fp,"Output Size: %d\n",outputSize);
	fprintf(fp,"Iterations: %d\n",iter);
	fprintf(fp,"-----------------------\n");
	fclose(fp);

	cout << "Iterations: " << iter << endl;
	time.printTimer();
	String outputFile = mainPath+"param-"+shape+"-Comp-Incomp.xml";
	CvFileStorage* storage = cvOpenFileStorage(outputFile.c_str(), 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
	/**/
/*
	String shape(argv[1]);
	Scripts::script_createAllTrainingLabels3b(shape);
	sleep(3);
	TestML ml(false);
	String mainPath = "NN3-"+shape+"/";
	int labelNum = ml.getIndexContainingShape(shape);
	//int labelNum = ml.getShapeIndex(shape);
	//shape = "Excavated";
	String path1 = mainPath+"Training/samples_path.csv";
	String path2 = mainPath+"Training/labels_path.csv";
	String path3 = mainPath+"log.txt";
	Timer time;
	time.begin();
	ml.importTrainingData(path1,path2,ml.getSize());
	Mat data = ml.getData();
	Mat labels = ml.getLabels().col(labelNum); //> change for different shapes
	Mat training_set = data;
	Mat training_labels = labels;
	int sampleSize = training_set.rows;
	int inputSize = training_set.cols;
	int outputSize = training_labels.cols;
	int hiddenNodes = 40;
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	printf("Img Size: %dx%d\n",ml.getSize().width,ml.getSize().height);
	printf("Input Size: %d\n",inputSize);
	printf("Hidden Nodes: %d\n",hiddenNodes);
	printf("Output Size: %d\n",outputSize);
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);

	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 4000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);

	time.end();
	FILE * fp;
	fp = fopen(path3.c_str(),"a");
	fprintf(fp,"%s\n",time.getTimerString().c_str());
	fprintf(fp,"%s",time.getEndTime().c_str());
	fprintf(fp,"Img Size: %dx%d\n",ml.getSize().width,ml.getSize().height);
	fprintf(fp,"Input Size: %d\n",inputSize);
	fprintf(fp,"Hidden Nodes: %d\n",hiddenNodes);
	fprintf(fp,"Output Size: %d\n",outputSize);
	fprintf(fp,"Iterations: %d\n",iter);
	fprintf(fp,"-----------------------\n");
	fclose(fp);

	cout << "Iterations: " << iter << endl;
	time.printTimer();
	String outputFile = mainPath+"param-"+shape+".xml";
	CvFileStorage* storage = cvOpenFileStorage(outputFile.c_str(), 0, CV_STORAGE_WRITE );
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

