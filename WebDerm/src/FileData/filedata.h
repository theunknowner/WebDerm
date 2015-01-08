/*
 * filedata.h
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#ifndef FILEDATA_H_INCLUDED
#define FILEDATA_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"

class FileData {
private:
	String file_path;
	deque< deque<String> > dataMatrix;
	Size imgSize;
	Mat matImage;

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
	int totalShades;
	double range;
	Size ksize;
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

	deque< deque<String> > m_ContrastMat;
	deque< deque<String> > d_HslMat;
	deque< deque<String> > hslPtMat;
	deque< deque<double> > cumHslMat;
	deque< deque<String> > minMaxHslMat;

	deque< deque<int> > shadeColorCount;
	deque<deque<double> > maxHslValues; //max HSL values of each color

	FileData(String file_path="");
	~FileData();
	void setFilePath(String file_path);
	String getFilePath();
	void getFileMatrix(deque< deque<String> > &vec);

	void setImage(Mat img);
	Mat getImage();

	void writeFileMetaData();
	void writeFileMetaData(int n_args, String* strArr, double *valArr);
	bool loadFileMatrix(String file_path);
	bool loadFileMatrix(String file_path, deque< deque<String> > &dataMat);

	int listFiles(String directory);
	bool getFilesFromDirectory(String directory, deque<String> &files, int flag=0);
	void deleteContainingName(String directory, String filename);
	void renameFiles(String directory, String oldname, String newname);

	void getMinMaxPts(String input, deque< deque<Point> > &pt);
	void extractRule3Data(String filename, Point loc);

	deque< deque<double> > stringVec2Double(deque< deque<String> > &vec);

};



#endif /* FILEDATA_H_ */
