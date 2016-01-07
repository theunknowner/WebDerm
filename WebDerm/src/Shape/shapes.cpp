/*
 * shapes.cpp
 *
 *  Created on: Jan 5, 2016
 *      Author: jason
 */

#include "shapes.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

vector<String> Shapes::shapeNames;
vector<String> Shapes::shapeNames2;
bool Shapes::SHAPES_IMPORTED = false;

Shapes::Shapes() {
	if(!Shapes::SHAPES_IMPORTED) {
		Shapes::SHAPES_IMPORTED = this->importShapes();
	}
}

bool Shapes::importShapes() {
	if(!Shapes::SHAPES_IMPORTED) {
		String folderName = "Thresholds/";
		String filename = folderName+"shape_names.csv";
		String filename2 = folderName+"shape_names2.csv";
		assert(fs::exists(filename)==true);
		assert(fs::exists(filename2)==true);
		fstream fs(filename);
		fstream fs2(filename2);
		if(fs.is_open() && fs2.is_open()) {
			String temp;
			vector<String> vec;
			while(getline(fs,temp)) {
				Shapes::shapeNames.push_back(temp);
			}
			while(getline(fs2,temp)) {
				Shapes::shapeNames2.push_back(temp);
			}
			fs.close();
			fs2.close();
			return true;
		} else {
			cout << "Importing shape_names.csv && shape_names2.csv failed!" << endl;
			return false;
		}
	}
	return true;
}

String Shapes::getShapeName(int num) {
	return Shapes::shapeNames.at(num);
}

String Shapes::getShapeName2(int num) {
	return Shapes::shapeNames2.at(num);
}

int Shapes::getShapeIndex(String shape) {
	auto it = std::find(Shapes::shapeNames.begin(),Shapes::shapeNames.end(),shape);
	if(it!=Shapes::shapeNames.end()) {
		return std::distance(Shapes::shapeNames.begin(),it);
	} else {
		printf("Shapes:getShapeIndex() %s does not exist\n",shape.c_str());
		exit(1);
	}
}

int Shapes::getShapeIndex2(String shape) {
	auto it = std::find(Shapes::shapeNames2.begin(),Shapes::shapeNames2.end(),shape);
	if(it!=Shapes::shapeNames2.end()) {
		return std::distance(Shapes::shapeNames2.begin(),it);
	} else {
		printf("Shapes:getShapeIndex2() %s does not exist\n",shape.c_str());
		exit(1);
	}
}
