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

int main(int argc, char** argv)
{
	hsl hsl;
	rgb rgb;
	//hsl.importThresholds();
	hsl.importLumThreshold();
	rgb.importThresholds();
	Mat img, img2, mask;
	img = runResizeImage(path+"Images/LPH","img3.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	//cout << hsl.calcLum()
	//cout << hsl.getHslColor(180, 122, 100) << endl;
	//cout << rgb.pushColor(133,107,101) << endl;
	//showPixelColor(148,112,118,Size(400,400));
	int col = 300;
	int row = 300;
	cout << "Result: " << testHysteresis(img2, row, col, Size(3,3)) << endl;
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

