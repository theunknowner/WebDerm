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
#include "Shades/shades.h"
#include "Entropy/entropy.h"
#include "Algorithms/quicksort.h"

int main(int argc,char** argv)
{
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
	Hsl hsl;
	Rgb rgb;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, mask,mask2;
	img = runResizeImage(path+"Images/Vesicles/","vesicles25.jpg",Size(700,700),0);
	//getSkin(img, mask);
	//img.copyTo(img2, mask);
	Point pt1(430,244);
	Point pt2(306,439);
	String file1 = "/home/jason/Desktop/Programs/Entropy Output/lph4_Entropy.csv";
	String file2 = "/home/jason/Desktop/Programs/Entropy Output/Psoriasis1_Entropy.csv";
	deque< deque<double> > pEnt1;
	deque< deque<double> > pEnt2;
	deque<int> colorShadeShift(allColors.size(),0);
	Entropy en;
/*
	FileData fd;
	fd.filename = "Psoriasis1";
	fd.loadFileMatrix("/home/jason/Desktop/Programs/"+fd.filename+"_ShadeColors_10x10.csv",fd.colorVec);
	fd.ksize = Size(10,10);
	pEnt1 = en.outputEntropy(fd,Size(25,25));

	FileData fd2;
	fd2.filename = "Psoriasis2";
	fd2.loadFileMatrix("/home/jason/Desktop/Programs/"+fd2.filename+"_ShadeColors_10x10.csv",fd2.colorVec);
	fd2.ksize = Size(10,10);
	pEnt2 = en.outputEntropy(fd2,Size(25,25));
	colorShadeShift = en.calcShadeShift(pEnt1, pEnt2);
	en.outputShiftedEntropy(fd,fd2,Size(25,25),colorShadeShift);
	*/
	deque< deque<String> > vec;
	FileData fd;
	//fd.renameFiles("/home/jason/Desktop/workspace/Qt/WebDermGui/test/","Link to ","");
	fd.filename = "Psoriasis2";
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Psoriasis2_ShadeColors_10x10.csv",fd.colorVec);
	c.output2ImageTargetColor(fd.colorVec,Size(10,10),fd.filename,"PinkViolet");
	//addNewColors(img2, pt1,pt2,"Gray", "Brown");
	//addNewColors(img2, Point(344,274), Point(346,275),"Gray", "Violet");
	//checkColorsFromList(img2,pt1,pt2);
	//generateColorRegionTable(img2, pt1,pt2);
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

