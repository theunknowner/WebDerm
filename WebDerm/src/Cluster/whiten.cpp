/*
 * whiten.cpp
 *
 *  Created on: Jul 7, 2015
 *      Author: jason
 */

#include "cluster.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/stddev.h"

//! Normalize a group of samples(rows) on a per feature(column) basis.
/* Each feature(column) is divided by its standard deviation across all
 * samples(row) to give it unit variance.
 */
vector<double> Cluster::whiten(vector<double> &data_vec) {
	if(data_vec.size()==0)
		return data_vec;

	vector<double> data_vec2;
	double stdev = Algos::standardDev(data_vec);
	for(unsigned int i=0; i<data_vec.size(); i++) {
		double val = data_vec.at(i)/stdev;
		data_vec2.push_back(val);
	}
	return data_vec2;
}
