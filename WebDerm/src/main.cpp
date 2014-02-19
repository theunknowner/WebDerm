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
	hsl.importThresholds();
	rgb.importThresholds();
	Mat img, img2, mask;
	img = runResizeImage(path+"Images/LPH","img3.jpg",Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	//displayGUI(img2);
	//cout << hsl.getHslColor(180, 122, 100) << endl;
	//cout << rgb.pushColor(133,107,101) << endl;
	//showPixelColor(148,112,118,Size(400,400));
	int col = 2;
	int row = 2;
	cout << "Result: " << testHysteresis(img2, row, col, Size(3,3)) << endl;
	//runNeuralNetworkTraining("data.csv");
	//createDataFile("rgb.txt", "NN_Data1.csv");
	//imwrite("img.png", img3);
	//runLPH(img4);
	//imshow("Img1", img2);
	//imshow("Img2", img5);
	//waitKey(0);
/*
	String filename;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img = imread(filename);
	Mat img2, img3, mask;
	if(img.cols>700 || img.rows>700)
		{resize(img, img2, Size(700,700),0,0,INTER_AREA);}
	if(img.cols<700 || img.rows<700)
		{resize(img, img2, Size(700,700),0,0, INTER_CUBIC);}
	mask = getSkin(img2);
	img2.copyTo(img3, mask);
	importThresholds3();
	String str;
	int ch;
	vector<double> vec;
	for(;;)
	{
		cout << "Enter col,row: ";
		cin >> str;
		getSubstr2(str,',',vec);
		cout << "Result: " << runHysteresis(img3, vec[1], vec[0], Size(5,5)) << endl;
		cout << "Press any key to continue...";
		getchar();
		for(;;)
		{
			ch = getchar();
			if(ch!=0)
			{
				cout << "\033[2J\033[1;1H";
				break;
			}
		}
	}
*/
	return 0;
}

