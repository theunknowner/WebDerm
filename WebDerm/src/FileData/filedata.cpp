/*
 * filedata.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#include "filedata.h"

FileData::FileData(String file_path) {
	this->file_path = file_path;
	shadeCount = 0;
	minIntensity = 0;
	maxIntensity = 0;
	minOutlier = 0;
	maxOutlier = 0;
	matSize = Size(0,0);
	localRatioScanSize = 0;
	localScanSize=0;
	range=0;
}

FileData::~FileData() {
	matImage.release();
	deque< deque<String> >().swap(windowVec);
	deque< deque<String> >().swap(hslMat);
	deque< deque<double> >().swap(absRatioMat);
	deque< deque<double> >().swap(relRatioMat);
	deque< deque<String> >().swap(colorVec);
	deque< deque<double> >().swap(intensityVec);
	deque< deque<double> >().swap(smoothIntensityVec);
	deque< deque<String> >().swap(shadeVec);
	deque<double>().swap(relRatioVec);
	deque<double>().swap(absRatioVec);
	printf("Destructor executed for %s\n",filename.c_str());
}

void FileData::setFilePath(String file_path) {
	this->file_path = file_path;
	filename = getFileName(file_path);
}
String FileData::getFilePath() { return file_path; }

/** gets the imported data matrix by passing as reference **/
void FileData::getFileMatrix(deque< deque<String> > &vec) {
	vec = dataMatrix;
}

void FileData::writeFileMetaData() {
	String filename = this->filename + "_FileData.csv";
	FILE * fp;
	fp = fopen(filename.c_str(), "w");
	fprintf(fp,"Filename,%s\n",filename.c_str());
	fprintf(fp,"Path,%s\n",this->file_path.c_str());
	fprintf(fp,"%s,%s\n",stringify(oldMinShade), oldMinShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(oldMaxShade), oldMaxShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(newMinShade), newMinShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(newMaxShade), newMaxShade.c_str());
	fprintf(fp,"%s,%0.2f\n",stringify(minIntensity), minIntensity);
	fprintf(fp,"%s,%0.2f\n",stringify(maxIntensity), maxIntensity);
	fprintf(fp,"%s,%d\n",stringify(shadeCount), shadeCount);
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
	filename = getFileName(file_path);
	fstream fs(file_path.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(1,vec.at(i).length()-2);
			}
			dataMatrix.push_back(vec);
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

