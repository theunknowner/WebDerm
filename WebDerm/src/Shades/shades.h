/*
 * shades.h
 *
 *  Created on: Aug 7, 2014
 *      Author: jason
 */

#ifndef SHADES_H_INCLUDED
#define SHADES_H_INCLUDED

#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"

extern deque<String> g_Shades;
extern deque< deque<double> > g_ShadeThresh;

class Shades {
public:
	bool importThresholds();
	int getShadeCount();
	String getShade(int index);
	int getShadeIndex(String shade);
	int extractShadeLevel(String shade);
	void release_memory();
};

#endif /* SHADES_H_ */
