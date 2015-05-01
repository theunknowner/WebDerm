
#include "poly.h"
#include "matrix.h"
#include "givensQR.h"

/*
		Finds the coefficients of a polynomial p(x) of degree n that fits the data,
		p(x(i)) to y(i), in a least squares sense. The result p is a row vector of
		length n+1 containing the polynomial coefficients in incremental powers.

		param:
			oX				x axis values
			oY				y axis values
			nDegree			polynomial degree including the constant

		return:
			coefficients of a polynomial starting at the constant coefficient and
			ending with the coefficient of power to nDegree. C++0x-compatible
			compilers make returning locally created vectors very efficient.

 */
std::vector<double> Poly::polyfit( const std::vector<double>& oX, const std::vector<double>& oY, int nDegree ) {
	if ( oX.size() != oY.size() )
		throw std::invalid_argument( "X and Y vector sizes do not match" );

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
	matrix<double> oXtMatrix( oXMatrix.transpose() );
	// multiply transposed X matrix with X matrix
	matrix<double> oXtXMatrix( oXtMatrix * oXMatrix );
	// multiply transposed X matrix with Y matrix
	matrix<double> oXtYMatrix( oXtMatrix * oYMatrix );

	Givens<double> oGivens;
	oGivens.Decompose( oXtXMatrix );
	matrix<double> oCoeff = oGivens.Solve( oXtYMatrix );
	// copy the result to coeff
	return oCoeff.data();
}

