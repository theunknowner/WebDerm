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

class Color {
public:
	bool containsColor(String color1, String color2);
	int containsMainColor(String color, String mColor);
	bool isSameColor(String color1, String color2);
	void extractColorFromString(String color, vector<String> &vecColor);
	int countColors(String color);
};
#endif /* COLOR_H_ */
