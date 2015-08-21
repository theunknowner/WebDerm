/*
 * state.cpp
 *
 *  Created on: Aug 21, 2015
 *      Author: jason
 */

#include "state.h"

//! basic constructor
State::State() {
	this->state = State::OUTSIDE;
	this->countStateVec.resize(this->STATE.size(),0);
}

//! constructor with initial state
State::State(int state) {
	this->state = state;
	this->countStateVec.resize(this->STATE.size(),0);
}

//! returns the current state
int State::currentState() {
	return this->state;
}

//! sets the current state
void State::setState(int state) {
	this->state = state;
	this->countStateVec.at(state)++;
}

//! returns number of times it went through specified state
int State::countStateAction(int state) {
	return this->countStateVec.at(state);
}

//! reset
void State::reset() {
	this->state = State::OUTSIDE;
	this->countStateVec.clear();
	this->countStateVec.resize(this->STATE.size(),0);
}

