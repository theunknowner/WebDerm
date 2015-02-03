/*
 * demo_entropy.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: jason
 */

#include "entropy.h"

void Entropy::demo_runCompareEntropy() {
	String input, input2;
	String name1, name2;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter 1st image: ";
	cin >> input;
	cout << "Enter 2nd image: ";
	cin >> input2;
	name1 = getFileName(input);
	name2 = getFileName(input2);
	deque<String> files;
	deque<deque<double> > vec2;
	FileData fd;
	String folder = "/home/jason/Desktop/Programs/TestYSV_Output/TestYSV_Output_Demo/";
	String filepath, name;
	double resultYSV=0, resultT=0;
	deque<int> origPos;
	bool flag[2] = {false};
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		filepath = folder+files.at(i);
		String targetName = getFileName(files.at(i),"-");
		if(targetName==name1) {
			flag[0] = this->loadEntropyFiles(filepath,vec1,colorNameVec);
			cout << "First image found!" << endl;
		}
		if(targetName==name2) {
			flag[1] = this->loadEntropyFiles(filepath,vec2,colorNameVec);
			cout<< "Second image found!" << endl;
		}
	}

	if(flag[0]==true && flag[1]==true) {
		double results=0;
		resultYSV = this->compareYSV(vec1,vec2,colorNameVec);
		resultT = this->compareT(vec1,vec2,colorNameVec);
		vec2.clear();
		vec2.shrink_to_fit();
		results = (resultYSV + resultT)/2;
		printf("%s vs. %s\n",name1.c_str(),name2.c_str());
		cout << "RESULTS: " << results << flush;
		//printf("RESULTS: %f", resultYSV);
		if(results>0.60)
			cout << "  *** MATCH ***" << endl;
		else
			cout << "  *** NO MATCH ***" << endl;

	}
}
