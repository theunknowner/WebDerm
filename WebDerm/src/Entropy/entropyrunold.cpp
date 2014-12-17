/*
 * run.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: jason
 */

#include "entropy.h"

void Entropy::runAllEntropyOld() {
	String filepath;
	deque<String> files;
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
				exit(1);
			}
		}
	}
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	hsl.importHslThresholds();
	rgb.importThresholds();
	sh.importThresholds();
	Size entSize(10,10);
	Size size(5,5);
	String strSize = toString(size.width)+"x"+toString(size.height);
	bool flag[2] = {false};
	for(unsigned int i=0; i<files.size(); i++) {
		Mat img = imread(files.at(i));
		if(img.data) {
			FileData fd;
			fd.filename = getFileName(files.at(i),"_");
			flag[0]=fd.loadFileMatrix(full_path.string()+"/"+fd.filename+"_ShadeColors_"+strSize+".csv",fd.colorVec);
			flag[1]=fd.loadFileMatrix(full_path.string()+"/"+fd.filename+"_HSL_"+strSize+".csv",fd.hslMat);
			if(flag[0]==true && flag[1]==true) {
				fd.ksize = size;
				this->eyeFnOld(fd,entSize,"","");
			}
		}
		img.release();
	}
	rgb.release_memory();
	hsl.release_memory();
	sh.release_memory();
}

void Entropy::runEntropyOld() {
	String input;
	cout << "Enter image name: ";
	cin >> input;
	bool flag[2] = {false};
	Size entSize(5,5);
	FileData fd;
	fd.filename = input;
	flag[0]=fd.loadFileMatrix(fd.filename+"_ShadeColors_5x5.csv",fd.colorVec);
	flag[1]=fd.loadFileMatrix(fd.filename+"_HSL_5x5.csv",fd.hslMat);
	if(flag[0]==true && flag[1]==true) {
		Rgb rgb;
		Hsl hsl;
		Shades sh;
		hsl.importHslThresholds();
		rgb.importThresholds();
		sh.importThresholds();
		this->importEntropyThresholds();
		fd.ksize=Size(5,5);
		this->eyeFn(fd,entSize,"","");
		rgb.release_memory();
		hsl.release_memory();
		sh.release_memory();
	}
}

void Entropy::runCompareEntropyOld(String targetName) {
	String input;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Old/";
		String filepath, name;
		double results;
		deque<double> resultVec;
		deque<String> nameVec;
		deque<int> origPos;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"_");
			if(name==targetName || targetName=="") {
			this->loadEntropyFiles(filepath,vec2,colorNameVec);
			results = this->compareEntropyOld(vec1,vec2,colorNameVec);
			resultVec.push_back(results);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			}
		}
		jaysort(resultVec,origPos);
		for(unsigned int i=0; i<resultVec.size(); i++) {
			printf("%s: %f\n",nameVec.at(origPos.at(i)).c_str(),resultVec.at(i));
		}
	}
}

void Entropy::runCompareEntropy2Old(String targetName) {
	String input;
	deque<String> colorNameVec;
	deque<deque<double> > vec1;
	cout << "Enter filename: ";
	cin >> input;
	Entropy en;
	if(en.loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Old/";
		String filepath, name;
		double results;
		deque<double> resultVec;
		deque<String> nameVec;
		deque<int> origPos;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"_");
			if(name==targetName || targetName=="") {
			en.loadEntropyFiles(filepath,vec2,colorNameVec);
			results = en.compareEntropy2Old(vec1,vec2,colorNameVec);
			resultVec.push_back(results);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			}
		}
		jaysort(resultVec,origPos);
		for(unsigned int i=0; i<resultVec.size(); i++) {
			printf("%s: %f\n",nameVec.at(origPos.at(i)).c_str(),resultVec.at(i));
		}
	}
}
