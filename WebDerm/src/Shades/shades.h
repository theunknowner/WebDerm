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

extern deque<String> g_Shades2;
extern deque< deque<double> > g_ShadeThresh2;

class Shades {
public:
	bool importThresholds();
	int getShadeCount();
	String getShade(int index);
	int getShadeIndex(String shade);
	int extractShadeLevel(String shade);
	String extractShade(String pix);
	String calcShade(double intensity);
	void release_memory();
	int shadeDifference(String shade1, String shade2);

	int getShadeIndex2(String shade);
	String getShade2(int index);
};

#endif /* SHADES_H_ */
