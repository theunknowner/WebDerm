/*
 * save_graph.cpp
 *
 *  Created on: May 19, 2015
 *      Author: jason
 */

#include "kneecurve.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"

void KneeCurve::save_graph(String out, vector<double> &vec) {
	Write::writeSeq2File(vec,out);
}
