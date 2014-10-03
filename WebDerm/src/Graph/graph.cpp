/*
 * graph.cpp
 *
 *  Created on: Sep 30, 2014
 *      Author: jason
 */

#include "graph.h"

void Graph::graph(deque< deque<double> > matrix, deque<String> xLabel, deque<String> yLabel, String filename)  {
	FILE * fp;
	filename += ".csv";
	fp = fopen(filename.c_str(),"w");
	if(xLabel.size()!=0)  {
		for(unsigned int i=0; i<xLabel.size(); i++)  {
			fprintf(fp,",%s",xLabel.at(i).c_str());
		}
		fprintf(fp,"\n");
	}
	for(unsigned int i=0; i<matrix.size(); i++)  {
		if(yLabel.size()!=0)
			fprintf(fp,"%s,",yLabel.at(i).c_str());
		for(unsigned int j=0; j<matrix.at(i).size(); j++)  {
			if(j<matrix.at(i).size()-1)
				fprintf(fp,"%f,", matrix.at(i).at(j));
			else
				fprintf(fp,"%f\n", matrix.at(i).at(j));
		}
	}
	fclose(fp);
}
