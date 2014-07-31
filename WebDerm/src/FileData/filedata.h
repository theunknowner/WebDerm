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
	double range;
	Size matSize;
	Mat matImage;
	Point pt; // current pt location of the filedata matrices
	double localRatioScanSize;
	double localScanSize;
	deque< deque<String> > windowVec;
	deque< deque<String> > hslMat;
	deque<double> absRatioVec;
	deque< deque<double> > absRatioMat;
	deque< deque<String> > colorVec;
	deque< deque<double> > intensityVec;
	deque< deque<double> > smoothIntensityVec;
	deque< deque<String> > shadeVec;

	FileData(String file_path="");
	~FileData();
	void setFilePath(String file_path);
	String getFilePath();
	void getFileMatrix(deque< deque<String> > &vec);

	void writeFileMetaData();
	void writeFileMetaData(int n_args, String* strArr, double *valArr);
	bool loadFileMatrix(String file_path);

	int listFiles(String directory);
	bool getFilesFromDirectory(String directory, deque<String> &files);
};



#endif /* FILEDATA_H_ */
