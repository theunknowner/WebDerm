/*
 * state.h
 *
 *  Created on: Aug 21, 2015
 *      Author: jason
 */

#ifndef STATE_H_
#define STATE_H_

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"

class State {
private:
	vector<String> STATE = {"ENTERED", "INSIDE", "OUTSIDE", "EXITED"};
	int state;
	vector<int> countStateVec;
public:
	enum {ENTERED, INSIDE, OUTSIDE, EXITED};
	State();
	State(int state);
	int currentState();
	void setState(int state);
	int countStateAction(int state);
	void reset();
};

#endif /* STATE_H_ */
