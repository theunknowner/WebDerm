/*
 * write.cpp
 *
 *  Created on: May 7, 2014
 *      Author: jason
 */

#include "write.h"

void writeSeq2File(deque< deque<String> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(vec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<vec.size(); i++)
		{
			for(unsigned int j=0; j<vec.at(i).size(); j++)
			{
				if(j<vec.at(i).size()-1)
					fprintf(fp,"%s,", vec.at(i).at(j).c_str());
				else
					fprintf(fp,"%s\n", vec.at(i).at(j).c_str());
			}
		}
	fclose(fp);
}

void writeSeq2File(deque< deque<int> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(vec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<vec.size(); i++)
		{
			for(unsigned int j=0; j<vec.at(i).size(); j++)
			{
				if(j<vec.at(i).size()-1)
					fprintf(fp,"%d,", vec.at(i).at(j));
				else
					fprintf(fp,"%d\n", vec.at(i).at(j));
			}
		}
	fclose(fp);
}

void writeSeq2File(deque< deque<double> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(vec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<vec.size(); i++)
		{
			for(unsigned int j=0; j<vec.at(i).size(); j++)
			{
				if(j<vec.at(i).size()-1)
					fprintf(fp,"%f,", vec.at(i).at(j));
				else
					fprintf(fp,"%f\n", vec.at(i).at(j));
			}
		}
	fclose(fp);
}

void writeSeq2File(deque<double> &vec, String name) {
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(vec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<vec.size(); i++) {
			fprintf(fp,"%f,",vec.at(i));
		}
	fclose(fp);
}

void writeSeq2File(deque<int> &vec, String name) {
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(vec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<vec.size(); i++) {
			fprintf(fp,"%d\n",vec.at(i));
		}
	fclose(fp);
}

void writeSeq2File(deque< deque<long double> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(vec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<vec.size(); i++)
		{
			for(unsigned int j=0; j<vec.at(i).size(); j++)
			{
				if(j<vec.at(i).size()-1)
					fprintf(fp,"%Lf,", vec.at(i).at(j));
				else
					fprintf(fp,"%Lf\n", vec.at(i).at(j));
			}
		}
	fclose(fp);
}

void writeSeq2File(deque<Point> &ptVec, String name) {
	String filename = name + ".txt";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	if(ptVec.size()==0)
		cout << name << " Write Sequence to File failed!" << endl;

	else
		for(unsigned int i=0; i<ptVec.size(); i++) {
			fprintf(fp,"(%d,%d)\n",ptVec.at(i).x, ptVec.at(i).y);
		}
	fclose(fp);
}

void writeSeq2File(double * arr, int length, String name) {
	String filename = name + ".txt";
	FILE * fp;
	fp = fopen(filename.c_str(), "w");
	for(int i=0; i<length; i++) {
		fprintf(fp,"%d: %f\n",i,arr[i]);
	}
	fclose(fp);
}

void writeSeq2File(Mat src, String name) {
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			fprintf(fp,"%d\n",src.at<uchar>(i,j));
		}
	}
	fclose(fp);
}

bool doesFileExist(String filename) {
	fstream fs(filename.c_str());
	if(fs.is_open()) {
		fs.close();
		return true;
	}
	return false;
}
