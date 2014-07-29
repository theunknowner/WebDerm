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

int main(int argc,char** argv)
{
	//runHysteresis();
/*
	String filename[]= {path+"Images/LPH/lph1.jpg",
						path+"Images/LPH/lph4.jpg",
						path+"Images/LPH/lph6.jpg",
						path+"Images/LPH/lph8.jpg",
						path+"Images/LPH/lph10.jpg",
						path+"Images/Vesicles/vesicles18.jpg",
						path+"Images/Psoriasis/Psoriasis1.jpg",
						path+"Images/Psoriasis/Psoriasis4.jpg"};
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
	img = runResizeImage(path+"Images/Psoriasis","Psoriasis4.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	Point pt1(560,359);
	Point pt2(563,363);
	//FileData fd;
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
	int col = 361;
	int row = 253;
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

