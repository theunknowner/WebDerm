/*
 * tools.cpp
 *
 *  Created on: Feb 24, 2016
 *      Author: jason
 */

#include "tools.h"
#include "../functions.h"
#include "../ImageData/imagedata.h"
#include "../ShadeShape/shadeshape.h"

namespace tools {

//! creates executable for clickable islands
void clickShowIslands(String filename) {
	String name = ip::getFileName(filename);
	Mat img = imread(filename,0);
	ImageData id(img,name);
	ip::prepareImage(id,Size(140,140));
	ShadeShape ss1;
	ss1.extract(id,false);
	//ss1.showInteractiveSubIslands();
	ss1.showInteractiveIslands();
}

} /* namespace tools */
