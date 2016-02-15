/*
 * save_graph.cpp
 *
 *  Created on: May 19, 2015
 *      Author: jason
 */

#include "kneecurve.h"
#include "../Algorithms/write.h"

void KneeCurve::save_graph(String out, vector<double> &vec) {
	Write::writeToCsv(vec,out);
}

void KneeCurve::save_graph(String out, deque<double> &vec) {
	Write::writeToCsv(vec,out);
}

void KneeCurve::save_graph(String out, vector<int> &vec) {
	Write::writeToCsv(vec,out);
}
