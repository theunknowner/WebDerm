/*
 * poly.cpp
 *
 *  Created on: Feb 6, 2015
 *      Author: jason
 */

#include "poly.h"

/*
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
}*/

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

void Poly::findExtremas(vector<double> vec, vector<double> &xmax, vector<int> &imax, vector<double> &xmin, vector<int> &imin) {
	Matlab mb;
	unsigned int numOfElem = vec.size();

	//Check for NaNs - but isnan() is broken so assume vec is all numbers
	vector<int> inans(vec.size(),0);
	vector<int> indx;
	for(unsigned int i=0; i<numOfElem; i++)
		indx.push_back(i);
	if(countNonZero(inans)>0) {
		for(unsigned int i=0; i<inans.size(); i++) {
			if(inans.at(i)>0) {
				indx.erase(indx.begin()+inans.at(i));
				vec.erase(vec.begin()+inans.at(i));
			}
		}
		numOfElem = vec.size();
	}

	vector<double> dx = mb.diff(vec);
	//if the vec line is horizontal return and exit
	if(countNonZero(dx)==0) return;
	//Find flat peaks and put middle element if there is
	vector<int> a = mb.indexOfNonMatchElem(dx,0.0);
	vector<int> lm = mb.indexOfNonMatchElem(mb.diff(a),1,1);
	vector<int> d;
	for(unsigned int i=0; i<lm.size();i++) {
		int val = a.at(lm.at(i)) - a.at(lm.at(i)-1);
		d.push_back(val);
	}
	for(unsigned int i=0; i<lm.size(); i++) {
		a.at(lm.at(i)) -= floor(d.at(i)/2);
	}
	a.push_back(numOfElem-1);

	//! Peaks?
	vector<double> xa;
	for(unsigned int i=0; i<a.size(); i++) {
		double val = vec.at(a.at(i));
		xa.push_back(val);
	}
	vector<int> b;
	vector<double> xaDiff = mb.diff(xa);
	for(unsigned int i=0; i<xaDiff.size(); i++) {
		int val = xaDiff.at(i)>0 ? 1 : 0;
		b.push_back(val);
	}
	//writeSeq2File(xa,"xa");
	vector<int> xb = mb.diff(b);
	imax = mb.indexOfMatchElem(xb,-1,1);
	imin = mb.indexOfMatchElem(xb,1,1);
	for(unsigned int i=0; i<imax.size(); i++) {
		imax.at(i) = a.at(imax.at(i));
	}
	for(unsigned int i=0; i<imin.size(); i++) {
		imin.at(i) = a.at(imin.at(i));
	}
	unsigned int imaxSize = imax.size();
	unsigned int iminSize = imin.size();

	//Maximum or minumim on a flat peak at the ends?
	if(imaxSize==0 && iminSize==0) {
		if(vec.at(0)>vec.at(numOfElem-1)) {
			xmax.clear();
			imax.clear();
			xmin.clear();
			imin.clear();
			xmax.push_back(vec.at(0));
			imax.push_back(indx.at(0));
			xmin.push_back(vec.at(numOfElem-1));
			imin.push_back(indx.at(numOfElem-1));
		}
		else if(vec.at(0)>vec.at(numOfElem-1)) {
			xmax.clear();
			imax.clear();
			xmin.clear();
			imin.clear();
			xmax.push_back(vec.at(numOfElem-1));
			imax.push_back(indx.at(numOfElem-1));
			xmin.push_back(vec.at(0));
			imin.push_back(indx.at(0));
		}
		return;
	}

	//Maximum or minumim at the ends?
	if(imaxSize==0) {
		imax.push_back(0);
		imax.push_back(numOfElem-1);
	}
	else if(iminSize==0) {
		imin.push_back(0);
		imin.push_back(numOfElem-1);
	}
	else {
		if(imax.front() < imin.front()) {
			vector<int> temp = imin;
			imin.resize(iminSize+1,0);
			for(int i=1; i<iminSize+1; i++) {
				imin.at(i) = temp.at(i-1);
			}
			imin.at(0) = 0;
		}
		else {
			vector<int> temp = imax;
			imax.resize(imaxSize+1,0);
			for(int i=1; i<imaxSize+1; i++) {
				imax.at(i) = temp.at(i-1);
			}
			imax.at(0) = 0;
		}
		if(imax.end() > imin.end())
			imin.push_back(numOfElem-1);
		else
			imax.push_back(numOfElem-1);
	}
	xmax.clear();
	xmin.clear();
	for(unsigned int i=0; i<imax.size(); i++) {
		xmax.push_back(vec.at(imax.at(i)));
	}
	for(unsigned int i=0; i<imin.size(); i++) {
		xmin.push_back(vec.at(imin.at(i)));
	}

	//NaN's
	if(countNonZero(inans)>0) {
		for(int i=0; i<imax.size(); i++) {
			imax.at(i) = indx.at(imax.at(i));
		}
		for(int i=0; i<imin.size(); i++) {
			imin.at(i) = indx.at(imin.at(i));
		}
	}
}
