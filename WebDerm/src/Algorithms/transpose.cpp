/*
 * transpose.cpp
 *
 *  Created on: Feb 20, 2015
 *      Author: jason
 */

#include "transpose.h"

void transpose(deque<deque<double> > &vec) {
	deque<deque<double> > _vec(vec.at(0).size(),deque<double>(vec.size()));
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(0).size(); j++) {
			_vec.at(j).at(i) = vec.at(i).at(j);
		}
	}
	vec = _vec;
}
