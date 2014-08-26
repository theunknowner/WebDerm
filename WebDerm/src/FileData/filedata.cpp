/*
 * filedata.cpp
 *
 *  Created on: Jul 17, 2014
 *      Author: jason
 */

#include "filedata.h"

FileData::FileData(String file_path) {
	setFilePath(file_path);
	if(!filename.empty())
		printf("Constructor executed for %s\n",filename.c_str());
	shadeCount = 0;
	minIntensity = 0;
	maxIntensity = 0;
	minOutlier = 0;
	maxOutlier = 0;
	matSize = Size(0,0);
	localRatioScanSize = 0;
	localScanSize=0;
	range=0;
	}

FileData::~FileData() {
	matImage.release();
	deque< deque<String> >().swap(windowVec);
	deque< deque<String> >().swap(hslMat);
	deque< deque<double> >().swap(absRatioMat);
	deque< deque<String> >().swap(colorVec);
	deque< deque<double> >().swap(intensityVec);
	deque< deque<double> >().swap(smoothIntensityVec);
	deque< deque<String> >().swap(shadeVec);
	deque<double>().swap(absRatioVec);

	deque< deque<String> >().swap(m_ContrastMat);
	deque< deque<String> >().swap(d_HslMat);
	deque< deque<String> >().swap(hslPtMat);
	deque< deque<double> >().swap(cumHslMat);
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
	fprintf(fp,"%s,%d\n",stringify(shadeCount), shadeCount);
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
			vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(1,vec.at(i).length()-2);
			}
			dataMatrix.push_back(vec);
			vec.clear();
		}
		deque<String>().swap(vec);
		fs.close();
		return true;
	}
	else
		cout << "Failed to load File matrix!" << endl;
	return false;
}

bool FileData::loadFileMatrix(String file_path, deque< deque<String> > &dataMat) {
	fstream fs(file_path.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(1,vec.at(i).length()-2);
			}
			dataMat.push_back(vec);
			vec.clear();
		}
		deque<String>().swap(vec);
		fs.close();
		return true;
	}
	else
		cout << "Failed to load File matrix!" << endl;
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

bool FileData::getFilesFromDirectory(String directory, deque<String> &files) {
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
			}
			catch ( const std::exception & ex ) {
				std::cout << dir_itr->path().filename() << " " << ex.what() << std::endl;
				exit(0);
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
}
