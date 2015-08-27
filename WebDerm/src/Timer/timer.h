/*
 * timer.h
 *
 *  Created on: Aug 27, 2015
 *      Author: jason
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class Timer {
private:
	boost::timer time;
	int hours=0,minutes=0,seconds=0;
public:
	Timer();
	void begin();
	void end();
	vector<int> getTime();
	void printTimer();
};

#endif /* TIMER_H_ */
