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
#include "Algorithms/jaysort.h"

int main(int argc,char** argv)
{
	//runRenameFiles();
	//runAllHysteresis();
	//runHysteresis();
	//runMouseColor();
	//runResizeAllImages();
	/*
	String filename[]= {path+"Images/Acne/acne1.jpg",
						path+"Images/Acne/acne2.jpg",
						path+"Images/Acne/acne3.jpg",
						path+"Images/Acne/acne5.jpg",
						path+"Images/Acne/acne6.jpg"};
	int fileSize = length(filename);
	runAllHysteresis(filename,fileSize);
	/**/
	Rgb rgb;
	Hsl hsl;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4,mask;
	img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/herpes5.jpg",Size(700,700),0);
	//img3 = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/clp4jpg",Size(700,700),0);
	//img2 = runResizeImage(path+"Images/Vesicles/","vesicles25.jpg",Size(700,700),0);
	//getSkin(img, mask);
	//img.copyTo(img2, mask);

	//Point pt1(430,244);
	//Point pt2(306,439);
	//String file1 = "/home/jason/Desktop/Programs/Entropy Output/lph4_Entropy.csv";
	//String file2 = "/home/jason/Desktop/Programs/Entropy Output/Psoriasis1_Entropy.csv";
	//deque< deque<double> > pEnt1;
	//deque< deque<double> > pEnt2;
	//deque<int> colorShadeShift(allColors.size(),0);
	Entropy en;
	//en.runCompareEntropy();
	//en.runCompareEntropy2();

	img2 = c.shadeCorrection(img);
	//img3 = c.changeImageBrightness(img2,-0.20,0);
	img4 = c.changeImageBrightness(img2,0.8,1);
	blur(img4,img4,Size(10,10));
	//img4 = c.correctGamma(img2,2.2);
	//namedWindow("img",CV_WINDOW_FREERATIO);
	//namedWindow("img3",CV_WINDOW_FREERATIO);
	namedWindow("img4",CV_WINDOW_FREERATIO);
	//imshow("img",img);
	//imshow("img3",img3);
	imshow("img4",img4);
	waitKey(0);
	imwrite("herpes5.png",img4);/**/
	//double a=1.0, b=0.001, p=0.33;
/*
	deque<deque<double> > vec1;
	deque<deque<double> > vec2;
	deque<deque<double> > matchVec;
	deque<double> resultVec;
	String name1 = "acne1";
	String name2 = "vesicles25";
	en.loadEntropyFiles("/home/jason/Desktop/Programs/Output/"+name1+"_10x10_YSV_Combined50x50.csv",vec1);
	en.loadEntropyFiles("/home/jason/Desktop/Programs/Output/"+name2+"_10x10_YSV_Combined50x50.csv",vec2);
	//resultVec = en.compareEntropy(vec1,vec2,matchVec);
	//en.importEntropyThresholds();
	en.compareEntropy(vec1,vec2,matchVec);
/*
	String filename2 = name1+"-"+name2+".csv";
	FILE * fp;
	fp = fopen(filename2.c_str(),"w");
	fprintf(fp,",Y,S,V,Match\n");
	for(unsigned int i=0; i<matchVec.size(); i++) {
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<matchVec.at(i).size(); j++) {
			fprintf(fp,"%f,",matchVec.at(i).at(j));
		}
		fprintf(fp,"%f\n",resultVec.at(i));
	}
	fclose(fp);
	en.releaseMemory();
/**/
/*
	FileData fd;
	fd.filename = "acne1";
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output/"+fd.filename+"_ShadeColors_10x10.csv",fd.colorVec);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Output/"+fd.filename+"_HSL_10x10.csv",fd.hslMat);
	fd.ksize = Size(10,10);
	Size entSize(50,50);
	String targetColor = "Pink";
	en.eyeFn(fd,entSize,targetColor);
	//pEnt1 = en.outputCombinedSigmoid(fd,Size(10,10),a,b,p);
	//pEnt1 = en.outputCombinedEntropy(fd,entSize);
	img3 = c.output2ImageTargetColor(fd.colorVec,Size(10,10),fd.filename,targetColor);
	//img3 = en.showEntropySquares(img3,entSize);
	img3 = en.showEyeFnSquares(img3,entSize,targetColor);
	//Mouse::mouseOutputColor(img3,fd);

	FileData fd2;
	fd2.filename = "vesicles25";
	fd2.loadFileMatrix("/home/jason/Desktop/Programs/Output/"+fd2.filename+"_ShadeColors_10x10.csv",fd2.colorVec);
	fd2.loadFileMatrix("/home/jason/Desktop/Programs/Output/"+fd2.filename+"_HSL_10x10.csv",fd2.hslMat);
	fd2.ksize = Size(10,10);
	String targetColor2 = "Pink";
	en.eyeFn(fd2,entSize,targetColor2);
	//pEnt2 = en.outputCombinedSigmoid(fd2,Size(10,10),a,b,p);
	//pEnt2 = en.outputCombinedEntropy(fd2,entSize);
	img4 = c.output2ImageTargetColor(fd2.colorVec,Size(10,10),fd2.filename,targetColor2);
	//img4 = en.showEntropySquares(img4,entSize);
	img4 = en.showEyeFnSquares(img4,entSize,targetColor2);
	//Mouse::mouseOutputColor(img4,fd2);
/**/
/*
	namedWindow(fd.filename+"_Squares",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	namedWindow(fd2.filename+"_Squares2",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	imshow(fd.filename+"_Squares",img3);
	imshow(fd2.filename+"_Squares2",img4);
	waitKey(0);
	//colorShadeShift = en.calcShadeShift(pEnt1, pEnt2);
	//en.outputShiftedEntropy(fd,fd2,Size(10,10),colorShadeShift);

	Point pt1(265,196);
	Point pt2(267,198);
	//addNewColors(img2, pt1,pt2,"Gray", "Brown");
	//addNewColors(img2, Point(344,274), Point(346,275),"Gray", "Violet");
	//checkColorsFromList(img2,pt1,pt2);
	generateColorRegionTable(img4, pt1,pt2);
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

