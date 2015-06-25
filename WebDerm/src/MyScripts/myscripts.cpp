/*
 * myscripts.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: jason
 */

#include "myscripts.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Create/createtrainingdata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/CIE/cie.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/cielab.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Colorspace/xyz.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Poly/poly.h"
#include "/home/jason/git/WebDerm/WebDerm/src/KneeCurve/kneecurve.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/cluster.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/shapemorph.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/shapecolor.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shape/ShadeShape/shadeshape.h"
#include "/home/jason/git/WebDerm/WebDerm/src/neuralnetworks/testml.h"

namespace Scripts {
void script1() {
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	String out = "/home/jason/Desktop/Programs/Color Normalized Non Blur 5x5/";
	deque<String> files;
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	Mat imgGray;
	for(unsigned int i=0; i<files.size(); i++) {
		String filename = folder+files.at(i);
		Mat img = imread(filename);
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));
		cvtColor(img,imgGray,CV_BGR2GRAY);
		filename = getFileName(filename);
		imwrite(out+filename+".png",img);
		imwrite(out+filename+"_Gray.png",imgGray);
	}
}

//! generate names of training data files
void script2() {
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental2/Negative_Pairs/";
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

//! create/modify stitches
void script3() {
	CreateTrainingData ctd;
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Positive_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int count = 1571;
	for(unsigned int i=0; i<files.size(); i++) {
		int flag=0;
		size_t pos=0;
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=4418 && seq<=4642) {
			String file = folder + files.at(i);
			Mat img = imread(file,0);
			Mat src1 = img(Rect(0,0,35,35));
			Mat src2 = img(Rect(35,0,35,35));
			Mat blank(src1.size(),CV_8U,Scalar(0));
			/*int v = img.at<uchar>(0,0);
			for(int row=0; row<img.rows; row++) {
				for(int col=0; col<img.cols; col++) {
					int val = img.at<uchar>(row,col);
					if(val==v)
						img.at<uchar>(row,col) = 0;
					if(val==10 && v!=10) {
						flag=1;
						break;
					}
				}
				if(flag==1) break;
			}*/
			if(flag==0) {
				img = ctd.stitchData(blank,src1);
				String newname = out + "neg_" + toString(count) + ".png";
				imwrite(newname,img);
				count++;
			}
		}
	}
}

//! swap the training data
void script4() {
	CreateTrainingData ctd;
	String out = "/home/jason/Desktop/Programs/Training_Samples/Test/Negative_Pairs/";
	Mat img = imread("/home/jason/Desktop/Programs/Training_Samples/Candidates/pos_can_9.png",0);
	Mat src1 = img(Rect(0,0,35,35));
	Mat src2 = img(Rect(35,0,35,35));
	img = ctd.stitchData(src2,src1);
	String name = out + "sample_742.png";
	imwrite(name,img);
}

//! get half of stitch
void script5() {
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Test/Negative_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	for(unsigned int i=0; i<12; i++) {
		String name = folder+files.at(i);
		String filename = files.at(i);
		Mat img = imread(name,0);
		Mat src1 = img(Rect(0,0,35,35));
		Mat src2 = img(Rect(35,0,35,35));
		name = out + "half_" + toString(i+1) + ".png";
		imwrite(name,src2);
	}
}

//! stitch together combinations of half-stitches
void script6() {
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Data/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	size_t pos=0;
	int count=4418;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=1112 && seq<=1126) {
			Mat src1 = imread(name,0);
			for(unsigned int j=0; j<files.size(); j++) {
				name = folder + files.at(j);
				seqStr = fd.getFileSequenceNum(files.at(j),"_",pos);
				seq = atoi(seqStr.c_str());
				if(seq>=1112 && seq<=1126) {
					Mat src2 = imread(name,0);
					Mat stitch = ctd.stitchData(src1,src2);
					name = out + "pos_" + toString(count) + ".png";
					imwrite(name,stitch);
					count++;
				}
			}
		}
	}
}

//! flip it
void script7() {
	CreateTrainingData ctd;
	Functions fn;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Negative_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	size_t pos=0;
	int count=928;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=49 && seq<=96) {
			Mat img = imread(name,0);
			Mat src1 = img(Rect(0,0,35,35));
			Mat src2 = img(Rect(35,0,35,35));
			flip(src1,src1,1);
			//flip(src2,src2,1);
			Mat stitch = ctd.stitchData(src1,src2);
			filename.replace(pos,seqStr.length(),toString(seq+count));
			name = out + filename;
			imwrite(name,stitch);
		}
	}
}

//! shifting/perturbing the training data
void script8() {
	Functions fn;
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Positive_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	size_t pos=0;
	int count = 577;
	for(unsigned i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		if(filename.find("copy")==string::npos) {
			Mat img = imread(name,0);
			Mat src1 = img(Rect(0,0,35,35));
			Mat src2 = img(Rect(35,0,35,35));
			Mat shifted2;
			String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
			int seq = atoi(seqStr.c_str());
			for(int a=-5; a<=5; a+=5) {
				for(int b=-5; b<=5; b+=5) {
					if(a!=0 || b!=0) {
						shifted2 = fn.shiftImage(src2,a,b,1);
						Mat stitch = ctd.stitchData(src1,shifted2);
						filename.replace(pos,seqStr.length(),toString(count));
						name = out + filename;
						imwrite(name,stitch);
					}
				}
			}
		}
	}
}

//! change background color
void script9() {
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Positive_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int count = 1553;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder+files.at(i);
		String filename = files.at(i);
		size_t pos=0;
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=666 && seq<=739) {
			Mat img = imread(name,0);
			vector<float> freq;
			int val = img.at<uchar>(0,0);
			freq.push_back((float)val);
			val = img.at<uchar>(0,img.rows-1);
			freq.push_back((float)val);
			val = img.at<uchar>(img.cols-1,0);
			freq.push_back((float)val);
			val = img.at<uchar>(img.cols-1,img.rows-1);
			freq.push_back((float)val);
			float most = majority(freq);
			for(int row=0; row<img.rows; row++) {
				for(int col=0; col<img.cols; col++) {
					if(img.at<uchar>(row,col)==(int)most) {
						img.at<uchar>(row,col) = 255;
					}
				}
			}
			filename.replace(pos,seqStr.length(),toString(count));
			name = out + filename;
			imwrite(name,img);
			count++;
		}
	}
}

//! add blanks of background color to stitch
void script10() {
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Positive_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int count = 1121;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder+files.at(i);
		String filename = files.at(i);
		size_t pos=0;
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=4418 && seq<=4642) {
			Mat img = imread(name,0);
			Mat src1 = img(Rect(0,0,35,35));
			Mat src2 = img(Rect(35,0,35,35));
			vector<float> freq;
			int val = img.at<uchar>(0,0);
			freq.push_back((float)val);
			val = img.at<uchar>(0,img.rows-1);
			freq.push_back((float)val);
			val = img.at<uchar>(img.cols-1,0);
			freq.push_back((float)val);
			val = img.at<uchar>(img.cols-1,img.rows-1);
			freq.push_back((float)val);
			float most = majority(freq);
			src1 = (int) most;
			img = ctd.stitchData(src1,src2);

			filename.replace(pos,seqStr.length(),toString(count));
			name = out + filename;
			imwrite(name,img);
			count++;
		}
	}
}

//!copy files from one folder to another
void script11() {
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Negative_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Data/Negative_Pairs/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.begin());
	for(unsigned i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		if(filename.find("copy")==string::npos) {
			Mat img = imread(name,0);
			name = out + filename;
			imwrite(name,img);
		}
	}
}

void script12() {
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Data/Negative_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	for(unsigned i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		Mat img = imread(name,0);
		vector<float> freq;
		int val = img.at<uchar>(0,0);
		freq.push_back((float)val);
		val = img.at<uchar>(img.rows-1,0);
		freq.push_back((float)val);
		val = img.at<uchar>(0,img.cols-1);
		freq.push_back((float)val);
		val = img.at<uchar>(img.rows-1,img.cols-1);
		freq.push_back((float)val);
		float most = majority(freq);
		for(int row=0; row<img.rows; row++) {
			for(int col=0; col<img.cols; col++) {
				if(img.at<uchar>(row,col)==(int)most) {
					img.at<uchar>(row,col) = 0;
				}
				else {
					img.at<uchar>(row,col) = 255;
				}
			}
		}
		name = out + files.at(i);
		imwrite(name,img);
	}
}

void script13() {
	Functions fn;
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Positive_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	size_t pos = 0;
	int count = 3186;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		String seqStr = fd.getFileSequenceNum(filename,"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=1 && seq<=1062) {
			Mat img = imread(name,0);
			Mat src1 = img(Rect(0,0,35,35));
			Mat src2 = img(Rect(35,0,35,35));
			Mat blank(src1.size(),CV_8U,Scalar(0));
			src1 = fn.rotateImage(src1,90.0);
			src2 = fn.rotateImage(src2,90.0);
			img = ctd.stitchData(src1,src2);

			filename.replace(pos,seqStr.length(),toString(seq+count));
			name = out + filename;
			imwrite(name,img);
		}
	}
}

void script14() {
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Negative_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Data/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		Mat img = imread(name,0);
		Mat src1 = img(Rect(0,0,35,35));
		Mat src2 = img(Rect(35,0,35,35));
		Mat blank(src1.size(),CV_8U,Scalar(0));
		name = out + filename;
		if(countNonZero(src1)==countNonZero(blank)) {
			imwrite(name,src2);
		}
		else {
			imwrite(name,src1);
		}
	}
}

void script15() {
	Functions fn;
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/webderm/Files/Run/Data/";
	String out = "/home/webderm/Files/Run/Positive_Pairs/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	vector<Mat> matVec;
	int count=1;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		Mat img = imread(name,0);
		Mat src1 = img(Rect(0,0,35,35));
		Mat src2 = img(Rect(35,0,35,35));
		for(double d=0.0; d<=9.0; d+=3.0) {
			Mat rot = fn.rotateImage(src1,d);
			rot = fn.fixBinaryImage(rot);
			for(int x=-5; x<=5; x+=2) {
				for(int y=-5; y<=5; y+=2) {
					Mat shift = fn.shiftImage(rot,x,y);
					for(double d2=0.0; d2<=9.0; d2+=3.0) {
						Mat rot2 = fn.rotateImage(src2,d2);
						rot2 = fn.fixBinaryImage(rot2);
						for(int x2=-5; x2<=5; x2+=2) {
							for(int y2=-5; y2<=5; y2+=2) {
								Mat shift2 = fn.shiftImage(rot2,x2,y2);
								Mat stitch = ctd.stitchData(shift,shift2);
								name = out + "pos_" + toString(count) + ".png";
								imwrite(name,stitch);
								printf("%s written\n",name.c_str());
								count++;
							}
						}
					}
				}
			}
		}
	}
}

void script16() {
	Functions fn;
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/webderm/Files/Run/Data/";
	String out = "/home/webderm/Files/Run/Negative_Pairs/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	vector<Mat> matVec;
	int count=1513;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		Mat img = imread(name,0);
		Mat src1 = img(Rect(0,0,35,35));
		Mat src2 = img(Rect(35,0,35,35));
		for(double d=0.0; d<360.0; d+=3.0) {
			if(d<=9.0 || d>=351.0) {
				Mat rot = fn.rotateImage(src1,d);
				rot = fn.fixBinaryImage(rot);
				for(int x=-5; x<=5; x+=2) {
					for(int y=-5; y<=5; y+=2) {
						Mat shift = fn.shiftImage(rot,x,y);
						Mat stitch1 = ctd.stitchData(shift,src2);
						name = out + "neg_" + toString(count) + ".png";
						imwrite(name,stitch1);
						printf("%s written\n",name.c_str());
						count++;
					}
				}
			}
		}
	}
}

//! Expressive Power of Hue
double epoh(double &sat, double &lum) {
	double result;
	if(lum<=50) {
		result = (sat/100.) * (lum/50.);
	}
	else {
		result = (sat/100.) * (100-lum)/50;
	}
	return result;
}

//using Minkowski distance
Mat epohTheHue(Mat &hMat, Mat &sMat, Mat &lMat) {
	Mat hc(hMat.size(),CV_32F, Scalar(0));
	for(int i=0; i<hMat.rows; i++) {
		for(int j=0; j<hMat.cols; j++) {
			double hue = hMat.at<float>(i,j);
			double sat = sMat.at<float>(i,j);
			double lum = lMat.at<float>(i,j);
			hue *= epoh(sat,lum);
			hc.at<float>(i,j) = roundDecimal(hue,2);
		}
	}
	return hc;
}

void script17() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "lph8";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));

	int row=12;
	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn
	for(int j=1; j<img.row(row).cols; j++) {
		Vec3b BGR = img.at<Vec3b>(row,j);
		Vec3b BGR0 = img.at<Vec3b>(row,j-1);
		XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
		LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
		XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
		LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
		HC = hc.at<float>(row,j);
		HC0 = hc.at<float>(row,j-1);
		int direction = HC - HC0;
		double dE = cie.deltaE76(LAB,LAB0);
		printf("Col: %d | %f\n",j,dE);
		if(direction<0)
			dE = -dE;
		deltaE.push_back(dE);
	}
	String str = "_row[" + toString(row) + "]";
	Write::writeSeq2File(deltaE,name+str,",");
	String folder = "/home/jason/git/WebDerm/WebDerm/";
	String py_file = "python /home/jason/Desktop/workspace/Pyth/poly_arg.py ";
	String py_arg = folder+name+str+".csv ";
	String out_arg = folder+name+str+"_polyfit.csv";
	py_file += py_arg + out_arg;
	system(py_file.c_str());
}

void script18() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "lph4";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));

	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> imgRowThresh;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		vector<double> pulldown;
		if(slope<0.07) {
			double pd = 0;
			for(unsigned int j=0; j<deltaE.size(); j++) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;

				pulldown.push_back(abs(pd));
			}
		}
		int bestIdx = kc.kneeCurvePoint(pulldown);
		double thresh = pulldown.at(bestIdx);
		imgRowThresh.push_back(thresh);
		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);

		deltaE.clear();
		pulldown.clear();
	}//end row

	Mat map(img.size(),CV_8U, Scalar(0));
	deltaE.clear();
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			deltaE.push_back(dE);
			if(abs(dE)>=imgRowThresh.at(i)) {
				map.at<uchar>(i,j) = 255;
			}
		}
		deltaE.clear();
	}
	imgshow(map);
}

void script19() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "herpes_zoster3";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));

	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);
}

void script20() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "herpes_zoster3";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));

	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=12.71) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	printf("BestIdx: %d\n",bestIdx);
	printf("Islands Count: %lu\n",islands.size());
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}
	img.copyTo(img2,unionMap);
	imgshow(unionMap);
	imgshow(img2);
}

void script21() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "tinea_corporis8a";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	String out = "/home/jason/Desktop/Programs/Discrete_New/";
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));

	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);
	clst.printInfo();

	double thresh = clst.getMin(clst.getNumOfClusters()-1);
	cout << thresh << endl;
	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=thresh) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	printf("BestIdx: %d\n",bestIdx);
	printf("Islands Count: %lu\n",islands.size());
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}

	ShapeColor sc;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	lcHaloTrans = (lcHaloTrans - 5) * 255;
	Mat maskFinal;
	lcHaloTrans.copyTo(maskFinal,lcHaloTrans);
	unionMap.copyTo(maskFinal,unionMap);
	img.copyTo(img2,maskFinal);

	img3 = sc.applyDiscreteShade(imgGray);
	img3.copyTo(img4,maskFinal);

	imwrite(out+name+"_discrete.png",img4);
	imgshow(maskFinal);
	imgshow(img4);
}

void script22() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	String out = "/home/jason/Desktop/Programs/Discrete_New/";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));

		Xyz xyz;
		CieLab lab;
		Cie cie;
		vector<float> XYZ, XYZ0, LAB, LAB0;
		vector<double> deltaE, HSL;
		Mat hvec(img.size(),CV_32F,Scalar(0));
		Mat svec(img.size(),CV_32F,Scalar(0));
		Mat lvec(img.size(),CV_32F,Scalar(0));
		double HC, HC0;
		for(int i=0; i<img.rows; i++) {
			for(int j=0; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
				hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
				svec.at<float>(i,j) = round(HSL.at(1) * 100);
				lvec.at<float>(i,j) = round(HSL.at(2) * 100);
			}
		}
		Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

		vector<double> pulldown;
		vector<double> imgRowSlope;
		vector<double> imgRowAvg;
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				deltaE.push_back(dE);
			}

			vector<double> oX;
			for(unsigned int i=0; i<deltaE.size(); i++) {
				oX.push_back(i);
			}
			vector<double> coeffs = poly.polyfit(oX,deltaE,1);
			vector<double> oVals = poly.polyval(coeffs,oX);
			double slope = (oVals.back() - oVals.front()) / oVals.size();
			double avg = (oVals.front() + oVals.back()) / 2;
			double pd=0;
			for(unsigned int j=0; j<deltaE.size(); j++) {
				if(slope<0.07) {
					if(avg>=0)
						pd = deltaE.at(j) - avg;
					else
						pd = deltaE.at(j) + avg;
				}
				else {
					pd = deltaE.at(j);
				}
				pulldown.push_back(abs(pd));
			}

			imgRowSlope.push_back(slope);
			imgRowAvg.push_back(avg);
			deltaE.clear();
		}//end row

		Cluster clst;
		clst.kmeansCluster(pulldown,3);

		double thresh = clst.getMin(clst.getNumOfClusters()-1);
		Mat map(img.size(),CV_8U, Scalar(0));
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				if(imgRowSlope.at(i)<0.07) {
					if(imgRowAvg.at(i)>=0)
						dE -= imgRowAvg.at(i);
					else
						dE += imgRowAvg.at(i);

				}
				if(abs(dE)>=thresh) {
					map.at<uchar>(i,j) = 255;
				}
			}
		}
		ShapeMorph sm;
		Mat map2 = sm.densityConnector(map,0.9999);
		Mat map3 = sm.haloTransform(map2,2);
		map3.convertTo(map3,CV_8U);
		map3 = (map3 - 5) * 255;
		vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
		vector<int> areas;
		for(unsigned int i=0; i<islands.size(); i++) {
			int count = countNonZero(islands.at(i));
			areas.push_back(count);
		}
		int bestIdx = kc.kneeCurvePoint(areas);
		islands.erase(islands.begin(),islands.begin()+bestIdx);
		Mat unionMap;
		for(unsigned int i=0; i<islands.size(); i++) {
			//imgshow(islands.at(i));
			islands.at(i).copyTo(unionMap,islands.at(i));
		}

		ShapeColor sc;
		cvtColor(img,imgGray,CV_BGR2GRAY);
		Mat src = sm.prepareImage(imgGray);
		Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
		Mat lcFilterMat = sc.filterKneePt(src);
		Mat lcFilterNoNoise;
		lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
		Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
		Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
		lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
		lcHaloTrans = (lcHaloTrans - 5) * 255;
		vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
		vector<int> areasLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			int count = countNonZero(islandsLC.at(i));
			areasLC.push_back(count);
		}
		bestIdx = kc.kneeCurvePoint(areasLC);
		islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
		Mat unionMapLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			//imgshow(islands.at(i));
			islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
		}

		Mat maskFinal;
		unionMapLC.copyTo(maskFinal,unionMapLC);
		unionMap.copyTo(maskFinal,unionMap);
		img.copyTo(img2,maskFinal);
		img3 = sc.applyDiscreteShade(imgGray);
		img3.copyTo(img4,maskFinal);
		imwrite(out+name+".png",img2);
		imwrite(out+name+"_discrete.png",img4);
		img2.release();
		img4.release();
	}
}

void script23() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "urticaria5";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);

	double thresh = clst.getMin(clst.getNumOfClusters()-1);
	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=thresh) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}

	ShapeColor sc;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	lcHaloTrans = (lcHaloTrans - 5) * 255;
	vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
	vector<int> areasLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		int count = countNonZero(islandsLC.at(i));
		areasLC.push_back(count);
	}
	bestIdx = kc.kneeCurvePoint(areasLC);
	islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
	Mat unionMapLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		//imgshow(islands.at(i));
		islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
	}

	Mat maskFinal;
	unionMapLC.copyTo(maskFinal,unionMapLC);
	unionMap.copyTo(maskFinal,unionMap);
	img.copyTo(img2,maskFinal);
	img3 = sc.applyDiscreteShade(imgGray);
	img3.copyTo(img4,maskFinal);

	ShadeShape ss;
	vector<Mat> shadeShapeVec = ss.extractShadeShape(img4);
	for(unsigned int i=0; i<shadeShapeVec.size(); i++) {
		imgshow(shadeShapeVec.at(i));
	}

}

//! extract shadeShape
void script24() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	String out = "/home/jason/Desktop/Programs/LC/";
	String out2 = "/home/jason/Desktop/Programs/DeltaE/";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));
		Xyz xyz;
		CieLab lab;
		Cie cie;
		vector<float> XYZ, XYZ0, LAB, LAB0;
		vector<double> deltaE, HSL;
		Mat hvec(img.size(),CV_32F,Scalar(0));
		Mat svec(img.size(),CV_32F,Scalar(0));
		Mat lvec(img.size(),CV_32F,Scalar(0));
		double HC, HC0;
		for(int i=0; i<img.rows; i++) {
			for(int j=0; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
				hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
				svec.at<float>(i,j) = round(HSL.at(1) * 100);
				lvec.at<float>(i,j) = round(HSL.at(2) * 100);
			}
		}
		Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

		vector<double> pulldown;
		vector<double> imgRowSlope;
		vector<double> imgRowAvg;
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				deltaE.push_back(dE);
			}

			vector<double> oX;
			for(unsigned int i=0; i<deltaE.size(); i++) {
				oX.push_back(i);
			}
			vector<double> coeffs = poly.polyfit(oX,deltaE,1);
			vector<double> oVals = poly.polyval(coeffs,oX);
			double slope = (oVals.back() - oVals.front()) / oVals.size();
			double avg = (oVals.front() + oVals.back()) / 2;
			double pd=0;
			for(unsigned int j=0; j<deltaE.size(); j++) {
				if(slope<0.07) {
					if(avg>=0)
						pd = deltaE.at(j) - avg;
					else
						pd = deltaE.at(j) + avg;
				}
				else {
					pd = deltaE.at(j);
				}
				pulldown.push_back(abs(pd));
			}

			imgRowSlope.push_back(slope);
			imgRowAvg.push_back(avg);
			deltaE.clear();
		}//end row

		Cluster clst;
		clst.kmeansCluster(pulldown,3);

		double thresh = clst.getMin(clst.getNumOfClusters()-1);
		Mat map(img.size(),CV_8U, Scalar(0));
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				if(imgRowSlope.at(i)<0.07) {
					if(imgRowAvg.at(i)>=0)
						dE -= imgRowAvg.at(i);
					else
						dE += imgRowAvg.at(i);

				}
				if(abs(dE)>=thresh) {
					map.at<uchar>(i,j) = 255;
				}
			}
		}
		ShapeMorph sm;
		Mat map2 = sm.densityConnector(map,0.9999);
		Mat map3 = sm.haloTransform(map2,2);
		map3.convertTo(map3,CV_8U);
		map3 = (map3 - 5) * 255;
		vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
		vector<int> areas;
		for(unsigned int i=0; i<islands.size(); i++) {
			int count = countNonZero(islands.at(i));
			areas.push_back(count);
		}
		int bestIdx = kc.kneeCurvePoint(areas);
		islands.erase(islands.begin(),islands.begin()+bestIdx);
		Mat unionMap;
		for(unsigned int i=0; i<islands.size(); i++) {
			//imgshow(islands.at(i));
			islands.at(i).copyTo(unionMap,islands.at(i));
		}

		ShapeColor sc;
		cvtColor(img,imgGray,CV_BGR2GRAY);
		Mat src = sm.prepareImage(imgGray);
		Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
		Mat lcFilterMat = sc.filterKneePt(src);
		Mat lcFilterNoNoise;
		lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
		Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
		Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
		lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
		lcHaloTrans = (lcHaloTrans - 5) * 255;
		vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
		vector<int> areasLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			int count = countNonZero(islandsLC.at(i));
			areasLC.push_back(count);
		}
		bestIdx = kc.kneeCurvePoint(areasLC);
		islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
		Mat unionMapLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			//imgshow(islands.at(i));
			islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
		}

		Mat maskFinal;
		//unionMapLC.copyTo(maskFinal,unionMapLC);
		//unionMap.copyTo(maskFinal,unionMap);
		img.copyTo(img2,unionMapLC);
		img.copyTo(img3,unionMap);

		String outName1 = out+name+".png";
		String outName2 = out2+name+".png";
		imwrite(outName1,img2);
		imwrite(outName2,img3);
		img2.release();
		img3.release();
	}
}

void script25() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String name = "tinea_corporis8a";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	String out = "/home/jason/Desktop/Programs/ShadeShape/";
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);

	double thresh = clst.getMin(clst.getNumOfClusters()-1);
	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=thresh) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}

	ShapeColor sc;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	lcHaloTrans = (lcHaloTrans - 5) * 255;
	vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
	vector<int> areasLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		int count = countNonZero(islandsLC.at(i));
		areasLC.push_back(count);
	}
	bestIdx = kc.kneeCurvePoint(areasLC);
	islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
	Mat unionMapLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		//imgshow(islands.at(i));
		islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
	}

	Mat maskFinal;
	unionMapLC.copyTo(maskFinal,unionMapLC);
	unionMap.copyTo(maskFinal,unionMap);
	img.copyTo(img2,maskFinal);
	img3 = sc.applyDiscreteShade(imgGray);
	img3.copyTo(img4,maskFinal);

	ShadeShape ss;
	ss.extract(img4);
	for(int i=0; i<ss.numOfFeatures(); i++) {
		for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
			//printf("%s_%d.%d: %d\n",name.c_str(),i+1,j+1,ss.feature(i).island(j).shade());
		}
	}
}

//! extract shadeShape
void script26() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	String out = "/home/jason/Desktop/Programs/ShadeShape/";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));
		Xyz xyz;
		CieLab lab;
		Cie cie;
		vector<float> XYZ, XYZ0, LAB, LAB0;
		vector<double> deltaE, HSL;
		Mat hvec(img.size(),CV_32F,Scalar(0));
		Mat svec(img.size(),CV_32F,Scalar(0));
		Mat lvec(img.size(),CV_32F,Scalar(0));
		double HC, HC0;
		for(int i=0; i<img.rows; i++) {
			for(int j=0; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
				hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
				svec.at<float>(i,j) = round(HSL.at(1) * 100);
				lvec.at<float>(i,j) = round(HSL.at(2) * 100);
			}
		}
		Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

		vector<double> pulldown;
		vector<double> imgRowSlope;
		vector<double> imgRowAvg;
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				deltaE.push_back(dE);
			}

			vector<double> oX;
			for(unsigned int i=0; i<deltaE.size(); i++) {
				oX.push_back(i);
			}
			vector<double> coeffs = poly.polyfit(oX,deltaE,1);
			vector<double> oVals = poly.polyval(coeffs,oX);
			double slope = (oVals.back() - oVals.front()) / oVals.size();
			double avg = (oVals.front() + oVals.back()) / 2;
			double pd=0;
			for(unsigned int j=0; j<deltaE.size(); j++) {
				if(slope<0.07) {
					if(avg>=0)
						pd = deltaE.at(j) - avg;
					else
						pd = deltaE.at(j) + avg;
				}
				else {
					pd = deltaE.at(j);
				}
				pulldown.push_back(abs(pd));
			}

			imgRowSlope.push_back(slope);
			imgRowAvg.push_back(avg);
			deltaE.clear();
		}//end row

		Cluster clst;
		clst.kmeansCluster(pulldown,3);

		double thresh = clst.getMin(clst.getNumOfClusters()-1);
		Mat map(img.size(),CV_8U, Scalar(0));
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				if(imgRowSlope.at(i)<0.07) {
					if(imgRowAvg.at(i)>=0)
						dE -= imgRowAvg.at(i);
					else
						dE += imgRowAvg.at(i);

				}
				if(abs(dE)>=thresh) {
					map.at<uchar>(i,j) = 255;
				}
			}
		}
		ShapeMorph sm;
		Mat map2 = sm.densityConnector(map,0.9999);
		Mat map3 = sm.haloTransform(map2,2);
		map3.convertTo(map3,CV_8U);
		map3 = (map3 - 5) * 255;
		vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
		vector<int> areas;
		for(unsigned int i=0; i<islands.size(); i++) {
			int count = countNonZero(islands.at(i));
			areas.push_back(count);
		}
		int bestIdx = kc.kneeCurvePoint(areas);
		islands.erase(islands.begin(),islands.begin()+bestIdx);
		Mat unionMap;
		for(unsigned int i=0; i<islands.size(); i++) {
			//imgshow(islands.at(i));
			islands.at(i).copyTo(unionMap,islands.at(i));
		}

		ShapeColor sc;
		cvtColor(img,imgGray,CV_BGR2GRAY);
		Mat src = sm.prepareImage(imgGray);
		Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
		Mat lcFilterMat = sc.filterKneePt(src);
		Mat lcFilterNoNoise;
		lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
		Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
		Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
		lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
		lcHaloTrans = (lcHaloTrans - 5) * 255;
		vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
		vector<int> areasLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			int count = countNonZero(islandsLC.at(i));
			areasLC.push_back(count);
		}
		bestIdx = kc.kneeCurvePoint(areasLC);
		islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
		Mat unionMapLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			//imgshow(islands.at(i));
			islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
		}

		Mat maskFinal;
		unionMapLC.copyTo(maskFinal,unionMapLC);
		unionMap.copyTo(maskFinal,unionMap);
		img.copyTo(img2,maskFinal);
		img3 = sc.applyDiscreteShade(imgGray);
		img3.copyTo(img4,maskFinal);

		ShadeShape ss;
		ss.extract(img4);
		for(int i=0; i<ss.numOfFeatures(); i++) {
			for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
				String outName = out+name+"_shadeShape_"+toString(i+1)+"."+toString(j+1)+".png";
				imwrite(outName,ss.feature(i).island(j).image());
			}
		}
		img2.release();
		img4.release();
	}
}

//! Create Training Labels
void script_createTrainingLabels() {
	String folderSetName = "Circles-Donut-Incomplete";
	int sets = 7; //number of differnt shapes/classes
	int positiveSet = 4; //set that is labeled positive

	String samplePath = "/home/jason/git/Samples/Samples/Training/"+folderSetName+"/";
	String labelPath = "/home/jason/git/Samples/Samples/Training/Labels/";
	FileData fd;
	deque<String> files;
	fd.getFilesFromDirectory(samplePath,files);
	sort(files.begin(),files.end());
	String out = labelPath+folderSetName+".csv";

	FILE * fp;
	fp = fopen(out.c_str(),"w");
	int label = -1;
	for(unsigned int i=0; i<files.size(); i++) {
		String filename = samplePath+files.at(i);
		filename = getFileName(filename);
		fprintf(fp,"%s,",filename.c_str());
		for(int j=0; j<sets; j++) {
			if(j==positiveSet-1) label = 1;
			else label = -1;
			if(j<(sets-1))
				fprintf(fp,"%d,",label);
			else
				fprintf(fp,"%d\n",label);
		}
	}
}

//! Create All Training Labels
void script_createAllTrainingLabels() {
	TestML ml;

	String samplePath = "/home/jason/git/Samples/Samples/Training/samples_path.csv";
	String labelPath = "/home/jason/git/Samples/Samples/Training/Labels/";
	fstream fs(samplePath);
	if(fs.is_open()) {
		String path;
		deque<String> vec;
		while(getline(fs,path)) {
			vec.push_back(path);
		}
		int sets = vec.size();
		for(unsigned int i=0; i<vec.size(); i++) {
			String folderSetName = getFolderName(vec.at(i));
			FileData fd;
			deque<String> files;
			fd.getFilesFromDirectory(vec.at(i),files);
			sort(files.begin(), files.end());
			String out = labelPath+folderSetName+".csv";

			FILE * fp;
			fp = fopen(out.c_str(),"w");
			int label = -1;
			for(unsigned int n=0; n<files.size(); n++) {
				String filename = vec.at(i)+files.at(n);
				filename = getFileName(filename);
				fprintf(fp,"%s,",filename.c_str());
				for(int j=0; j<sets; j++) {
					if(j==i) label = 1;
					else label = -1;
					if(j<(sets-1))
						fprintf(fp,"%d,",label);
					else
						fprintf(fp,"%d\n",label);
				}
			}
			fclose(fp);
		}
		fs.close();
	}
}

void script_checkAllTestData() {
	String folder = "/home/jason/Desktop/workspace/Test_Islands/";
	deque<String> files;
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	String out = folder+"results.csv";
	ofstream ofs;
	ofs.open(out);
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	vector<Mat> sampleVec;
	vector<String> nameVec;
	for(unsigned int i=0; i<files.size(); i++) {
		String filename = folder+files.at(i);
		Mat sample = imread(filename,0);
		if(!sample.empty()) {
			filename = getFileName(filename);
			nameVec.push_back(filename);
			printf("%s: %d\n",filename.c_str(),countNonZero(sample));
			sample *= 255;
			sample = ml.prepareImage(sample,Size(20,20));
			sampleVec.push_back(sample);
		}
	}
	Mat results = ml.runANN(param,sampleVec);
	for(int i=0; i<results.rows; i++) {
		if(ofs.is_open())
			ofs << nameVec.at(i) << "," << results.row(i) << endl;
	}
	ofs.close();
}

void script_checkHitRatioTestData() {
	String folder = "/home/jason/Desktop/workspace/TestNN/";
	String file = folder+"result_labels.csv";
	ifstream ifs(file);
	vector<int> myLabelVec;
	if(ifs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(ifs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==1)
					myLabelVec.push_back(atoi(vec.at(i).c_str()));
			}
			vec.clear();
		}
		ifs.close();
	}

	deque<String> files;
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	String out = folder+"results.csv";
	ofstream ofs(out);
	ofs << "Name, 0,1,2,3,4,5,6, Result, MyLabel, Y/N" << endl;
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	vector<Mat> sampleVec;
	vector<String> nameVec;
	for(unsigned int i=0; i<files.size(); i++) {
		String filename = folder+files.at(i);
		Mat sample = imread(filename,0);
		if(!sample.empty()) {
			sample *= 255;
			sample = ml.prepareImage(sample,Size(20,20));
			sampleVec.push_back(sample);
			filename = getFileName(filename);
			nameVec.push_back(filename);
		}
	}
	Mat results = ml.runANN(param,sampleVec);
	int hitCount = 0;
	for(int i=0; i<results.rows; i++) {
		if(ofs.is_open())
			ofs << nameVec.at(i) << "," << results.row(i) << ",";
		float max = -2.0;
		int labelNum = -1;
		for(int j=0; j<results.cols; j++) {
			if(results.at<float>(i,j)>max) {
				max = results.at<float>(i,j);
				labelNum = j;
			}
		}
		if(max<0) labelNum = 4;
		ofs << labelNum << "," << myLabelVec.at(i )<< ",";

		if(labelNum==myLabelVec.at(i)) {
			hitCount++;
			ofs << "Y" << endl;
		}
		else
			ofs << "N" << endl;
	}
	float ratio = (float)hitCount/results.rows;
	ofs << "Hit Ratio: ," << ratio << endl;
	ofs.close();
}

int getPeakPos(vector<double> &data_vec) {
	vector<double> densityVec;
	for(int i=0; i<8; i++) {
		Cluster clst2;
		clst2.kmeansCluster(data_vec,i+1);
		int totalPts=0;
		double totalDensity = 0.0;
		for(int j=0; j<clst2.getNumOfClusters(); j++) {
			totalPts += clst2.getCenterCount(j);
		}
		for(int j=0; j<clst2.getNumOfClusters(); j++) {
			int numPts = clst2.getCenterCount(j);
			double minVal = clst2.getMin(j);
			double maxVal = clst2.getMax(j);
			double density = numPts/(maxVal-minVal);
			totalDensity += ((double)numPts/totalPts) * density;
		}
		//printf("# of clsts: %d, Density: %f\n",i+1,totalDensity);
		densityVec.push_back(totalDensity);
	}
	vector<double> changeVec;
	double change = -1.0;
	for(unsigned int i=0; i<densityVec.size(); i++) {
		if(i>0) {
			change = densityVec.at(i)/densityVec.at(i-1);
			changeVec.push_back(change);
			printf("%d) Density: %f, Change: %f\n",i+1,densityVec.at(i),change);
		}
		else {
			printf("%d) Density: %f, Change: %f\n",i+1,densityVec.at(i),change);
			changeVec.push_back(change);
		}
	}
	int changeCount = 0;
	int peakPos = 1;
	for(unsigned int i=0; i<changeVec.size(); i++) {
		double change = changeVec.at(i);
		if(change<=1.13 && change>=0) changeCount++;
		else changeCount = 0;
		printf("%d: %f, %d\n",i+1,change,changeCount);
		if(changeCount>=2) {
			peakPos = i-2;
			peakPos++;
			break;
		}
	}
	if(peakPos==1) {
		vector<double>::iterator it = max_element(changeVec.begin(),changeVec.end());
		peakPos = (it - changeVec.begin())+1;
		peakPos++;
	}
	if(peakPos>4) peakPos = 4;
	return peakPos;
}

//! extract shadeShape
void script27() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	String out = "/home/jason/Desktop/Programs/ShadeShape/";
	String name = "tinea_corporis8b";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);

	double thresh = clst.getMin(clst.getNumOfClusters()-1);
	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=thresh) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}

	ShapeColor sc;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	lcHaloTrans = (lcHaloTrans - 5) * 255;
	vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
	vector<int> areasLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		int count = countNonZero(islandsLC.at(i));
		areasLC.push_back(count);
	}
	bestIdx = kc.kneeCurvePoint(areasLC);
	islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
	Mat unionMapLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		//imgshow(islands.at(i));
		islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
	}

	Mat maskFinal;
	unionMapLC.copyTo(maskFinal,unionMapLC);
	unionMap.copyTo(maskFinal,unionMap);
	imgGray.copyTo(img2,maskFinal);

	vector<double> data_vec;
	for(int i=0; i<img2.rows; i++) {
		for(int j=0; j<img2.cols; j++) {
			double val = img2.at<uchar>(i,j);
			if(val>0)
				data_vec.push_back(val);
		}
	}
	int peakPos = getPeakPos(data_vec);
	int minVal = *min_element(data_vec.begin(),data_vec.end());
	int maxVal = *max_element(data_vec.begin(),data_vec.end());
	img3 = sc.applyDiscreteShade(img2,minVal,maxVal,peakPos);

	ShadeShape ss;
	ss.extract(img3);
	for(int i=0; i<ss.numOfFeatures(); i++) {
		for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
			//printf("%s_%d.%d: Area: %d, Shade: %d\n",name.c_str(),i+1,j+1,ss.feature(i).island(j).area(),ss.feature(i).island(j).shade());
			String outName = out+name+"_shadeShape_"+toString(i+1)+"."+toString(j+1)+".png";
			imwrite(outName,ss.feature(i).island(j).image());
		}
	}
}

//! single discrete image
void script28a() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FileData fd;
	deque<String> files;
	String name = "tinea_corporis8b";
	String out = "/home/jason/Desktop/Programs/Discrete_New/";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);

	double thresh = clst.getMin(clst.getNumOfClusters()-1);
	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=thresh) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}

	ShapeColor sc;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	lcHaloTrans = (lcHaloTrans - 5) * 255;
	vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
	vector<int> areasLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		int count = countNonZero(islandsLC.at(i));
		areasLC.push_back(count);
	}
	bestIdx = kc.kneeCurvePoint(areasLC);
	islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
	Mat unionMapLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		//imgshow(islands.at(i));
		islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
	}
	Mat maskFinal;
	unionMapLC.copyTo(maskFinal,unionMapLC);
	unionMap.copyTo(maskFinal,unionMap);
	imgGray.copyTo(img2,maskFinal);

	vector<double> data_vec;
	for(int i=0; i<img2.rows; i++) {
		for(int j=0; j<img2.cols; j++) {
			double val = img2.at<uchar>(i,j);
			if(val>0)
				data_vec.push_back(val);
		}
	}

	int peakPos = getPeakPos(data_vec);
	int minVal = *min_element(data_vec.begin(),data_vec.end());
	int maxVal = *max_element(data_vec.begin(),data_vec.end());
	printf("PeakPos: %d\n",peakPos);
	//printf("MinVal: %d\n",minVal);
	//printf("MaxVal: %d\n",maxVal);
	img3 = sc.applyDiscreteShade(img2,minVal,maxVal,peakPos);

	String outName = out+name+"_discrete.png";
	img.copyTo(img4,maskFinal);
	imwrite(out+name+".png",img4);
	imwrite(outName,img3);
}

//! multiple discrete image
void script28b() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	String out = "/home/jason/Desktop/Programs/Discrete_New/";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));
		Xyz xyz;
		CieLab lab;
		Cie cie;
		vector<float> XYZ, XYZ0, LAB, LAB0;
		vector<double> deltaE, HSL;
		Mat hvec(img.size(),CV_32F,Scalar(0));
		Mat svec(img.size(),CV_32F,Scalar(0));
		Mat lvec(img.size(),CV_32F,Scalar(0));
		double HC, HC0;
		for(int i=0; i<img.rows; i++) {
			for(int j=0; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
				hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
				svec.at<float>(i,j) = round(HSL.at(1) * 100);
				lvec.at<float>(i,j) = round(HSL.at(2) * 100);
			}
		}
		Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

		vector<double> pulldown;
		vector<double> imgRowSlope;
		vector<double> imgRowAvg;
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				deltaE.push_back(dE);
			}

			vector<double> oX;
			for(unsigned int i=0; i<deltaE.size(); i++) {
				oX.push_back(i);
			}
			vector<double> coeffs = poly.polyfit(oX,deltaE,1);
			vector<double> oVals = poly.polyval(coeffs,oX);
			double slope = (oVals.back() - oVals.front()) / oVals.size();
			double avg = (oVals.front() + oVals.back()) / 2;
			double pd=0;
			for(unsigned int j=0; j<deltaE.size(); j++) {
				if(slope<0.07) {
					if(avg>=0)
						pd = deltaE.at(j) - avg;
					else
						pd = deltaE.at(j) + avg;
				}
				else {
					pd = deltaE.at(j);
				}
				pulldown.push_back(abs(pd));
			}

			imgRowSlope.push_back(slope);
			imgRowAvg.push_back(avg);
			deltaE.clear();
		}//end row

		Cluster clst;
		clst.kmeansCluster(pulldown,3);

		double thresh = clst.getMin(clst.getNumOfClusters()-1);
		Mat map(img.size(),CV_8U, Scalar(0));
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				if(imgRowSlope.at(i)<0.07) {
					if(imgRowAvg.at(i)>=0)
						dE -= imgRowAvg.at(i);
					else
						dE += imgRowAvg.at(i);

				}
				if(abs(dE)>=thresh) {
					map.at<uchar>(i,j) = 255;
				}
			}
		}
		ShapeMorph sm;
		Mat map2 = sm.densityConnector(map,0.9999);
		Mat map3 = sm.haloTransform(map2,2);
		map3.convertTo(map3,CV_8U);
		map3 = (map3 - 5) * 255;
		vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
		vector<int> areas;
		for(unsigned int i=0; i<islands.size(); i++) {
			int count = countNonZero(islands.at(i));
			areas.push_back(count);
		}
		int bestIdx = kc.kneeCurvePoint(areas);
		islands.erase(islands.begin(),islands.begin()+bestIdx);
		Mat unionMap;
		for(unsigned int i=0; i<islands.size(); i++) {
			//imgshow(islands.at(i));
			islands.at(i).copyTo(unionMap,islands.at(i));
		}

		ShapeColor sc;
		cvtColor(img,imgGray,CV_BGR2GRAY);
		Mat src = sm.prepareImage(imgGray);
		Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
		Mat lcFilterMat = sc.filterKneePt(src);
		Mat lcFilterNoNoise;
		lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
		Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
		Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
		lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
		lcHaloTrans = (lcHaloTrans - 5) * 255;
		vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
		vector<int> areasLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			int count = countNonZero(islandsLC.at(i));
			areasLC.push_back(count);
		}
		bestIdx = kc.kneeCurvePoint(areasLC);
		islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
		Mat unionMapLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			//imgshow(islands.at(i));
			islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
		}

		Mat maskFinal;
		unionMapLC.copyTo(maskFinal,unionMapLC);
		unionMap.copyTo(maskFinal,unionMap);
		imgGray.copyTo(img2,maskFinal);

		vector<double> data_vec;
		for(int i=0; i<img2.rows; i++) {
			for(int j=0; j<img2.cols; j++) {
				double val = img2.at<uchar>(i,j);
				if(val>0)
					data_vec.push_back(val);
			}
		}
		int peakPos = getPeakPos(data_vec);
		int minVal = *min_element(data_vec.begin(),data_vec.end());
		int maxVal = *max_element(data_vec.begin(),data_vec.end());
		img3 = sc.applyDiscreteShade(img2,minVal,maxVal,peakPos+1);

		String outName = out+name+"_discrete.png";
		img.copyTo(img4,maskFinal);
		imwrite(out+name+".png",img4);
		imwrite(outName,img3);
		/*
		ShadeShape ss;
		ss.extract(img3);
		for(int i=0; i<ss.numOfFeatures(); i++) {
			for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
				String outName = out+name+"_shadeShape_"+toString(i+1)+"."+toString(j+1)+".png";
				imwrite(outName,ss.feature(i).island(j).image());
			}
		}*/
		img2.release();
		img4.release();
	}
}

void script29() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Looks_Like/";
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/island-list.csv","w");
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	fd.getFilesFromDirectory(folder,files);
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		name = getFileName(name);
		img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(140,140));
		Xyz xyz;
		CieLab lab;
		Cie cie;
		vector<float> XYZ, XYZ0, LAB, LAB0;
		vector<double> deltaE, HSL;
		Mat hvec(img.size(),CV_32F,Scalar(0));
		Mat svec(img.size(),CV_32F,Scalar(0));
		Mat lvec(img.size(),CV_32F,Scalar(0));
		double HC, HC0;
		for(int i=0; i<img.rows; i++) {
			for(int j=0; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
				hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
				svec.at<float>(i,j) = round(HSL.at(1) * 100);
				lvec.at<float>(i,j) = round(HSL.at(2) * 100);
			}
		}
		Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

		vector<double> pulldown;
		vector<double> imgRowSlope;
		vector<double> imgRowAvg;
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				deltaE.push_back(dE);
			}

			vector<double> oX;
			for(unsigned int i=0; i<deltaE.size(); i++) {
				oX.push_back(i);
			}
			vector<double> coeffs = poly.polyfit(oX,deltaE,1);
			vector<double> oVals = poly.polyval(coeffs,oX);
			double slope = (oVals.back() - oVals.front()) / oVals.size();
			double avg = (oVals.front() + oVals.back()) / 2;
			double pd=0;
			for(unsigned int j=0; j<deltaE.size(); j++) {
				if(slope<0.07) {
					if(avg>=0)
						pd = deltaE.at(j) - avg;
					else
						pd = deltaE.at(j) + avg;
				}
				else {
					pd = deltaE.at(j);
				}
				pulldown.push_back(abs(pd));
			}

			imgRowSlope.push_back(slope);
			imgRowAvg.push_back(avg);
			deltaE.clear();
		}//end row

		Cluster clst;
		clst.kmeansCluster(pulldown,3);

		double thresh = clst.getMin(clst.getNumOfClusters()-1);
		Mat map(img.size(),CV_8U, Scalar(0));
		for(int i=0; i<img.rows; i++) {
			for(int j=1; j<img.cols; j++) {
				Vec3b BGR = img.at<Vec3b>(i,j);
				Vec3b BGR0 = img.at<Vec3b>(i,j-1);
				XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
				LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
				XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
				LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
				HC = hc.at<float>(i,j);
				HC0 = hc.at<float>(i,j-1);
				int direction = HC - HC0;
				double dE = cie.deltaE76(LAB,LAB0);
				if(direction<0)
					dE = -dE;
				if(imgRowSlope.at(i)<0.07) {
					if(imgRowAvg.at(i)>=0)
						dE -= imgRowAvg.at(i);
					else
						dE += imgRowAvg.at(i);

				}
				if(abs(dE)>=thresh) {
					map.at<uchar>(i,j) = 255;
				}
			}
		}
		ShapeMorph sm;
		Mat map2 = sm.densityConnector(map,0.9999);
		Mat map3 = sm.haloTransform(map2,2);
		map3.convertTo(map3,CV_8U);
		map3 = (map3 - 5) * 255;
		vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
		vector<int> areas;
		for(unsigned int i=0; i<islands.size(); i++) {
			int count = countNonZero(islands.at(i));
			areas.push_back(count);
		}
		int bestIdx = kc.kneeCurvePoint(areas);
		islands.erase(islands.begin(),islands.begin()+bestIdx);
		Mat unionMap;
		for(unsigned int i=0; i<islands.size(); i++) {
			//imgshow(islands.at(i));
			islands.at(i).copyTo(unionMap,islands.at(i));
		}

		ShapeColor sc;
		cvtColor(img,imgGray,CV_BGR2GRAY);
		Mat src = sm.prepareImage(imgGray);
		Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
		Mat lcFilterMat = sc.filterKneePt(src);
		Mat lcFilterNoNoise;
		lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
		Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
		Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
		lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
		lcHaloTrans = (lcHaloTrans - 5) * 255;
		vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
		vector<int> areasLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			int count = countNonZero(islandsLC.at(i));
			areasLC.push_back(count);
		}
		bestIdx = kc.kneeCurvePoint(areasLC);
		islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
		Mat unionMapLC;
		for(unsigned int i=0; i<islandsLC.size(); i++) {
			//imgshow(islands.at(i));
			islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
		}

		Mat maskFinal;
		unionMapLC.copyTo(maskFinal,unionMapLC);
		unionMap.copyTo(maskFinal,unionMap);
		imgGray.copyTo(img2,maskFinal);

		vector<double> data_vec;
		for(int i=0; i<img2.rows; i++) {
			for(int j=0; j<img2.cols; j++) {
				double val = img2.at<uchar>(i,j);
				if(val>0)
					data_vec.push_back(val);
			}
		}
		int peakPos = getPeakPos(data_vec);
		int minVal = *min_element(data_vec.begin(),data_vec.end());
		int maxVal = *max_element(data_vec.begin(),data_vec.end());
		img3 = sc.applyDiscreteShade(img2,minVal,maxVal,peakPos+1);

		ShadeShape ss;
		ss.extract(img3);
		vector<Mat> matVec;
		vector<String> nameVec;
		vector<int> shadeVec;
		for(int i=0; i<ss.numOfFeatures(); i++) {
			for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
				matVec.push_back(ss.feature(i).island(j).image());
				String outName = name+"_shadeShape_"+toString(i+1)+"."+toString(j+1);
				nameVec.push_back(outName);
				shadeVec.push_back(ss.feature(i).island(j).shade());
			}
		}
		img2.release();
		img4.release();

		vector<Mat> sampleVec;
		for(unsigned int i=0; i<matVec.size(); i++) {
			Mat sample = matVec.at(i);
			sample *= 255;
			sample = ml.prepareImage(sample,Size(20,20));
			sampleVec.push_back(sample);
		}
		Mat results = ml.runANN(param,sampleVec);
		for(int i=0; i<results.rows; i++) {
			float max = -2.0;
			int labelNum = -1;
			for(int j=0; j<results.cols; j++) {
				if(results.at<float>(i,j)>max) {
					max = results.at<float>(i,j);
					labelNum = j;
				}
			}
			if(max<0) labelNum = 4;
			fprintf(fp,"%s,%d,%d\n",nameVec.at(i).c_str(),shadeVec.at(i),labelNum);
		}
	}
}

void script30() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	Poly poly;
	KneeCurve kc;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5, imgGray;
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/island-list.csv","w");
	fprintf(fp,"Name,Shade,Shape\n");
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	String name = "tinea_corporis8b";
	img = imread("/home/jason/Desktop/Programs/Looks_Like/"+name+".jpg");
	img = runColorNormalization(img);
	img = runResizeImage(img,Size(140,140));
	Xyz xyz;
	CieLab lab;
	Cie cie;
	vector<float> XYZ, XYZ0, LAB, LAB0;
	vector<double> deltaE, HSL;
	Mat hvec(img.size(),CV_32F,Scalar(0));
	Mat svec(img.size(),CV_32F,Scalar(0));
	Mat lvec(img.size(),CV_32F,Scalar(0));
	double HC, HC0;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			HSL = hsl.rgb2hsl(BGR[2],BGR[1],BGR[0]);
			hvec.at<float>(i,j) = HSL.at(0) - floor(HSL.at(0)/180.) * 360.;
			svec.at<float>(i,j) = round(HSL.at(1) * 100);
			lvec.at<float>(i,j) = round(HSL.at(2) * 100);
		}
	}
	Mat hc = epohTheHue(hvec,svec,lvec); // for direction of up or down the mtn

	vector<double> pulldown;
	vector<double> imgRowSlope;
	vector<double> imgRowAvg;
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			deltaE.push_back(dE);
		}

		vector<double> oX;
		for(unsigned int i=0; i<deltaE.size(); i++) {
			oX.push_back(i);
		}
		vector<double> coeffs = poly.polyfit(oX,deltaE,1);
		vector<double> oVals = poly.polyval(coeffs,oX);
		double slope = (oVals.back() - oVals.front()) / oVals.size();
		double avg = (oVals.front() + oVals.back()) / 2;
		double pd=0;
		for(unsigned int j=0; j<deltaE.size(); j++) {
			if(slope<0.07) {
				if(avg>=0)
					pd = deltaE.at(j) - avg;
				else
					pd = deltaE.at(j) + avg;
			}
			else {
				pd = deltaE.at(j);
			}
			pulldown.push_back(abs(pd));
		}

		imgRowSlope.push_back(slope);
		imgRowAvg.push_back(avg);
		deltaE.clear();
	}//end row

	Cluster clst;
	clst.kmeansCluster(pulldown,3);

	double thresh = clst.getMin(clst.getNumOfClusters()-1);
	Mat map(img.size(),CV_8U, Scalar(0));
	for(int i=0; i<img.rows; i++) {
		for(int j=1; j<img.cols; j++) {
			Vec3b BGR = img.at<Vec3b>(i,j);
			Vec3b BGR0 = img.at<Vec3b>(i,j-1);
			XYZ = xyz.rgb2xyz(BGR[2],BGR[1],BGR[0]);
			LAB = lab.xyz2lab(XYZ[0],XYZ[1],XYZ[2]);
			XYZ0 = xyz.rgb2xyz(BGR0[2],BGR0[1],BGR0[0]);
			LAB0 = lab.xyz2lab(XYZ0[0],XYZ0[1],XYZ0[2]);
			HC = hc.at<float>(i,j);
			HC0 = hc.at<float>(i,j-1);
			int direction = HC - HC0;
			double dE = cie.deltaE76(LAB,LAB0);
			if(direction<0)
				dE = -dE;
			if(imgRowSlope.at(i)<0.07) {
				if(imgRowAvg.at(i)>=0)
					dE -= imgRowAvg.at(i);
				else
					dE += imgRowAvg.at(i);

			}
			if(abs(dE)>=thresh) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	ShapeMorph sm;
	Mat map2 = sm.densityConnector(map,0.9999);
	Mat map3 = sm.haloTransform(map2,2);
	map3.convertTo(map3,CV_8U);
	map3 = (map3 - 5) * 255;
	vector<Mat> islands = sm.liquidFeatureExtraction(map3,0,-1);
	vector<int> areas;
	for(unsigned int i=0; i<islands.size(); i++) {
		int count = countNonZero(islands.at(i));
		areas.push_back(count);
	}
	int bestIdx = kc.kneeCurvePoint(areas);
	islands.erase(islands.begin(),islands.begin()+bestIdx);
	Mat unionMap;
	for(unsigned int i=0; i<islands.size(); i++) {
		//imgshow(islands.at(i));
		islands.at(i).copyTo(unionMap,islands.at(i));
	}

	ShapeColor sc;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat lcFilterMat = sc.filterKneePt(src);
	Mat lcFilterNoNoise;
	lcFilterMat.copyTo(lcFilterNoNoise,mapOfNonNoise);
	Mat lcDenseConnect = sm.densityConnector(lcFilterNoNoise,0.9999);
	Mat lcHaloTrans = sm.haloTransform(lcDenseConnect,2);
	lcHaloTrans.convertTo(lcHaloTrans,CV_8U);
	lcHaloTrans = (lcHaloTrans - 5) * 255;
	vector<Mat> islandsLC = sm.liquidFeatureExtraction(lcHaloTrans,0,-1);
	vector<int> areasLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		int count = countNonZero(islandsLC.at(i));
		areasLC.push_back(count);
	}
	bestIdx = kc.kneeCurvePoint(areasLC);
	islandsLC.erase(islandsLC.begin(),islandsLC.begin()+bestIdx);
	Mat unionMapLC;
	for(unsigned int i=0; i<islandsLC.size(); i++) {
		//imgshow(islands.at(i));
		islandsLC.at(i).copyTo(unionMapLC,islandsLC.at(i));
	}

	Mat maskFinal;
	unionMapLC.copyTo(maskFinal,unionMapLC);
	unionMap.copyTo(maskFinal,unionMap);
	imgGray.copyTo(img2,maskFinal);

	vector<double> data_vec;
	for(int i=0; i<img2.rows; i++) {
		for(int j=0; j<img2.cols; j++) {
			double val = img2.at<uchar>(i,j);
			if(val>0)
				data_vec.push_back(val);
		}
	}
	int peakPos = getPeakPos(data_vec);
	int minVal = *min_element(data_vec.begin(),data_vec.end());
	int maxVal = *max_element(data_vec.begin(),data_vec.end());
	img3 = sc.applyDiscreteShade(img2,minVal,maxVal,peakPos);

	ShadeShape ss;
	ss.extract(img3);
	for(int i=0; i<ss.numOfFeatures(); i++) {
		for(int j=0; j<ss.feature(i).numOfIslands(); j++) {
			String outName = name+"_shadeShape_"+toString(i+1)+"."+toString(j+1);
			int shade = ss.feature(i).island(j).shade();
			int shape = ss.feature(i).island(j).shape();
			String shapeName = ss.feature(i).island(j).shape_name();
			fprintf(fp,"%s,%d,%d,%s\n",outName.c_str(),shade,shape,shapeName.c_str());
			cout << outName << endl;
			cout << ss.feature(i).island(j).nn_results() << endl;
		}
	}
}

}// end namespace
