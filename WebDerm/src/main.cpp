/*
 * main.cpp
 *
 *  Created on: Nov 19, 2013
 *      Author: jason
 */

#include "run.h"
#include "rgb/rgb.h"
#include "test.h"
#include "hsl/hsl.h"
#include "skin/Skin.h"
#include "test/testfunctions.h"

void testSat()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorThresh.csv","w");
	hsl hsl;
	rgb rgb;
	int r,g,b;
	String pix;
	String grayLevel;
	int colorLevel=0;
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		grayLevel=rgb.calcGrayLevel(r,g,b);
		colorLevel = rgb.calcColorLevel(r,g,b);
		pix = grayLevel + rgbColors.at(i) + toString(colorLevel);
		fprintf(fp,"%s,%d,%d,%d\n",pix.c_str(),r,g,b);
	}
}

int main(int argc, char** argv)
{
	hsl hsl;
	rgb rgb;
	//hsl.importThresholds();
	rgb.importThresholds();
	hsl.importLsThreshold();
	Mat img, img2, mask;
	img = runResizeImage(path+"Images/LPH","IMG_0450.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	testSat();
	//cout << rgb.pushColor(88,69,66) << endl;
	//showPixelColor(148,112,118,Size(400,400));
	int col = 281;
	int row = 97;
	//cout << "Result: " << testHysteresis(img2, row, col, Size(3,3)) << endl;
	//runNeuralNetworkTraining("data.csv");
	//createDataFile("rgb.txt", "NN_Data1.csv");
	//imwrite("img.png", img3);
	//runLPH(img4);
	//imshow("Img1", crop);
	//imshow("Img2", img5);
	//waitKey(0);
/**/
	return 0;
}

