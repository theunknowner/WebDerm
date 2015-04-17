/*
 * scripts.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: jason
 */

#include "scripts.h"

void script1() {
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Positive_Pairs/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int fileLabel= 116;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder+files.at(i);
		String filename = files.at(i);
		Mat img = imread(name,0);
		Mat src1 = img(Rect(0,0,35,35));
		Mat src2 = img(Rect(35,0,35,35));
		Mat stitch = ctd.stitchData(src2,src1);
		size_t beginPos=0;
		String seqStr = fd.getFileSequenceNum(filename,"_",beginPos);
		int seq = atoi(seqStr.c_str());
		String seqStrNext = toString(seq+fileLabel);
		if((seq+fileLabel)<1000)
			seqStrNext = "0" + seqStrNext;
		filename.replace(beginPos,seqStr.length(),seqStrNext.c_str());
		String newname = "/home/jason/Desktop/Programs/Training_Samples/Test3/" + filename;
		imwrite(newname,stitch);
	}
}

void script2() {
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Negative_Pairs/";
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	FILE * fp;
	fp = fopen("pos.csv","w");
	for(unsigned int i=0; i<files.size(); i++) {
		String name = getFileName(files.at(i));
		fprintf(fp,"%s\n",name.c_str());
	}
	fclose(fp);
}
