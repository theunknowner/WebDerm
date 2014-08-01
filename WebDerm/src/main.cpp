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
#include "skin/Skin.h"
#include "test/testfunctions.h"
#include "scripts/scripts.h"
#include "Math/maths.h"
#include "Contrast/contrast.h"
#include "FileData/filedata.h"
#include "Histogram/histogram.h"

int main(int argc,char** argv)
{
	//runAllHysteresis();
	//runHysteresis();
/*
	String filename[]= {//path+"Images/Melanoma/melanoma1.jpg",
						path+"Images/Melanoma/melanoma2.jpg",
						path+"Images/Melanoma/melanoma3.jpg",
						path+"Images/Melanoma/melanoma4.jpg",
						path+"Images/Melanoma/melanoma5.jpg",
						path+"Images/Melanoma/melanoma7.jpg"};
	int fileSize = length(filename);
	runAllHysteresis(filename,fileSize);
	/**/
	hsl hsl;
	rgb rgb;
	Color c;
	Intensity in;
	rgb.importThresholds();
	hsl.importHslThresholds();
	in.importThresholds();
	Mat img, img2,img3, mask,mask2;
	img = runResizeImage(path+"Images/LPH","lph4.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	Point pt1(475,35);
	Point pt2(476,36);
	FileData fd;
	fd.listFiles("/home/jason/Desktop/workspace/Images/LPH/");
	//fd.loadFileMatrix("/home/jason/Desktop/Programs/Hysteresis/vesicles18_MainColors.csv");
	//deque< deque<String> > vec;
	//fd.getFileMatrix(vec);
	//dataDeduplicationGrayRGB(0);
	//addNewColors(img2, pt1,pt2,"Gray", "Brown");
	//addNewColors(img2, Point(344,274), Point(346,275),"Gray", "Violet");
	//checkColorsFromList(img2,pt1,pt2);
	//generateColorRegionTable(img2, pt1,pt2);
	//generateColorRegionTable(img2, Point(422,265), Size(3,3));
/*
	int col = 475;
	int row = 35;
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

