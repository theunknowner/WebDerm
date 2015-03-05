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
#include "/home/jason/git/WebDerm/WebDerm/src/Matlab/matlab.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "matrix.h"
#include "givensQR.h"

class Poly {
public:
	vector<double> polyfit(const vector<double>& oX, const vector<double>& oY, int nDegree );
	vector<double> polyval(const vector<double>& oCoeff, const vector<double>& oX );
	vector<double> polyder(vector<double> &p);

	int lineIntersect(vector<double> &vec);

	void loadVectorFile(String path, vector<double> &vec);

	void findExtremas(vector<double> vec, vector<double> &xmax, vector<int> &imax, vector<double> &xmin, vector<int> &imin);
};

#endif /* POLY_H_ */
