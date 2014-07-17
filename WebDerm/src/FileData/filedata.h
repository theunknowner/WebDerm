/*
 * filedata.h
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#ifndef FILEDATA_H_
#define FILEDATA_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class FileData {
private:
	String filename;
	String file_path;
	String oldMinShade;
	String newMinShade;
	String oldMaxShade;
	String newMaxShade;
	double oldMinIntensity;
	double oldMaxIntensity;
	double newMinIntensity;
	double newMaxIntensity;
	int shadeCount;

public:
	FileData(String filename="");
	void setFilename(String filename);
	String getFilename();
	void writeFileMetaData();
};



#endif /* FILEDATA_H_ */
