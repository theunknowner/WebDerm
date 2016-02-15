/*
 * shades.h
 *
 *  Created on: Aug 7, 2014
 *      Author: jason
 */

#ifndef SHADES_H_INCLUDED
#define SHADES_H_INCLUDED

#include "../global.h"
#include "peakcluster.h"

class Shades: public PeakCluster {
public:
	static deque<String> g_Shades;
	static deque< deque<double> > g_ShadeThresh;

	static deque<String> g_Shades2;
	static deque< deque<double> > g_ShadeThresh2;
	static bool THRESH_IMPORTED;

	Shades();
	bool importThresholds();
	int getShadeCount();
	String getShade(int index);
	int getShadeIndex(String shade);
	int extractShadeLevel(String shade);
	String extractShade(String pix);
	String calcShade(double intensity);
	String calcShade2(double intensity);
	void release_memory();
	int shadeDifference(String shade1, String shade2);

	int getShadeIndex2(String shade);
	String getShade2(int index);

	String combineShades(String shade);

};

#endif /* SHADES_H_ */
