/*
 * rules.h
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#ifndef RULES_H_
#define RULES_H_


#include "/home/jason/git/WebDerm/WebDerm/headers/functions2.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "intensity.h"

String init_specialRules(String pix, int r, int g, int b);
bool specialRules(Mat &img, String &pix, deque< deque<String> > &windowVec, double &indexChange,
					String &shade, String &shadePrev, Point pt, int loc,
					deque<int> &ruleNo, deque< deque<String> > &hslMat,
					deque< deque<String> > &colorVec);
#endif /* RULES_H_ */
