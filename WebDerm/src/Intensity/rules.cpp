/*
 * rules.cpp
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#include "rules.h"

/** general rule #1 - Contrast with IndexChange**/
double rule1(double &indexChange, String &shade, String &newShade) {
	Shades sh;
	bool flag=false;
	double ruleNum=1;
	double indexChangeThresh = 2.25; // different than thresh for contrast of colors
	if(abs(indexChange)>=indexChangeThresh) {
		int index = sh.getShadeIndex(shade);
		index += (indexChange/indexChangeThresh);
		newShade = sh.getShade(index);
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
	//if(fd.pt.x==699 && fd.pt.y==0) {
	//	cout << hue << endl << sat << endl << lum << endl;
	//}
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

// contrast rule
double rule3(FileData &fd, String &newPix) {
	double ruleNum =3;
	bool flag=false;
	Color c;
	Functions fn;
	Point pt = fd.pt;
	int localScanSize = 20;
	String color = c.getMainColor(newPix);
	int HSL[3] = {0};
	HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',1);
	HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2)*100;
	HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3)*100;
	HSL[0] = (int)(HSL[0] - floor(HSL[0]/180.) * 360);
	HSL[1] = 100-HSL[1];
	double currCumHSL = HSL[0] + HSL[1] + HSL[2];
	fd.cumHslMat.at(pt.y).at(pt.x) = currCumHSL;
	int HSL_0[2][3] = {0};
	int HSL_45[2][3] = {0};
	int HSL_90[2][3] = {0};
	Point hslPt_0[2];
	Point hslPt_45[2];
	Point hslPt_90[2];
	double unitThresh[3] = {6.0,5.0,4.0};
	deque<double> minMaxBucket0;
	deque<double> minMaxBucket45;
	deque<double> minMaxBucket90;
	deque<Point> minMaxBucketPts0;
	deque<Point> minMaxBucketPts45;
	deque<Point> minMaxBucketPts90;
	String prevColor_0, prevColor_45, prevColor_90;

	if((color=="BrownPink" || color=="Brown") && pt.y>0) {
		int j=pt.x-1;
		int x = j;
		int endY = (pt.y-localScanSize);
		for(int i=(pt.y-1); i>=endY; i--) {
			if(x<0 && j<0 && i<0) break;
			double currHSL = 0;
			if(x>=0) {
				HSL_0[0][0] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',1);
				HSL_0[0][1] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',2)*100;
				HSL_0[0][2] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',3)*100;
				HSL_0[0][0] = (int)(HSL_0[0][0] - floor(HSL_0[0][0]/180.) * 360);
				HSL_0[0][1] = 100-HSL_0[0][1];
				currHSL = HSL_0[0][0] + HSL_0[0][1] + HSL_0[0][2];
				minMaxBucket0.push_back(currHSL);
				minMaxBucketPts0.push_back(Point(x,pt.y));
			}
			--x;
			if(j>=0 && i>=0) {
				HSL_45[0][0] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
				HSL_45[0][1] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2)*100;
				HSL_45[0][2] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3)*100;
				HSL_45[0][0] = (int)(HSL_45[0][0] - floor(HSL_45[0][0]/180.) * 360);
				HSL_45[0][1] = 100-HSL_45[0][1];
				currHSL = HSL_45[0][0] + HSL_45[0][1] + HSL_45[0][2];
				minMaxBucket45.push_back(currHSL);
				minMaxBucketPts45.push_back(Point(j,i));
			}
			--j;
			if(i>=0) {
				HSL_90[0][0] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',1);
				HSL_90[0][1] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',2)*100;
				HSL_90[0][2] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',3)*100;
				HSL_90[0][0] = (int)(HSL_90[0][0] - floor(HSL_90[0][0]/180.) * 360);
				HSL_90[0][1] = 100-HSL_90[0][1];
				currHSL = HSL_90[0][0] + HSL_90[0][1] + HSL_90[0][2];
				minMaxBucket90.push_back(currHSL);
				minMaxBucketPts90.push_back(Point(pt.x,i));
			}
		}
		int minIndex=0, maxIndex=0;
		fn.getMin(minMaxBucket0,minIndex);
		hslPt_0[0] = minMaxBucketPts0.at(minIndex);
		fn.getMin(minMaxBucket45,minIndex);
		hslPt_45[0] = minMaxBucketPts45.at(minIndex);
		fn.getMin(minMaxBucket90,minIndex);
		hslPt_90[0] = minMaxBucketPts90.at(minIndex);

		fn.getMax(minMaxBucket0,maxIndex);
		hslPt_0[1] = minMaxBucketPts0.at(maxIndex);
		fn.getMax(minMaxBucket45,maxIndex);
		hslPt_45[1] = minMaxBucketPts45.at(maxIndex);
		fn.getMax(minMaxBucket90,maxIndex);
		hslPt_90[1] = minMaxBucketPts90.at(maxIndex);

		double deltaHSL_0[2][3] = {0};
		double deltaHSL_45[2][3] = {0};
		double deltaHSL_90[2][3] = {0};
		double measuredContrast_0[2] = {0};
		double measuredContrast_45[2] ={0};
		double measuredContrast_90[2] = {0};
		double tempHSL_0[2][3] = {0};
		double tempHSL_45[2][3] = {0};
		double tempHSL_90[2][3] = {0};
		for(int i=0; i<2; i++) {
			for(int j=0; j<3; j++) {
				tempHSL_0[i][j] = fn.getDelimitedValuesFromString(fd.hslMat.at(hslPt_0[i].y).at(hslPt_0[i].x),';',j+1);
				tempHSL_45[i][j] = fn.getDelimitedValuesFromString(fd.hslMat.at(hslPt_45[i].y).at(hslPt_45[i].x),';',j+1);
				tempHSL_90[i][j] = fn.getDelimitedValuesFromString(fd.hslMat.at(hslPt_90[i].y).at(hslPt_90[i].x),';',j+1);
				if(j==0) {
					HSL_0[i][j] = (int)(tempHSL_0[i][j] - floor(tempHSL_0[i][j]/180.) * 360);
					HSL_45[i][j] = (int)(tempHSL_45[i][j] - floor(tempHSL_45[i][j]/180.) * 360);
					HSL_90[i][j] = (int)(tempHSL_90[i][j] - floor(tempHSL_90[i][j]/180.) * 360);
				}
				if(j==1) {
					HSL_0[i][j] = (int)(100-(tempHSL_0[i][j]*100));
					HSL_45[i][j] = (int)(100-(tempHSL_45[i][j]*100));
					HSL_90[i][j] = (int)(100-(tempHSL_90[i][j]*100));
				}
				if(j==2) {
					HSL_0[i][j] = (int)(tempHSL_0[i][j]*100);
					HSL_45[i][j] = (int)(tempHSL_45[i][j]*100);
					HSL_90[i][j] = (int)(tempHSL_90[i][j]*100);
				}
				/*
				if(i==0) {
					deltaHSL_0[i][j] = HSL[j] - HSL_0[i][j];
					deltaHSL_45[i][j] = HSL[j]- HSL_45[i][j];
					deltaHSL_90[i][j] = HSL[j] - HSL_90[i][j];
				}
				if(i==1) {
					deltaHSL_0[i][j] = HSL_0[i][j] - HSL[j];
					deltaHSL_45[i][j] = HSL_45[i][j] - HSL[j];
					deltaHSL_90[i][j] = HSL_90[i][j] - HSL[j];
				}*/
				deltaHSL_0[i][j] = HSL[j] - HSL_0[i][j];
				deltaHSL_45[i][j] = HSL[j]- HSL_45[i][j];
				deltaHSL_90[i][j] = HSL[j] - HSL_90[i][j];
				measuredContrast_0[i] += (deltaHSL_0[i][j]/unitThresh[j]);
				measuredContrast_45[i] += (deltaHSL_45[i][j]/unitThresh[j]);
				measuredContrast_90[i] += (deltaHSL_90[i][j]/unitThresh[j]);
			}
		}

		//entering pink
		if(fn.countLesser(4,measuredContrast_0[1],measuredContrast_45[1],measuredContrast_90[1],-2.0)>=2) {
			if(HSL[1]<=70) {
				hsl hsl;
				double hue,sat,lum;
				double currHue, currSat, currLum;
				currHue = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',1);
				currSat = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2);
				currLum = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3);
				int index=-1, currIndex=-1;
				hsl.getHslColor(hue,sat,lum,currIndex);
				double xCurr = satThresh.at(currIndex).at(1) - currSat;
				double x0=0, x45=0, x90=0;
				double xMax0=0, xMax45=0, xMax90=0;
				int countColor0=0,countColor45=0,countColor90=0;
				for(unsigned int i=0; i<minMaxBucket0.size(); i++) {
					Point pt0((pt.x-i+1),pt.y);
					Point pt45((pt.x-i+1),(pt.y-i+1));
					Point pt90(pt.x,(pt.y-i+1));
					if(color==fd.colorVec.at(pt0.y).at(pt0.x)) {
						countColor0++;
						hue = fn.getDelimitedValuesFromString(fd.hslMat.at(pt0.y).at(pt0.x),';',1);
						sat = fn.getDelimitedValuesFromString(fd.hslMat.at(pt0.y).at(pt0.x),';',2);
						lum = fn.getDelimitedValuesFromString(fd.hslMat.at(pt0.y).at(pt0.x),';',3);
						hsl.getHslColor(hue,sat,lum,index);
						x0 = satThresh.at(index).at(1) - sat;
						if(x0>xMax0) xMax0 = x0;
					}
					if(color==fd.colorVec.at(pt45.y).at(pt45.x)) {
						countColor45++;
						hue = fn.getDelimitedValuesFromString(fd.hslMat.at(pt45.y).at(pt45.x),';',1);
						sat = fn.getDelimitedValuesFromString(fd.hslMat.at(pt45.y).at(pt45.x),';',2);
						lum = fn.getDelimitedValuesFromString(fd.hslMat.at(pt45.y).at(pt45.x),';',3);
						hsl.getHslColor(hue,sat,lum,index);
						x45 = satThresh.at(index).at(1) - sat;
						if(x45>xMax45) xMax45 = x45;
					}
					if(color==fd.colorVec.at(pt90.y).at(pt90.x)) {
						countColor90++;
						hue = fn.getDelimitedValuesFromString(fd.hslMat.at(pt90.y).at(pt90.x),';',1);
						sat = fn.getDelimitedValuesFromString(fd.hslMat.at(pt90.y).at(pt90.x),';',2);
						lum = fn.getDelimitedValuesFromString(fd.hslMat.at(pt90.y).at(pt90.x),';',3);
						hsl.getHslColor(hue,sat,lum,index);
						x90 = satThresh.at(index).at(1) - sat;
						if(x90>xMax90) xMax90 = x90;
					}
				}
				if(countColor0<10) xMax0=0;
				if(countColor45<10) xMax45=0;
				if(countColor90<10) xMax90=0;
				if(fn.countGreater(4,xMax0/xCurr,xMax45/xCurr,xMax90/xCurr,5)>=2) {
					newPix = hsl.getHslColor(currIndex+1);
					flag=true;
					ruleNum = 3.1;
				}
			}
		}
		//in pink
		if(fn.countGreaterEqual(4,measuredContrast_0[1],measuredContrast_45[1],measuredContrast_90[1],-2.0)>=2 &&
				fn.countLesserEqual(4,measuredContrast_0[1],measuredContrast_45[1],measuredContrast_90[1],2.0)>=2) {
			if(fn.countLesser(4,measuredContrast_0[0],measuredContrast_45[0],measuredContrast_90[0],2.0)>=2){
				prevColor_0 = fd.colorVec.at(hslPt_0[1].y).at(hslPt_0[1].x);
				prevColor_45 = fd.colorVec.at(hslPt_45[1].y).at(hslPt_45[1].x);
				prevColor_90 = fd.colorVec.at(hslPt_90[1].y).at(hslPt_90[1].x);
				if(fn.countEqual("4",prevColor_0.c_str(),prevColor_45.c_str(),prevColor_90.c_str(),"Pink")>=2) {
					newPix = "Pink";
					flag=true;
					ruleNum = 3.2;
				}
			}
		}

		String strCon = "("+toString(measuredContrast_0[0])+";"+toString(measuredContrast_0[1])+")";
		strCon += "("+toString(measuredContrast_45[0])+";"+toString(measuredContrast_45[1])+")";
		strCon += "("+toString(measuredContrast_90[0])+";"+toString(measuredContrast_90[1])+")";
		fd.m_ContrastMat.at(pt.y).at(pt.x) = strCon;
		String strHsl = "Min("+toString(deltaHSL_0[0][0])+";"+toString(deltaHSL_0[0][1])+";"+toString(deltaHSL_0[0][2])+")";
		strHsl += ";("+toString(deltaHSL_45[0][0])+";"+toString(deltaHSL_45[0][1])+";"+toString(deltaHSL_45[0][2])+")";
		strHsl += ";("+toString(deltaHSL_90[0][0])+";"+toString(deltaHSL_90[0][1])+";"+toString(deltaHSL_90[0][2])+")";
		strHsl += "Max("+toString(deltaHSL_0[1][0])+";"+toString(deltaHSL_0[1][1])+";"+toString(deltaHSL_0[1][2])+")";
		strHsl += ";("+toString(deltaHSL_45[1][0])+";"+toString(deltaHSL_45[1][1])+";"+toString(deltaHSL_45[1][2])+")";
		strHsl += ";("+toString(deltaHSL_90[1][0])+";"+toString(deltaHSL_90[1][1])+";"+toString(deltaHSL_90[1][2])+")";
		fd.d_HslMat.at(pt.y).at(pt.x) = strHsl;
		char chArr[100];
		sprintf(chArr,"Min(%d;%d);(%d;%d);(%d;%d)",hslPt_0[0].x+1,hslPt_0[0].y+1,hslPt_45[0].x+1,hslPt_45[0].y+1,hslPt_90[0].x+1,hslPt_90[0].y+1);
		String ptStr(chArr);
		sprintf(chArr,"Max(%d;%d);(%d;%d);(%d;%d)",hslPt_0[1].x+1,hslPt_0[1].y+1,hslPt_45[1].x+1,hslPt_45[1].y+1,hslPt_90[1].x+1,hslPt_90[1].y+1);
		String ptStr2(chArr);
		ptStr += ptStr2;
		fd.hslPtMat.at(pt.y).at(pt.x) = ptStr;
		/**/
	} // end if(brownpink)
	else {
		fd.m_ContrastMat.at(pt.y).at(pt.x) = color;
		fd.d_HslMat.at(pt.y).at(pt.x) = color;
		fd.hslPtMat.at(pt.y).at(pt.x) = color;
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

	if(newShade.find("Dark")!=string::npos) {
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
