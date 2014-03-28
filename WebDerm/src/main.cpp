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
#include "scripts/scripts.h"
#include "Math/maths.h"

int main(int argc, char** argv)
{
	hsl hsl;
	rgb rgb;
	//hsl.importThresholds();
	rgb.importThresholds();
	hsl.importLsThreshold();
	Mat img, img2,img3, mask;
	img = runResizeImage(path+"Images/LPH","lph6.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	//showPushColorOnImage(img2,34);
	//outputFarRGBScript(img2,"vesicles18");
	//colorThreshNamingScript();
	//colorThreshScript();
	//testColorIndex(img2,133);
	//rgb.showPixelColor(198,170,160,Size(400,400));
	int col = 355;
	int row = 280;
	cout << "Result: " << testHysteresis(img2, row, col, Size(2,2)) << endl;
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

