/*
 * cielab.cpp
 *
 *  Created on: Mar 16, 2015
 *      Author: jason
 */

#include "cielab.h"

vector<float> CieLab::xyz2lab(float x, float y, float z) {
	//Observer= 2°, Illuminant= D65
	float ref_X =  95.047;
	float ref_Y = 100.000;
	float ref_Z = 108.883;
	double X = x/ref_X;
	double Y = y/ref_Y;
	double Z = z/ref_Z;

	if ( X > 0.008856 ) X = pow(X,1/3.);
	else X = ( 7.787 * X) + (16.0 / 116.0);

	if ( Y > 0.008856 ) Y = pow(Y,1/3.);
	else Y = ( 7.787 * Y) + (16. / 116.);

	if ( Z > 0.008856 ) Z = pow(Z, 1/3.);
	else Z = ( 7.787 * Z) + (16. / 116.);

	vector<float> labVec(3,0);
	labVec.at(0) = 116 * Y - 16;
	labVec.at(1) = 500 * (X-Y);
	labVec.at(2) = 200 * (Y-Z);

	return labVec;
}

vector<float> CieLab::lab2xyz(float l, float a, float b) {
	float Y = ( l + 16 ) / 116.;
	float X = a / 500. + Y;
	float Z = Y - b / 200.;

	if ( pow(Y,3) > 0.008856 ) Y = pow(Y,3);
	else Y = (Y - 16 / 116. ) / 7.787;
	if ( pow(X,3) > 0.008856 ) X = pow(X,3);
	else X = (X - 16 / 116. ) / 7.787;
	if ( pow(Z,3) > 0.008856 ) Z = pow(Z,3);
	else Z = ( Z - 16 / 116. ) / 7.787;

	//Observer= 2°, Illuminant= D65
	float ref_X =  95.047;
	float ref_Y = 100.000;
	float ref_Z = 108.883;
	X *= ref_X;
	Y *= ref_Y;
	Z *= ref_Z;
	vector<float> xyzVec(3,0);
	xyzVec.at(0) = X;
	xyzVec.at(1) = Y;
	xyzVec.at(2) = Z;

	return xyzVec;
}
