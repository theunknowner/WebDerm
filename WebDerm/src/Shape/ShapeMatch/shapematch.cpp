/*
 * shapematch.cpp
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#include "shapematch.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "../ShadeShape/shadeshape.h"

/******************* PRIVATE FUNCTIONS ******************/

//! sorts the islands of a feature by area size
void ShapeMatch::areaSort(ShadeShape &ss) {
	//for(int i=0 i<)
}

/******************* PUBLIC FUNCTIONS ******************/

float ShapeMatch::match(ShadeShape upSS, ShadeShape dbSS) {
	for(int i=0; i<dbSS.numOfFeatures(); i++) {
		for(int j=0; j<dbSS.feature(i).numOfIslands(); j++) {

		}
	}
}
