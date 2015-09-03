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
	this->time.restart();
}

void Timer::end() {
	this->seconds = round(this->time.elapsed());
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
vector<int> Timer::getTime() {
	vector<int> time = {this->hours,this->minutes,this->seconds};
	return time;
}

String Timer::getTimeString() {
	char text[100];
	sprintf(text,"Time Elapsed: %dh:%dm:%ds\n",this->hours,this->minutes,this->seconds);
	String timeStr(text);
	return timeStr;
}

void Timer::printTimer() {
	printf("Time Elapsed: %dh:%dm:%ds\n",this->hours,this->minutes,this->seconds);
}
