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

/** general rule #1 - Contrast with IndexChange**/
double rule1(double &indexChange, String &shade, String &newShade) {
	Intensity in;
	bool flag=false;
	double ruleNum=1;
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
double rule2(String &pix, String &newPix, String &newShade) {
	bool flag=false;
	double ruleNum=2;
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
double rule3(String &pix, String &newPix) {
	bool flag=false;
	double ruleNum=3;
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
double rule4(String &pix, String &newPix, String &newShade) {
	bool flag=false;
	double ruleNum=4;
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

/** Special Rule #5 - Determining Dark Grey **/
double rule5(Mat &img, String &pix, String &newPix, String &newShade, Point pt) {
	double ruleNum=5;
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

/** rule 6 - Assign Dark Grey **/
double rule6(String& pix, String& newPix, String& newShade) {
	double ruleNum = 6;
	bool flag = false;
	rgb rgb;
	Color c;
	String color = c.getMainColor(newPix);
	double grayLevel = rgb.getGrayLevel1(newPix);

	if(newShade=="Dark") {
		if(grayLevel>=90) {
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

/** P0: General rule 7 - Fix non-existent double colors **/
double rule7(String &pix, String &newPix) {
	double ruleNum = 7;
	bool flag=false;
	Color c;
	String color = c.getMainColor(newPix);
	if(color.find("Blue")!=string::npos && color.find("Purple")!=string::npos) {
		newPix = "Purple";
		flag=true;
	}

	if(flag==true) return ruleNum;

	return 0;
}

/** P1: Special rule 8- Using indexChange to promote colors on boundaries */
double rule8(FileData &fd, String &newPix, int loc) {
	double ruleNum = 8;
	bool flag=false;
	hsl hsl;
	rgb rgb;
	Color c;
	Intensity in;
	Point pt = fd.pt;
	String color = c.getMainColor(fd.windowVec.at(pt.y).at(pt.x));
	int index=-1;
	double localScanSize = 20;
	//double indexChangeThresh = 1.0; //different than thresh for contrast of shades
	//double pThreshMove = 1.35;
	double hue = getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',1);
	double sat = getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2);
	double lum = getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3);
	//double fSat_promote=0;
	//double fSat_demote=0;
	double promoteThresh = 1.0/1.45;
	double demoteThresh = 1.25;
	//double currIntensity = fd.smoothIntensityVec.at(pt.y).at(pt.x);
	//double localIntensity = fd.smoothIntensityVec.at(pt.y).at(loc);
	double currCL = rgb.getColorLevel(fd.windowVec.at(pt.y).at(pt.x));
	double localCL_0deg, localCL_45deg=0, localCL_90deg=0;
	double relCL_0deg=0,relCL_45deg=0,relCL_90deg=0;
	//double intensity_45deg=0, intensity_90deg=0;
	//double indexChange_0deg=0, indexChange_45deg=0, indexChange_90deg=0;

	int deg0_flag=0; // flag =  -1->demote; 1->promote
	int deg45_flag=0;
	int deg90_flag=0;
	String shade_0deg, shade_45deg, shade_90deg;
	hsl.getHslColor(hue,sat,lum,index);
	if(index<0) {
		printf("Rule8: Index Out of bounds!\n");
		printf("Point(%d,%d)\n",pt.x,pt.y);
		printf("HSL(%0.0f,%0.2f,%0.2f)\n",hue,sat,lum);
		exit(0);
	}

	if(pt.y>0) {
		int j = pt.x-1; //45deg
		int x = j; //0deg
		int endY = (pt.y-localScanSize);
		for(int i=(pt.y-1); i>=endY; i--) {
			if(x<0 && j<0 && i<0) break;
			if(x>=0) {
				localCL_0deg = rgb.getColorLevel(fd.windowVec.at(pt.y).at(x));
				relCL_0deg = currCL/localCL_0deg;
				if(relCL_0deg<=promoteThresh)
					deg0_flag += 1;
				if(relCL_0deg>=demoteThresh)
					deg0_flag -= 1;
			}
			--x;
			if(j>=0 && i>=0) {
				localCL_45deg = rgb.getColorLevel(fd.windowVec.at(i).at(j));
				relCL_45deg = currCL/localCL_45deg;
				if(relCL_45deg<=promoteThresh)
					deg45_flag += 1;
				if(relCL_45deg>=demoteThresh)
					deg45_flag -= 1;
			}
			--j;
			if(i>=0) {
				localCL_90deg = rgb.getColorLevel(fd.windowVec.at(i).at(pt.x));
				relCL_90deg = currCL/localCL_90deg;
				if(relCL_90deg<=promoteThresh)
					deg90_flag += 1;
				if(relCL_90deg>=demoteThresh)
					deg90_flag -= 1;
			}
			if(pt.x==300 && pt.y==303) {
				printf("%0.2f ; %0.2f ; %0.2f\n",relCL_0deg,relCL_45deg,relCL_90deg);
				//printf("%.0f ; %.0f ; %.0f\n",localCL_0deg,localCL_45deg,localCL_90deg);
				//printf("%d ; %d ; %d\n",deg0_flag,deg45_flag,deg90_flag);
			}
			if(abs(deg45_flag)>10 && abs(deg90_flag)>10)
				break;
			if(abs(deg0_flag)>10 && abs(deg90_flag)>10)
				break;
			if(abs(deg0_flag)>10 && abs(deg45_flag)>10)
				break;
		}
		if(pt.x==300 && pt.y==303)
			printf("%d ; %d ; %d\n",deg0_flag,deg45_flag,deg90_flag);

	}

	if(deg0_flag<0) deg0_flag = -1;
	if(deg0_flag>0) deg0_flag = 1;
	if(deg45_flag<0) deg45_flag = -1;
	if(deg45_flag>0) deg45_flag = 1;
	if(deg90_flag<0) deg90_flag = -1;
	if(deg90_flag>0) deg90_flag = 1;
	if((deg0_flag+deg45_flag)>=2||(deg0_flag+deg90_flag)>=2||(deg45_flag+deg90_flag)>=2) {
		lum += 0.10;
		newPix = hsl.getHslColor(hue,sat,lum,index);
		flag=true;
		ruleNum = 8.1;
	}
	if((deg0_flag+deg45_flag)<=-2||(deg0_flag+deg90_flag)<=-2||(deg45_flag+deg90_flag)<=-2) {
		lum -= 0.10;
		newPix = hsl.getHslColor(hue,sat,lum,index);
		flag=true;
		ruleNum = 8.2;
	}

	if(flag==true) return ruleNum;

	return 0;
}

/** P2: Special rule 9 - GrayIndexChange - for non-grays that look gray **/
double rule9(FileData &fd, String &newPix, int ratioLoc) {
	double ruleNum = 9;
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

	if(prevColor_0deg=="" || prevColor_0deg=="Zero") {
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
	}

	if(flag==true) return ruleNum;

	return 0;
}

bool specialRules(FileData &fd, String &pix, double &indexChange, String &shade,
					int ratioLoc, int loc, deque<double> &ruleNo) {
	bool flag=false;
	String newShade = shade;
	String newPix = pix;
	deque<double> ruleNumVec;

	ruleNumVec.push_back(rule1(indexChange, shade, newShade));
	//ruleNumVec.push_back(rule5(img,pix,newPix,newShade, pt));
	ruleNumVec.push_back(rule6(pix,newPix,newShade));
	ruleNumVec.push_back(rule9(fd,newPix,ratioLoc));
	ruleNumVec.push_back(rule8(fd,newPix,loc));
	ruleNumVec.push_back(rule7(pix,newPix));
	for(unsigned int i=0; i<ruleNumVec.size(); i++) {
		if(ruleNumVec.at(i)!=0) {
			ruleNo.push_back(ruleNumVec.at(i));
			flag=true;
		}
	}
	deque<double>().swap(ruleNumVec);
	shade = newShade;
	pix = newPix;
	return flag;
}
