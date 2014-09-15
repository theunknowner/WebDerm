
#ifndef COLOR_H_
#define COLOR_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"

class Color {
public:
	bool containsColor(String color1, String color2);
	bool containsColor(String argNum, ...);
	bool containsAllColor(String argNum, ...);
	int containsMainColor(String color, String mColor);
	bool isSameColor(String color1, String color2);
	void extractColorFromString(String color, deque<String> &vecColor);
	int countColors(String color);
	String reassignLevels(String color, int r, int g, int b);
	String getMainColor(String color);
	String extractShade(String pix);
	void output2ImageGray(deque< deque<String> > &input, String name);
	void output2ImageColor(deque< deque<String> > &window, String name);
	String fixColors(String pix, double r, double g, double b);
	Mat changeImageBrightness(Mat &img, double amt);
	Mat changeImageContrast(Mat img, double alpha=1, double beta=0);
	void changeContrast(double &r, double &g, double &b, double alpha=1, double beta=0);
	String optimizeColor(String pix);
	String optimizeColor2(String pix);
	double* extractHSL(String inputString);
	String removeColor(String color, String colorRemove);
	String replaceColor(String color, String colorTarget, String colorReplace);
	void avgImageLuminance(Mat &src);
	void imgRgb2Gray(Mat &src, Mat &dst);
	int* changeRgbRelLum(double r, double g, double b, double amt);
};
#endif /* COLOR_H_ */
