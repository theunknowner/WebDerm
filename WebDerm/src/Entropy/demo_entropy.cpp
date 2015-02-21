/*
 * demo_entropy.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: jason
 */

#include "entropy.h"

void Entropy::demo_runCompareEntropy() {
	String input;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		deque<double> resultVecYSV;
		deque<double> resultVecT;
		deque<double> resultVecYSV2;
		deque<double> resultVecT2;
		deque<String> nameVec;
		deque<int> origPos;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name, inputName;
		inputName = getFileName(input,"-");
		double resultsYSV, resultsT;
		int countMatch=0, countSortOfMatch=0, flagMatch=0;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			this->loadEntropyFiles(filepath,vec2,colorNameVec);
			resultsYSV = this->compareYSV(vec1,vec2,colorNameVec);
			resultsT = this->compareT(vec1,vec2,colorNameVec);
			resultVecYSV.push_back(resultsYSV);
			resultVecT.push_back(resultsT);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			//printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
		}

		jaysort(resultVecYSV,origPos);
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
			printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
		}
	}
}

void Entropy::sortMatch(deque<double> &ysvVec, deque<double> &tVec, deque<int> &origPos) {
	deque<double> _ysvVec = ysvVec;
	deque<double> _tVec = tVec;
	deque<int> _origPos = origPos;
	jaysort(_ysvVec,origPos);

}
