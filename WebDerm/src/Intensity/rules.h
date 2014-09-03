/*
 * rules.h
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED


#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "intensity.h"

class FileData;
double rule3(FileData &fd, String &newPix, String &newShade);

bool specialRules(FileData &fd, String &pix, double &indexChange, String &shade,
					int ratioLoc, int loc, deque<double> &ruleNo);

void testRule(Point pt);
#endif /* RULES_H_ */
