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

String specialRules(String pix, int r, int g, int b);
bool specialRules(String &pix, double &indexChange,String &shade, String &shadePrev, double &darkness);
#endif /* RULES_H_ */
