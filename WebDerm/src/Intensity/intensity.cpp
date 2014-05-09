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

int shadeCount=0;
String shadeArr[] = {"Light","Low","High","Dark"};
String Intensity::getShade(int index) {
	//String shadeArr[] = {"White","White","White","Light","Light","Light",
	//		"","","","Dark","Dark","Dark","Black","Black","Black"};
	//String shadeArr[] = {"White","Light","","Dark","Black"};
	shadeCount = length(shadeArr);
	int ind=index;
	if(ind<0) ind=0;
	if(ind>(shadeCount-1)) ind=(shadeCount-1);
	return shadeArr[ind];
}

int Intensity::getShadeIndex(String shade) {
	unsigned int index=0;
	shadeCount = length(shadeArr);
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
	//double sigMin = sigmoidFn(minRange);
	//double sigMax = sigmoidFn(maxRange);
	//double sigRange = sigMax-sigMin;
	vector<double> iVec1;
	vector< vector<double> > iVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			intensity = vec.at(i).at(j);
			//convert to %
			intensity = (intensity-minIntensity)/range;
			//normalize between minRange:maxRange
			//intensity = normalizeToRange(intensity,minRange,maxRange);
			//Differential Inverse Sigmoid Function
			//if(intensity>maxRange) intensity = maxRange+0.1;
			//if(intensity<minRange) intensity = minRange-0.1;
			//intensity = sigmoidFn(intensity);
			//normalize to %
			//intensity = (intensity-sigMin)/sigRange;
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

vector< vector<String> > Intensity::calcMainColorMatrix(vector< vector<String> > &windowVec, String name) {
	Color c;
	contrast con;
	int flag=0;
	double threshold = 0.2; //using round
	//unsigned int localScanSize=7;
	String pix, shade, maxShade, minShade;
	double indexChange=0, ccCurr=0, localCC=0;
	int shadeIndex=0, localIndex=0, minShadeIndex=100, maxShadeIndex=-100;
	vector< vector<double> > intensityVec;
	vector< vector<double> > normIntensityVec;
	vector< vector<double> > contrastVec;
	vector< vector<double> > cumConVec;
	vector< vector<String> > colorVec2;
	vector<String> colorVec1;
	Point pt;
	vector<Point> ptVec;
	intensityVec = calcIntensityMatrix(windowVec);
	normIntensityVec = calcNormalizedIntensityMatrix(intensityVec);
	contrastVec = calcUniDimensionContrast(normIntensityVec);
	//contrastVec = con.calcContrastFromMatrix(normIntensityVec);
	cumConVec = con.calcCumulativeContrast(contrastVec);
	for(unsigned int i=0; i<normIntensityVec.size(); i++) {
		for(unsigned int j=0; j<normIntensityVec.at(i).size(); j++) {
			pix = windowVec.at(i).at(j);
			pix = c.getMainColor(pix);
			ccCurr = normIntensityVec.at(i).at(j);
			if(flag==0 && pix!="Black") { //initial first pixel-area
				//shade = calcShade(intensityVec.at(i).at(j));
				shade = "High";
				shadeIndex = getShadeIndex(shade);
				//shadeIndex = 2;
				if(shadeIndex<minShadeIndex) minShadeIndex=getShadeIndex(shade);
				if(shadeIndex>maxShadeIndex) maxShadeIndex=getShadeIndex(shade);
				localCC = ccCurr;
				localIndex = shadeIndex;
				flag=1;
			}
			else if(flag!=0) {
				indexChange = myRound((ccCurr-localCC)/threshold);
				shadeIndex = localIndex + (int)indexChange;
				shade = getShade(shadeIndex);
				if(shadeIndex<minShadeIndex) minShadeIndex=getShadeIndex(shade);
				if(shadeIndex>maxShadeIndex) maxShadeIndex=getShadeIndex(shade);
				if(ccCurr<0 || ccCurr>1) {
					shade=""; //no shade assigned to outliers
					pt.x = j;
					pt.y = i;
					ptVec.push_back(pt); //store (x,y) of outliers
				}
			}
			if(pix!="Black" && pix!="White")
				pix = shade + pix + toString(shadeIndex);
			colorVec1.push_back(pix);
		}
		flag=0;
		colorVec2.push_back(colorVec1);
		colorVec1.clear();
	}
	//assigning shades to outliers
	int x=0,y=0;
	for(unsigned int i=0; i<ptVec.size(); i++) {
		x = ptVec.at(i).x;
		y = ptVec.at(i).y;
		pix = colorVec2.at(y).at(x);
		minShade = getShade(minShadeIndex-1);
		maxShade = getShade(maxShadeIndex+1);
		if(normIntensityVec.at(y).at(x)<0)
			pix = minShade+pix;
		else if(normIntensityVec.at(y).at(x)>1)
			pix = maxShade+pix;
		colorVec2.at(y).at(x) = pix;
	}
	writeIntensityMatrix(intensityVec,name);
	writeNormalizedIntensityMatrix(normIntensityVec,name);
	writeContrastMatrix(contrastVec,name);
	writeCumConMatrix(cumConVec,name);
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
	String filename = name + "NormIntensity";
	writeSeq2File(vec,filename);
}

void Intensity::writeIntensityMatrix(vector< vector<double> > &intensityVec, String name) {
	String filename = name + "ColorIntensity";
	writeSeq2File(intensityVec,filename);
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
	//conVec = con.calcContrastFromMatrix(normVec);
	conVec = calcUniDimensionContrast(normVec);
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
	colorVec = calcMainColorMatrix(windowVec, name);
	String filename = name + "MainColors";
	writeSeq2File(colorVec,filename);
	vector< vector<String> >().swap(colorVec);
}

void Intensity::writeContrastMatrix(vector< vector<double> > &vec, String name) {
	String filename = name + "ContrastNormIntensity";
	writeSeq2File(vec,filename);
}

void Intensity::writeCumConMatrix(vector< vector<double> > &vec, String name) {
	String filename = name + "CumulContrast";
	writeSeq2File(vec,filename);
}

vector< vector<double> > Intensity::calcUniDimensionContrast(vector< vector<double> > &intensityVec) {
	vector< vector<double> > vec2;
	vector<double> vec1;
	double contrast=0,intensity=0;
	int flag=0,counter=0;
	for(unsigned int i=0; i<intensityVec.size(); i++) {
		for(unsigned int j=0; j<intensityVec.at(i).size(); j++) {
			intensity = intensityVec.at(i).at(j);
			intensity=(intensity*range)+minIntensity;
			if(intensity<900 && intensity>=0 && flag==0) {
				flag=1;
			}
			if(flag==1 && counter<10) {
				counter++;
			}
			if(counter==10) {
				if(i>575 || j>575) contrast=0;
				else {
					contrast = intensityVec.at(i+2).at(j+3) + intensityVec.at(i+1).at(j+3) +
					intensityVec.at(i).at(j+3) - intensityVec.at(i+2).at(j) -
					intensityVec.at(i+1).at(j) - intensityVec.at(i).at(j);
				}
			}
			vec1.push_back(contrast);
		}
		flag=0;
		counter=0;
		contrast=0;
		vec2.push_back(vec1);
		vec1.clear();
	}
	return vec2;
}

