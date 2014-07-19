/*
 * filedata.h
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#ifndef FILEDATA_H_INCLUDED
#define FILEDATA_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

class FileData {
private:
	String file_path;
	deque< deque<String> > dataMatrix;

public:
	String filename;
	String oldMinShade;
	String newMinShade;
	String oldMaxShade;
	String newMaxShade;
	double minIntensity;
	double maxIntensity;
	double minOutlier;
	double maxOutlier;
	int shadeCount;
	Size matSize;
	Mat matImage;

	FileData(String file_path="");
	void setFilePath(String file_path);
	String getFilePath();
	void getFileMatrix(deque< deque<String> > &vec);

	void writeFileMetaData();
	void writeFileMetaData(int n_args, String* strArr, double *valArr);
	bool loadFileMatrix(String file_path);
	void printMatrixData(int x, int y, String data="");
};



#endif /* FILEDATA_H_ */
