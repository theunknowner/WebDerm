/*
 * shapematch.h
 *
 *  Created on: Jun 16, 2015
 *      Author: jason
 */

#ifndef SHAPEMATCH_H_
#define SHAPEMATCH_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "../ShadeShape/shadeshape.h"

class ShadeShape;
class ShapeMatch {
private:
	vector<float> prop_bins;
public:
	float match(ShadeShape upSS, ShadeShape dbSS);
};

#endif /* SHAPEMATCH_H_ */
