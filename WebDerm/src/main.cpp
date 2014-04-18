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
	runHysteresis();
	/*
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
	//colorThreshRenamingScript();
	//addNewColors(img2,Point(386,245),Point(389,249),"Gray","Gray");
	//cout << rgb.calcGrayLevel2(117, 91, 88) << endl;
	//cout << rgb.calcColorLevel2(117, 91, 88) << endl;
	/*
	int i=1;
	String pix;
	double hue=0;
	for(int row=248; row<=248; row++) {
		for(int col=312; col<=370; col++) {
			cout << i << ") " << col <<  ": ";
			pix = testColorAtLoc(img2, Point(col,row),hue);
			cout << "Colorfn0: " << con.calcColorfulness2(hue,pix) << endl;
			i++;
		};
	}
	cout << con.calcColorfulness2(0.4,"Gray85Pink60") << endl;
	int col = 330;
	int row = 248;
	//cout << "Result: " << testHysteresis(img2, row, col, Size(2,2)) << endl;
	//cout << con.calcContrast(0.50,0.555,"Gray67Brown43","Gray67Brown43") << endl;
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

