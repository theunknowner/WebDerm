/*
 * intensity.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#include "intensity.h"

double Intensity::calcIntensity(String color) {
	rgb rgb;
	double colorLevel[mainColors.size()];
	double cHue[mainColors.size()];
	double colorIntensity[mainColors.size()];
	fill_n(colorLevel,mainColors.size(),0);
	fill_n(cHue,mainColors.size(),0);
	fill_n(colorIntensity,mainColors.size(),0);
	double totalLevel=0;
	double totalColorIntensity=0;
	vector<int> index;
	vector<double> vec,vec2;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		if(color.find(mainColors.at(i))!=string::npos) {
			colorLevel[i] = rgb.getColorLevel(color,mainColors.at(i));
			index.push_back(i);
			if(mainColors.at(i)!="Gray") {
				totalLevel+=colorLevel[i];
			}
			cHue[i] = colorFactors.at(i);
		}
	}
	for(unsigned int i=0; i<index.size(); i++) {
		colorIntensity[index.at(i)] = colorLevel[index.at(i)] * cHue[index.at(i)];
		if(mainColors.at(index.at(i))!="Gray") {
			colorIntensity[index.at(i)] *= (colorLevel[index.at(i)]/totalLevel);
		}
		totalColorIntensity += colorIntensity[index.at(i)];
	}
	return totalColorIntensity;
}
static double minIntensity = 900;
static double maxIntensity = 0;
static double range=0;
void Intensity::setMinMax(double intensity) {
	if(intensity>maxIntensity && intensity!=900)
		maxIntensity = intensity;
	if(intensity<minIntensity)
		minIntensity = intensity;

	range = maxIntensity-minIntensity;
}

const int shadeCount=5;
String Intensity::getShade(int index) {
	String shadeArr[] = {"White","Light","","Dark","Black"};
	int ind=index;
	if(ind<0) ind=0;
	if(ind>(shadeCount-1)) ind=(shadeCount-1);
	return shadeArr[ind];
}

int Intensity::getShadeIndex(String shade) {
	int index=0;
	for(int i=0; i<shadeCount; i++) {
		if(shade==getShade(i)) {
			index=i;
			break;
		}
	}
	return index;
}

String Intensity::calcShade(double inten) {
	String shade;
	double thresh1 = 0.25;
	double thresh2 = 0.75;
	double intensity = (inten-minIntensity)/range;
	intensity = pow(intensity,power);
	if(intensity<=thresh1)
		shade = "Light";
	if(intensity>thresh1 && intensity<=thresh2)
		shade = "";
	if(intensity>thresh2)
		shade = "Dark";

	return shade;
}

vector< vector<double> > Intensity::calcNormalizedIntensityMatrix(vector< vector<double> > vec) {
	double intensity=0;
	vector<double> iVec1;
	vector< vector<double> > iVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			intensity = (vec.at(i).at(j)-minIntensity)/range;
			intensity = pow(intensity,power);
			iVec1.push_back(intensity);
		}
		iVec2.push_back(iVec1);
		iVec1.clear();
	}
	return iVec2;
}

vector< vector<double> > Intensity::calcIntensityMatrix(vector <vector<String> > &windowVec) {
	double colorIntensity=0;
	String color;
	vector<double> vec;
	vector< vector<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<windowVec.at(i).size(); j++) {
			color = windowVec.at(i).at(j);
			colorIntensity = calcIntensity(color);
			setMinMax(colorIntensity);
			vec.push_back(colorIntensity);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	return vec2;
}

vector< vector<String> > Intensity::calcMainColorMatrix(vector< vector<String> > &windowVec) {
	contrast con;
	Color c;
	int flag=0,change=0;
	double threshold = 0.1;
	String pix, shade, shadePrev;
	double indexChange=0, ccCurr=0, ccPrev=0,shadePrevIndex=0, shadeCurrIndex=0;
	double contrast=0;
	vector< vector<double> > intensityVec;
	vector< vector<double> > normIntensityVec;
	vector< vector<double> > contrastVec;
	vector< vector<double> > cumConVec;
	vector< vector<String> > colorVec2;
	vector<String> colorVec1;
	intensityVec = calcIntensityMatrix(windowVec);
	normIntensityVec = calcNormalizedIntensityMatrix(intensityVec);
	contrastVec = con.calcContrastFromMatrix(normIntensityVec);
	cumConVec = con.calcCumulativeContrast(contrastVec);
	for(unsigned int i=0; i<cumConVec.size(); i++) {
		for(unsigned int j=0; j<cumConVec.at(i).size(); j++) {
			pix = windowVec.at(i).at(j);
			pix = c.getMainColor(pix);
			if(flag==0 && pix!="Black") {
				shadePrev = calcShade(intensityVec.at(i).at(j));
				shadePrevIndex = getShadeIndex(shadePrev);
				ccPrev = cumConVec.at(i).at(j)/threshold;
				pix = shadePrev + pix;
				++flag;
			}
			else if(flag!=0) {
				if(intensityVec.at(i).at(j)==intensityVec.at(i).at(j-1)) {
					shade = shadePrev;
					shadeCurrIndex = shadePrevIndex;
				}
				else {
					if(change==0) {
						contrast = contrastVec.at(i).at(j);
						if(contrast>0) ccPrev = 0.01;
						if(contrast<0) ccPrev = -0.01;
						change=1;
					}
					ccCurr = cumConVec.at(i).at(j)/threshold;
					indexChange = floor(ccCurr) - floor(ccPrev);
					shadeCurrIndex = shadePrevIndex + indexChange;
					shade = getShade(shadeCurrIndex);
				}
				if(pix!="Black" && pix!="White")
					pix = shade + pix;
			}
			colorVec1.push_back(pix);
		}
		flag=0;
		change=0;
		colorVec2.push_back(colorVec1);
		colorVec1.clear();
	}
	return colorVec2;
}

void Intensity::writeNormalizedIntensityMatrix(vector< vector<String> > &windowVec, String name) {
	vector< vector<double> > normVec;
	vector< vector<double> > intVec;
	intVec = calcIntensityMatrix(windowVec);
	normVec = calcNormalizedIntensityMatrix(intVec);
	String filename = name + "NormIntensity";
	writeSeq2File(normVec,name);
	vector< vector<double> >().swap(normVec);
	vector< vector<double> >().swap(intVec);
}

void Intensity::writeNormalizedIntensityMatrix(vector< vector<double> > &vec, String name) {
	vector< vector<double> > normVec;
	normVec = calcNormalizedIntensityMatrix(vec);
	String filename = name + "NormIntensity";
	writeSeq2File(normVec,filename);
	vector< vector<double> >().swap(normVec);
}

void Intensity::writeIntensityMatrix(vector< vector<String> > &windowVec, String name) {
	vector< vector<double> > intVec;
	intVec = calcIntensityMatrix(windowVec);
	String filename = name + "ColorIntensity";
	writeSeq2File(intVec,filename);

	/* write normalizeIntensity */
	vector< vector<double> > normVec;
	normVec = calcNormalizedIntensityMatrix(intVec);
	filename = name + "NormIntensity";
	writeSeq2File(normVec,filename);

	/* write contrast */
	contrast con;
	vector< vector<double> > conVec;
	conVec = con.calcContrastFromMatrix(normVec);
	filename = name + "ContrastNormIntensity";
	writeSeq2File(conVec,filename);

	/*write cumulative contrast */
	vector< vector<double> > cumConVec;
	cumConVec = con.calcCumulativeContrast(conVec);
	filename = name + "CumulContrast";
	writeSeq2File(cumConVec,filename);
	vector< vector<double> >().swap(cumConVec);
	/* end write cumulative contrast */

	vector< vector<double> >().swap(conVec);
	/* end write contrast */

	vector< vector<double> >().swap(normVec);
	/* end write normalizedIntensity */

	vector< vector<double> >().swap(intVec);
}

void Intensity::writeMainColorMatrix(vector< vector<String> > &windowVec, String name) {
	vector< vector<String> > colorVec;
	colorVec = calcMainColorMatrix(windowVec);
	String filename = name + "MainColors";
	writeSeq2File(colorVec,filename);
	vector< vector<String> >().swap(colorVec);
}
