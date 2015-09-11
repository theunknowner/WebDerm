/*
 * timer.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: jason
 */

#include "timer.h"

Timer::Timer() {
	this->begin();
}

void Timer::begin() {
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	this->start_time = std::string(asctime(timeinfo));

	this->btimer.restart();
}

void Timer::end() {
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	this->end_time = std::string(asctime(timeinfo));

	this->seconds = round(this->btimer.elapsed());
	if(this->seconds>=60) {
		this->minutes = floor(this->seconds/60);
		this->seconds %= 60;
		if(this->minutes>=60) {
			this->hours = floor(this->minutes/60);
			this->minutes %= 60;
		}
	}
}

//returns time in vector form of {0=hours,1=minutes,2=seconds}
vector<int> Timer::getTimer() {
	vector<int> timeVec = {this->hours,this->minutes,this->seconds};
	return timeVec;
}

String Timer::getTimerString() {
	char text[100];
	sprintf(text,"Time Elapsed: %dh:%dm:%ds",this->hours,this->minutes,this->seconds);
	String timeStr(text);
	return timeStr;
}

void Timer::printTimer() {
	printf("Time Elapsed: %dh:%dm:%ds\n",this->hours,this->minutes,this->seconds);
}

//! has "\n" already
String Timer::getStartTime() {
	return this->start_time;
}

//! has "\n" already
String Timer::getEndTime() {
	return this->end_time;
}

//! has "\n" already
String Timer::getCurrentTime() {
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	return std::string(asctime(timeinfo));
}

void Timer::printCurrentTime() {
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);
	printf ("Current local time and date: %s", asctime(timeinfo));
}
