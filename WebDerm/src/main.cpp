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

int main(int argc,char** argv)
{
	//Entropy en;
	//en.runAllEntropy();
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
	//img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/vesicles18.jpg",Size(140,140),0);
	//img = runResizeImage("/home/jason/Desktop/Programs/Color Normalized/acne12-2.png",Size(140,140),0);
	//img3 = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/clp4jpg",Size(700,700),0);
/*
	FileData fd;
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Test_Output/urticaria7_ShadeColors_5x5.csv",fd.colorVec);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Test_Output/urticaria7_HSL_5x5.csv",fd.hslMat);
	rule5(fd);
	//cout << fd.colorVec.at(72).at(61) << endl;
*/
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

	Entropy en;
	en.importEntropyThresholds();
	en.runCompareEntropy();
	en.runCompareEntropy2();

/*
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	FileData fd;
	Size size(10,10);
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		img = imread(folder+files.at(i));
		if(img.data) {
			String name = getFileName(files.at(i));
			img2 = runColorNormalization(img);
			img4 = runResizeImage(img2,Size(700/size.width,700/size.height));
			blur(img4,img4,size);
			imwrite(name+"-3.png",img4);
		}
	}
	//img4 = c.correctGamma(img2,2.2);
	//namedWindow("img",CV_WINDOW_FREERATIO);
	//namedWindow("img3",CV_WINDOW_FREERATIO);
	//namedWindow("img4",CV_WINDOW_FREERATIO);
	//imshow("img",img);
	//imshow("img3",img3);
	//imshow("img4",img4);
	//waitKey(0);
	//imwrite("ves18-2.png",img4);/**/
	//double a=1.0, b=0.001, p=0.33;

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
*/
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
	generateColorRegionTable(img, pt1,pt2);
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

