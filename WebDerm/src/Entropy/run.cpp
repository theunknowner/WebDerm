/*
 * run.cpp
 *
 *  Created on: Nov 14, 2014
 *      Author: jason
 */

#include "entropy.h"

void Entropy::runAllEntropy() {
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
				exit(0);
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
				eyeFn(fd,entSize,"");
			}
		}
		img.release();
	}
	rgb.release_memory();
	hsl.release_memory();
	sh.release_memory();
}

void Entropy::runEntropy() {
	String input;
	cout << "Enter image name: ";
	cin >> input;
	bool flag[2] = {false};
	Size entSize(50,50);
	FileData fd;
	fd.filename = input;
	flag[0]=fd.loadFileMatrix(fd.filename+"_ShadeColors_10x10.csv",fd.colorVec);
	flag[1]=fd.loadFileMatrix(fd.filename+"_HSL_10x10.csv",fd.hslMat);
	if(flag[0]==true && flag[1]==true) {
		Rgb rgb;
		Hsl hsl;
		Shades sh;
		hsl.importHslThresholds();
		rgb.importThresholds();
		sh.importThresholds();
		fd.ksize=Size(10,10);
		eyeFn(fd,entSize,"");
		rgb.release_memory();
		hsl.release_memory();
		sh.release_memory();
	}
}

void Entropy::runCompareEntropy() {
	String input;
	deque<deque<double> > vec1;
	cout << "Enter filename: ";
	cin >> input;
	Entropy en;
	if(en.loadEntropyFiles(input,vec1)) {
		deque<String> files;
		deque<deque<double> > vec2;
		deque<deque<double> > matchVec;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/Test2/";
		String filepath, name;
		double results;
		deque<double> resultVec;
		deque<String> nameVec;
		deque<int> origPos;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"_");
			en.loadEntropyFiles(filepath,vec2);
			results = en.compareEntropy(vec1,vec2,matchVec);
			resultVec.push_back(results);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			matchVec.clear();
			matchVec.shrink_to_fit();
		}
		jaysort(resultVec,origPos);
		for(unsigned int i=0; i<resultVec.size(); i++) {
			printf("%s: %f\n",nameVec.at(origPos.at(i)).c_str(),resultVec.at(i));
		}
	}
}

void Entropy::runCompareEntropy2() {
	String input;
	deque<deque<double> > vec1;
	cout << "Enter filename: ";
	cin >> input;
	Entropy en;
	if(en.loadEntropyFiles(input,vec1)) {
		deque<String> files;
		deque<deque<double> > vec2;
		deque<deque<double> > matchVec;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/Test2/";
		String filepath, name;
		double results;
		deque<double> resultVec;
		deque<String> nameVec;
		deque<int> origPos;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"_");
			en.loadEntropyFiles(filepath,vec2);
			results = en.compareEntropy2(vec1,vec2);
			resultVec.push_back(results);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			matchVec.clear();
			matchVec.shrink_to_fit();
		}
		jaysort(resultVec,origPos);
		for(unsigned int i=0; i<resultVec.size(); i++) {
			printf("%s: %f\n",nameVec.at(origPos.at(i)).c_str(),resultVec.at(i));
		}
	}
}
