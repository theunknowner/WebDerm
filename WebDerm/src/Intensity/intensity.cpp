/*
 * intensity.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#include "intensity.h"

static double minIntensity = 900;
static double maxIntensity = 0;
static double range=0;

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
	totalColorIntensity = round(totalColorIntensity);
	return totalColorIntensity;
}

double Intensity::sigmoidFn(double intensity) {
	double x=intensity;
	const int a=1,b=1,c=22028;
	const double k=5;
	double result=0;
	result = (a*exp(k*x)-(a*exp(-k*x))+(2*b*x)+c);
	return result;
}



inline void _setMinMax(double intensity) {
	if(intensity>maxIntensity && intensity!=900)
		maxIntensity = intensity;
	if(intensity<minIntensity)
		minIntensity = intensity;

	range = maxIntensity-minIntensity;
}

void Intensity::setMinMax(vector< vector<double> > &input) {
	deque<double> vec;
	double intensity=0;
	double thresh = 0.01;
	for(unsigned int i=0; i<input.size(); i++) {
		for(unsigned int j=0; j<input.at(i).size(); j++) {
			intensity = input.at(i).at(j);
			if(intensity!=900)
				vec.push_back(intensity);
		}
	}
	quicksort(vec,0,vec.size()-1);
	double t = vec.size()*thresh;
	t = round(t);
	for(int i=0; i<t; i++) {
		vec.pop_back();
		vec.pop_front();
	}
	for(unsigned int i=0; i<vec.size(); i++) {
		_setMinMax(vec.at(i));
	}
}

unsigned int shadeCount=0;
String shadeArr[] = {"Light","Low","High","Dark"};
String Intensity::getShade(int index) {
	//String shadeArr[] = {"White","White","White","Light","Light","Light",
	//		"","","","Dark","Dark","Dark","Black","Black","Black"};
	//String shadeArr[] = {"White","Light","","Dark","Black"};
	shadeCount = length(shadeArr);
	unsigned int ind=index;
	if(ind<0) ind=0;
	if(ind>(shadeCount-1)) ind=(shadeCount-1);
	return shadeArr[ind];
}

int Intensity::getShadeIndex(String shade) {
	unsigned int index=0;
	shadeCount = length(shadeArr);
	for(unsigned int i=0; i<shadeCount; i++) {
		if(shade==getShade(i)) {
			index=i;
			break;
		}
	}
	return index;
}

String Intensity::calcShade(double inten) {
	String shade;
	double intensity=inten;
	double thresh1 = 0.20;
	double thresh2 = 0.755;
	if(inten>1) {
		intensity = (inten-minIntensity)/range;
		intensity = pow(intensity,power);
	}
	if(intensity<=thresh1)
		shade = "Light";
	if(intensity>thresh1 && intensity<=thresh2)
		shade = "";
	if(intensity>thresh2)
		shade = "Dark";

	return shade;
}

vector< vector<double> > Intensity::calcNormalizedIntensityMatrix(vector< vector<double> > &vec) {
	double intensity=0;
	double sigMin = sigmoidFn(minRange);
	double sigMax = sigmoidFn(maxRange);
	double sigRange = sigMax-sigMin;
	vector<double> iVec1;
	vector< vector<double> > iVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			intensity = vec.at(i).at(j);
			//convert to %
			intensity = (intensity-minIntensity)/range;
			//normalize between minRange:maxRange
			intensity = normalizeToRange(intensity,minRange,maxRange);
			//Differential Inverse Sigmoid Function
			if(intensity>maxRange) intensity = maxRange+0.1;
			if(intensity<minRange) intensity = minRange-0.1;
			intensity = sigmoidFn(intensity);
			//normalize to %
			intensity = (intensity-sigMin)/sigRange;
			//intensity /=sigmoidFn(maxRange);
			//intensity /= 20;
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
			vec.push_back(colorIntensity);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	setMinMax(vec2);
	printf("%f;%f\n",minIntensity,maxIntensity);
	return vec2;
}

vector< vector<String> > Intensity::calcMainColorMatrix(vector< vector<String> > &windowVec) {
	Color c;
	contrast con;
	int flag=0;
	double threshold = 0.2; //using round
	unsigned int localScanSize=10;
	String pix, shade;
	double indexChange=0, ccCurr=0, ccPrev=0, localCC=0;
	int shadeIndex=0, localMinIndex=0, localMaxIndex=0, localIndex=0;
	double localMinCC=0, localMaxCC=0;
	vector< vector<double> > intensityVec;
	vector< vector<double> > normIntensityVec;
	vector< vector<double> > contrastVec;
	vector< vector<double> > cumConVec;
	vector< vector<String> > colorVec2;
	vector<String> colorVec1;
	deque<double> localIndexes;
	deque<double> localCCs;
	intensityVec = calcIntensityMatrix(windowVec);
	normIntensityVec = calcNormalizedIntensityMatrix(intensityVec);
	contrastVec = con.calcContrastFromMatrix(normIntensityVec);
	cumConVec = con.calcCumulativeContrast(contrastVec);
	for(unsigned int i=0; i<cumConVec.size(); i++) {
		for(unsigned int j=0; j<cumConVec.at(i).size(); j++) {
			pix = windowVec.at(i).at(j);
			pix = c.getMainColor(pix);
			ccCurr = cumConVec.at(i).at(j);
			if(flag==0 && pix!="Black") { //initial first pixel-area
				//shade = calcShade(intensityVec.at(i).at(j));
				shade = "High";
				shadeIndex = getShadeIndex(shade);
				shadeIndex = 2;
				localCC = ccCurr;
				localIndex = shadeIndex;
				flag=1;
			}
			else if(flag!=0) {
				ccPrev = cumConVec.at(i).at(j-1);
				if((ccCurr-ccPrev)<0) {
					localCC = localMaxCC;
					localIndex = localMaxIndex;
				}
				if((ccCurr-ccPrev)>0) {
					localCC = localMinCC;
					localIndex = localMinIndex;
				}
				indexChange = round((ccCurr-localCC)/threshold);
				shadeIndex = localIndex + indexChange;
				shade = getShade(shadeIndex);
			}
			if(pix!="Black") {
				if(localIndexes.size()==localScanSize) localIndexes.pop_front();
				if(localCCs.size()==localScanSize) localCCs.pop_front();
				localIndexes.push_back(shadeIndex);
				localCCs.push_back(ccCurr);
				int index = distance(localCCs.begin(),max_element(localCCs.begin(),localCCs.end()));
				localMaxCC = localCCs.at(index);
				localMaxIndex = localIndexes.at(index);
				index = distance(localCCs.begin(),min_element(localCCs.begin(),localCCs.end()));
				localMinCC = localCCs.at(index);
				localMinIndex = localIndexes.at(index);
			}
			if(normIntensityVec.at(i).at(j)<0) shade = "White";
			if(normIntensityVec.at(i).at(j)>1) shade = "Black";
			if(pix!="Black" && pix!="White")
				pix = shade + pix + toString(shadeIndex);
			colorVec1.push_back(pix);
		}
		flag=0;
		localIndexes.clear();
		localCCs.clear();
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
