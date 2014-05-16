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

double Intensity::getMinIntensity() {
	return minIntensity;
}

double Intensity::getMaxIntensity() {
	return maxIntensity;
}
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
	deque<int> index;
	deque<double> vec,vec2;
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

void Intensity::setMinMax(deque< deque<double> > &input) {
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
	minIntensity = vec.at(0);
	maxIntensity = vec.at(vec.size()-1);
	printf("%f;%f\n",minIntensity,maxIntensity);
	double t = vec.size()*thresh;
	t = round(t);
	for(int i=0; i<t; i++) {
		vec.pop_back();
		vec.pop_front();
	}
	//double stdDev = standardDevNoSort(vec);
	minIntensity = vec.at(0);
	maxIntensity = vec.at(vec.size()-1);
	//double avg = (minIntensity+maxIntensity)/2;
	//minIntensity = avg-(2*stdDev);
	//maxIntensity = avg+(2*stdDev);
	range = maxIntensity-minIntensity;
	//freqOfList(vec);
}

int shadeCount=0;
int shadeAmt=0;
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
	double thresh1 = 0.25;
	double thresh2 = 0.5;
	double thresh3 = 0.75;
	if(intensity<=thresh1)
		shade = "Light";
	if(intensity>thresh1 && intensity<=thresh2)
		shade = "Low";
	if(intensity>thresh2 && intensity<=thresh3)
		shade = "High";
	if(intensity>thresh3)
		shade = "Dark";

	return shade;
}

deque< deque<double> > Intensity::calcNormalizedIntensityMatrix(deque< deque<double> > &vec) {
	double intensity=0;
	//double sigMin = sigmoidFn(minRange);
	//double sigMax = sigmoidFn(maxRange);
	//double sigRange = sigMax-sigMin;
	deque<double> iVec1;
	deque< deque<double> > iVec2;
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

deque< deque<double> > Intensity::calcIntensityMatrix(deque <deque<String> > &windowVec) {
	double colorIntensity=0;
	String color;
	deque<double> vec;
	deque< deque<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<windowVec.at(i).size(); j++) {
			color = windowVec.at(i).at(j);
			if(color.find("Black")!=string::npos)
				colorIntensity = 900;
			else
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

deque< deque<double> > Intensity::calcSmoothedIntensityMatrix(deque< deque<double> > &intensityVec) {
	double intensity=0;
	double totalIntensity=0;
	int flag=0;
	int counter=0;
	deque<double> vec1;
	deque< deque<double> > vec2;
	unsigned int x=0,y=0;
	int scanSize=3; //3x3
	unsigned int tempX=scanSize, tempY=scanSize; //scan size
	while(y<intensityVec.size()) {
		while(x<intensityVec.at(y).size()) {
			if(x>intensityVec.size()-tempX) tempX--;
			if(y>intensityVec.size()-tempY) tempY--;
			for(unsigned int i=y; i<y+tempY; i++) {
				for(unsigned int j=x; j<x+tempX; j++) {
					intensity = intensityVec.at(i).at(j);
					intensity = (intensity*range)+minIntensity;
					if(i==y && j==x) {
						if(intensity>=900)
							flag=1;
					}
					if(flag==0) {
						if(intensity<900) {
							totalIntensity+=intensityVec.at(i).at(j);
							counter++;
						}
					}
					else {
						totalIntensity=intensityVec.at(i).at(j);
						goto break_nested_loop;
					}
				}
			}
break_nested_loop:
			if(flag==0) totalIntensity /= counter;
			vec1.push_back(totalIntensity);
			totalIntensity=0;
			flag=0;
			counter=0;
			x++;
		}
		vec2.push_back(vec1);
		vec1.clear();
		y++;
		x=0;
		tempX=scanSize;
	}
	return vec2;
}

deque< deque<String> > Intensity::calcMainColorMatrix(deque< deque<String> > &windowVec, String name) {
	Color c;
	contrast con;
	int flag=0;
	unsigned int localScanSize=10;
	String pix, shade, maxShade, minShade;
	int maxIndex=0, minIndex=0;
	double indexChange=0, ccCurr=0, localCC=0, ccPrev=0;
	int shadeIndex=0, localIndex=0, localMaxIndex=0, localMinIndex=0;
	double localMinCC=0, localMaxCC=0;
	deque< deque<double> > intensityVec;
	deque< deque<double> > normIntensityVec;
	deque< deque<double> > smoothNormIntensityVec;
	deque< deque<double> > contrastVec;
	deque< deque<double> > cumConVec;
	deque< deque<String> > colorVec2;
	deque<String> colorVec1;
	Point pt;
	deque<Point> ptVec;
	deque<double> localIndexes;
	deque<double> localCCs;
	intensityVec = calcIntensityMatrix(windowVec);
	normIntensityVec = calcNormalizedIntensityMatrix(intensityVec);
	smoothNormIntensityVec = calcSmoothedIntensityMatrix(normIntensityVec);
	//contrastVec = calcUniDimensionContrast(normIntensityVec);
	contrastVec = con.calcContrastFromMatrix(normIntensityVec);
	cumConVec = con.calcCumulativeContrast(contrastVec);
	maxShade = "Dark";
	minShade = "Light";
	maxIndex = getShadeIndex(maxShade);
	minIndex = getShadeIndex(minShade);
	shadeAmt = (maxIndex-minIndex)+1;
	double thresh = 1.0/shadeAmt;
	for(unsigned int i=0; i<smoothNormIntensityVec.size(); i++) {
		for(unsigned int j=0; j<smoothNormIntensityVec.at(i).size(); j++) {
			pix = windowVec.at(i).at(j);
			pix = c.getMainColor(pix);
			ccCurr = smoothNormIntensityVec.at(i).at(j);
			if(flag==0) { //initial first pixel-area
				if(ccCurr<=1 && ccCurr>=0 && pix!="Black") {
					shade = calcShade(ccCurr);
					//shade = "High";
					shadeIndex = getShadeIndex(shade);
					localCC = ccCurr;
					localIndex = shadeIndex;
					flag=1;
				}
				else if(ccCurr<0 && pix!="Black") {
					shadeIndex = minIndex-1;
					shade = getShade(shadeIndex);
				}
				else if(ccCurr>1 && pix!="Black") {
					shadeIndex = maxIndex+1;
					shade = getShade(shadeIndex);
				}
			}
			else if(flag!=0) {
				ccPrev = smoothNormIntensityVec.at(i).at(j-1);
				if((ccCurr-ccPrev)<0) {
					localCC = localMaxCC;
					localIndex = localMaxIndex;
				}
				if((ccCurr-ccPrev)>0) {
					localCC = localMinCC;
					localIndex = localMinIndex;
				}
				shade = calcShade(ccCurr);
				indexChange = myRound((ccCurr-localCC)/(thresh));
				//indexChange = trunc((ccCurr-localCC)/threshold);
				shadeIndex = localIndex + (int)indexChange;
				if(localCC>1) shadeIndex--;
				if(localCC<0) shadeIndex++;
				if(ccCurr<0) {
					shade = getShade(minIndex-1);
					shadeIndex--;
				}
				else if(ccCurr>1) {
					shade = getShade(maxIndex+1);
					shadeIndex++;
				}
				/*if(smoothNormIntensityVec.at(i).at(j)<0 || smoothNormIntensityVec.at(i).at(j)>1) {
					shade=""; //no shade assigned to outliers
					pt.x = j;
					pt.y = i;
					ptVec.push_back(pt); //store (x,y) of outliers
				}*/
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
			if(pix!="Black")
				pix = shade + pix + toString(shadeIndex);
			colorVec1.push_back(pix);
		}
		flag=0;
		localIndexes.clear();
		localCCs.clear();
		colorVec2.push_back(colorVec1);
		colorVec1.clear();
	}
	//assigning shades to outliers
/*	int x=0,y=0;
	for(unsigned int i=0; i<ptVec.size(); i++) {
		x = ptVec.at(i).x;
		y = ptVec.at(i).y;
		pix = colorVec2.at(y).at(x);
		minShadeIndex -= 1;
		maxShadeIndex += 1;
		if(minShadeIndex<0) minShade = "White";
		else minShade = getShade(minShadeIndex);
		if(maxShadeIndex>=shadeCount) maxShade = "Black";
		else maxShade = getShade(maxShadeIndex);
		if(smoothNormIntensityVec.at(y).at(x)<0)
			pix = minShade+pix;
		else if(smoothNormIntensityVec.at(y).at(x)>1)
			pix = maxShade+pix;
		colorVec2.at(y).at(x) = pix;
	}*/
	writeIntensityMatrix(intensityVec,name);
	writeNormalizedIntensityMatrix(normIntensityVec,name);
	writeSmoothIntensityMatrix(smoothNormIntensityVec,name);
	//writeContrastMatrix(contrastVec,name);
	//writeCumConMatrix(cumConVec,name);
	return colorVec2;
}

void Intensity::writeNormalizedIntensityMatrix(deque< deque<String> > &windowVec, String name) {
	deque< deque<double> > normVec;
	deque< deque<double> > intVec;
	intVec = calcIntensityMatrix(windowVec);
	normVec = calcNormalizedIntensityMatrix(intVec);
	String filename = name + "NormIntensity";
	writeSeq2File(normVec,name);
	deque< deque<double> >().swap(normVec);
	deque< deque<double> >().swap(intVec);
}

void Intensity::writeNormalizedIntensityMatrix(deque< deque<double> > &vec, String name) {
	String filename = name + "NormIntensity";
	writeSeq2File(vec,filename);
}

void Intensity::writeIntensityMatrix(deque< deque<double> > &intensityVec, String name) {
	String filename = name + "ColorIntensity";
	writeSeq2File(intensityVec,filename);
}

void Intensity::writeIntensityMatrix(deque< deque<String> > &windowVec, String name) {
	deque< deque<double> > intVec;
	intVec = calcIntensityMatrix(windowVec);
	String filename = name + "ColorIntensity";
	writeSeq2File(intVec,filename);

	/* write normalizeIntensity */
	deque< deque<double> > normVec;
	normVec = calcNormalizedIntensityMatrix(intVec);
	filename = name + "NormIntensity";
	writeSeq2File(normVec,filename);

	/* write contrast */
	contrast con;
	deque< deque<double> > conVec;
	//conVec = con.calcContrastFromMatrix(normVec);
	conVec = calcUniDimensionContrast(normVec);
	filename = name + "ContrastNormIntensity";
	writeSeq2File(conVec,filename);

	/*write cumulative contrast */
	deque< deque<double> > cumConVec;
	cumConVec = con.calcCumulativeContrast(conVec);
	filename = name + "CumulContrast";
	writeSeq2File(cumConVec,filename);
	deque< deque<double> >().swap(cumConVec);
	/* end write cumulative contrast */

	deque< deque<double> >().swap(conVec);
	/* end write contrast */

	deque< deque<double> >().swap(normVec);
	/* end write normalizedIntensity */

	deque< deque<double> >().swap(intVec);
}

void Intensity::writeMainColorMatrix(deque< deque<String> > &windowVec, String name) {
	deque< deque<String> > colorVec;
	colorVec = calcMainColorMatrix(windowVec, name);
	String filename = name + "MainColors";
	writeSeq2File(colorVec,filename);
	deque< deque<String> >().swap(colorVec);
}

void Intensity::writeContrastMatrix(deque< deque<double> > &vec, String name) {
	String filename = name + "ContrastNormIntensity";
	writeSeq2File(vec,filename);
}

void Intensity::writeCumConMatrix(deque< deque<double> > &vec, String name) {
	String filename = name + "CumulContrast";
	writeSeq2File(vec,filename);
}

void Intensity::writeSmoothIntensityMatrix(deque< deque<double> > &vec, String name) {
	String filename = name + "SmoothNormIntensity";
	writeSeq2File(vec,filename);
}

deque< deque<double> > Intensity::calcUniDimensionContrast(deque< deque<double> > &intensityVec) {
	deque< deque<double> > vec2;
	deque<double> vec1;
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
				contrast /= 3;
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

