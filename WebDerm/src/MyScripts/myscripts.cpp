/*
 * myscripts.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: jason
 */

#include "myscripts.h"

void script1() {
	CreateTrainingData ctd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Data/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int fileLabel= 4249;
	for(unsigned int i=0; i<6; i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		Mat src1 = imread(name,0);
		for(unsigned int j=6; j<34;j++) {
			name = folder + files.at(j);
			Mat src2 = imread(name,0);
			if(i!=j) {
				Mat stitch = ctd.stitchData(src1,src2);
				name = out + "pos_" + toString(fileLabel) + ".png";
				fileLabel++;
				imwrite(name,stitch);
			}
		}
	}
}

//! generate names of training data files
void script2() {
	FileData fd;
	deque<String> files;
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Simple/Positive_Pairs/";
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
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Negative_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int count = 736;
	for(unsigned int i=0; i<files.size(); i++) {
		int flag=0;
		size_t pos=0;
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=97 && seq<=503) {
			String file = folder + files.at(i);
			Mat img = imread(file,0);
			Mat src1 = img(Rect(0,0,35,35));
			Mat src2 = img(Rect(35,0,35,35));
			//Mat blank(src1.size(),CV_8U,Scalar(0));
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
				img = ctd.stitchData(src2,src1);
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
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Test/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Test/Positive_Pairs/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int count=1;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		Mat src1 = imread(name,0);
		for(unsigned int j=0; j<files.size(); j++) {
			name = folder + files.at(j);
			Mat src2 = imread(name,0);
			Mat stitch = ctd.stitchData(src1,src2);
			name = out + "pos_" + toString(count) + ".png";
			imwrite(name,stitch);
			count++;
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
	String folder = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Positive_Pairs/";
	String out = "/home/jason/Desktop/Programs/Training_Samples/Experimental/Test/";
	FileData fd;
	fd.getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	int count = 282;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder+files.at(i);
		String filename = files.at(i);
		size_t pos=0;
		String seqStr = fd.getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq>=1331 && seq<=1552) {
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
