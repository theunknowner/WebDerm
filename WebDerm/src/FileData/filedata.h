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
	deque< deque<String> > rulesMat;
	bool inState;

	deque< deque<String> > m_ContrastMat;
	deque< deque<String> > d_HslMat;
	deque< deque<String> > hslPtMat;
	deque< deque<double> > cumHslMat;

	FileData(String file_path="");
	~FileData();
	void setFilePath(String file_path);
	String getFilePath();
	void getFileMatrix(deque< deque<String> > &vec);

	void writeFileMetaData();
	void writeFileMetaData(int n_args, String* strArr, double *valArr);
	bool loadFileMatrix(String file_path);
	bool loadFileMatrix(String file_path, deque< deque<String> > &dataMat);

	int listFiles(String directory);
	bool getFilesFromDirectory(String directory, deque<String> &files);
	void deleteContainingName(String directory, String filename);
	void renameFiles(String directory, String oldname, String newname);

};



#endif /* FILEDATA_H_ */
