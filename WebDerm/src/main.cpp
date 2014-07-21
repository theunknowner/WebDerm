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
	runHysteresis();
	/*
	String filename[]= {path+"Images/CLP/clp3.jpg",
						path+"Images/LPH/lph4.jpg",
						path+"Images/LPH/lph6.jpg",
						path+"Images/LPH/lph10.jpg",
						path+"Images/Vesicles/vesicles18.jpg"};
	int fileSize = length(filename);
	runAllHysteresis(filename,fileSize);
	/*
	hsl hsl;
	rgb rgb;
	Color c;
	FileData fd;
	Intensity in;
	rgb.importThresholds();
	hsl.importHslThresholds();
	in.importThresholds();
	Mat img, img2,img3, mask,mask2;
	img = runResizeImage(path+"Images/Psoriasis","Psoriasis4.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	Point pt1(313,400);
	Point pt2(314,401);
	fd.loadFileMatrix("/home/jason/Desktop/Programs/Hysteresis/Psoriasis4.csv");
	deque< deque<String> > vec;
	fd.getFileMatrix(vec);
	String pix, color;
	double currGL=0, currCL=0, currRatio=0, prevRatio=0;
	double localMinRatio=0, localMaxRatio=0, localRatio=0;
	double minRatioIndex=0, maxRatioIndex=0, localRatioIndex=0, ratioLoc=0;
	deque<double> localRatios;
	double localRatioScanSize = 20;
	double relativeRatio=0;
	int flag=0;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			pix = vec.at(i).at(j);
			color = c.getMainColor(pix);
			currGL = rgb.getGrayLevel1(pix);
			currCL = rgb.getColorLevel(pix);
			currRatio = currGL/currCL;
			if(currCL==0) currRatio = -1;
			if(flag==0) {
				if(color!="Black") {
					currGL = rgb.getGrayLevel1(pix);
					currCL = rgb.getColorLevel(pix);
					currRatio = currGL/currCL;
					localRatio = currRatio;
					prevRatio = currRatio;
					flag=1;
				}
			}
			else if(flag!=0 && color!="Black") {
				currGL = rgb.getGrayLevel1(pix);
				currCL = rgb.getColorLevel(pix);
				currRatio = currGL/currCL;
				if(currRatio-prevRatio>0) {
					localRatio = localMinRatio;
					localRatioIndex = minRatioIndex;
				}
				else if(currRatio-prevRatio<0) {
					localRatio = localMaxRatio;
					localRatioIndex = maxRatioIndex;
				}
				else {
					localRatioIndex--;
					if(localRatioIndex<0) localRatioIndex=0;
				}
				localRatio = localRatios.at(localRatioIndex);
				relativeRatio = currRatio/localRatio;
				if(j==103 && i==621) {
					ratioLoc  = j-(localRatios.size()-localRatioIndex);
					cout << "CurrPix: " << vec.at(i).at(j) << endl;
					cout << "PrevPix: " << vec.at(i).at(j-1) << endl;
					cout << "LocalMinMaxPix: " << vec.at(i).at(ratioLoc) << endl;
					cout << "CurrRatio: " << currRatio << endl;
					cout << "PrevRatio: " << prevRatio << endl;
					cout << "Difference: " << roundDecimal(currRatio-prevRatio,2) << endl;
					cout << "LocalMinRatio:" << localMinRatio << endl;
					cout << "LocalMaxRatio: " << localMaxRatio << endl;
					cout << "LocalRatio: " << localRatio << endl;
					cout << "LocalRatioIndex: " << localRatioIndex << endl;
					cout << "RatioLoc: " << ratioLoc+1 << endl;
					cout << "RelativeRatio: " << roundDecimal(relativeRatio,2) << endl;
				}
				prevRatio = currRatio;
			}
			if(color!="Black") {
				if(localRatios.size()==localRatioScanSize) localRatios.pop_front();
				localRatios.push_back(currRatio);
				maxRatioIndex = distance(localRatios.begin(),max_element(localRatios.begin(),localRatios.end()));
				localMaxRatio = localRatios.at(maxRatioIndex);
				minRatioIndex = distance(localRatios.begin(),min_element(localRatios.begin(),localRatios.end()));
				localMinRatio = localRatios.at(minRatioIndex);
			}
		}
		flag=0;
		localRatioIndex=0;
		localRatios.clear();
	}
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

