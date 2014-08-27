/*
 * run.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"

Mat runResizeImage(String foldername, String filename, Size size,int write)
{
	String file = foldername+"/"+filename;
	deque<String> vec;
	Mat img = imread(file);
	Mat img2;
	if(img.cols>=size.width || img.rows>=size.height)
		{resize(img, img2, size,0,0,INTER_AREA);}
	if(img.cols<size.width || img.rows<size.height)
		{resize(img, img2, size,0,0, INTER_CUBIC);}
	getSubstr(filename,'.',vec);
	if(write==1) imwrite(vec[0]+".png",img2);

	deque<String>().swap(vec);
	return img2;
}

Mat runResizeImage(String filename, Size size,int write)
{
	deque<String> vec;
	Mat img = imread(filename);
	Mat img2;
	if(!img.empty()) {
		if(img.cols>=size.width || img.rows>=size.height)
			{resize(img, img2, size,0,0,INTER_AREA);}
		if(img.cols<size.width || img.rows<size.height)
			{resize(img, img2, size,0,0, INTER_CUBIC);}
		getSubstr(filename,'.',vec);
		if(write==1) imwrite(vec[0]+".png",img2);

		img.release();
		deque<String>().swap(vec);
	}
	return img2;
}

void runHysteresis()
{
	rgb rgb;
	hsl hsl;
	Shades shade;
	Color c;
	String filename;
	String name;
	Size size(2,2);
	cout << "Enter filename: ";
	cin >> filename;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	name = getFileName(filename);
	int s = 3;
	bool flag[s];
	flag[0]=rgb.importThresholds();
	flag[1]=hsl.importHslThresholds();
	flag[2]=shade.importThresholds();
	for(int i=0; i<s; i++) {
		if(flag[i]==false) {
			flag[0] = false;
			break;
		}
	}
	if(flag[0]==true) {
		FileData fd(filename);
		fd.matSize = size;
		fd.matImage = img2;
		//hysteresis(fd);

		String windowVecFile = "/home/jason/Desktop/Programs/" + name + ".csv";
		String hslVecFile = "/home/jason/Desktop/Programs/" + name + "_HSL.csv";
		Intensity in;
		fd.loadFileMatrix(windowVecFile, fd.windowVec);
		fd.loadFileMatrix(hslVecFile, fd.hslMat);
		fd.colorVec = in.calcMainColorMatrix(fd.matImage,fd.windowVec,fd.hslMat,fd.filename,fd);
/**/
		writeSeq2File(fd.windowVec,name);
		writeSeq2File(fd.hslMat,name+"_HSL");
		fd.writeFileMetaData();
		c.output2ImageColor(fd.colorVec,name);
		writeSeq2File(fd.absRatioMat,name+"_AbsoluteRatios");
		writeSeq2File(fd.intensityVec,name+"_ColorIntensity");
		writeSeq2File(fd.smoothIntensityVec,name+"_SmoothIntensity");
		writeSeq2File(fd.colorVec,name+"_ShadeColors");
		writeSeq2File(fd.rulesMat,name+"_RulesMat");
		writeSeq2File(fd.m_ContrastMat,name+"_measuredContrast");
		writeSeq2File(fd.d_HslMat,name+"_deltaHSL");
		writeSeq2File(fd.hslPtMat,name+"_hslPts");
		writeSeq2File(fd.cumHslMat,name+"_cumHSL");
	}

	img.release(); img2.release(); mask.release();
	rgb.release_memory();
	hsl.release_memory();
	shade.release_memory();
}

void runAllHysteresis(String *filenames, int fileSize) {
	rgb rgb;
	hsl hsl;
	Shades shade;
	Color c;
	String name;
	Size size(2,2);
	Mat img, img2, mask;
	int s = 3;
	bool flag[s];
	flag[0]=rgb.importThresholds();
	flag[1]=hsl.importHslThresholds();
	flag[2]=shade.importThresholds();
	for(int i=0; i<s; i++) {
		if(flag[i]==false) {
			flag[0] = false;
			break;
		}
	}
	if(flag[0]==true) {
		for(int i=0; i<fileSize; i++) {
			img = runResizeImage(filenames[i],Size(700,700),0);
			getSkin(img, mask);
			img.copyTo(img2, mask);
			name = getFileName(filenames[i]);
			FileData fd(filenames[i]);
			fd.matImage = img2;
			fd.matSize = size;
			hysteresis(fd);
			writeSeq2File(fd.windowVec,name);
			writeSeq2File(fd.hslMat,name+"_HSL");
			fd.writeFileMetaData();
			c.output2ImageColor(fd.colorVec,name);
			writeSeq2File(fd.absRatioMat,name+"_AbsoluteRatios");
			writeSeq2File(fd.intensityVec,name+"_ColorIntensity");
			writeSeq2File(fd.smoothIntensityVec,name+"_SmoothIntensity");
			writeSeq2File(fd.colorVec,name+"_ShadeColors");
			writeSeq2File(fd.rulesMat,name+"_RulesMat");
			writeSeq2File(fd.m_ContrastMat,name+"_measuredContrast");
			writeSeq2File(fd.d_HslMat,name+"_deltaHSL");
			writeSeq2File(fd.hslPtMat,name+"_hslPts");
			writeSeq2File(fd.cumHslMat,name+"_cumHSL");

			//release images for next use
			img.release(); img2.release(); mask.release();
		}
	}
	rgb.release_memory();
	hsl.release_memory();
	shade.release_memory();
}

void runAllHysteresis() {
	rgb rgb;
	hsl hsl;
	Shades shade;
	Color c;
	String folder, full_path;
	cout << "Enter folder_name: ";
	cin >> folder;
	FileData fdFiles;
	deque<String> files;
	String name;
	Size size(2,2);
	Mat img, img2,img3, mask;
	int s = 4;
	bool flag[s];
	flag[0]=rgb.importThresholds();
	flag[1]=hsl.importHslThresholds();
	flag[2]=shade.importThresholds();
	flag[3]=fdFiles.getFilesFromDirectory(folder, files);
	for(int i=0; i<s; i++) {
		if(flag[i]==false) {
			flag[0] = false;
			break;
		}
	}
	if(flag[0]==true) {
		for(unsigned int i=0; i<files.size(); i++) {
			full_path = folder+files.at(i);
			img = runResizeImage(full_path,Size(700,700),0);
			getSkin(img, mask);
			img.copyTo(img2, mask);
			name = getFileName(full_path);
			FileData fd(full_path);
			fd.matImage = img2;
			fd.matSize = size;
			hysteresis(fd);
/*
			String windowVecFile = "/home/jason/Desktop/Programs/Hysteresis/" + name + ".csv";
			String hslVecFile = "/home/jason/Desktop/Programs/Hysteresis/" + name + "_HSL.csv";
			Intensity in;
			fd.loadFileMatrix(windowVecFile, fd.windowVec);
			//fd.getFileMatrix(fd.windowVec);
			fd.loadFileMatrix(hslVecFile, fd.hslMat);
			//fd.getFileMatrix(fd.hslMat);
			fd.colorVec = in.calcMainColorMatrix(fd.matImage,fd.windowVec,fd.hslMat,fd.filename,fd);
/**/
			writeSeq2File(fd.windowVec,name);
			writeSeq2File(fd.hslMat,name+"_HSL");
			fd.writeFileMetaData();
			c.output2ImageColor(fd.colorVec,name);
			writeSeq2File(fd.absRatioMat,name+"_AbsoluteRatios");
			writeSeq2File(fd.intensityVec,name+"_ColorIntensity");
			writeSeq2File(fd.smoothIntensityVec,name+"_SmoothIntensity");
			writeSeq2File(fd.colorVec,name+"_ShadeColors");
			writeSeq2File(fd.rulesMat,name+"_RulesMat");
			writeSeq2File(fd.m_ContrastMat,name+"_measuredContrast");
			writeSeq2File(fd.d_HslMat,name+"_deltaHSL");
			writeSeq2File(fd.hslPtMat,name+"_hslPts");
			writeSeq2File(fd.cumHslMat,name+"_cumHSL");

			//release images for next use
			img.release(); img2.release(); mask.release();
		}
	}
	rgb.release_memory();
	hsl.release_memory();
	shade.release_memory();
}

 void runGetSkin()
 {
	String filename;
	String input;
	cout << "Enter filename: ";
	cin >> filename;
	cout << "Do you want to write image? (y/n)";
	cin >> input;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2, mask);
	if(input=="y")
	{
		String name = getFileName(filename);
		imwrite(name+".png",img2);
	}
	imshow("Img", img2);
	waitKey(0);
	img.release(); img2.release(); mask.release();
 }

 void runAllGetSkin() {
	 FileData fdFiles;
	 deque<String> files;
	 String input, folder, full_path;
	 Mat img, img2, mask;
	 cout << "Enter folder_name: ";
	 cin >> folder;
	 if(fdFiles.getFilesFromDirectory(folder, files)) {
		 cout << "Do you want to write image? (y/n)";
		 cin >> input;
		 for(unsigned int i=0; i<files.size(); i++) {
			 full_path = folder+files.at(i);
			 img = runResizeImage(full_path,Size(700,700),0);
			 getSkin(img, mask);
			 img.copyTo(img2, mask);
			 if(input=="y")
			 {
				 String name = getFileName(full_path);
				 imwrite(name+".png",img2);
			 }
			 imshow("Img", img2);
			 waitKey(0);
			 img.release(); img2.release(); mask.release();
		 }
	 }
 }

 //runs hysteresis with info output to terminal
void runMouseHysteresis()
{
	rgb rgb;
	hsl hsl;
	hsl.importHslThresholds();
	rgb.importThresholds();
	Mat img2, mask;
	String filename;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2,mask);
	namedWindow("Hysteresis");
	cvSetMouseCallback("Hysteresis", onMouseHysteresis, &img2);
	while (waitKey(1)<0)
	{
	imshow("Hysteresis", img2);
	}
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
}

//runs hysteresis with info output to image window
void runMouseHysteresis2()
{
	rgb rgb;
	hsl hsl;
	hsl.importHslThresholds();
	rgb.importThresholds();
	Mat img2, mask;
	String filename;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img = runResizeImage(filename,Size(700,700),0);
	getSkin(img, mask);
	img.copyTo(img2,mask);
	namedWindow("Hysteresis");
	cvSetMouseCallback("Hysteresis", onMouseHysteresis2, &img2);
	imshow("Hysteresis", img2);
	waitKey(0);
	img.release(); img2.release(); mask.release();
	rgb.release_memory();
	hsl.release_memory();
}

void outputFreqColor(Mat &img)
{
	rgb rgb;
	int arr[rgbColors.size()];
	fill_n(arr, rgbColors.size(), 0);
	int ind=0;
	double dist=0;
	int r,g,b;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			rgb.pushColor(r,g,b,ind,dist);
			++arr[ind];
		}
	}
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorFreq.txt","w");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		if(arr[i]!=0)
		{
			fprintf(fp,"%d: %s(%d)\n", i+2, rgbColors.at(i).c_str(), arr[i]);
		}
	}
}

void runMouseColor() {
	String filename,input;
	String name;
	Size size(2,2);
	cout << "Enter filename: ";
	cin >> filename;
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	cout << "Get skin? (y/n) ";
	cin >> input;
	if(input=="y") getSkin(img, mask);
	img.copyTo(img2, mask);
	Mouse::mouseColor(img2);
	img.release(); img2.release(); mask.release();
}
