/*
 * griddisplay.h
 *
 *  Created on: Feb 1, 2015
 *      Author: jason
 */

#ifndef GRIDDISPLAY_H_
#define GRIDDISPLAY_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class GridDisplay {
private:
	Mat gridMat;
	Mat img;
	Size matSize;
	Size gridSize;
public:
	void setGridVec(deque<deque<double> > gridVec);
	void setImage(Mat img);
	void setImageSize(Size size);
	void setGridSize(Size size);
	void drawGrid();
	void displayGrid();
};

#endif /* GRIDDISPLAY_H_ */
