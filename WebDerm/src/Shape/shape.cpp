/*
 * shape.cpp
 *
 *  Created on: Dec 22, 2014
 *      Author: jason
 */

#include "shape.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

void Shape::importPoints(String file, vector<vector<Point> > &points) {
	fstream fs(file.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		vector<Point> pts;
		size_t pos=0;
		int x,y;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				pos = vec.at(i).find(";");
				if(pos!=string::npos) {
					x = atoi(vec.at(i).substr(0,pos).c_str());
					y = atoi(vec.at(i).substr(pos+1,vec.at(i).length()).c_str());
					pts.push_back(Point(x,y));
				}
			}
			points.push_back(pts);
			vec.clear();
			pts.clear();
		}
		fs.close();
	}
	else {
		cout << "Import Points Failed!" << endl;
	}
}

//color in the points on the  binary image
void Shape::pointsToImage(Mat &img, vector<Point> points, int connect) {
	int x,y;
	int offset = 90;
	Point first(points.at(0).x+offset,points.at(0).y+offset);
	for(unsigned int i=0; i<points.size(); i++) {
		x = points.at(i).x;
		y = points.at(i).y;
		Point start(x+offset,y+offset);
		if(connect==1) {
			if(i==points.size()-1) {
				line(img,start,first,Scalar(255));
			}
			else {
				Point end(points.at(i+1).x+offset,points.at(i+1).y+offset);
				line(img,start,end,Scalar(255));
			}
		}
		else {
			img.at<uchar>(y+offset,x+offset) = 255;
		}
	}
}
