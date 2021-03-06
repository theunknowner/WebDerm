/*
 * filedata.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#include "filedata.h"
#include "../functions.h"
#include "../Color/color.h"
#include "../rgb/rgb.h"

using namespace ip;

FileData::FileData(String file_path) {
	setFilePath(file_path);
	if(!filename.empty())
		printf("Constructor executed for %s\n",filename.c_str());
	totalShades = 0;
	minIntensity = 0;
	maxIntensity = 0;
	minOutlier = 0;
	maxOutlier = 0;
	ksize = Size(0,0);
	localRatioScanSize = 0;
	localScanSize=0;
	range=0;

	ksize = Size(2,2);

	this->maxHslValues.resize(Rgb::allColors.size(),deque<double>(3,0));
}

FileData::~FileData() {
	matImage.release();
	windowVec.clear();
	windowVec.shrink_to_fit();
	hslMat.clear();
	hslMat.shrink_to_fit();
	absRatioMat.clear();
	absRatioMat.shrink_to_fit();
	colorVec.clear();
	colorVec.shrink_to_fit();
	intensityVec.clear();
	intensityVec.shrink_to_fit();
	smoothIntensityVec.clear();
	smoothIntensityVec.shrink_to_fit();
	shadeVec.clear();
	shadeVec.shrink_to_fit();
	absRatioVec.clear();
	absRatioVec.shrink_to_fit();

	m_ContrastMat.clear();
	m_ContrastMat.shrink_to_fit();
	d_HslMat.clear();
	d_HslMat.shrink_to_fit();
	hslPtMat.clear();
	hslPtMat.shrink_to_fit();
	cumHslMat.clear();
	cumHslMat.shrink_to_fit();

	if(!filename.empty())
		printf("Destructor executed for %s\n",filename.c_str());
}

void FileData::setFilePath(String file_path) {
	this->file_path = file_path;
	filename = getFileName(file_path);
}
String FileData::getFilePath() { return file_path; }

/** gets the imported data matrix by passing as reference **/
void FileData::getFileMatrix(deque< deque<String> > &vec) {
	vec = dataMatrix;
	dataMatrix.clear();
	deque< deque<String> >().swap(dataMatrix);
}

//sets image and size
void FileData::setImage(Mat img) {
	this->matImage = img.clone();
	this->imgSize = Size(img.cols, img.rows);
}

Mat FileData::getImage() {
	return this->matImage;
}

void FileData::writeFileMetaData() {
	String filename = this->filename + "_FileData.csv";
	FILE * fp;
	fp = fopen(filename.c_str(), "w");
	fprintf(fp,"Filename,%s\n",filename.c_str());
	fprintf(fp,"Path,%s\n",this->file_path.c_str());
	fprintf(fp,"%s,%s\n",stringify(oldMinShade), oldMinShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(oldMaxShade), oldMaxShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(newMinShade), newMinShade.c_str());
	fprintf(fp,"%s,%s\n",stringify(newMaxShade), newMaxShade.c_str());
	fprintf(fp,"%s,%0.2f\n",stringify(minIntensity), minIntensity);
	fprintf(fp,"%s,%0.2f\n",stringify(maxIntensity), maxIntensity);
	fprintf(fp,"%s,%d\n",stringify(totalShades), totalShades);
	fclose(fp);
}

void FileData::writeFileMetaData(int n_args, String* strArr, double* valArr) {
	String filename = this->filename + "_FileData.csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(int i=0; i<n_args; i++) {
		fprintf(fp,"%s,%s\n",strArr[i].c_str(),toString(valArr[i]).c_str());
	}
	fclose(fp);
}
/** imports the matrix/csv files **/
bool FileData::loadFileMatrix(String file_path) {
	fstream fs(file_path.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			//vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(1,vec.at(i).length()-2);
			}
			dataMatrix.push_back(vec);
			vec.clear();
		}
		vec.clear();
		vec.shrink_to_fit();
		fs.close();
		return true;
	}
	else
		cout << "Failed to load File matrix!" << endl;
	return false;
}

//loads file into a 2D string vector
bool FileData::loadFileMatrix(String file_path, deque< deque<String> > &dataMat) {
	fstream fs(file_path.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			//vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(0,vec.at(i).length());
			}
			dataMat.push_back(vec);
			vec.clear();
		}
		vec.clear();
		vec.shrink_to_fit();
		fs.close();
		return true;
	}
	else
		printf("%s: Failed to load File matrix!\n",this->filename.c_str());
	return false;
}

int FileData::listFiles(String directory) {
	fs::path full_path( fs::initial_path<fs::path>() );
	full_path = fs::system_complete( fs::path( directory ) );
	unsigned long file_count = 0;
	unsigned long dir_count = 0;
	unsigned long other_count = 0;
	unsigned long err_count = 0;

	if ( !fs::exists( full_path ) )
	{
		cout << "Not found: " << full_path.string() << endl;
		return 1;
	}

	if ( fs::is_directory( full_path ) )
	{
		std::cout << "In directory: " << full_path.string() << "\n\n";
		fs::directory_iterator end_iter;
		for ( fs::directory_iterator dir_itr( full_path );
				dir_itr != end_iter;
				++dir_itr )
		{
			try
			{
				if ( fs::is_directory( dir_itr->status() ) )
				{
					++dir_count;
					std::cout << dir_itr->path().filename() << " [directory]\n";
				}
				else if ( fs::is_regular_file( dir_itr->status() ) )
				{
					++file_count;
					std::cout << dir_itr->path().filename() << "\n";
				}
				else
				{
					++other_count;
					std::cout << dir_itr->path().filename() << " [other]\n";
				}

			}
			catch ( const std::exception & ex )
			{
				++err_count;
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
			}
		}
		std::cout << "\n" << file_count << " files\n"
				<< dir_count << " directories\n"
				<< other_count << " others\n"
				<< err_count << " errors\n";
	}
	else // must be a file
	{
		std::cout << "\nFound: " << full_path.filename().string() << "\n";
	}

	return 0;
}

//!set flag to 1 to include files from subfolders
bool FileData::getFilesFromDirectory(String directory, deque<String> &files, int flag) {
	String filepath;
	fs::path full_path( fs::initial_path<fs::path>() );
	full_path = fs::system_complete( fs::path( directory ) );

	if ( !fs::exists( full_path ) )
	{
		std::cout << "Not found: " << full_path.string() << std::endl;
		return false;
	}

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
				else {
					if(flag==1) {
						filepath = dir_itr->path().string();
						this->getFilesFromDirectory(filepath,files);
					}
				}
			}
			catch ( const std::exception & ex ) {
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
				exit(1);
			}
		}
	}
	return true;
}

/** deletes files containing filename,
 * directory must end with a "/" **/
void FileData::deleteContainingName(String directory, String filename) {
	deque<String> files;
	String full_path;
	getFilesFromDirectory(directory,files);
	for(unsigned int i=0; i<files.size(); i++) {
		full_path = directory+files.at(i);
		if(files.at(i).find(filename)!=string::npos) {
			if(remove(full_path.c_str())==0)
				printf("%s deleted!\n", files.at(i).c_str());
			else
				printf("Error deleting %s\n",files.at(i).c_str());
		}
	}
	files.clear();
	files.shrink_to_fit();
}

/** rename files containing oldname,
 * directory must end with a "/" **/
void FileData::renameFiles(String directory, String oldname, String newname) {
	size_t pos=0;
	String name;
	deque<String> files;
	String full_path_old, full_path_new;
	getFilesFromDirectory(directory,files);
	for(unsigned int i=0; i<files.size(); i++) {
		full_path_old = directory+files.at(i);
		pos = files.at(i).find(oldname);
		if(pos!=string::npos) {
			name = files.at(i).replace(pos,oldname.length(),newname);
			full_path_new = directory+name;
			if(rename(full_path_old.c_str(),full_path_new.c_str())==0)
				printf("%s renamed!\n", files.at(i).c_str());
			else
				printf("Error renaming %s\n",files.at(i).c_str());
		}
	}
	files.clear();
	files.shrink_to_fit();
}

bool FileData::isFileExist(String file) {
	fstream fs(file.c_str());
	if(fs.is_open()) {
		fs.close();
		return true;
	}
	fs.close();
	return false;
}

void FileData::getMinMaxPts(String input, deque< deque<Point> > &pt) {
	size_t posMins=0;
	size_t posMaxs=0;
	posMins = input.find("Min");
	posMaxs = input.find("Max");
	String minStr = input.substr(posMins,posMaxs-posMins);
	String maxStr = input.substr(posMaxs,input.length()-posMaxs);
	size_t posBeginPar=0;
	size_t posEndPar=0;
	size_t posDelimiter=0;
	Point tempPt;
	deque<Point> tempPtVec;
	while(minStr.length()>0) {
		posBeginPar = minStr.find("(");
		posEndPar = minStr.find(")");
		posDelimiter = minStr.find(";");
		tempPt.x = atoi(minStr.substr(posBeginPar+1,posDelimiter-posBeginPar+1).c_str());
		tempPt.y = atoi(minStr.substr(posDelimiter+1,posEndPar-posDelimiter+1).c_str());
		tempPtVec.push_back(tempPt);
		if(posEndPar==(minStr.length()-1))
			minStr = minStr.substr(posEndPar+1,minStr.length()-posEndPar+1);
		else
			minStr = minStr.substr(posEndPar+2,minStr.length()-posEndPar+2);
	}
	pt.push_back(tempPtVec);
	tempPtVec.clear();
	while(maxStr.length()>0) {
		posBeginPar = maxStr.find("(");
		posEndPar = maxStr.find(")");
		posDelimiter = maxStr.find(";");
		tempPt.x = atoi(maxStr.substr(posBeginPar+1,posDelimiter-posBeginPar+1).c_str());
		tempPt.y = atoi(maxStr.substr(posDelimiter+1,posEndPar-posDelimiter+1).c_str());
		tempPtVec.push_back(tempPt);
		if(posEndPar==(maxStr.length()-1))
			maxStr = maxStr.substr(posEndPar+1,maxStr.length()-posEndPar+1);
		else
			maxStr = maxStr.substr(posEndPar+2,maxStr.length()-posEndPar+2);
	}
	pt.push_back(tempPtVec);
	tempPtVec.clear();
	tempPtVec.shrink_to_fit();
}

void FileData::extractRule3Data(String filename, Point loc) {
	deque< deque<String> > hslPtsVec;
	deque< deque<String> > mConVec;
	deque< deque<String> > deltaHslVec;
	deque< deque<String> > hslVec;
	deque< deque<String> > colorVec;
	loc = Point(loc.x-1,loc.y-1);
	String location  = "/home/jason/Desktop/Programs/";
	String hslPtsFile = location + filename + "_hslPts.csv";
	String mConFile = location + filename + "_measuredContrast.csv";
	String deltaHslFile = location + filename + "_deltaHSL.csv";
	String hslFile = location + filename + "_HSL.csv";
	String colorFile = location + filename + "_ShadeColors.csv";
	loadFileMatrix(hslPtsFile,hslPtsVec);
	loadFileMatrix(mConFile,mConVec);
	loadFileMatrix(deltaHslFile,deltaHslVec);
	loadFileMatrix(hslFile,hslVec);
	loadFileMatrix(colorFile,colorVec);
	String outputFilename = "/home/jason/Desktop/workspace/" + filename + "_Rule3Data.csv";
	FILE * fp;
	fp = fopen(outputFilename.c_str(),"w");
	String minMaxPts = hslPtsVec.at(loc.y).at(loc.x);
	String mCon = mConVec.at(loc.y).at(loc.x);
	String deltaHSL = deltaHslVec.at(loc.y).at(loc.x);
	String hsl = hslVec.at(loc.y).at(loc.x);
	deque< deque<Point> > pt;
	getMinMaxPts(minMaxPts, pt);
	String hslMin = "";
	String hslMax = "";
	String colorsMin = "";
	String colorsMax = "";
	Color c;
	String color = c.extractShade(colorVec.at(loc.y).at(loc.x))+
			c.getMainColor(colorVec.at(loc.y).at(loc.x));
	for(unsigned int i=0; i<pt.at(0).size(); i++) {
		hslMin += "("+ hslVec.at(pt.at(0).at(i).y-1).at(pt.at(0).at(i).x-1)+")";
		hslMax += "("+ hslVec.at(pt.at(1).at(i).y-1).at(pt.at(1).at(i).x-1)+")";
		colorsMin += "("+c.extractShade(colorVec.at(pt.at(0).at(i).y-1).at(pt.at(0).at(i).x-1))+
				c.getMainColor(colorVec.at(pt.at(0).at(i).y-1).at(pt.at(0).at(i).x-1))+")";
		colorsMax += "("+c.extractShade(colorVec.at(pt.at(1).at(i).y-1).at(pt.at(1).at(i).x-1))+
				c.getMainColor(colorVec.at(pt.at(1).at(i).y-1).at(pt.at(1).at(i).x-1))+")";
	}
	fprintf(fp,"%s,(%d;%d) - HSL(%s) - %s\n",filename.c_str(),loc.x+1,loc.y+1,hsl.c_str(),color.c_str());
	fprintf(fp,"Points,%s\n",minMaxPts.c_str());
	fprintf(fp,"mCon,%s\n",mCon.c_str());
	fprintf(fp,"dHSL,%s\n",deltaHSL.c_str());
	fprintf(fp,"HslMin,%s\n",hslMin.c_str());
	fprintf(fp,"HslMax,%s\n",hslMax.c_str());
	fprintf(fp,"ColorsMin,%s\n", colorsMin.c_str());
	fprintf(fp,"ColorsMax,%s\n", colorsMax.c_str());
	fclose(fp);
}

deque< deque<double> > FileData::stringVec2Double(deque< deque<String> > &vec) {
	deque< deque<double> > doubleVec;
	deque<double> tempVec;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			tempVec.push_back(atof(vec.at(i).at(j).c_str()));
		}
		doubleVec.push_back(tempVec);
		tempVec.clear();
	}
	return doubleVec;
}

Mat FileData::stringVec2Mat1D(deque<deque<String> > &vec) {
	deque<double> vec1d;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			vec1d.push_back(atof(vec.at(i).at(j).c_str()));
		}
	}
	Mat results(vec1d.size(),1,CV_32F,Scalar(0));
	for(unsigned int i=0; i<vec1d.size(); i++) {
		results.at<float>(i,0) = vec1d.at(i);
	}
	return results;
}

//! fixes the number-labels of sequential files
//! digits = num of digits of the number-label sequence
void FileData::fixFileNumberSequence(String folder, String beginDelimit, int digits) {
	deque<String> files;
	this->getFilesFromDirectory(folder,files);
	size_t beginPos=0, endPos=0;
	for(unsigned int i=0; i<files.size(); i++) {
		beginPos = files.at(i).find(beginDelimit);
		beginPos+=beginDelimit.length();
		for(unsigned int j=beginPos; j<files.at(i).length(); j++) {
			if(isdigit(files.at(i).at(j))==0) {
				endPos = j;
				break;
			}
		}
		String str = files.at(i).substr(beginPos,endPos-beginPos);
		if(str.length()<digits) {
			int num = atoi(str.c_str());
			int numOfZero=digits - str.length();
			for(int k=0; k<numOfZero; k++) {
				str = "0" + str;
			}

			String newname = files.at(i);
			newname.replace(beginPos,endPos-beginPos,str);
			newname = folder + newname;
			String oldname = folder + files.at(i);
			rename(oldname.c_str(),newname.c_str());
			printf("%s fixed!\n",newname.c_str());
		}
		else {
			printf("%s has %d or more digits already!\n",files.at(i).c_str(),digits);
		}
	}
}

String FileData::getFileSequenceNum(String name, String beginDelimit, size_t &beginPos) {
	size_t begin = name.find(beginDelimit);
	size_t end=begin;
	begin += beginDelimit.length();
	for(unsigned int i=begin;  i<name.length(); i++) {
		if(isdigit(name.at(i))==0) {
			end = i;
			break;
		}
	}
	String str = name.substr(begin,end-begin);
	beginPos = begin;
	return str;
}

void FileData::reorderFileSequence(String folder) {
	deque<String> files;
	this->getFilesFromDirectory(folder,files);
	sort(files.begin(),files.end());
	size_t pos=0;
	for(unsigned int i=0; i<files.size(); i++) {
		String name = folder + files.at(i);
		String filename = files.at(i);
		String seqStr = this->getFileSequenceNum(files.at(i),"_",pos);
		int seq = atoi(seqStr.c_str());
		if(seq!=(i+1)) {
			filename.replace(pos,seqStr.length(),toString(i+1));
			String newname = folder + filename;
			rename(name.c_str(),newname.c_str());
		}
	}
}
