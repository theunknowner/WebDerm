/*
 * rules.cpp
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#include "rules.h"

String init_specialRules(String pix, int r, int g, int b) {
	Color c;
	rgb rgb;
	String pix2 = pix;
	String color = c.getMainColor(pix);
	double grayLevel = rgb.calcGrayLevel(r,g,b);
	//double grayLumLevel = rgb.calcGrayLevel3(r,g,b);
	double colorLevel = rgb.calcColorLevel2(r,g,b);

	/** rule 1 **
	if(color.find("Violet")==string::npos && grayLevel>=90)
		pix2 = "Grey" + toString(colorLevel);

	/** rule 2 **/
	if(color.find("Pink")!=string::npos && grayLevel>=80 && colorLevel>=60)
		pix2 = "Grey" + toString(colorLevel);

	/** rule 2a **/
	if(color.find("Pink")!=string::npos && grayLevel>=85 && colorLevel>=54)
		pix2 = "Grey" + toString(colorLevel);

	/** rule 3 **/
	if(color=="Brown" && colorLevel>=70 && grayLevel>=80)
		pix2 = "Grey" + toString(colorLevel);

	/** rule 4 **/
	if(color=="Green" && grayLevel>=90)
		pix2 = "Grey" + toString(colorLevel);

	/** rule 5 **/
	if(color=="Violet" && grayLevel>=85 && colorLevel>=55) {
		pix2 = "Grey" + toString(colorLevel);
	}

	return pix2;
}

/** provisional rule #1 - Contrast with IndexChange**/
int rule1(double &indexChange, String &shade, String &newShade) {
	Intensity in;
	bool flag=false;
	int ruleNum=1;
	double indexChangeThresh = 2.25; // different than thresh for contrast of colors
	if(abs(indexChange)>=indexChangeThresh) {
		int index = in.getShadeIndex(shade);
		index += (indexChange/indexChangeThresh);
		newShade = in.getShade(index);
		flag=true;
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** provisional rule #2 - Determining Gray **/
int rule2(String &pix, String &newPix, String &newShade) {
	bool flag=false;
	int ruleNum=2;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);
	double ratio = roundDecimal(grayLumLevel/colorLevel,2);
	if(grayLumLevel>=90 && color!="Violet" && ratio>=0.82) {
		newPix = "Grey" + toString(colorLevel);
		if(colorLevel>=60) {
			newShade = "Dark";
		}
		flag=true;
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** provisional rule #3 for Grayish Pink ONLY **/
int rule3(String &pix, String &newPix) {
	bool flag=false;
	int ruleNum=3;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(pix);
	if(color=="Pink" || color=="BrownPink") {
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(grayLumLevel>=39 && colorLevel>=45 && colorLevel<=50 && ratio>=0.75 && ratio<=0.8) {
			newPix = "GreyPink" + toString(colorLevel);
			flag=true;
		}
		if(grayLumLevel>=39 && colorLevel>=55 && colorLevel<=70 && ratio>=0.75 && ratio<=0.8) {
			newPix = "GreyPink" + toString(colorLevel);
			flag=true;
		}
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** provisional rule #4 for DarkGrayPink ONLY **/
int rule4(String &pix, String &newPix, String &newShade) {
	bool flag=false;
	int ruleNum=4;
	rgb rgb;
	Color c;
	double grayLumLevel = rgb.getGrayLevel2(pix);
	double colorLevel = rgb.getColorLevel(pix);
	String color = c.getMainColor(newPix);
	if(newShade=="Dark" &&  color=="GreyPink"){
		double ratio = roundDecimal(grayLumLevel/colorLevel,2);
		if(ratio>=0.75) {
			newPix="Grey" + toString(colorLevel);
			flag=true;
		}
	}
	if(flag==true) return ruleNum;

	return 0;
}

/** Rule #5 - Determining Dark Grey **/
int rule5(Mat &img, String &pix, String &newPix, String &newShade, Point pt) {
	int ruleNum=5;
	bool flag=false;
	double r,g,b;
	int rgbFlag=0;
	double dist=0;
	rgb rgb;
	hsl hsl;
	Color c;
	String color = c.getMainColor(newPix);
	double grayLevel = rgb.getGrayLevel1(newPix);
	double grayLumLevel = rgb.getGrayLevel2(newPix);
	double colorLevel = rgb.getColorLevel(newPix);
	double ratio = roundDecimal(grayLumLevel/colorLevel,2);

	if(color.find("Pink")!=string::npos && grayLevel>=85)
		if(newShade=="Dark")
			if(ratio>=1.17) {
				newShade="Dark";
				newPix = "Grey";
				flag=true;
			}

		if(flag==false)
			for(int i=pt.y; i<(pt.y+2); i++)
				for(int j=pt.x; j<(pt.x+2); j++) {
					r = img.at<Vec3b>(i,j)[2];
					g = img.at<Vec3b>(i,j)[1];
					b = img.at<Vec3b>(i,j)[0];
					if(hsl.maxRGB(r,g,b)<100)
						if(g<=r && r<=(g*1.25))
							if((0.8*b)<=g && g<=(b*1.2)) {
								newShade = "Dark";
								newPix = "Grey";
								flag=true;
								continue;
							}

					for(unsigned int k=0; k<grayRGB.size(); k++) {
						dist = rgb.absEucDist(r,g,b,grayRGB.at(k));
						if(dist<1.5) {
							rgbFlag++;
							break;
						}
					}
					if(rgbFlag>=4) {
						newPix = "Grey";
						flag=true;
					}
				}
/*
	if(color.find("Pink")!=string::npos && grayLevel>=85)
		if(newShade=="High")
			if(ratio>=1.17) {
				newShade="High";
				newPix="Grey";
				flag=true;
			}

	if(color=="Brown" && grayLevel>=70)
		if(newShade=="Dark")
			if(ratio>=1.2) {
				newShade = "Dark";
				newPix = "Grey";
				flag=true;
			}
/*
	if(color=="Grey")
		if(newShade=="High" || newShade=="Dark")
			if(ratio>=1.15) {
				newShade= "Dark";
				newPix = "Grey";
				flag=true;
			}*/
	if(flag==true) return ruleNum;

	return 0;
}

/** rule 6 - fix colors **/
int rule6(String& pix, String& newPix, String& newShade) {
	int ruleNum = 6;
	bool flag = false;
	rgb rgb;
	Color c;
	String color = c.getMainColor(newPix);
	double grayLevel = rgb.getGrayLevel1(newPix);
	double grayLumLevel = rgb.getGrayLevel2(newPix);
	double colorLevel = rgb.getColorLevel(newPix);
	//double darkness = rgb.colorLevel2Brightness(colorLevel);
	//double ratio = darkness/grayLumLevel;
	//double ratio = grayLumLevel/colorLevel;

	if(newShade=="Dark") {
		if(grayLevel>=85) {
			newPix = "Grey";
			flag=true;
		}
	}
	/*
	else if(newShade=="High" || newShade=="Low") {
		if(grayLevel>=85) {
			if(color.find("Pink")!=string::npos || color.find("Violet")!=string::npos) {
				if(colorLevel<60 && ratio>1.25) {
					newPix = "Grey";
					flag=true;
				}
			}
			else if(color.find("Purple")!=string::npos) {
				if(colorLevel<55 && ratio>1.3) {
					newPix = "Grey";
					flag=true;
				}
			}
			else {
				newPix = color;
				flag=true;
			}
		}
	}*/

	if(flag==true) return ruleNum;

	return 0;
}

/** rule 7 - All violets are pinks, fixes other colors **/
int rule7(String &pix, String &newPix) {
	int ruleNum = 7;
	bool flag=false;
	Color c;
	String color = c.getMainColor(newPix);
	if(color.find("Blue")!=string::npos && color.find("Purple")!=string::npos) {
		newPix = "Purple";
		flag=true;
	}
	if(color.find("Violet")!=string::npos) {
		newPix = "Pink";
		flag=true;
	}

	if(flag==true) return ruleNum;

	return 0;
}

/** rule 8 - Using indexChange to promote colors on boundaries **/
int rule8(String &newPix, double &indexChange, Point pt,
			deque< deque<String> > &windowVec, deque< deque<String> > &hslMat) {
	int ruleNum = 8;
	bool flag=false;
	hsl hsl;
	Color c;
	String color = c.getMainColor(newPix);
	String prevColor = c.getMainColor(windowVec.at(pt.y).at(pt.x-1));
	int ind=-1;
	double indexChangeThresh = 1.0; //different than thresh for contrast of shades
	double pThreshMove = 1.35;
	double hue,sat,lum;
	hue = getDelimitedValuesFromString(hslMat.at(pt.y).at(pt.x),';',1);
	sat = getDelimitedValuesFromString(hslMat.at(pt.y).at(pt.x),';',2)/100;
	lum = getDelimitedValuesFromString(hslMat.at(pt.y).at(pt.x),';',3)/100;
	double fSat=0;
	if(indexChange>=indexChangeThresh) {
		if(color=="Grey") {
			hsl.getHslColor(hue,sat,lum,ind);
			fSat = (sat-satThresh.at(ind).at(0));
			fSat /= (satThresh.at(ind).at(1)-satThresh.at(ind).at(0));
			fSat *= pThreshMove;
			if(fSat>1) {
				newPix = hslColors.at(ind+1);
				flag = true;
			}
		}
	}
	else {
		if(color=="Grey" && prevColor=="Grey") {
			hsl.getHslColor(hue,sat,lum,ind);
			fSat = (sat-satThresh.at(ind).at(0));
			fSat /= (satThresh.at(ind).at(1)-satThresh.at(ind).at(0));
			fSat *= pThreshMove;
			if(fSat>1) {
				newPix = hslColors.at(ind+1);
				flag = true;
			}
		}
	}

	if(flag==true) return ruleNum;

	return 0;
}

/** rule 9 - GrayIndexChange - for non-grays that look gray **/
int rule9(FileData &fd, String &newPix, int ratioLoc) {
	int ruleNum = 9;
	bool flag = false;
	rgb rgb;
	Color c;
	Point pt = fd.pt;
	String prevPix_0deg =fd.windowVec.at(pt.y).at(ratioLoc);
	String color = c.getMainColor(newPix);
	String prevColor_0deg = c.getMainColor(fd.colorVec.at(pt.y).at(ratioLoc));
	String prevColor_45deg, prevColor_90deg;
	String pix0=newPix,pix45=newPix,pix90=newPix;
	double grayLevel = rgb.getGrayLevel1(newPix);
	double colorLevel = rgb.getColorLevel(newPix);

	double absRatioThresh = 1.35;
	double relRatioThreshUpper = 1.25;
	double relRatioThreshLower = 0.85;
	double absRatio = roundDecimal(grayLevel/colorLevel,2);
	double relRatio_0deg = absRatio/fd.absRatioVec.at(ratioLoc);
	double relRatio_45deg=0, relRatio_90deg=0;
	bool relRatio_45deg_flag = false;
	bool relRatio_90deg_flag = false;
	double localRatioScanSize = fd.localRatioScanSize;

	if(prevColor_0deg=="" || prevColor_0deg=="Black") {
		prevColor_0deg = color;
		prevPix_0deg = newPix;
	}

	if(c.containsColor(toString(4),color.c_str(),"Brown","Pink","Blue")) {
		if(relRatio_0deg>relRatioThreshUpper && absRatio>absRatioThresh) {
			pix0 = "Grey";
		}
		else if(relRatio_0deg>=relRatioThreshLower&&relRatio_0deg<=relRatioThreshUpper && prevColor_0deg=="Grey") {
			pix0 = prevColor_0deg;
		}
		if(pt.y>0) {
			int j=pt.x-1;
			int endY=(pt.y-localRatioScanSize);
			for(int i=(pt.y-1); i>=endY; i--) {
				if(i<0 && j<0) break;

				if(relRatio_45deg_flag==false && j>=0 && i>=0) {
					relRatio_45deg = absRatio/fd.absRatioMat.at(i).at(j);
					relRatio_45deg = roundDecimal(relRatio_45deg,2);
					prevColor_45deg = fd.colorVec.at(i).at(j);
					if(relRatio_45deg>relRatioThreshUpper && absRatio>absRatioThresh) {
						pix45 = "Grey";
						relRatio_45deg_flag = true;
					}
					else if(relRatio_45deg>=relRatioThreshLower&&relRatio_45deg<=relRatioThreshUpper && prevColor_45deg=="Grey") {
						pix45 = prevColor_45deg;
						relRatio_45deg_flag = true;
					}
				}
				--j;
				if(relRatio_90deg_flag==false && i>=0) {
					relRatio_90deg = absRatio/fd.absRatioMat.at(i).at(pt.x);
					relRatio_90deg = roundDecimal(relRatio_90deg,2);
					prevColor_90deg = fd.colorVec.at(i).at(pt.x);
					if(relRatio_90deg>relRatioThreshUpper && absRatio>absRatioThresh) {
						pix90 = "Grey";
						relRatio_90deg_flag = true;
					}
					else if(relRatio_90deg>=relRatioThreshLower&&relRatio_90deg<=relRatioThreshUpper && prevColor_90deg=="Grey") {
						pix90 = prevColor_90deg;
						relRatio_90deg_flag = true;
					}
				}
				if(relRatio_45deg_flag==true && relRatio_90deg_flag==true)
					break;
			}
		}
		if(pix0==pix45||pix0==pix90) {
			newPix=pix0;
			flag=true;
		}
		else if(pix45==pix90) {
			newPix=pix45;
			flag=true;
		}
		else
			newPix = color;
		/*if(pt.x==57 && pt.y==441) {
			cout << "NewPix: " << newPix << endl;
			cout << "Color: " << color << endl;
			cout << "Pix0: " << pix0 << endl;
			cout << "Pix45: " << pix45 << endl;
			cout << "Pix90: " << pix90 << endl;
			cout << "PrevColor0: " << prevColor_0deg << endl;
			cout << "PrevColor45: " << prevColor_45deg << endl;
			cout << "PrevColor90: " << prevColor_90deg << endl;
		}*/
	}

	if(flag==true) return ruleNum;

	return 0;
}

bool specialRules(Mat &img, String &pix, deque< deque<String> > &windowVec, double &indexChange,
					String &shade, String &shadePrev,Point pt,int loc,
					double relRatio, deque<int> &ruleNo, deque< deque<String> > &hslMat,
					deque< deque<String> > &colorVec, FileData &fd) {
	bool flag=false;
	String newShade = shade;
	String newPix = pix;
	deque<int> ruleNumVec;

	ruleNumVec.push_back(rule1(indexChange, shade, newShade));
	//ruleNumVec.push_back(rule5(img,pix,newPix,newShade, pt));
	ruleNumVec.push_back(rule6(pix,newPix,newShade));
	//ruleNumVec.push_back(rule8(newPix,indexChange,pt,windowVec,hueMat,satMat,lumMat));
	ruleNumVec.push_back(rule9(fd,newPix,loc));
	ruleNumVec.push_back(rule7(pix,newPix));
	for(unsigned int i=0; i<ruleNumVec.size(); i++) {
		if(ruleNumVec.at(i)!=0) {
			ruleNo.push_back(ruleNumVec.at(i));
			flag=true;
		}
	}
	//rule2(pix, newPix, newShade, flag, ruleNo);
	//rule3(pix,newPix, flag, ruleNo);
	//rule4(pix, newPix, newShade, flag, ruleNo);

	deque<int>().swap(ruleNumVec);
	shade = newShade;
	pix = newPix;
	return flag;
}

bool specialRules(FileData &fd, String &pix, double &indexChange, String &shade, int loc,
					deque<int> &ruleNo) {
	bool flag=false;
	String newShade = shade;
	String newPix = pix;
	deque<int> ruleNumVec;

	ruleNumVec.push_back(rule1(indexChange, shade, newShade));
	//ruleNumVec.push_back(rule5(img,pix,newPix,newShade, pt));
	ruleNumVec.push_back(rule6(pix,newPix,newShade));
	//ruleNumVec.push_back(rule8(newPix,indexChange,pt,windowVec,hueMat,satMat,lumMat));
	ruleNumVec.push_back(rule9(fd,newPix,loc));
	ruleNumVec.push_back(rule7(pix,newPix));
	for(unsigned int i=0; i<ruleNumVec.size(); i++) {
		if(ruleNumVec.at(i)!=0) {
			ruleNo.push_back(ruleNumVec.at(i));
			flag=true;
		}
	}
	//rule2(pix, newPix, newShade, flag, ruleNo);
	//rule3(pix,newPix, flag, ruleNo);
	//rule4(pix, newPix, newShade, flag, ruleNo);

	deque<int>().swap(ruleNumVec);
	shade = newShade;
	pix = newPix;
	return flag;
}
