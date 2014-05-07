/*
 * write.cpp
 *
 *  Created on: May 7, 2014
 *      Author: jason
 */

#include "write.h"

void writeSeq2File(vector< vector<String> > &vec, String name)
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

void writeSeq2File(vector< vector<String> > &vec, String pathname, String name)
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

void writeSeq2File(vector< vector<int> > &vec, String name)
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

void writeSeq2File(vector< vector<double> > &vec, String name)
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

void writeSeq2File(vector<double> &vec, String name) {
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++) {
		fprintf(fp,"%f,",vec.at(i));
	}
}

void writeSeq2File(vector< vector<long double> > &vec, String name)
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
