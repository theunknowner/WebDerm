/*
 * color.h
 *
 *  Created on: Mar 26, 2014
 *      Author: jason
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "/home/jason/git/WebDerm/WebDerm/src/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Contrast/contrast.h"

class Color {
public:
	bool containsColor(String color1, String color2);
	int containsMainColor(String color, String mColor);
	bool isSameColor(String color1, String color2);
	void extractColorFromString(String color, deque<String> &vecColor);
	int countColors(String color);
	String reassignLevels(String color, int r, int g, int b);
	String getMainColor(String color);
	void output2Image(deque< deque<double> > &input);
};
#endif /* COLOR_H_ */
