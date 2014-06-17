/*
 * table.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/table.h"

void generateTable(FILE *fp, deque< deque<String> > &data) {
	for(unsigned int i=0; i<data.size(); i++) {
		for(unsigned int j=0; j<data.at(i).size(); j++) {
			fprintf(fp,"%s,",data.at(i).at(j).c_str());
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}

void generateTable(FILE *fp, deque<String> &data) {
	for(unsigned int i=0; i<data.size(); i++) {
		fprintf(fp,"%s,",data.at(i).c_str());
	}
	fprintf(fp,"\n");
}
