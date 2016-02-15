/*
 * rules.h
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED

class FileData;

#include "../global.h"

double rule1(FileData &fd, String &newShade);
double rule3(FileData &fd, String &newPix, String &newShade);
double rule5(FileData &fd, String &newPix, String &newShade);
double rule5(FileData &fd);

bool specialRules(FileData &fd, String &pix, double &indexChange, String &shade,
					int ratioLoc, int loc, deque<double> &ruleNo);

void testRules();

#endif /* RULES_H_ */
