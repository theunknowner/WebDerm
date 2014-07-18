/*
 * filedata.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#include "filedata.h"

FileData::FileData(String file_path) {
	this->file_path = file_path;
	this->shadeCount = 0;
	this->minIntensity = 0;
	this->maxIntensity = 0;
	this->minOutlier = 0;
	this->maxOutlier = 0;
	this->matSize = Size(0,0);
}

void FileData::setFilePath(String file_path) {
	this->file_path = file_path;
	this->filename = getFileName(file_path);
}
String FileData::getFilePath() { return this->file_path; }

/** gets the imported data matrix by passing as reference **/
void FileData::getFileMatrix(deque< deque<String> > &vec) {
	vec = this->dataMatrix;
}

void FileData::writeFileMetaData() {
	String filename = this->filename + "_FileData.csv";
	FILE * fp;
	fp = fopen(filename.c_str(), "w");
	fprintf(fp,"Filename,%s\n",this->filename.c_str());
	fprintf(fp,"Path,%s\n",this->file_path.c_str());
	fprintf(fp,"%s,%s\n",stringify(oldMinShade),this->oldMinShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(oldMaxShade),this->oldMaxShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(newMinShade),this->newMinShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(newMaxShade),this->newMaxShade.c_str());
	fprintf(fp,"%s,%0.2f\n",stringify(minIntensity),this->minIntensity);
	fprintf(fp,"%s,%0.2f\n",stringify(maxIntensity),this->maxIntensity);
	fprintf(fp,"%s,%d\n",stringify(shadeCount),this->shadeCount);
	fclose(fp);
}

void FileData::writeFileMetaData(int n_args, String* strArr, double* valArr) {
	String filename = this->filename + "_FileData.csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(int i=0; i<n_args; i++) {
		fprintf(fp,"%s,%s\n",strArr[i].c_str(),toString(valArr[i]).c_str());
	}
	fclose(fp);
}
/** imports the matrix/csv files **/
bool FileData::loadFileMatrix(String file_path) {
	this->filename = getFileName(file_path);
	fstream fs(file_path.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			vec.pop_back(); //removes the weird empty space at the end of deque
			this->dataMatrix.push_back(vec);
			vec.clear();
		}
		deque<String>().swap(vec);
		fs.close();
		return true;
	}
	else
		cout << "Failed to load File matrix!" << endl;
	return false;
}

