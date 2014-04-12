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
	//cout << con.calcContrast(0.511806,0.507639,"Gray8Pink9","Gray8Pink8") << endl;
	//changeColors(img2,Point(367,193),Point(368,201));
	//colorThreshNamingScript();
	//addNewColors(img2,Point(386,245),Point(389,249),"Gray","Gray");
	//int col = 313;
		int row = 219;
		int i=1;
		for(int col=304; col<=327; col++) {
			cout << i << ") ";
			testColorAtLoc(img2, Point(col,row));
			i++;
		}
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

