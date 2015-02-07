/*
 * poly.h
 *
 *  Created on: Feb 6, 2015
 *      Author: jason
 */

#ifndef POLY_H_
#define POLY_H_

#ifdef BOOST_UBLAS_TYPE_CHECK
#	undef BOOST_UBLAS_TYPE_CHECK
#endif
#define BOOST_UBLAS_TYPE_CHECK 0
#ifndef _USE_MATH_DEFINES
#	define _USE_MATH_DEFINES
#endif

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <stdexcept>

#include "/home/jason/git/WebDerm/WebDerm/headers/global.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"


class Poly {
public:
	vector<double> polyfit(const vector<double>& oX, const vector<double>& oY, int nDegree );
	vector<double> polyval(const vector<double>& oCoeff, const vector<double>& oX );

	void loadVectorFile(String path, vector<double> &vec);
};

#endif /* POLY_H_ */
