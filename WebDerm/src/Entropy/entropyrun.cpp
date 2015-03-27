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
				exit(1);
			}
		}
	}
	Rgb rgb;
	Hsl hsl;
	Shades sh;
	ShapeMorph sm;
	ShapeColor sc;
	hsl.importHslThresholds();
	rgb.importThresholds();
	sh.importThresholds();
	Size entSize(10,10);
	Size size(5,5);
	Size imgSize(140,140);
	String strSize = toString(size.width)+"x"+toString(size.height);
	bool flag[2] = {false};
	this->entSize = entSize;
	for(unsigned int i=0; i<files.size(); i++) {
		Mat img = imread(files.at(i));
		if(img.data) {
			FileData fd;
			fd.filename = getFileName(files.at(i),"-");
			cout << fd.filename << endl;
			String file = "/home/jason/Desktop/Programs/Looks_Like/"+fd.filename+".jpg";
			Mat regImg = imread(file);
			Mat normImg = runColorNormalization(regImg);
			regImg = runResizeImage(normImg,imgSize);
			Mat grayImg, blurImg, img, img2;
			blur(regImg,blurImg,size);
			cvtColor(regImg,grayImg,CV_BGR2GRAY);
			fd.setImage(grayImg);
			flag[0]=fd.loadFileMatrix(full_path.string()+"/"+fd.filename+"-ShadeColors-"+strSize+".csv",fd.colorVec);
			flag[1]=fd.loadFileMatrix(full_path.string()+"/"+fd.filename+"-HSL-"+strSize+".csv",fd.hslMat);
			Mat shadeColor = imread(full_path.string()+"/"+fd.filename+"-outputShades-5x5.png");
			if(flag[0]==true && flag[1]==true) {
				fd.ksize = size;
				Mat src = sm.prepareImage(grayImg);
				Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
				Mat map = sc.getShapeUsingColor2(regImg,mapOfNonNoise,0.65);
				Mat dst = sm.origFilter(grayImg);
				grayImg.copyTo(img,map);
				grayImg.copyTo(img,dst);
				shadeColor.copyTo(img2,map);
				shadeColor.copyTo(img2,dst);
				imwrite(fd.filename+"-Entropy-outputShades-5x5.png",img2);
				this->shapeFn(fd);
				this->eyeFn(fd,entSize,img,"","");
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
	Size entSize(10,10);
	Size size(5,5);
	Size imgSize(140,140);
	FileData fd;
	fd.filename = input;
	String file = "/home/jason/Desktop/Programs/Looks_Like/"+fd.filename+".jpg";
	Mat regImg = runResizeImage(file,imgSize,0);
	Mat normImg = runColorNormalization(regImg);
	Mat grayImg;
	cvtColor(normImg,grayImg,CV_BGR2GRAY);
	fd.setImage(grayImg);
	flag[0]=fd.loadFileMatrix(fd.filename+"-ShadeColors-5x5.csv",fd.colorVec);
	flag[1]=fd.loadFileMatrix(fd.filename+"-HSL-5x5.csv",fd.hslMat);
	if(flag[0]==true && flag[1]==true) {
		Rgb rgb;
		Hsl hsl;
		Shades sh;
		hsl.importHslThresholds();
		rgb.importThresholds();
		sh.importThresholds();
		this->importEntropyThresholds();
		fd.ksize=size;
		this->entSize = entSize;
		this->shapeFn(fd);
		eyeFn(fd,entSize,Mat(),"","");
		rgb.release_memory();
		hsl.release_memory();
		sh.release_memory();
	}
}

void Entropy::runCompareEntropy(String targetName) {
	String input;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		FileData fd;
		//String folder = "/home/jason/Desktop/Programs/TestYSV_New/";
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name;
		double resultsYSV, resultsT;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			if(name==targetName || targetName=="") {
				this->loadEntropyFiles(filepath,vec2,colorNameVec);
				resultsYSV = this->compareYSV(vec1,vec2,colorNameVec);
				resultsT = this->compareT(vec1,vec2,colorNameVec);
				vec2.clear();
				vec2.shrink_to_fit();
				printf("%s: %f | %f\n",name.c_str(),resultsYSV, resultsT);
			}
		}
	}
}

void Entropy::runCompareEntropy2(String targetName) {
	String input;
	deque<String> colorNameVec;
	deque<deque<double> > vec1;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name;
		double results;
		deque<double> resultVec;
		deque<String> nameVec;
		deque<int> origPos;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			if(name==targetName || targetName=="") {
				this->loadEntropyFiles(filepath,vec2,colorNameVec);
				results = this->compareEntropy2(vec1,vec2,colorNameVec);
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

void Entropy::runCompareEntropyList(String fileList, String folder) {
	fstream fs(fileList.c_str());
	if(fs.is_open()) {
		String temp,filepath,name;
		deque<String> listPairVec;
		deque<deque<double> > vec1;
		deque<deque<double> > vec2;
		deque<String> colorNameVec;
		deque<String> files;
		double resultsYSV, resultsT;
		float countPairs=0, countMatch=0, countSortOfMatch=0;
		FileData fd;
		fd.getFilesFromDirectory(folder,files);
		while(getline(fs,temp)) {
			countPairs++;
			getSubstr(temp,',',listPairVec);
			for(unsigned int i=0; i<files.size(); i++) {
				filepath = folder+files.at(i);
				name = getFileName(files.at(i),"-");
				if(name==listPairVec.at(0)) {
					break;
				}
			}
			if(this->loadEntropyFiles(filepath,vec1,colorNameVec)) {
				for(unsigned int i=0; i<files.size(); i++) {
					int flagMatch = 0;
					filepath = folder+files.at(i);
					name = getFileName(files.at(i),"-");
					if(name==listPairVec.at(1)) {
						this->loadEntropyFiles(filepath,vec2,colorNameVec);
						resultsYSV = this->compareYSV(vec1,vec2,colorNameVec);
						resultsT = this->compareT(vec1,vec2,colorNameVec);
						resultsYSV = roundDecimal(resultsYSV,2);
						if(resultsYSV>=0.70 && resultsT>=1.0) {
							countMatch++;
							flagMatch=1;
						}
						else if(resultsYSV>=0.70 && resultsT==0.0) {
							countSortOfMatch++;
							flagMatch=0;
						}
						else if(resultsYSV<0.70 && resultsYSV>=0.60 && resultsT>=1.0) {
							countSortOfMatch++;
							flagMatch=0;
						}
						else {
							flagMatch=-1;
						}
						printf("%s-%s: %f | %f => %d\n",listPairVec.at(0).c_str(),listPairVec.at(1).c_str(),resultsYSV,resultsT,flagMatch);
					}
				}
			}
			listPairVec.clear();
		}
		float ratio = countMatch/countPairs;
		float ratio2 = countSortOfMatch/countPairs;
		printf("%.f/%.f = %f matched\n",countMatch,countPairs,ratio);
		printf("%.f/%.f = %f sort of matched\n",countSortOfMatch,countPairs,ratio2);
		fs.close();
	}
	else {
		cout << "Failed to open file!" << endl;
	}
}

void Entropy::runCompareEntropyList2(String fileList, String folder) {
	fstream fs(fileList.c_str());
	if(fs.is_open()) {
		String temp,filepath,name;
		deque<String> listPairVec;
		deque<deque<double> > vec1;
		deque<deque<double> > vec2;
		deque<String> colorNameVec;
		deque<String> files;
		float results;
		float countPairs=0, countResults=0;
		FileData fd;
		fd.getFilesFromDirectory(folder,files);
		while(getline(fs,temp)) {
			countPairs++;
			getSubstr(temp,',',listPairVec);
			for(unsigned int i=0; i<files.size(); i++) {
				filepath = folder+files.at(i);
				name = getFileName(files.at(i),"-");
				if(name==listPairVec.at(0)) {
					break;
				}
			}
			if(this->loadEntropyFiles(filepath,vec1,colorNameVec)) {
				for(unsigned int i=0; i<files.size(); i++) {
					filepath = folder+files.at(i);
					name = getFileName(files.at(i),"_");
					if(name==listPairVec.at(1)) {
						this->loadEntropyFiles(filepath,vec2,colorNameVec);
						results = this->compareEntropy2(vec1,vec2,colorNameVec);
						if(results>=0.70) countResults++;
						printf("%s-%s: %f\n",listPairVec.at(0).c_str(),listPairVec.at(1).c_str(),results);
					}
				}
			}
			listPairVec.clear();
		}
		float ratio = countResults/countPairs;
		printf("%.f/%.f = %f matched\n",countResults,countPairs,ratio);
		fs.close();
	}
	else {
		cout << "Failed to open file!" << endl;
	}
}

void Entropy::runCompareAllEntropy(String folder) {
	String input;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		deque<double> resultVecYSV;
		deque<double> resultVecT;
		deque<String> nameVec;
		deque<int> origPos;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name, inputName;
		inputName = getFileName(input,"-");
		double resultsYSV, resultsT;
		int countMatch=0, countSortOfMatch=0, flagMatch=0;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			this->loadEntropyFiles(filepath,vec2,colorNameVec);
			resultsYSV = this->compareYSV(vec1,vec2,colorNameVec);
			resultsT = this->compareT(vec1,vec2,colorNameVec);
			resultVecYSV.push_back(resultsYSV);
			resultVecT.push_back(resultsT);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			//printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
		}
		jaysort(resultVecYSV,origPos);
		FILE *fp;
		String filename = inputName+"-ListOfMatches.csv";
		if(!fd.isFileExist(filename))
			fp=fopen(filename.c_str(),"w");
		else {
			cout << "File exist! Overwrite? (y/n)" << endl;
			cin >> input;
			if(input=="y")
				fp=fopen(filename.c_str(),"w");
		}
		for(unsigned int i=0; i<resultVecYSV.size(); i++) {
			resultsYSV = resultVecYSV.at(i);
			resultsYSV = roundDecimal(resultsYSV,2);
			resultsT = resultVecT.at(origPos.at(i));
			name = nameVec.at(origPos.at(i));
			if(resultsYSV>=0.70 && resultsT>=1.0) {
				countMatch++;
				flagMatch=1;
			}
			else if(resultsYSV>=0.70 && resultsT==0.0) {
				countSortOfMatch++;
				flagMatch=0;
			}
			else if(resultsYSV<0.70 && resultsYSV>=0.60 && resultsT>=1.0) {
				countSortOfMatch++;
				flagMatch=0;
			}
			else {
				flagMatch=-1;
			}
			printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
			fprintf(fp,"%s,%f,%f,%d\n",name.c_str(),resultsYSV,resultsT,flagMatch);
		}
		fclose(fp);
	}
}

void Entropy::runCompareAllEntropy2(String folder) {
	String input;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		deque<double> resultVecYSV;
		deque<double> resultVecT;
		deque<String> nameVec;
		deque<int> origPos;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name, inputName;
		inputName = getFileName(input,"-");
		double resultsYSV, resultsT;
		int countMatch=0, countSortOfMatch=0, flagMatch=0;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			this->loadEntropyFiles(filepath,vec2,colorNameVec);
			resultsYSV = this->compareEntropy2(vec1,vec2,colorNameVec);
			resultVecYSV.push_back(resultsYSV);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			//printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
		}
		jaysort(resultVecYSV,origPos);
		FILE *fp;
		String filename = inputName+"-ListOfMatchesAA.csv";
		if(!fd.isFileExist(filename))
			fp=fopen(filename.c_str(),"w");
		else {
			cout << "File exist! Overwrite? (y/n)" << endl;
			cin >> input;
			if(input=="y")
				fp=fopen(filename.c_str(),"w");
		}
		for(unsigned int i=0; i<resultVecYSV.size(); i++) {
			resultsYSV = resultVecYSV.at(i);
			resultsYSV = roundDecimal(resultsYSV,2);
			name = nameVec.at(origPos.at(i));
			if(resultsYSV>=0.75) {
				countMatch++;
				flagMatch=1;
			}
			else if(resultsYSV<0.75 && resultsYSV>=0.60) {
				countSortOfMatch++;
				flagMatch=0;
			}
			else {
				flagMatch=-1;
			}
			printf("%d) %s: %f => %d\n",i,name.c_str(),resultsYSV, flagMatch);
			fprintf(fp,"%s,%f,%d\n",name.c_str(),resultsYSV,flagMatch);
		}
		fclose(fp);
	}
}

//test run function for test_compareEntropy2a
void Entropy::test_runCompareEntropy2a(String targetName) {
	String input;
	deque<String> colorNameVec;
	deque<deque<double> > vec1;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name;
		double results;
		deque<double> resultVec;
		deque<String> nameVec;
		deque<int> origPos;
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			if(name==targetName || targetName=="") {
				this->loadEntropyFiles(filepath,vec2,colorNameVec);
				results = this->test_compareEntropy2a(vec1,vec2,colorNameVec);
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

void Entropy::test_runAllCompareEntropy2a(String folder) {
	String input;
	deque<deque<double> > vec1;
	deque<String> colorNameVec;
	cout << "Enter filename: ";
	cin >> input;
	if(this->loadEntropyFiles(input,vec1,colorNameVec)) {
		deque<String> files;
		deque<deque<double> > vec2;
		deque<double> resultVecYSV;
		deque<double> resultVecT;
		deque<String> nameVec;
		deque<int> origPos;
		FileData fd;
		String folder = "/home/jason/Desktop/Programs/TestYSV_Output/";
		String filepath, name, inputName;
		inputName = getFileName(input,"-");
		double resultsYSV, resultsT;
		int countMatch=0, countSortOfMatch=0, flagMatch=0;
		String metricT_String="";
		String matchString="";
		fd.getFilesFromDirectory(folder,files);
		for(unsigned int i=0; i<files.size(); i++) {
			filepath = folder+files.at(i);
			name = getFileName(files.at(i),"-");
			this->loadEntropyFiles(filepath,vec2,colorNameVec);
			resultsYSV = this->test_compareEntropy2a(vec1,vec2,colorNameVec);
			resultsT = this->compareT(vec1,vec2,colorNameVec);
			resultVecYSV.push_back(resultsYSV);
			resultVecT.push_back(resultsT);
			nameVec.push_back(name);
			vec2.clear();
			vec2.shrink_to_fit();
			//printf("%d) %s: %f | %f => %d\n",i,name.c_str(),resultsYSV, resultsT, flagMatch);
		}
		jaysort(resultVecYSV,origPos);
		FILE *fp;
		String filename = inputName+"-ListOfMatchesBB.csv";
		if(!fd.isFileExist(filename))
			fp=fopen(filename.c_str(),"w");
		else {
			cout << "File exist! Overwrite? (y/n)" << endl;
			cin >> input;
			if(input=="y")
				fp=fopen(filename.c_str(),"w");
		}
		for(unsigned int i=0; i<resultVecYSV.size(); i++) {
			resultsYSV = resultVecYSV.at(i);
			resultsYSV = roundDecimal(resultsYSV,2);
			resultsT = resultVecT.at(origPos.at(i));
			name = nameVec.at(origPos.at(i));
			if(resultsT>=1.0) metricT_String = "Circle";
			if(resultsT==0.0) metricT_String = "Varying Circle";
			if(resultsT==-0.5) metricT_String = "Random";
			if(resultsT<=-1.0) metricT_String = "Random/Circle";
			if(resultsYSV>=0.70 && resultsT>=1.0) {
				countMatch++;
				flagMatch=1;
				matchString="Match";
			}
			else if(resultsYSV>=0.70 && resultsT==-0.5) {
				countSortOfMatch++;
				flagMatch=0;
				matchString="Partial";
			}
			else if(resultsYSV<0.70 && resultsYSV>=0.60 && resultsT>=1.0) {
				countSortOfMatch++;
				flagMatch=0;
				matchString="Partial";
			}
			else {
				flagMatch=-1;
				matchString="No_Match";
			}
			printf("%d) %s: %f | %s => %s\n",i,name.c_str(),resultsYSV, metricT_String.c_str(), matchString.c_str());
			fprintf(fp,"%s,%f,%s,%s\n",name.c_str(),resultsYSV,metricT_String.c_str(),matchString.c_str());
		}
		fclose(fp);
	}
}
