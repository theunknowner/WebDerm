/*
 * ssheet.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: jason
 */

#include "ssheet.h"

String minFormulaR1C1(int begin, int end) {
	String min = "=MIN(R[-" + toString(end) + "]C:R[-" + toString(begin) + "]C)";
	return min;
}

String maxFormulaR1C1(int begin, int end) {
	String max = "=MAX(R[-" + toString(end) + "]C:R[-" + toString(begin) + "]C)";
	return max;
}
