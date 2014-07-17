/*
 * filedata.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#include "filedata.h"

FileData::FileData(String filename) {
	this->filename = filename;
	this->shadeCount = 0;
	this->oldMinIntensity = 0;
	this->oldMaxIntensity = 0;
	this->newMinIntensity = 0;
	this->newMaxIntensity = 0;
}

void FileData::setFilename(String filename) { this->filename = filename; }

String FileData::getFilename() { return this->filename; }

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
	fprintf(fp,"%s,%d\n",stringify(shadeCount),this->shadeCount);
}
