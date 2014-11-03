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
	Mat img, img2,img3, img4,mask;
	img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/herpes3.jpg",Size(700,700),0);
	//img3 = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/clp4.jpg",Size(700,700),0);
	//img2 = runResizeImage(path+"Images/Vesicles/","vesicles25.jpg",Size(700,700),0);
	//getSkin(img, mask);
	//img.copyTo(img2, mask);

	//Point pt1(430,244);
	//Point pt2(306,439);
	//String file1 = "/home/jason/Desktop/Programs/Entropy Output/lph4_Entropy.csv";
	//String file2 = "/home/jason/Desktop/Programs/Entropy Output/Psoriasis1_Entropy.csv";
	deque< deque<double> > pEnt1;
	deque< deque<double> > pEnt2;
	//deque<int> colorShadeShift(allColors.size(),0);
	Entropy en;
	/*
	Hsv hsv;
	double * HSV;
	int * RGB;
	HSV = hsv.rgb2hsv(160,123,113);
	printf("HSV(%f,%.2f,%.2f)\n",HSV[0],HSV[1],HSV[2]);
	RGB = hsv.hsv2rgb(13,0.29,0.63);
	printf("RGB(%d,%d,%d)\n",RGB[0],RGB[1],RGB[2]);*/
/*
	Hsv hsv;
	double * HSV;
	long double coef[6][6] = {0};
	double mul[6] = {1};
	int r,g,b;
	double v[6] = {0};
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			HSV = hsv.rgb2hsv(r,g,b);
			HSV[2] = roundDecimal(HSV[2],2);
			v[0] += HSV[2];
			v[1] += j * HSV[2];
			v[2] += i * HSV[2];
			v[3] += j * i * HSV[2];
			v[4] += pow(j,2) * HSV[2];
			v[5] += pow(i,2) * HSV[2];
			mul[1] = j+1;
			mul[2] = i+1;
			mul[3] = (i+1)*(j+1);
			mul[4] = pow(j+1,2);
			mul[5] = pow(i+1,2);
			for(int m=0; m<6; m++) {
				for(int k=0; k<6; k++) {
					coef[m][k] += mul[k] * mul[m];
				}
			}
		}
	}
	for(int m=0; m<6; m++) {
		for(int k=0; k<6; k++) {
			cout << coef[m][k] <<" ";
		}
		cout << endl;
	}
	cout << endl;
	for(int k=0; k<6; k++) {
		cout << v[k] << ";";
	}/**/

	img2 = img.clone();
	Hsv hsv;
	int r,g,b;
	double * HSV;
	int * RGB;
	double z=0;
	double mul[6] = {1};

	double p[6] = {0.824388692200190,-0.000584800073449605,0.000127670789000708,
				8.50818976015699e-07, 4.12684390316410e-07, -6.40042067865217e-07};
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			HSV = hsv.rgb2hsv(r,g,b);
			if(j==497 && i==366)
				printf("RGB(%d,%d,%d)\n",r,g,b);
			mul[1] = j+1;
			mul[2] = i+1;
			mul[3] = (i+1)*(j+1);
			mul[4] = pow(j+1,2);
			mul[5] = pow(i+1,2);
			for(int k=0; k<6; k++) {
				z += p[k] * mul[k];
			}

			HSV[2] /= z;
			if(HSV[2]>1) HSV[2] = 1;
			RGB = hsv.hsv2rgb(HSV[0],HSV[1],HSV[2]);
			if(j==497 && i==366)
				printf("RGB(%d,%d,%d)\n",RGB[0],RGB[1],RGB[2]);
			img2.at<Vec3b>(i,j)[2] = RGB[0];
			img2.at<Vec3b>(i,j)[1] = RGB[1];
			img2.at<Vec3b>(i,j)[0] = RGB[2];
			z=0;
		}
	}
	namedWindow("img",CV_WINDOW_FREERATIO);
	namedWindow("img2",CV_WINDOW_FREERATIO);
	imshow("img",img);
	imshow("img2",img2);
	waitKey(0);
	/*
	//double a=1.0, b=0.001, p=0.33;
	FileData fd;
	fd.filename = "photo2a";
	fd.loadFileMatrix("/home/jason/Desktop/Pics1/"+fd.filename+"_ShadeColors_10x10.csv",fd.colorVec);
	fd.loadFileMatrix("/home/jason/Desktop/Pics1/"+fd.filename+"_HSL_10x10.csv",fd.hslMat);
	fd.ksize = Size(10,10);
	Size entSize(50,50);
	String targetColor = "Blue";
	en.eyeFn(fd,entSize,targetColor);
	//pEnt1 = en.outputCombinedSigmoid(fd,Size(10,10),a,b,p);
	//pEnt1 = en.outputCombinedEntropy(fd,entSize);
	img3 = c.output2ImageTargetColor(fd.colorVec,Size(10,10),fd.filename,targetColor);
	//img3 = en.showEntropySquares(img3,entSize);
	img3 = en.showEyeFnSquares(img3,entSize,targetColor);
	//Mouse::mouseOutputColor(img3,fd);

	FileData fd2;
	fd2.filename = "photo2b";
	fd2.loadFileMatrix("/home/jason/Desktop/Pics1/"+fd2.filename+"_ShadeColors_10x10.csv",fd2.colorVec);
	fd2.loadFileMatrix("/home/jason/Desktop/Pics1/"+fd2.filename+"_HSL_10x10.csv",fd2.hslMat);
	fd2.ksize = Size(10,10);
	String targetColor2 = "Blue";
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
/*
	//deque< deque<String> > vec;
	//FileData fd;
	//fd.renameFiles("/home/jason/Desktop/workspace/Qt/WebDermGui/test/","Link to ","");
	//fd.filename = "acne1";
	//waitKey(0);
/*
	FileData fd2;
	fd2.filename = "herpes5";
	fd2.loadFileMatrix("/home/jason/Desktop/Programs/"+fd2.filename+"_ShadeColors_10x10.csv",fd2.colorVec);
	c.output2ImageTargetColor(fd2.colorVec,Size(10,10),fd2.filename,"BrownOrange");
	waitKey(0);
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

