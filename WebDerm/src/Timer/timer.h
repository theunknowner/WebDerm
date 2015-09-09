/*
 * timer.h
 *
 *  Created on: Aug 27, 2015
 *      Author: jason
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include <time.h>

class Timer {
private:
	boost::timer btimer;
	int hours=0,minutes=0,seconds=0;
	String start_time, end_time;
public:
	Timer();
	void begin();
	void end();
	vector<int> getTimer();
	String getTimerString();
	void printTimer();

	String getStartTime();
	String getEndTime();
	String getCurrentTime();
	void printCurrentTime();
};

#endif /* TIMER_H_ */
