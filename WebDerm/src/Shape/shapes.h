/*
 * shapes.h
 *
 *  Created on: Jan 5, 2016
 *      Author: jason
 */

#ifndef SHAPES_H_
#define SHAPES_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Shapes {
protected:
	static vector<String> shapeNames;
	static vector<String> shapeNames2;
	static bool SHAPES_IMPORTED;
public:
	Shapes();
	bool importShapes();
	String getShapeName(int num);
	String getShapeName2(int num);
	int getShapeIndex(String shape);
	int getShapeIndex2(String shape);
};

#endif /* SHAPES_H_ */
