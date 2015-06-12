/*
 * islands.h
 *
 *  Created on: Jun 12, 2015
 *      Author: jason
 */

#ifndef ISLANDS_H_
#define ISLANDS_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Islands {
private:
	int islArea;
	int islShadeLevel;
	Mat islandImg;
public:
	Islands(Mat islandImg);
	int area();
	int shade();
	Mat image();
};

#endif /* ISLANDS_H_ */
