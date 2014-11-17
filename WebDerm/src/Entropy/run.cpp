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
	Size entSize(50,50);
	bool flag[2] = {false};
	for(unsigned int i=0; i<files.size(); i++) {
		Mat img = imread(files.at(i));
		if(img.data) {
			FileData fd;
			fd.filename = getFileName(files.at(i),"_");
			flag[0]=fd.loadFileMatrix(full_path.string()+"/"+fd.filename+"_ShadeColors_10x10.csv",fd.colorVec);
			flag[1]=fd.loadFileMatrix(full_path.string()+"/"+fd.filename+"_HSL_10x10.csv",fd.hslMat);
			if(flag[0]==true && flag[1]==true) {
				fd.ksize = Size(10,10);
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
