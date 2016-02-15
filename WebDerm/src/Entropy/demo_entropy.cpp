/*
 * demo_entropy.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: jason
 */

#include "entropy.h"
#include "../run.h"
#include "../FileData/filedata.h"
#include "../functions.h"
#include "../Algorithms/jaysort.h"
#include "../Shape/shapemorph.h"

using namespace ip;

void Entropy::demo_runCompareEntropy() {
	String input, inputName, name;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Input image: ";
	cin >> input;
	inputName = getFileName(input);
	deque<String> files;
	deque<deque<double> > vec2;
	FileData fd;
	String folder = "/home/jason/Desktop/Programs/Demo/YSV_Output/";
	String filepath;
	bool flag = false;
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		filepath = folder+files.at(i);
		String targetName = getFileName(files.at(i),"-");
		if(targetName==inputName) {
			flag = this->loadEntropyFiles(filepath,vec1,colorNameVec);
		}
	}
	if(flag) {
		deque<double> resultVecYSV;
		deque<double> resultVecT;
		deque<String> nameVec;
		deque<int> origPos;
		double resultsYSV, resultsT;
		int countMatch=0, countSortOfMatch=0, flagMatch=0;
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			this->loadEntropyFiles(filepath,vec2,colorNameVec);
			resultsYSV = this->test_compareEntropy2a(vec1,vec2,colorNameVec);
			resultsT = this->compareT(vec1,vec2,colorNameVec);
			resultVecYSV.push_back(resultsYSV);
			resultVecT.push_back(resultsT);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			//printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
		}
		jaysort(resultVecYSV,origPos);
		resultVecYSV.pop_back();
		origPos.pop_back();
		for(unsigned int i=0; i<resultVecYSV.size(); i++) {
			resultsYSV = resultVecYSV.at(i);
			resultsYSV = roundDecimal(resultsYSV,2);
			resultsT = resultVecT.at(origPos.at(i));
			name = nameVec.at(origPos.at(i));
			if(resultsYSV>=0.70 && resultsT>=1.0) {
				countMatch++;
				flagMatch=1;
			}
			else if(resultsYSV>=0.70 && resultsT==0.0) {
				countSortOfMatch++;
				flagMatch=0;
			}
			else if(resultsYSV<0.70 && resultsYSV>=0.60 && resultsT>=1.0) {
				countSortOfMatch++;
				flagMatch=0;
			}
			else {
				flagMatch=-1;
			}
			printf("%s: %f | %f\n",name.c_str(),resultsYSV, resultsT);
		}
		if(inputName!="acne1" && inputName!="photo2a")
			this->demo_showShape(inputName);
	}
	else {
		cout << "File not found!" << endl;
	}
}

void Entropy::demo_showShape(String name) {
	String file = "/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg";
	Mat img = Run::runResizeImage(file,Size(140,140),0);
	ShapeMorph sm;
	Size size(5,5);
	img = Run::runColorNormalization(img);
	cvtColor(img,img,CV_BGR2GRAY);
	vector<Mat> matVec = sm.lumFilter1(img);
	//vector<Mat> matVec2 = sm.lumFilter2(img);
	imgshow(matVec.at(0));
	//imgshow(matVec2.at(0));
}
