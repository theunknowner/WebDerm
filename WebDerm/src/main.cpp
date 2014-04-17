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
	//double vec1[] = {-65.4972,33.628465};
	//double vec2[] = {-66.4083,33.402785};
	//cout << cosineDist(vec1,vec2) * 1000;
	//colorThreshRenamingScript();
	//addNewColors(img2,Point(386,245),Point(389,249),"Gray","Gray");
	/*
	int i=1;
	String pix;
	double hue=0;
	for(int row=248; row<=248; row++) {
		for(int col=333; col<=465; col++) {
			cout << i << ") " << col <<  ": ";
			pix = testColorAtLoc(img2, Point(col,row),hue);
			cout << "Colorfn0: " << con.calcColorfulness2(hue,pix) << endl;
			i++;
		};
	}*/
	//cout << "Angle: " << con.calcContrast2(0.50625,0.488889,"Gray89Pink61","Gray90Pink62") << endl;
	//cout << "Angle: " << con.calcContrast2(0.517361,0.513889,"Gray84Pink52","Gray87Pink57") << endl;
	//cout << "Angle: " << con.calcContrast2(0.513889,0.50625,"Gray87Pink57","Gray89Pink61") << endl;
	int col = 328;
	int row = 248;
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

