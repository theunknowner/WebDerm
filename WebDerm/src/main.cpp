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
#include "Contrast/contrast.h"

int main(int argc, char** argv)
{
	hsl hsl;
	rgb rgb;
	contrast con;
	Color c;
	rgb.importThresholds();
	hsl.importLsThreshold();
	hsl.importHslThresholds();
	//hsl.importThresholds();
	Mat img, img2,img3, mask;
	img = runResizeImage(path+"Images/","img1.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	//colorThreshRenamingScript();
	//addNewColors(img,Point(518,526),Point(534,536),"Gray","Gray");
	//cout << rgb.calcGrayLevel2(134, 82, 85) << endl;
	//cout << rgb.calcColorLevel2(134, 82, 85) << endl;
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
	}*/
	//cout << con.calcColorfulness2(0.4,"Gray85Pink60") << endl;
	int col = 581;
	int row = 538;
	cout << "Result: " << testHysteresis(img, row, col, Size(2,2)) << endl;
	cout << con.calcContrast(0.50,0.555,"Gray76Blue47","Gray81Blue49") << endl;
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

