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
	rgb.importThresholds();
	hsl.importLsThreshold();
	hsl.importHslThresholds();
	//hsl.importThresholds();
	Mat img, img2,img3, mask;
	img = runResizeImage(path+"Images/LPH","lph4.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	//cout << con.calcContrast(0.566667,0.563889,"Gray8Brown9","Gray9Brown9") << endl;
	//changeColors(img2,Point(367,193),Point(368,201));
	//colorThreshNamingScript();
	//addNewColors(img2,Point(386,245),Point(389,249),"Gray","Gray");
	//int col = 313;
	int row = 172;
	int i=1;
	String pix;
	double hue=0;
	for(int col=355; col<=382; col++) {
		cout << i << ") " << col <<  ": ";
		pix = testColorAtLoc(img2, Point(col,row),hue);
		cout << "Colorfn0: " << con.calcColorfulness2(hue,pix) << endl;
		i++;
	}
	//cout << "Colorfn0: " << con.calcColorfulness2(0.586111,"Gray77Brown47") << endl;
	//cout << "Colorfn1: " << con.calcColorfulness2(0.527778,"Gray5Pink7") << endl;
	//cout << "Colorfn2: " << con.calcColorfulness2(0.536111,"Gray46Pink42") << endl;
	//cout << "Colorfn3: " << con.calcColorfulness2(0.530556,"Gray44Pink40") << endl;
	//cout << "Result: " << testHysteresis(img2, row, col, Size(2,2)) << endl;
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

