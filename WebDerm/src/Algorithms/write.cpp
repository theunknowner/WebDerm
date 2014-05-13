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
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"[%s],", vec.at(i).at(j).c_str());
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(deque< deque<String> > &vec, String pathname, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%s,", vec.at(i).at(j).c_str());
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(deque< deque<int> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%d,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(deque< deque<double> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%f,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(deque<double> &vec, String name) {
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++) {
		fprintf(fp,"%f,",vec.at(i));
	}
}

void writeSeq2File(deque< deque<long double> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%f,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(deque<Point> &ptVec, String name) {
	String filename = name + ".txt";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<ptVec.size(); i++) {
		fprintf(fp,"(%d,%d)\n",ptVec.at(i).x, ptVec.at(i).y);
	}
}

void writeSeq2File(double * arr, int length, String name) {
	String filename = name + ".txt";
	FILE * fp;
	fp = fopen(filename.c_str(), "w");
	for(int i=0; i<length; i++) {
		fprintf(fp,"%d: %d\n",i,arr[i]);
	}
}
