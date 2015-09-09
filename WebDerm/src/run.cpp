/*
 * run.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/run.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/skin/Skin.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Mouse/mouse.h"

void runBlurImage() {
	String filename;
	String input;
	Size size(10,10);
	cout << "Enter filename: ";
	cin >> filename;
	cout << "Do you want to write image? (y/n)";
	cin >> input;
	Mat img = runResizeImage(filename,Size(700,700),0);
	blur(img,img,size);
	if(input=="y")
	{
		String name = getFileName(filename);
		name += "_Blur_" + toString(size.width) + "x" + toString(size.height);
		imwrite(name+".png",img);
	}
	imshow("Img", img);
	waitKey(0);
	img.release();
}

void runResizeImage() {
	String filename;
	String input;
	cout << "Enter filename: ";
	cin >> filename;
	cout << "Do you want to write image? (y/n)";
	cin >> input;
	Mat img = runResizeImage(filename,Size(700,700),0);
	if(input=="y")
	{
		String name = getFileName(filename);
		imwrite(name+".png",img);
	}
	imshow("Img", img);
	waitKey(0);
}

void runResizeAllImages() {
	String folder,filename,name;
	cout << "Enter folder_name: ";
	cin >> folder;
	deque<String> files;
	FileData fd;
	if(fd.getFilesFromDirectory(folder,files)) {
		for(unsigned int i=0; i<files.size(); i++) {
			filename = folder + files.at(i);
			Mat img = runResizeImage(filename,Size(700,700),0);
			String name = getFileName(filename);
			imwrite(name+".png",img);
			cout << name+" resized!" << endl;
		}
	}
	files.clear();
	files.shrink_to_fit();
}

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

	vec.clear();
	vec.shrink_to_fit();
	return img2;
}

Mat runResizeImage(String filename, Size size, int write)
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
		vec.clear();
		vec.shrink_to_fit();
	}
	return img2;
}

Mat runResizeImage(Mat img, Size size)
{
	Mat img2;
	if(!img.empty()) {
		if(img.cols>=size.width || img.rows>=size.height)
		{resize(img, img2, size,0,0,INTER_AREA);}
		if(img.cols<size.width || img.rows<size.height)
		{resize(img, img2, size,0,0, INTER_CUBIC);}
		img.release();
	} else {
		printf("runResizeImg(): img is empty!\n");
		exit(1);
	}
	return img2;
}

void runHysteresis()
{
	Rgb rgb;
	Hsl hsl;
	Shades shade;
	Color c;
	String filename;
	String name;
	Size size(5,5);
	cout << "Enter filename: ";
	cin >> filename;
	Mat img = imread(filename);
	if(img.data) {
		img = runColorNormalization(img);
		img = runResizeImage(img,Size(700/size.width,700/size.height));
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
			boost::timer time;
			FileData fd(filename);
			fd.ksize = size;
			fd.setImage(img);
			hysteresis(fd);

			cout << "Writing Files..." << flush;
			String strSize = toString(size.width)+"x"+toString(size.height);
			Write::writeToCsv(fd.windowVec,name+"-"+strSize);
			Write::writeToCsv(fd.hslMat,name+"-HSL-"+strSize);

			c.output2ImageColor(fd.colorVec,size,name);
			Write::writeToCsv(fd.colorVec,name+"-ShadeColors-"+strSize);
			cout << "Done!" << endl;

			int hours=0, minutes=0, seconds=0;
			seconds = round(time.elapsed());
			if(seconds>=60) {
				minutes = floor(seconds/60);
				seconds %= 60;
				if(minutes>=60) {
					hours = floor(minutes/60);
					minutes = minutes%60;
				}
			}
			printf("Time Elapsed: %dh:%dm:%ds\n",hours,minutes,seconds);
		}
		img.release();
		rgb.release_memory();
		hsl.release_memory();
		shade.release_memory();
	}
	else {
		printf("Image input incorrect!\n");
		printf("Filename: %s\n", filename.c_str());
		printf("Image Dimensions: %dx%d\n",img.rows,img.cols);
	}
}

void runAllHysteresis() {
	Rgb rgb;
	Hsl hsl;
	Shades shade;
	Color c;
	String folder, full_path;
	cout << "Enter folder_name: ";
	cin >> folder;
	if(folder.at(folder.length()-1)!='/') folder += "/";

	FileData fdFiles;
	deque<String> files;
	String name;
	Size size(5,5);
	Mat img;
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
		boost::timer time;
		int file_count=0;
		for(unsigned int i=0; i<files.size(); i++) {
			full_path = folder+files.at(i);
			img = imread(full_path);
			if(img.data) {
				file_count++;
				img = runColorNormalization(img);
				img = runResizeImage(img,Size(140,140));
				name = getFileName(full_path);
				FileData fd(full_path);
				fd.setImage(img);
				fd.ksize = size;
				hysteresis(fd);

				cout << "Writing Files..." << flush;
				String strSize = toString(size.width)+"x"+toString(size.height);
				Write::writeToCsv(fd.windowVec,name+"-"+strSize);
				Write::writeToCsv(fd.hslMat,name+"-HSL-"+strSize);
				//fd.writeFileMetaData();
				c.output2ImageColor(fd.colorVec,size,name);
				Write::writeToCsv(fd.colorVec,name+"-ShadeColors-"+strSize);
				cout << "Done!" << endl;

				//release images for next use
				img.release();
			}
		}
		int hours=0, minutes=0, seconds=0;
		seconds = round(time.elapsed());
		if(seconds>=60) {
			minutes = floor(seconds/60);
			seconds %= 60;
			if(minutes>=60) {
				hours = floor(minutes/60);
				minutes = minutes%60;
			}
		}
		printf("Total Files Processed: %d\n",file_count);
		printf("Time Elapsed: %dh:%dm:%ds\n",hours,minutes,seconds);
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
	Skin::getSkin(img, mask);
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
			Skin::getSkin(img, mask);
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
	files.clear();
	files.shrink_to_fit();
}

void runMouseColor() {
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	hsl.importHslThresholds();
	rgb.importThresholds();
	sh.importThresholds();
	String filename,input,name;
	cout << "Enter filename: ";
	cin >> filename;
	name = getFileName(filename);
	Mat img, img2, mask;
	img = runResizeImage(filename,Size(700,700),0);
	cout << "Get skin? (y/n) ";
	cin >> input;
	if(input=="y") Skin::getSkin(img, mask);
	img.copyTo(img2, mask);
	Mouse m;
	m.mouseColor(img2, name);
	img.release(); img2.release(); mask.release();
}

void runMouseOutputColor() {
	FileData fd;
	String filename,filepath;
	cout << "Enter filename: ";
	cin >> filename;
	Mat img;
	img = runResizeImage(filename,Size(700,700),0);
	size_t pos1=0, pos2=0;
	pos2 = filename.find("_outputShades");
	for(int i=pos2; i>=0; i--) {
		if(filename[i]=='/') {
			pos1=i+1;
			break;
		}
	}
	fd.filename = filename.substr(pos1,pos2-pos1);
	filepath = filename.substr(0,pos1);
	filepath = filepath+fd.filename+"_ShadeColors.csv";
	fd.loadFileMatrix(filepath,fd.colorVec);
	Mouse m;
	m.mouseOutputColor(img,fd);
	img.release();
}

void runRenameFiles() {
	String old_input, new_input;
	cout << "Enter old name: ";
	cin >> old_input;
	cout << "Enter new name: ";
	cin >> new_input;
	String new_name,old_name;
	size_t pos=0;
	deque<String> files;
	String filepath;
	fs::path full_path( fs::current_path() );
	if ( fs::is_directory( full_path ) )
	{
		fs::directory_iterator end_iter;
		for ( fs::directory_iterator dir_itr( full_path );
				dir_itr != end_iter; ++dir_itr ) { //for loop continued
			try {
				if ( fs::is_regular_file( dir_itr->status() ) ) {
					filepath = dir_itr->path().filename().string();
					files.push_back(filepath);
				}
			}
			catch ( const std::exception & ex ) {
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
				exit(0);
			}
		}
	}
	for(unsigned int i=0; i<files.size(); i++) {
		pos = files.at(i).find(old_input);
		if(pos!=string::npos) {
			old_name = files.at(i);
			new_name = files.at(i).replace(pos,old_input.length(),new_input);
			if(rename(old_name.c_str(),new_name.c_str())==0)
				printf("%s -> %s!\n", old_name.c_str(), new_name.c_str());
			else
				printf("Cannot rename %s\n",files.at(i).c_str());
		}
	}
}

Mat runColorNormalization(Mat &img) {
	Color c;
	Mat img2 = c.shadeCorrection(img);
	img2 = c.changeImageBrightness(img2,0.80,1);
	return img2;
}
