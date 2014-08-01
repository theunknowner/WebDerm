/*
 * rules.cpp
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#include "rules.h"

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

/** General rule 2 - Assigning Gray+Color **/
double rule2(FileData &fd, String &newPix) {
	double ruleNum = 2;
	bool flag=false;
	hsl hsl;
	Color c;
	Functions fn;
	double pThreshMove=1.15;
	int index=-1;
	double hue = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(fd.pt.x),';',1);
	double sat = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(fd.pt.x),';',2);
	double lum = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(fd.pt.x),';',3);
	double fSat=0, fSat2 = 0;
	String color = hsl.getHslColor(hue,sat,lum,index);
	String grey;

	try {
		fSat = (sat-satThresh.at(index).at(1));
		fSat /= (satThresh.at(index).at(1)-satThresh.at(index).at(0));
		fSat *= pThreshMove;
	} catch(const std::out_of_range& oor) {
		printf("Rule2: Index Out of bounds!\n");
		printf("Point(%d,%d)\n",fd.pt.x,fd.pt.y);
		printf("HSL(%0.0f,%0.2f,%0.2f)\n",hue,sat,lum);
		exit(0);
	}
	try {
		grey = hslColors.at(index-1);
	} catch(const std::out_of_range& oor) {
		printf("Rule2: Index Out of bounds!\n");
		printf("Index{%d} out of bounds!\n",index-1);
		printf("Point(%d,%d)\n",fd.pt.x,fd.pt.y);
		printf("HSL(%0.0f,%0.2f,%0.2f)\n",hue,sat,lum);
		exit(0);
	}
	if(!c.containsColor(toString(4),color.c_str(),"Grey","White","Black")) {
		if(grey=="Grey") {
			fSat2 = sat - satThresh.at(index-1).at(1);
			if(fSat<(-1) || fSat2<=0.02) {
				newPix = "Grey" + color;
				flag=true;
			}
		}
	}

	if(flag==true) return ruleNum;

	return 0;
}

double rule3(FileData &fd, String &newPix) {
	double ruleNum =3;
	bool flag=false;
	hsl hsl;
	Color c;
	Functions fn;
	int localScanSize = 30;
	double thresh = 1.04;
	Point pt = fd.pt;
	String color = c.getMainColor(newPix);
	double HSL_0[3] = {0};
	double HSL_45[3] = {0};
	double HSL_90[3] = {0};
	bool deg0_flag=false, deg45_flag=false, deg90_flag=false;
	double result_0=0, result_45=0, result_90=0;
	String color_0, color_45, color_90;
	double s = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2);
	double l = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3);
	if(pt.x==623 && pt.y==308) {
		cout << color << endl;
	}
	if(color=="BrownPink" && pt.y>0) {
		int j=pt.x-1;
		int x = j;
		int endY = (pt.y-localScanSize);
		for(int i=(pt.y-1); i>=endY; i--) {
			if(x<0 && j<0 && i<0) break;
			//deg0_flag==false &&
			if(x>=0) {
				HSL_0[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',1);
				HSL_0[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',2);
				HSL_0[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',3);
				int index=-1;
				color_0 = hsl.getHslColor(HSL_0[0],HSL_0[1],HSL_0[2],index);
				result_0 = ((1-s)*l)/((1-HSL_0[1])*HSL_0[2]);
				result_0 = roundDecimal(result_0,2);
				if(color_0=="BrownPink" && result_0>thresh)
					deg0_flag = true;
			}
			--x;
			//deg45_flag==false &&
			if(j>=0 && i>=0) {
				HSL_45[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
				HSL_45[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
				HSL_45[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
				int index=-1;
				color_45 = hsl.getHslColor(HSL_45[0],HSL_45[1],HSL_45[2],index);
				result_45 = ((1-s)*l)/((1-HSL_45[1])*HSL_45[2]);
				result_45 = roundDecimal(result_45,2);
				if(color_45=="BrownPink" && result_45>thresh)
					deg45_flag = true;
			}
			--j;
			//deg90_flag==false &&
			if(i>=0) {
				HSL_90[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',1);
				HSL_90[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',2);
				HSL_90[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',3);
				int index=-1;
				color_90 = hsl.getHslColor(HSL_90[0],HSL_90[1],HSL_90[2],index);
				result_90 = ((1-s)*l)/((1-HSL_90[1])*HSL_90[2]);
				result_90 = roundDecimal(result_90,2);
				if(color_90=="BrownPink" && result_90>thresh)
					deg90_flag = true;
			}
			if(pt.x==623 && pt.y==308) {
				printf("%s:%.2f ; %s:%.2f ; %s:%.2f\n",color_0.c_str(),result_0,
						color_45.c_str(),result_45,
						color_90.c_str(),result_90);
			}
			/*if(deg45_flag==true && deg90_flag==true)
				break;
			if(deg0_flag==true && deg90_flag==true)
				break;
			if(deg0_flag==true && deg45_flag==true)
				break; */
		}

		if((deg0_flag+deg45_flag)>=2||(deg0_flag+deg90_flag)>=2||(deg45_flag+deg90_flag)>=2) {
			newPix = "Brown";
			flag=true;
		}
	}

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
	Functions fn;
	Point pt = fd.pt;
	String color = c.getMainColor(fd.windowVec.at(pt.y).at(pt.x));
	int index=-1;
	double localScanSize = 20;
	double hue = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',1);
	double sat = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2);
	double lum = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3);
	double promoteThresh = 1.0/1.75;
	double demoteThresh = 1.75;
	double currCL = rgb.getColorLevel(fd.windowVec.at(pt.y).at(pt.x));
	double localCL_0deg, localCL_45deg=0, localCL_90deg=0;
	double relCL_0deg=0,relCL_45deg=0,relCL_90deg=0;

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
				else if(relCL_0deg>=demoteThresh)
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
			/*if(pt.x==300 && pt.y==303) {
				printf("%0.2f ; %0.2f ; %0.2f\n",relCL_0deg,relCL_45deg,relCL_90deg);
				//printf("%.0f ; %.0f ; %.0f\n",localCL_0deg,localCL_45deg,localCL_90deg);
				//printf("%d ; %d ; %d\n",deg0_flag,deg45_flag,deg90_flag);
			}*/
			if(abs(deg45_flag)>10 && abs(deg90_flag)>10)
				break;
			if(abs(deg0_flag)>10 && abs(deg90_flag)>10)
				break;
			if(abs(deg0_flag)>10 && abs(deg45_flag)>10)
				break;
		}
		//if(pt.x==300 && pt.y==303)
			//printf("%d ; %d ; %d\n",deg0_flag,deg45_flag,deg90_flag);

	}

	if(deg0_flag<0) deg0_flag = -1;
	if(deg0_flag>0) deg0_flag = 1;
	if(deg45_flag<0) deg45_flag = -1;
	if(deg45_flag>0) deg45_flag = 1;
	if(deg90_flag<0) deg90_flag = -1;
	if(deg90_flag>0) deg90_flag = 1;
	if((deg0_flag+deg45_flag)>=2||(deg0_flag+deg90_flag)>=2||(deg45_flag+deg90_flag)>=2) {
		lum += 0.10;
		if(lum>1) lum = 1;
		newPix = hsl.getHslColor(hue,sat,lum,index);
		flag=true;
		ruleNum = 8.1;
	}
	if((deg0_flag+deg45_flag)<=-2||(deg0_flag+deg90_flag)<=-2||(deg45_flag+deg90_flag)<=-2) {
		lum -= 0.10;
		if(lum<0) lum=0;
		newPix = hsl.getHslColor(hue,sat,lum,index);
		flag=true;
		ruleNum = 8.2;
	}

	if(flag==true) return ruleNum;

	return 0;
}

/** P2: Special rule 9 - GrayIndexChange - for non-grays that look gray **/
double rule9(FileData &fd, String &newPix) {
	double ruleNum = 9;
	bool flag = false;
	rgb rgb;
	Color c;
	Point pt = fd.pt;
	String color = c.getMainColor(newPix);
	String prevColor_0deg, prevColor_45deg, prevColor_90deg;
	String pix0=newPix,pix45=newPix,pix90=newPix;
	double grayLevel = rgb.getGrayLevel1(newPix);
	double colorLevel = rgb.getColorLevel(newPix);

	double absRatioThresh = 1.35;
	double relRatioThreshUpper = 1.25;
	double relRatioThreshLower = 0.85;
	double absRatio = roundDecimal(grayLevel/colorLevel,2);
	double relRatio_0deg;
	double relRatio_45deg=0, relRatio_90deg=0;
	bool relRatio_0deg_flag = false;
	bool relRatio_45deg_flag = false;
	bool relRatio_90deg_flag = false;
	double localRatioScanSize = fd.localRatioScanSize;

	if(c.containsColor(toString(4),color.c_str(),"Brown","Pink","Blue")) {
		if(pt.y>0) {
			int j=pt.x-1; //45deg
			int x = j; //0deg
			int endY=(pt.y-localRatioScanSize);
			for(int i=(pt.y-1); i>=endY; i--) {
				if(x<0 && j<0 && i<0) break;

				if(relRatio_0deg_flag==false && x>=0) {
					relRatio_0deg = absRatio/fd.absRatioVec.at(x);
					relRatio_0deg = roundDecimal(relRatio_0deg,2);
					prevColor_0deg = fd.colorVec.at(pt.y).at(x);
					if(relRatio_0deg>relRatioThreshUpper && absRatio>absRatioThresh && grayLevel>=70) {
						pix0 = "Grey";
						relRatio_0deg_flag = true;
					}
					else if(relRatio_0deg>=relRatioThreshLower&&relRatio_0deg<=relRatioThreshUpper && prevColor_0deg=="Grey" && grayLevel>=70) {
						pix45 = prevColor_0deg;
						relRatio_0deg_flag = true;
					}
				}
				--x;
				if(relRatio_45deg_flag==false && j>=0 && i>=0) {
					relRatio_45deg = absRatio/fd.absRatioMat.at(i).at(j);
					relRatio_45deg = roundDecimal(relRatio_45deg,2);
					prevColor_45deg = fd.colorVec.at(i).at(j);
					if(relRatio_45deg>relRatioThreshUpper && absRatio>absRatioThresh && grayLevel>=70) {
						pix45 = "Grey";
						relRatio_45deg_flag = true;
					}
					else if(relRatio_45deg>=relRatioThreshLower&&relRatio_45deg<=relRatioThreshUpper && prevColor_45deg=="Grey" && grayLevel>=70) {
						pix45 = prevColor_45deg;
						relRatio_45deg_flag = true;
					}
				}
				--j;
				if(relRatio_90deg_flag==false && i>=0) {
					relRatio_90deg = absRatio/fd.absRatioMat.at(i).at(pt.x);
					relRatio_90deg = roundDecimal(relRatio_90deg,2);
					prevColor_90deg = fd.colorVec.at(i).at(pt.x);
					if(relRatio_90deg>relRatioThreshUpper && absRatio>absRatioThresh && grayLevel>=70) {
						pix90 = "Grey";
						relRatio_90deg_flag = true;
					}
					else if(relRatio_90deg>=relRatioThreshLower&&relRatio_90deg<=relRatioThreshUpper && prevColor_90deg=="Grey" && grayLevel>=70) {
						pix90 = prevColor_90deg;
						relRatio_90deg_flag = true;
					}
				}
				if(relRatio_0deg_flag==true && relRatio_45deg_flag==true && relRatio_90deg_flag==true)
					break;

				/*if(pt.x==451 && pt.y==361) {
					printf("%0.2f ; %0.2f ; %0.2f\n",relRatio_0deg,relRatio_45deg,relRatio_90deg);
					//printf("(%.0f;%.3f;%.0f;%.3f)", gl_0deg,glRatio_0deg,cl_0deg,clRatio_0deg);
					//printf("(%.0f;%.3f;%.0f;%.3f)", gl_45deg,glRatio_45deg,cl_45deg,clRatio_45deg);
					//printf("(%.0f;%.3f;%.0f;%.3f)\n", gl_90deg,glRatio_90deg,cl_90deg,clRatio_90deg);
				}*/
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
	ruleNumVec.push_back(rule2(fd,newPix));
	ruleNumVec.push_back(rule3(fd,newPix));
	ruleNumVec.push_back(rule6(pix,newPix,newShade));
	ruleNumVec.push_back(rule9(fd,newPix));
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
