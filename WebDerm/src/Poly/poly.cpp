/*
 * poly.cpp
 *
 *  Created on: Feb 6, 2015
 *      Author: jason
 */

#include "poly.h"

//! returns coeff in ascending power
vector<double> Poly::polyfit(const vector<double>& oX, const vector<double>& oY, int nDegree ) {
	using namespace boost::numeric::ublas;

	if ( oX.size() != oY.size() )
		throw invalid_argument( "X and Y vector sizes do not match" );

	// more intuative this way
	nDegree++;

	size_t nCount =  oX.size();
	matrix<double> oXMatrix( nCount, nDegree );
	matrix<double> oYMatrix( nCount, 1 );

	// copy y matrix
	for ( size_t i = 0; i < nCount; i++ )
	{
		oYMatrix(i, 0) = oY[i];
	}

	// create the X matrix
	for ( size_t nRow = 0; nRow < nCount; nRow++ )
	{
		double nVal = 1.0f;
		for ( int nCol = 0; nCol < nDegree; nCol++ )
		{
			oXMatrix(nRow, nCol) = nVal;
			nVal *= oX[nRow];
		}
	}

	// transpose X matrix
	matrix<double> oXtMatrix( trans(oXMatrix) );
	// multiply transposed X matrix with X matrix
	matrix<double> oXtXMatrix( prec_prod(oXtMatrix, oXMatrix) );
	// multiply transposed X matrix with Y matrix
	matrix<double> oXtYMatrix( prec_prod(oXtMatrix, oYMatrix) );

	// lu decomposition
	permutation_matrix<int> pert(oXtXMatrix.size1());
	const size_t singular = lu_factorize(oXtXMatrix, pert);
	// must be singular
	BOOST_ASSERT( singular == 0 );

	// backsubstitution
	lu_substitute(oXtXMatrix, pert, oXtYMatrix);

	// copy the result to coeff
	return std::vector<double>( oXtYMatrix.data().begin(), oXtYMatrix.data().end() );
}

vector<double> Poly::polyval( const vector<double>& oCoeff, const vector<double>& oX ) {
	size_t nCount =  oX.size();
	size_t nDegree = oCoeff.size();
	std::vector<double>	oY( nCount );

	for ( size_t i = 0; i < nCount; i++ )
	{
		double nY = 0;
		double nXT = 1;
		double nX = oX[i];
		for ( size_t j = 0; j < nDegree; j++ )
		{
			// multiply current x by a coefficient
			nY += oCoeff[j] * nXT;
			// power up the X
			nXT *= nX;
		}
		oY[i] = nY;
	}

	return oY;
}

//! returns derivative of polynomial
vector<double> Poly::polyder(vector<double> &p) {
	vector<double> pDeriv;
	int nDegree = p.size()-1;
	for(int i=0; i<=nDegree; i++) {
		double val = p.at(i)*i;
		if(val!=0)
			pDeriv.push_back(val);
	}
	return pDeriv;
}

//! check if a line from start-end point intersects the curve
//! if true->returns pos, if false->returns -1
int Poly::lineIntersect(vector<double> &vec) {
	vector<double> xVec;
	for(unsigned int i=0; i<vec.size(); i++) {
		xVec.push_back((double)i);
	}
	double slope = (vec.at(vec.size()-1)-vec.at(0))/(xVec.at(xVec.size()-1)-xVec.at(0));
	vector<double> lineVec(vec.size(),vec.at(0));
	for(unsigned int i=1; i<vec.size(); i++) {
		double val = slope*(xVec.at(i)-xVec.at(i-1))+lineVec.at(i-1);
		lineVec.at(i) = val;
	}
	for(unsigned int i=0; i<lineVec.size(); i++) {
		double val = round(lineVec.at(i));
		if(val>vec.at(i))
			return i;
	}

	return -1;
}

void Poly::loadVectorFile(String path, vector<double> &vec) {
	fstream fs(path.c_str());
	if(fs.is_open()) {
		String temp;
		while(getline(fs,temp)) {
			vec.push_back(atof(temp.c_str()));
		}
		fs.flush();
		fs.close();
	}
}
