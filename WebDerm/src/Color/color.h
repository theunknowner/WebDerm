
#ifndef COLOR_H_
#define COLOR_H_

#include "../global.h"

class Color {
public:
	int row, col;
	bool containsColor(String color1, String color2);
	bool containsColor(String argNum, ...);
	bool containsAllColor(String argNum, ...);
	int containsMainColor(String color, String mColor);
	void extractColorFromString(String color, deque<String> &vecColor);
	bool isSameColor(String color1, String color2);
	int countColors(String color);
	String reassignLevels(String color, int r, int g, int b);
	String reassignLevels(String color, double h, double s, double l);
	String getMainColor(String color);
	String extractShade(String pix);
	void output2ImageGray(deque< deque<String> > &input, String name);
	void output2ImageColor(deque< deque<String> > &window, Size size, String name);
	String fixColors(String pix, double r, double g, double b);
	Mat changeImageBrightness(Mat &img, double amt, int type=0);
	Mat changeImageSaturation(Mat img, double amt);
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

	String combineColors(String color);
	Mat output2ImageTargetColor(deque< deque<String> > &window, Size size, String name, String colorTarget, String shadeTarget);

	Mat correctGamma(Mat& img, double gamma);

	Mat shadeCorrection(Mat img);

	String majority(vector<String> strVec);
};
#endif /* COLOR_H_ */
