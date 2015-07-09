/*
 * intensity.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: jason
 */

#include "intensity.h"
#include "rules.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hsl/hsl.h"
#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"
#include "/home/jason/git/WebDerm/WebDerm/src/rgb/rgb.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Math/maths.h"
#include "/home/jason/git/WebDerm/WebDerm/src/hysteresis/hysteresis.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Shades/shades.h"
#include "/home/jason/git/WebDerm/WebDerm/src/FileData/filedata.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/write.h"
#include "/home/jason/git/WebDerm/WebDerm/src/Algorithms/quicksort.h"

//String shadeArr[] = {"Dark1","Dark2","High1","High2","Low1","Low2","Low3","Light1","Light2","Light3","White"};

Intensity::Intensity() {
	global_flag=0;
	shadeCount=g_Shades.size();
	minIntensity = 0;
	maxIntensity = 255;
	range=0;
	outlierCorrection = 4;
	status = "NA";
	oldMinShade=status;oldMaxShade=status;
	newMinShade=status;newMaxShade=status;
	minIndex=0;
	maxIndex=0;
	oldShadeAmt=0;
	oldInterval=0;
	minOutlier=0;
	maxOutlier=0;
	thresh=NULL;
	shadeIndex=NULL;
}

Intensity::~Intensity() {
	delete[] thresh;
	delete[] shadeIndex;
	thresh = NULL;
	shadeIndex = NULL;
}

double Intensity::getMinIntensity() {
	return minIntensity;
}

double Intensity::getMaxIntensity() {
	return maxIntensity;
}

String Intensity::getOldMinShade() {
	return oldMinShade;
}
String Intensity::getOldMaxShade() {
	return oldMaxShade;
}
String Intensity::getNewMinShade() {
	return newMinShade;
}
String Intensity::getNewMaxShade() {
	return newMaxShade;
}

double Intensity::calcIntensity(String pix) {
	Rgb rgb;
	Color c;
	String color;
	double colorLevel[mainColors.size()];
	double colorIntensity[mainColors.size()];
	fill_n(colorLevel,mainColors.size(),0);
	fill_n(colorIntensity,mainColors.size(),0);
	double totalLevel=0;
	double totalColorIntensity=0;
	deque<int> index;
	deque<double> vec,vec2;
	color = c.getMainColor(pix);
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		if(color.find(mainColors.at(i))!=string::npos) {
			colorLevel[i] = rgb.getColorLevel(pix,mainColors.at(i));
			colorLevel[i] = 100 - colorLevel[i];
			index.push_back(i);
			totalLevel+=colorLevel[i];
		}
	}
	for(unsigned int i=0; i<index.size(); i++) {
		if(totalLevel==0) {
			totalColorIntensity=0;
			break;
		}
		colorIntensity[index.at(i)] = colorLevel[index.at(i)]/100;
		colorIntensity[index.at(i)] *= 255;
		colorIntensity[index.at(i)] *= (colorLevel[index.at(i)]/totalLevel);
		totalColorIntensity += colorIntensity[index.at(i)];
	}
	return totalColorIntensity;
}

//assigns min/max shades to an image. Only runs once per image.
void Intensity::setMinMaxShades() {
	double min,max;
	for(unsigned int i=0; i<g_ShadeThresh.size(); i++) {
		try {
			min = g_ShadeThresh.at(i).at(0);
			max = g_ShadeThresh.at(i).at(1);
			if(minIntensity>=min && minIntensity<max)
				oldMinShade = g_Shades.at(i);
			if(maxIntensity>=min && maxIntensity<max)
				oldMaxShade = g_Shades.at(i);
			if(maxIntensity>=g_ShadeThresh.at(g_ShadeThresh.size()-1).at(1))
				oldMaxShade = g_Shades.at(g_Shades.size()-1);
			if(oldMinShade!=status && oldMaxShade!=status) break;
		}
		catch (const std::out_of_range &oor) {
			printf("Intensity::setMinMaxShades() out of range!\n");
			printf("g_ShadeThresh.Size: %lu\n",g_ShadeThresh.size());
			exit(1);
		}
	}
}

//sets the min/max intensity of image.
//also sets min/max shades using setMinMaxShades()
void Intensity::setMinMax(deque< deque<double> > &input) {
	deque<double> vec;
	double intensity=0;
	double thresh = 0.05; //percentage of outlier removal
	unsigned int x=0, y=0;
	try {
		for(unsigned int i=0; i<input.size(); i++) {
			y=i;
			for(unsigned int j=0; j<input.at(i).size(); j++) {
				x=j;
				intensity = input.at(i).at(j);
				if(intensity!=0)
					vec.push_back(intensity);
			}
		}
	}
	catch (const std::out_of_range &oor) {
		printf("setMinMax() out of range!\n");
		printf("input.Rows: %lu\n",input.size());
		printf("Point(%d,%d)\n",x,y);
		exit(1);
	}
	quicksort(vec,0,vec.size()-1);
	//freqOfList(vec);
	minIntensity = vec.at(0);
	maxIntensity = vec.at(vec.size()-1);
	printf("Before: %f;%f\n",minIntensity,maxIntensity);
	double t = vec.size()*thresh;
	t = round(t);
	/*for(int i=0; i<t; i++) {
		vec.pop_back();
		vec.pop_front();
	}*/
	vec.erase(vec.begin(),vec.begin()+t);
	vec.erase(vec.end()-t,vec.end());
	//minIntensity /= 2;
	//maxIntensity += (255-maxIntensity)/2;
	minIntensity = vec.at(0);
	maxIntensity = vec.at(vec.size()-1);
	range = maxIntensity-minIntensity;
	printf("After: %f;%f\n",minIntensity,maxIntensity);
	//assign initial min/max shade to image.
	setMinMaxShades();
	//oldMinShade = "High";
	//oldMaxShade = "Low";
	newMinShade = oldMinShade;
	newMaxShade = oldMaxShade;
	deque<double>().swap(vec);
}

//calculates the shade given the intensity value while excluding outliers
String Intensity::calcShadeExcludingOutliers(double inten) {
	Shades sh;
	if(global_flag==0) {
		minIndex = sh.getShadeIndex(oldMinShade);
		maxIndex = sh.getShadeIndex(oldMaxShade);
		oldShadeAmt = (maxIndex-minIndex)+1;
		oldInterval = range/oldShadeAmt;
		minOutlier = minIntensity - outlierCorrection;
		maxOutlier = maxIntensity + outlierCorrection;
		thresh = new double[oldShadeAmt];
		shadeIndex = new int[oldShadeAmt];
		//initialize thresholds to array
		for(int i=0; i<oldShadeAmt; i++) {
			thresh[i] = minIntensity + (i*oldInterval);
			if(i==0) thresh[i] = minOutlier;
			shadeIndex[i] = sh.getShadeIndex(oldMinShade) + i;
			cout << thresh[i] << ";";
		}
		global_flag=1;
		cout << maxOutlier << endl;
	}
	String shade;
	double indexChange=0;
	//calculate shade
	for(int i=0; i<oldShadeAmt; i++) {
		if(i<(oldShadeAmt-1)) {
			if(inten>thresh[i] && inten<=thresh[i+1]) {
				shade = sh.getShade(shadeIndex[i]);
				break;
			}
			if(inten<=minOutlier) {
				indexChange = (inten-minOutlier)/oldInterval;
				indexChange = floor(indexChange);
				shade = sh.getShade(minIndex+indexChange);
				newMinShade = shade;
				break;
			}
		}
		else {
			if(inten>thresh[i] && inten<=maxOutlier) {
				shade = sh.getShade(shadeIndex[i]);
				break;
			}
			if(inten>maxOutlier) {
				indexChange = (inten-maxOutlier)/oldInterval;
				indexChange = ceil(indexChange);
				shade = sh.getShade(maxIndex+indexChange);
				newMaxShade = shade;
				break;
			}
		}
	}
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
			intensity = roundDecimal(intensity,2);
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
	String pix;
	deque<double> vec;
	deque< deque<double> > vec2;
	unsigned int x=0, y=0;
	try {
		for(unsigned int i=0; i<windowVec.size(); i++) {
			y=i;
			for(unsigned int j=0; j<windowVec.at(i).size(); j++) {
				x=j;
				pix = windowVec.at(i).at(j);
				if(pix.find("Zero")!=string::npos)
					colorIntensity = 0;
				else
					colorIntensity = calcIntensity(pix);
				colorIntensity = round(colorIntensity);
				vec.push_back(colorIntensity);
			}
			vec2.push_back(vec);
			vec.clear();
		}
		setMinMax(vec2);
	}
	catch(const std::out_of_range &oor) {
		printf("Intensity::calcIntensityMatrix() out of range!\n");
		printf("WindowVec.Rows: %lu\n",windowVec.size());
		printf("Point(%d,%d)\n",x,y);
		exit(1);
	}
	return vec2;
}

deque< deque<double> > Intensity::calcSmoothedIntensityMatrix(deque< deque<double> > &intensityVec) {
	double intensity=0;
	double totalIntensity=0;
	int flag=0;
	int counter=0;
	int scanSize = 2; //3x3
	deque<double> vec1;
	deque< deque<double> > vec2;
	unsigned int x=0,y=0;
	unsigned int tempX=scanSize, tempY=scanSize; //scan size
	try {
		while(y<intensityVec.size()) {
			while(x<intensityVec.at(y).size()) {
				if(x>intensityVec.size()-tempX) tempX--;
				if(y>intensityVec.size()-tempY) tempY--;
				for(unsigned int i=y; i<y+tempY; i++) {
					for(unsigned int j=x; j<x+tempX; j++) {
						intensity = intensityVec.at(i).at(j);
						if(i==y && j==x) {
							if(intensity<=0)
								flag=1;
						}
						if(flag==0) {
							if(intensity>0) {
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
				totalIntensity = round(totalIntensity);
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
	}
	catch(const std::out_of_range &oor) {
		printf("Intensity::calcSmoothedIntensityMatrix() out of range!\n");
		printf("Point(%d,%d)\n",x,y);
		exit(1);
	}
	return vec2;
}

deque< deque<String> > Intensity::calcMainColorMatrix(Mat img, deque< deque<String> > &windowVec,
		deque< deque<String> > &hslMat,String name, FileData &fd) {
	Rgb rgb;
	Hsl hsl;
	Color c;
	Shades sh;
	Functions fn;
	FILE *fp;
	String filename = name + "-Rule-Table.csv";
	fp=fopen(filename.c_str(),"w");
	fprintf(fp,"Color,NewColor,HSL,Shade,NewShade,IndexChange,Rules,Coord,Image\n");
	int flag=0;
	unsigned int localScanSize=10;
	String pix, pix2, shade,localMinShade,localMaxShade,localShade;
	int maxShadeIndex=0, minShadeIndex=0, maxIndex=0,minIndex=0,index=0,loc=0;
	double indexChange=0, ccCurr=0, localCC=0, ccPrev=0;
	int shadeIndex=0, localIndex=0, localMaxIndex=0, localMinIndex=0;
	double localMinCC=0, localMaxCC=0;
	deque< deque<String> > colorVec3;
	deque<int> localIndexes;
	deque<double> localCCs;
	deque<String> localShades;
	deque<double> ruleNo;
	deque<String> strVec1;
	deque<String> rulesRow;
	fd.intensityVec = calcIntensityMatrix(fd.windowVec);
	fd.smoothIntensityVec = calcSmoothedIntensityMatrix(fd.intensityVec);
	fd.range = range;
	fd.colorVec.resize(fd.smoothIntensityVec.size(),deque<String>(fd.smoothIntensityVec.at(0).size(),""));
	for(unsigned int i=0; i<fd.smoothIntensityVec.size(); i++) {
		for(unsigned int j=0; j<fd.smoothIntensityVec.at(i).size(); j++) {
			try {
				pix = windowVec.at(i).at(j);
				pix2 = c.getMainColor(pix);
				ccCurr = fd.smoothIntensityVec.at(i).at(j);
				if(flag==0) { //initial first pixel-area
					if(pix!="Zero") {
						shade = calcShadeExcludingOutliers(ccCurr);
						flag=1;
					}
				}
				else if(flag!=0) {
					if(pix!="Zero")
						shade = calcShadeExcludingOutliers(ccCurr);
					else
						shade = "";
				}
				fd.colorVec.at(i).at(j) = shade;
			}
			catch(const std::out_of_range &oor) {
				printf("Intensity::calcMainColorMatrix()-1st Half out of range!\n");
				cout << "Row Size: " << fd.smoothIntensityVec.size() << endl;
				printf("Point(%d,%d)\n", j,i );
				exit(1);
			}
		}
		flag=0;
	}

	maxShadeIndex = sh.getShadeIndex(newMaxShade);
	minShadeIndex = sh.getShadeIndex(newMinShade);
	printf("%s;%s\n",oldMinShade.c_str(),oldMaxShade.c_str());
	printf("%s;%s\n",newMinShade.c_str(),newMaxShade.c_str());
	int newShadeAmt = (maxShadeIndex-minShadeIndex)+1;
	double newInterval = range/newShadeAmt;

	double currGL=0, currCL=0, currRatio=0, prevRatio=0;
	double localMinRatio=0, localMaxRatio=0, localRatio=0;
	double minRatioIndex=0, maxRatioIndex=0, localRatioIndex=0, ratioLoc=0;
	deque<double> localRatios;
	double localRatioScanSize = 20;
	double relativeRatio=0;
	fd.shadeVec = fd.colorVec;
	fd.localRatioScanSize = localRatioScanSize;
	fd.localScanSize = localScanSize;
	fd.minIntensity = minIntensity;
	fd.maxIntensity = maxIntensity;
	fd.minOutlier = minIntensity - outlierCorrection;
	fd.maxOutlier = maxIntensity + outlierCorrection;
	fd.oldMinShade = oldMinShade;
	fd.oldMaxShade = oldMaxShade;
	fd.newMinShade = newMinShade;
	fd.newMaxShade = newMaxShade;
	fd.totalShades = newShadeAmt;
	init_2D_Deque(fd.m_ContrastMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.d_HslMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.hslPtMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.cumHslMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	init_2D_Deque(fd.minMaxHslMat,fd.windowVec.size(),fd.windowVec.at(0).size());
	cout << "Calculating Contrast..." << flush;
	for(unsigned int i=0; i<fd.smoothIntensityVec.size(); i++) {
		for(unsigned int j=0; j<fd.smoothIntensityVec.at(i).size(); j++) {
			try {
				pix = windowVec.at(i).at(j);
				pix2 = c.getMainColor(pix);
				ccCurr = fd.smoothIntensityVec.at(i).at(j);
				currGL = rgb.getGrayLevel1(pix);
				currCL = rgb.getColorLevel(pix);
				currRatio = currGL/currCL;
				if(flag==0) { //initial first pixel-area
					if(pix2!="Zero") {
						currGL = rgb.getGrayLevel1(pix);
						currCL = rgb.getColorLevel(pix);
						currRatio = currGL/currCL;
						shade = fd.shadeVec.at(i).at(j);
						shadeIndex = sh.getShadeIndex(shade);
						localCC = ccCurr;
						localIndex = shadeIndex;
						localShade = shade;
						ccPrev = ccCurr;
						localRatio = currRatio;
						prevRatio = currRatio;
						flag=1;
					}
				}
				else if(flag!=0 && pix2!="Zero") {
					currGL = rgb.getGrayLevel1(pix);
					currCL = rgb.getColorLevel(pix);
					currRatio = currGL/currCL;
					if(currRatio-prevRatio>0) {
						localRatio = localMinRatio;
						localRatioIndex = minRatioIndex;
					}
					else if(currRatio-prevRatio<0) {
						localRatio = localMaxRatio;
						localRatioIndex = maxRatioIndex;
					}
					else localRatioIndex--;
					if(localRatioIndex<0) localRatioIndex=0;

					if(windowVec.at(i).at(j-1)!="Zero")
						ccPrev = fd.smoothIntensityVec.at(i).at(j-1);
					if((ccCurr-ccPrev)<0) {
						localCC = localMaxCC;
						localIndex = localMaxIndex;
						localShade = localMaxShade;
						index = maxIndex;
					}
					else if((ccCurr-ccPrev)>0) {
						localCC = localMinCC;
						localIndex = localMinIndex;
						localShade = localMinShade;
						index = minIndex;
					}
					else index--;
					if(index<0) index=0;
					shade = fd.shadeVec.at(i).at(j);
					indexChange = MyMath::myRound((ccCurr-localCC)/(newInterval));
					shadeIndex = localIndex + (int)indexChange;
					ccPrev=ccCurr;
					prevRatio = currRatio;
					relativeRatio = currRatio/localRatio;
				}
				relativeRatio = currRatio/localRatio;
				relativeRatio = roundDecimal(relativeRatio,2);
				if(localRatio==0 || pix2=="Zero")  {
					relativeRatio = -1;
					currRatio = -1;
				}
				fd.absRatioVec.push_back(roundDecimal(currRatio,2));
				double h=0,s=0,l=0;
				if(pix2!="Zero") {
					fd.pt = Point(j,i);
					loc = j-(localIndexes.size()-index);
					ratioLoc  = j-(localRatios.size()-localRatioIndex);
					bool flag2 = specialRules(fd,pix,indexChange,shade,ratioLoc,loc,ruleNo);
					if(flag2==true) pix2 = c.getMainColor(pix);
					else ruleNo.push_back(0);
					h = fn.getDelimitedValuesFromString(hslMat.at(i).at(j),';',1);
					s = fn.getDelimitedValuesFromString(hslMat.at(i).at(j),';',2);
					l = fn.getDelimitedValuesFromString(hslMat.at(i).at(j),';',3);
					int ind=-1;
					hsl.getHslColor(h,s,l,ind,fd.pt);
					s = roundDecimal(s,2);
					l = roundDecimal(l,2);
					String str = toString(h)+";"+toString(s)+";"+toString(l);
					str = "("+str+")";
					pix2 = str + shade + pix2 + toString(indexChange) + ";" + toString(loc+1)+ ";" + toString(ratioLoc+1);
				}
				else
					ruleNo.push_back(0);
				if(pix2!="Zero") {
					if(localIndexes.size()==localScanSize) localIndexes.pop_front();
					if(localCCs.size()==localScanSize) localCCs.pop_front();
					if(localShades.size()==localScanSize) localShades.pop_front();
					localIndexes.push_back(shadeIndex);
					localCCs.push_back(ccCurr);
					localShades.push_back(shade);
					maxIndex = distance(localCCs.begin(),max_element(localCCs.begin(),localCCs.end()));
					localMaxCC = localCCs.at(maxIndex);
					localMaxIndex = localIndexes.at(maxIndex);
					localMaxShade = localShades.at(maxIndex);
					minIndex = distance(localCCs.begin(),min_element(localCCs.begin(),localCCs.end()));
					localMinCC = localCCs.at(minIndex);
					localMinIndex = localIndexes.at(minIndex);
					localMinShade = localShades.at(minIndex);
					if(localRatios.size()==localRatioScanSize) localRatios.pop_front();
					localRatios.push_back(currRatio);
					maxRatioIndex = distance(localRatios.begin(),max_element(localRatios.begin(),localRatios.end()));
					localMaxRatio = localRatios.at(maxRatioIndex);
					minRatioIndex = distance(localRatios.begin(),min_element(localRatios.begin(),localRatios.end()));
					localMinRatio = localRatios.at(minRatioIndex);
				}
				/** generate rule table **/
				if(ruleNo.size()>0) {
					char buffer[40];
					sprintf(buffer,"(%d;%d)",j,i);
					String coord(buffer);
					String ruleNum;
					String oldPix = windowVec.at(i).at(j);
					String oldShade = fd.colorVec.at(i).at(j);
					char buffer2[50];
					sprintf(buffer2,"(%.0f;%.2f;%.2f)",h,s,l);
					String hslStr(buffer2);
					strVec1.push_back(oldPix);
					strVec1.push_back(pix);
					strVec1.push_back(hslStr);
					strVec1.push_back(oldShade);
					strVec1.push_back(shade);
					strVec1.push_back(toString(indexChange));
					for(unsigned int n=0; n<ruleNo.size(); n++) {
						ruleNum += toString(ruleNo.at(n)) + ";";
					}
					strVec1.push_back(ruleNum);
					strVec1.push_back(coord);
					strVec1.push_back(name);
					for(unsigned int i=0; i<strVec1.size(); i++) {
						fprintf(fp,"%s,",strVec1.at(i).c_str());
					}
					fprintf(fp,"\n");
					strVec1.clear();
					rulesRow.push_back(ruleNum);
				}
				fd.colorVec.at(i).at(j) = pix2;
				ruleNo.clear();
			}
			catch(const std::out_of_range &oor) {
				printf("Intensity::calcMainColorMatrix()-2nd Half out of range!\n");
				printf("smoothIntensityVec.Size: %lu\n",fd.smoothIntensityVec.size());
				printf("Point(%d,%d)\n",j,i);
				printf("Color: %s\n", pix2.c_str());
				exit(1);
			}
		} // end col
		fd.absRatioMat.push_back(fd.absRatioVec);
		fd.absRatioVec.clear();
		fd.rulesMat.push_back(rulesRow);
		rulesRow.clear();
		flag=0;
		indexChange=0;
		index=0;
		localIndexes.clear();
		localCCs.clear();
		localShades.clear();
		localRatioIndex=0;
		localRatios.clear();
		localRatio=0;
	} // end row
	fclose(fp);
	colorVec3.clear(); colorVec3.shrink_to_fit();
	localIndexes.clear(); localIndexes.shrink_to_fit();
	localCCs.clear(); localCCs.shrink_to_fit();
	localShades.clear(); localShades.shrink_to_fit();
	ruleNo.clear(); ruleNo.shrink_to_fit();
	strVec1.clear(); strVec1.shrink_to_fit();
	rulesRow.clear(); rulesRow.shrink_to_fit();

	//fd.writeFileMetaData();
	//c.output2ImageColor(fd.colorVec,name);
	//writeSeq2File(fd.absRatioMat,name+"_AbsoluteRatios");
	//writeSeq2File(fd.intensityVec,name+"_ColorIntensity");
	//writeSeq2File(fd.smoothIntensityVec,name+"_SmoothIntensity");
	//writeSeq2File(shadeVec2,"shadeVec");

	return fd.colorVec;
}
