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

int main(int argc,char** argv)
{
	runHysteresis();
	/*
	hsl hsl;
	rgb rgb;
	Color c;
	Intensity in;
	rgb.importThresholds();
	hsl.importLsThreshold();
	hsl.importHslThresholds();
	in.importThresholds();
	//hsl.importThresholds();
	Mat img, img2,img3, mask;
	img = runResizeImage(path+"Images/LPH","lph6.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	//generateColorRegionTable(img2, Point(393,244), Point(394,251));
	//generateColorRegionTable(img2, Point(284,270), Size(2,2));
/*
	int ind=0;
	double dist=0;
	String pix;
	int r,g,b;
	int x=218-1;
	int y=298-1;
	for(int i=y; i<y+3; i++) {
		for(int j=x; j<x+3; j++) {
			r = img2.at<Vec3b>(i,j)[2];
			g = img2.at<Vec3b>(i,j)[1];
			b = img2.at<Vec3b>(i,j)[0];
			cout << j+1 << "," << i+1 << endl;
			pix = rgb.calcColor2(r,g,b);
			cout << pix << endl;
			if(pix=="OTHER") {
				cout << rgb.pushColor(r,g,b,dist,ind) << endl;
				cout << "Dist: " << dist << endl;
				cout << "Index: " << ind+2 << endl;
			}
			cout << "Brightness: " << rgb.calcPerceivedBrightness(r,g,b) << endl;
			cout << "GrayLevel: " << rgb.calcGrayLevel(r,g,b) << endl ;
			cout << "GrayLumLevel: " << rgb.calcGrayLevel2(r,g,b) << endl;
			cout << "ColorLevel: " << rgb.calcColorLevel2(r,g,b) << endl;
		}
	}
	//colorThreshRenamingScript();
	//addNewColors(img,Point(363,314),Point(365,316),"Gray","Gray");
	//cout << rgb.calcGrayLevel2(202, 192, 193) << endl;
	//cout << rgb.calcColorLevel2(202, 192, 193) << endl;
	/*
	int i=1;
	String pix;
	double hue=0;
	for(int row=185; row<=185; row++) {
		for(int col=345; col<=420; col++) {
			cout << i << ") " << col <<  ": ";
			pix = testColorAtLoc(img2, Point(col,row),hue);
			cout << "Colorfn0: " << con.calcColorfulness2(hue,pix) << endl;
			i++;
		};
	}
	int col = 414;
	int row = 212;
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

