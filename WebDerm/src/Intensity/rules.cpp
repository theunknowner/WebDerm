/*
 * rules.cpp
 *
 *  Created on: May 28, 2014
 *      Author: jason
 */

#include "rules.h"

/** general rule #1 - Shade Contrast with IndexChange**/
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

/** General rule #2 - Assigning Gray+Color **/
double rule2(FileData &fd, String &newPix) {
	double ruleNum = 2;
	bool flag=false;
	Hsl hsl;
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
		if((index-1)>=0)
			grey = hslColors.at(index-1);
	} catch(const std::out_of_range& oor) {
		printf("Rule2: Index Out of bounds!\n");
		printf("Index{%d} out of bounds!\n",index-1);
		printf("Point(%d,%d)\n",fd.pt.x,fd.pt.y);
		printf("HSL(%0.0f,%0.2f,%0.2f)\n",hue,sat,lum);
		exit(0);
	}
	if(!c.containsColor(toString(5),color.c_str(),"Grey","White","Black","Zero")) {
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

//rule#3 - contrast rule for Brown/BrownPink/Pink
double rule3(FileData &fd, String &newPix, String &newShade) {
	double ruleNum =3;
	bool flag=false;
	Color c;
	Functions fn;
	Shades sh;
	Point pt = fd.pt;
	int localScanSize = 20;
	String color = c.getMainColor(newPix);
	int HSL[3] = {0};
	try {
		HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',1);
		HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2)*100;
		HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3)*100;
	} catch (const std::out_of_range &oor) {
		printf("Rule3: Try/Catch #1 Out of Range!\n");
		printf("fd.hslMat.size(): %lu\n",fd.hslMat.size());
		printf("Point(%d,%d)\n",pt.x,pt.y);
	}
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
	double deltaHSL_0[2][3] = {0};
	double deltaHSL_45[2][3] = {0};
	double deltaHSL_90[2][3] = {0};
	double measuredContrast_0[2] = {0};
	double measuredContrast_45[2] ={0};
	double measuredContrast_90[2] = {0};

	String shade0="",shade45="",shade90="";
	int currShadeIndex=-1,shadeIndex0=-1,shadeIndex45=-1,shadeIndex90=-1;
	currShadeIndex = sh.getShadeIndex(newShade);
	int flag0=0, flag45=0, flag90=0;

	if((color=="BrownPink" || color=="Brown") && HSL[0]<30 && pt.y>0) {
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

		int minIndex=-1, maxIndex=-1;
		// getMin|getMax returns -1 when bucket is empty due to being at
		// col=0 or row=0
		fn.getMin(minMaxBucket0,minIndex);
		if(minIndex<0) hslPt_0[0] = Point(pt.x,pt.y);
		else hslPt_0[0] = minMaxBucketPts0.at(minIndex);

		fn.getMin(minMaxBucket45,minIndex);
		if(minIndex<0) hslPt_45[0] = Point(pt.x,pt.y);
		else hslPt_45[0] = minMaxBucketPts45.at(minIndex);

		fn.getMin(minMaxBucket90,minIndex);
		if(minIndex<0) hslPt_90[0] = Point(pt.x,pt.y);
		else hslPt_90[0] = minMaxBucketPts90.at(minIndex);

		fn.getMax(minMaxBucket0,maxIndex);
		if(maxIndex<0) hslPt_0[1] = Point(pt.x,pt.y);
		else hslPt_0[1] = minMaxBucketPts0.at(maxIndex);

		fn.getMax(minMaxBucket45,maxIndex);
		if(maxIndex<0) hslPt_45[1] = Point(pt.x,pt.y);
		else hslPt_45[1] = minMaxBucketPts45.at(maxIndex);

		fn.getMax(minMaxBucket90,maxIndex);
		if(maxIndex<0) hslPt_90[1] = Point(pt.x,pt.y);
		else hslPt_90[1] = minMaxBucketPts90.at(maxIndex);

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
				if(fn.countGreater(4,round(abs(deltaHSL_0[1][0]/unitThresh[0])),round(abs(deltaHSL_45[1][0]/unitThresh[0])),round(abs(deltaHSL_90[1][0]/unitThresh[0])),0.)>=2) {
					if(color=="Brown") newPix=="BrownPink";
					if(color=="BrownPink") newPix = "Pink";
					flag=true;
					ruleNum = 3.1;
				}
			}
		}
		//in Pink/BrownPink
		if(fn.countGreaterEqual(4,measuredContrast_0[1],measuredContrast_45[1],measuredContrast_90[1],-2.0)>=2 &&
				fn.countLesserEqual(4,measuredContrast_0[1],measuredContrast_45[1],measuredContrast_90[1],2.0)>=2) {
			if(fn.countLesser(4,measuredContrast_0[0],measuredContrast_45[0],measuredContrast_90[0],1.0)>=2){
				if(HSL[1]<=70) {
					prevColor_0 = c.getMainColor(fd.colorVec.at(hslPt_0[1].y).at(hslPt_0[1].x));
					prevColor_45 = c.getMainColor(fd.colorVec.at(hslPt_45[1].y).at(hslPt_45[1].x));
					prevColor_90 = c.getMainColor(fd.colorVec.at(hslPt_90[1].y).at(hslPt_90[1].x));
					if(fn.countEqual("4",prevColor_0.c_str(),prevColor_45.c_str(),prevColor_90.c_str(),"Pink")>=2) {
						if(color=="Brown") newPix=="BrownPink";
						if(color=="BrownPink") newPix = "Pink";
						flag=true;
						ruleNum = 3.2;
					}
					else if(fn.countEqual("4",prevColor_0.c_str(),prevColor_45.c_str(),prevColor_90.c_str(),"BrownPink")>=2) {
						shade0 = c.extractShade(fd.colorVec.at(hslPt_0[1].y).at(hslPt_0[1].x));
						shade45 = c.extractShade(fd.colorVec.at(hslPt_45[1].y).at(hslPt_45[1].x));
						shade90 = c.extractShade(fd.colorVec.at(hslPt_90[1].y).at(hslPt_90[1].x));
						shadeIndex0 = sh.getShadeIndex(shade0);
						shadeIndex45 = sh.getShadeIndex(shade45);
						shadeIndex90 = sh.getShadeIndex(shade90);
						if((shadeIndex0-currShadeIndex)>=1 && (shadeIndex0-currShadeIndex)<=2) flag0=1;
						if((shadeIndex45-currShadeIndex)>=1 && (shadeIndex45-currShadeIndex)<=2) flag45=1;
						if((shadeIndex90-currShadeIndex)>=1 && (shadeIndex90-currShadeIndex)<=2) flag90=1;
						if(fn.countEqual(4,flag0,flag45,flag90,1)>=2) {
							if(fn.countLesserEqual(4,deltaHSL_0[1][0],deltaHSL_45[1][0],deltaHSL_90[1][0],-3.0)>=2) {
								if(color=="Brown") newPix=="BrownPink";
								if(color=="BrownPink") newPix = "Pink";
								flag=true;
								ruleNum = 3.21;
							}
						}
					}
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
		sprintf(chArr,"Min(%d;%d);(%d;%d);(%d;%d)",hslPt_0[0].x,hslPt_0[0].y,hslPt_45[0].x,hslPt_45[0].y,hslPt_90[0].x,hslPt_90[0].y);
		String ptStr(chArr);
		sprintf(chArr,"Max(%d;%d);(%d;%d);(%d;%d)",hslPt_0[1].x,hslPt_0[1].y,hslPt_45[1].x,hslPt_45[1].y,hslPt_90[1].x,hslPt_90[1].y);
		String ptStr2(chArr);
		ptStr += ptStr2;
		fd.hslPtMat.at(pt.y).at(pt.x) = ptStr;
		/**/
	} // end if(brownpink)

	else if(color=="Pink" && HSL[0]<30 && pt.y>0) {
		int j=pt.x-1;
		int x = j;
		int endY = (pt.y-localScanSize);
		for(int i=(pt.y-1); i>=endY; i--) {
			if(x<0 && j<0 && i<0) break;
			double currI = 0;
			if(x>=0) {
				currI = fd.smoothIntensityVec.at(pt.y).at(x);
				minMaxBucket0.push_back(currI);
				minMaxBucketPts0.push_back(Point(x,pt.y));
			}
			--x;
			if(j>=0 && i>=0) {
				currI = fd.smoothIntensityVec.at(i).at(j);
				minMaxBucket45.push_back(currI);
				minMaxBucketPts45.push_back(Point(j,i));
			}
			--j;
			if(i>=0) {
				currI = fd.smoothIntensityVec.at(i).at(pt.x);
				minMaxBucket90.push_back(currI);
				minMaxBucketPts90.push_back(Point(pt.x,i));
			}
		}
		int maxIndex=-1;
		// getMin|getMax returns -1 when bucket is empty due to being at
		// col=0 or row=0
		fn.getMax(minMaxBucket0,maxIndex);
		if(maxIndex<0) hslPt_0[1] = Point(pt.x,pt.y);
		else hslPt_0[1] = minMaxBucketPts0.at(maxIndex);

		fn.getMax(minMaxBucket45,maxIndex);
		if(maxIndex<0) hslPt_45[1] = Point(pt.x,pt.y);
		else hslPt_45[1] = minMaxBucketPts45.at(maxIndex);

		fn.getMax(minMaxBucket90,maxIndex);
		if(maxIndex<0) hslPt_90[1] = Point(pt.x,pt.y);
		else hslPt_90[1] = minMaxBucketPts90.at(maxIndex);

		hslPt_0[0] = Point(pt.x,pt.y);
		hslPt_45[0] = Point(pt.x,pt.y);
		hslPt_90[0] = Point(pt.x,pt.y);

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
				deltaHSL_0[i][j] = HSL[j] - HSL_0[i][j];
				deltaHSL_45[i][j] = HSL[j]- HSL_45[i][j];
				deltaHSL_90[i][j] = HSL[j] - HSL_90[i][j];
				measuredContrast_0[i] += (deltaHSL_0[i][j]/unitThresh[j]);
				measuredContrast_45[i] += (deltaHSL_45[i][j]/unitThresh[j]);
				measuredContrast_90[i] += (deltaHSL_90[i][j]/unitThresh[j]);
			}
		}

		prevColor_0 = c.getMainColor(fd.windowVec.at(hslPt_0[1].y).at(hslPt_0[1].x));
		prevColor_45 = c.getMainColor(fd.windowVec.at(hslPt_45[1].y).at(hslPt_45[1].x));
		prevColor_90 = c.getMainColor(fd.windowVec.at(hslPt_90[1].y).at(hslPt_90[1].x));

		int thresh = 20;

		if(prevColor_0=="Pink") {
			shade0 = c.extractShade(fd.colorVec.at(hslPt_0[1].y).at(hslPt_0[1].x));
			shadeIndex0 = sh.getShadeIndex(shade0);
			if((shadeIndex0-currShadeIndex)>1 && (shadeIndex0-currShadeIndex)<3 && abs(deltaHSL_0[1][1])<=thresh) flag0=1;
		}
		if(prevColor_45=="Pink") {
			shade45 = c.extractShade(fd.colorVec.at(hslPt_45[1].y).at(hslPt_45[1].x));
			shadeIndex45 = sh.getShadeIndex(shade45);
			if((shadeIndex45-currShadeIndex)>1 && (shadeIndex45-currShadeIndex)<3 && abs(deltaHSL_45[1][1])<=thresh) flag45=1;
		}
		if(prevColor_90=="Pink") {
			shade90 = c.extractShade(fd.colorVec.at(hslPt_90[1].y).at(hslPt_90[1].x));
			shadeIndex90 = sh.getShadeIndex(shade90);
			if((shadeIndex90-currShadeIndex)>1 && (shadeIndex90-currShadeIndex)<3 && abs(deltaHSL_90[1][1])<=thresh) flag90=1;
		}

		if(fn.countEqual(4,flag0,flag45,flag90,1)>=2) {
			newPix = "PinkRed";
			flag=true;
			ruleNum = 3.3;
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
		sprintf(chArr,"Min(%d;%d);(%d;%d);(%d;%d)",hslPt_0[0].x,hslPt_0[0].y,hslPt_45[0].x,hslPt_45[0].y,hslPt_90[0].x,hslPt_90[0].y);
		String ptStr(chArr);
		sprintf(chArr,"Max(%d;%d);(%d;%d);(%d;%d)",hslPt_0[1].x,hslPt_0[1].y,hslPt_45[1].x,hslPt_45[1].y,hslPt_90[1].x,hslPt_90[1].y);
		String ptStr2(chArr);
		ptStr += ptStr2;
		fd.hslPtMat.at(pt.y).at(pt.x) = ptStr;

	}/**/
	/*
	else if(color.find("Purple")==string::npos && color.find("Violet")==string::npos && HSL[1]>=75 && pt.y>0) {
		int colorCount0=0, colorCount45=0, colorCount90=0;
		flag0=0;flag45=0;flag90=0;
		int currHSL0[3]={0},currHSL45[3]={0},currHSL90[3]={0};
		int deltaHSL0[3]={0},deltaHSL45[3]={0},deltaHSL90[3]={0};
		String origColor0,origColor45,origColor90;
		int j=pt.x-1;
		int x = j;
		int endY = (pt.y-localScanSize);
		for(int i=(pt.y-1); i>=endY; i--) {
			if(x<0 && j<0 && i<0) break;
			if(x>=0) {
				prevColor_0 = c.getMainColor(fd.colorVec.at(pt.y).at(x));
				currHSL0[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',1);
				currHSL0[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(x),';',3)*100;
				currHSL0[0] = (int)(currHSL0[0] - floor(currHSL0[0]/180.) * 360);
				deltaHSL0[0] = abs(HSL[0]-currHSL0[0]);
				deltaHSL0[2] = abs(HSL[2]-currHSL0[2]);
				if(deltaHSL0[0]<10 && deltaHSL0[2]<=8 && (prevColor_0.find("Violet")!=string::npos || prevColor_0.find("Purple")!=string::npos)) {
					++colorCount0;
				}
			}
			if(j>=0 && i>=0) {
				prevColor_45 = c.getMainColor(fd.colorVec.at(i).at(j));
				currHSL45[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
				currHSL45[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3)*100;
				currHSL45[0] = (int)(currHSL45[0] - floor(currHSL45[0]/180.) * 360);
				deltaHSL45[0] = abs(HSL[0]-currHSL45[0]);
				deltaHSL45[2] = abs(HSL[2]-currHSL45[2]);
				if(deltaHSL45[0]<10 && deltaHSL45[2]<=8 && (prevColor_45.find("Violet")!=string::npos || prevColor_45.find("Purple")!=string::npos)) {
					++colorCount45;
				}
			}
			if(i>=0) {
				prevColor_90 = c.getMainColor(fd.colorVec.at(i).at(pt.x));
				currHSL90[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',1);
				currHSL90[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(pt.x),';',3)*100;
				currHSL90[0] = (int)(currHSL90[0] - floor(currHSL90[0]/180.) * 360);
				deltaHSL90[0] = abs(HSL[0]-currHSL90[0]);
				deltaHSL90[2] = abs(HSL[2]-currHSL90[2]);
				if(deltaHSL90[0]<10 && deltaHSL90[2]<=8 && (prevColor_90.find("Violet")!=string::npos || prevColor_90.find("Purple")!=string::npos)) {
					++colorCount90;
				}
			}
			--x; --j;
		}

		if(colorCount0>=10) flag0=1;
		if(colorCount45>=10) flag45=1;
		if(colorCount90>=10) flag90=1;

		if(fn.countEqual(4,flag0,flag45,flag90,1)>=2) {
			newPix = "GreyPurple";
			flag=true;
			ruleNum = 3.4;
		}
	}/**/
	else {
		fd.m_ContrastMat.at(pt.y).at(pt.x) = color;
		fd.d_HslMat.at(pt.y).at(pt.x) = color;
		fd.hslPtMat.at(pt.y).at(pt.x) = color;
	}

	if(flag==true) return ruleNum;

	return 0;
}

//rule#4 - contrast rule 2 for Grey that looks Violet/Purple
double rule4(FileData &fd, String &newPix, String newShade) {
	double ruleNum=4;
	double flag=false;
	Color c;
	Functions fn;
	Point pt = fd.pt;
	int localScanSize = 20;
	String color = c.getMainColor(newPix);
	String prevColor_0, prevColor_45, prevColor_90;
	int colorCount0=0, colorCount45=0, colorCount90=0;
	int flag0=0, flag45=0, flag90=0;
	int HSL[3] = {0};
	HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',1);
	HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',2)*100;
	HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(pt.y).at(pt.x),';',3)*100;
	if((color.find("Grey")!=string::npos || newShade.find("Dark")!=string::npos) && HSL[1]<=25 && pt.y>0) {
		int j=pt.x-1;
		int x = j;
		int endY = (pt.y-localScanSize);
		for(int i=(pt.y-1); i>=endY; i--) {
			if(x<0 && j<0 && i<0) break;
			if(x>=0) {
				prevColor_0 = c.getMainColor(fd.colorVec.at(pt.y).at(x));
				if(prevColor_0.find("Violet")!=string::npos) {
					++colorCount0;
				}
			}
			--x;
			if(j>=0 && i>=0) {
				prevColor_45 = c.getMainColor(fd.colorVec.at(i).at(j));
				if(prevColor_45.find("Violet")!=string::npos) {
					++colorCount45;
				}
			}
			--j;
			if(i>=0) {
				prevColor_90 = c.getMainColor(fd.colorVec.at(i).at(pt.x));
				if(prevColor_90.find("Violet")!=string::npos) {
					++colorCount90;
				}

			}
		}

		if(colorCount0>=10) flag0=1;
		if(colorCount45>=10) flag45=1;
		if(colorCount90>=10) flag90=1;

		if(fn.countGreaterEqual(4,flag0,flag45,flag90,1)>=2) {
			newPix = "Purple";
			flag=true;
		}
	}
	if(flag==true) return ruleNum;

	return 0;
}

//rule #5 - Pink -> DarkPink
double rule5(FileData &fd, String &newPix, String &newShade) {
	double ruleNum = 5;
	bool flag=false;
	Color c;
	Hsl hsl;
	Rgb rgb;
	Functions fn;
	String color = c.getMainColor(newPix);
	String pix;
	double HSL[3];
	int *RGB;
	double nextHSL[3];
	int *nextRGB;
	const double H = 0.72;
	const double enterDemarcThresh = -0.0046;
	const double exitDemarcThresh = 0.005;
	String shade = "Dark3";
	static Point enterDemarcPos(-1,-1);
	static Point exitDemarcPos(-1,-1);
	static int demarcFlag=0;
	double currentRelLum =0, nextRelLum=0;
	double tempSlope=0, slope=0;
	unsigned int step=fd.pt.x;
	if(enterDemarcPos.y!=fd.pt.y || (color!="Pink" && color!="PinkRed")) {
		enterDemarcPos = Point(-1,-1);
		exitDemarcPos = Point(-1,-1);
		demarcFlag=0;
	}
	if(color=="Pink" || color=="PinkRed") {
		while((color=="Pink" || color=="PinkRed") && step<(fd.hslMat.at(0).size()-1)) {
			try {
				HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',1);
				HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',2);
				HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',3);
				RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
				currentRelLum = rgb.calcPerceivedBrightness(RGB[0],RGB[1],RGB[2])/255.0;
				nextHSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step+1),';',1);
				nextHSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step+1),';',2);
				nextHSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step+1),';',3);
				nextRGB = hsl.hsl2rgb(nextHSL[0],nextHSL[1],nextHSL[2]);
				nextRelLum = rgb.calcPerceivedBrightness(nextRGB[0],nextRGB[1],nextRGB[2])/255.0;

				tempSlope = (nextRelLum - currentRelLum);
				slope = (slope * H) + (tempSlope*(1.0-H));
				//printf() for debugging

				printf("(%d,%d) - Curr:%f, Temp:%f, Slope:%f,",step,fd.pt.y,currentRelLum,tempSlope,slope);
				printf(" HSL(%.f,%.2f,%.2f), Flag: %d\n",HSL[0],HSL[1],HSL[2],demarcFlag);

				///////////////////////
				step++;
				if(slope<=enterDemarcThresh && demarcFlag==0) {
					demarcFlag=1; //entering DarkPink
					break;
				}
				if(slope>=exitDemarcThresh) {
					demarcFlag=1;
					break;
				}
				pix = fd.colorVec.at(fd.pt.y).at(step);
				color = c.getMainColor(pix);

			} catch (const std::out_of_range &oor) {
				printf("\nRule5: Try/Catch #1 Out of Range!\n");
				printf("fd.hslMat.size(): %lu\n",fd.hslMat.size());
				printf("fd.hslMat.at(%d).size(): %lu\n",fd.pt.y,fd.hslMat.at(fd.pt.y).size());
				printf("fd.windowVec.size(): %lu\n",fd.colorVec.size());
				printf("fd.windowVec.at(%d).size(): %lu\n",fd.pt.y,fd.colorVec.at(fd.pt.y).size());
				printf("Point(%d,%d)\n",step,fd.pt.y);
				printf("fd.pt(%d,%d)\n",fd.pt.x,fd.pt.y);
				exit(1);
			}
		}
		//printf() for debugging

		HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',1);
		HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',2);
		HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',3);
		RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
		currentRelLum = rgb.calcPerceivedBrightness(RGB[0],RGB[1],RGB[2])/255.0;
		printf("(%d,%d) - Curr:%f, Flag: %d\n",step,fd.pt.y,currentRelLum,demarcFlag);

		///////////////////////
		if(demarcFlag==1) {
			/*try {
				maxAvgSlope = 0;
				HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',1);
				HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',2);
				HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',3);
				RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
				currentRelLum = rgb.calcPerceivedBrightness(RGB[0],RGB[1],RGB[2])/255.0;

				try {
					nextHSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',1);
					nextHSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',2);
					nextHSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',3);
					nextRGB = hsl.hsl2rgb(nextHSL[0],nextHSL[1],nextHSL[2]);
					nextRelLum = rgb.calcPerceivedBrightness(nextRGB[0],nextRGB[1],nextRGB[2])/255.0;
					avgSlope = currentRelLum - nextRelLum;
					if(abs(avgSlope)>maxAvgSlope) {
						maxAvgSlope = abs(avgSlope);
						if(avgSlope<0 && enterDemarcPos.x==-1 && enterDemarcPos.y==-1) {
							enterDemarcPos = Point(step,fd.pt.y);
						}
						else if(avgSlope>0 && exitDemarcPos.x==-1 && exitDemarcPos.y==-1) {
							enterDemarcPos = Point(fd.pt.x,fd.pt.y);
							exitDemarcPos = Point(step,fd.pt.y);
						}
					}
				} catch (const std::out_of_range &oor) {
					printf("\nRule5: Try/Catch #2B Out of Range1\n");
					printf("fd.hslMat.size(): %lu\n",fd.hslMat.size());
					printf("Point(%d,%d)\n",step,fd.pt.y);

					//printf() for debugging
					//printf("(%d,%d) - Curr:%f, AvgSlope:%f,",i,fd.pt.y,currentRelLum,avgSlope);
					//printf(" HSL(%.f,%.f,%.f)\n",nextHSL[0],nextHSL[1],nextHSL[2]);
					////////////////////////
				}
			} catch (const std::out_of_range &oor) {
				printf("\nRule5: Try/Catch #2A Out of Range1\n");
				printf("fd.hslMat.size(): %lu\n",fd.hslMat.size());
				printf("Point(%d,%d)\n",step,fd.pt.y);
				exit(1);
			}*/
			if(slope<=enterDemarcThresh) {
				enterDemarcPos = Point(step,fd.pt.y);
			}
			else if(slope>=exitDemarcThresh) {
				if(enterDemarcPos.x==-1 && enterDemarcPos.y==-1)
					enterDemarcPos = Point(fd.pt.x,fd.pt.y);
				exitDemarcPos = Point(step,fd.pt.y);
			}
		}

		printf("Flag: %d\n",demarcFlag);
		printf("%d,%d\n",fd.pt.x,fd.pt.y);
		printf("enterDemarcPos - %d,%d\n",enterDemarcPos.x,enterDemarcPos.y);
		printf("exitDemarcPos - %d,%d\n",exitDemarcPos.x,exitDemarcPos.y);

		if(enterDemarcPos.x!=-1 && enterDemarcPos.y!=-1) {
			if(fd.pt.x>=enterDemarcPos.x && fd.pt.y>=enterDemarcPos.y) {
				if(exitDemarcPos.x==-1 && exitDemarcPos.y==-1) {
					newShade = shade;
					newPix = "Pink";
					flag=true;
				}
				else if(fd.pt.x<=exitDemarcPos.x && fd.pt.y<=exitDemarcPos.y) {
					newShade = shade;
					newPix = "Pink";
					flag=true;
				}
				else {
					enterDemarcPos = Point(-1,-1);
					exitDemarcPos = Point(-1,-1);
					demarcFlag=0;
				}
			}
		}
		/*else {
			if(fd.pt.x<=exitDemarcPos.x && fd.pt.y<=exitDemarcPos.y) {
				newShade = shade;
				flag=true;
			}
			else {
				enterDemarcPos = Point(-1,-1);
				exitDemarcPos = Point(-1,-1);
				demarcFlag=0;
			}
		}*/
	}
	if(flag==true) return ruleNum;
	return 0;
}

//rule 5 - Pink to DarkPink
double rule5(FileData &fd) {
	double ruleNum = 5;
	bool flag=false;
	Color c;
	Hsl hsl;
	Rgb rgb;
	Functions fn;
	Shades sh;
	String oldPix, newPix,color,pix, oldShade,newShade;
	double HSL[3];
	int *RGB;
	double nextHSL[3];
	int *nextRGB;
	const double H = 0.72;
	const double enterDemarcThresh = -0.0046;
	const double exitDemarcThresh = 0.005;
	String shade = "Dark3";
	Point enterDemarcPos(-1,-1);
	Point exitDemarcPos(-1,-1);
	int demarcFlag=0;
	double currentRelLum =0, nextRelLum=0;
	double tempSlope=0, slope=0;
	unsigned int step=0;
	//size_t pos1=0, pos2=0;
	for(unsigned int i=0; i<fd.colorVec.size(); i++) {
		for(unsigned int j=0; j<fd.colorVec.at(i).size(); j++) {
			fd.pt = Point(j,i);
			oldPix = c.getMainColor(fd.colorVec.at(i).at(j));
			oldShade = sh.extractShade(fd.colorVec.at(i).at(j));
			newPix = oldPix;
			newShade = oldShade;
			color = newPix;
			step = fd.pt.x;
			slope=0;
			if(enterDemarcPos.y!=fd.pt.y || (color!="Pink" && color!="PinkRed")) {
				enterDemarcPos = Point(-1,-1);
				exitDemarcPos = Point(-1,-1);
				demarcFlag=0;
			}
			if(color=="Pink" || color=="PinkRed") {
				while((color=="Pink" || color=="PinkRed") && step<(fd.hslMat.at(0).size()-1)) {
					try {
						HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',1);
						HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',2);
						HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',3);
						RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
						currentRelLum = rgb.calcPerceivedBrightness(RGB[0],RGB[1],RGB[2])/255.0;
						nextHSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step+1),';',1);
						nextHSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step+1),';',2);
						nextHSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step+1),';',3);
						nextRGB = hsl.hsl2rgb(nextHSL[0],nextHSL[1],nextHSL[2]);
						nextRelLum = rgb.calcPerceivedBrightness(nextRGB[0],nextRGB[1],nextRGB[2])/255.0;

						tempSlope = (nextRelLum - currentRelLum);
						slope = (slope * H) + (tempSlope*(1.0-H));
						//printf() for debugging
/*
						printf("(%d,%d) - Curr:%f, Temp:%f, Slope:%f,",step,fd.pt.y,currentRelLum,tempSlope,slope);
						printf(" HSL(%.f,%.2f,%.2f), Flag: %d\n",HSL[0],HSL[1],HSL[2],demarcFlag);
*/
						///////////////////////
						step++;
						if(slope<=enterDemarcThresh && demarcFlag==0) {
							demarcFlag=1; //entering DarkPink
							break;
						}
						if(slope>=exitDemarcThresh) {
							demarcFlag=1;
							break;
						}
						pix = fd.colorVec.at(fd.pt.y).at(step);
						color = c.getMainColor(pix);

					} catch (const std::out_of_range &oor) {
						printf("\nRule5: Try/Catch #1 Out of Range!\n");
						printf("fd.hslMat.size(): %lu\n",fd.hslMat.size());
						printf("fd.hslMat.at(%d).size(): %lu\n",fd.pt.y,fd.hslMat.at(fd.pt.y).size());
						printf("fd.windowVec.size(): %lu\n",fd.colorVec.size());
						printf("fd.windowVec.at(%d).size(): %lu\n",fd.pt.y,fd.colorVec.at(fd.pt.y).size());
						printf("Point(%d,%d)\n",step,fd.pt.y);
						printf("fd.pt(%d,%d)\n",fd.pt.x,fd.pt.y);
						exit(1);
					}
				}
				//printf() for debugging
/*
				HSL[0] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',1);
				HSL[1] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',2);
				HSL[2] = fn.getDelimitedValuesFromString(fd.hslMat.at(fd.pt.y).at(step),';',3);
				RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
				currentRelLum = rgb.calcPerceivedBrightness(RGB[0],RGB[1],RGB[2])/255.0;
				printf("(%d,%d) - Curr:%f, Flag: %d\n",step,fd.pt.y,currentRelLum,demarcFlag);
*/
				///////////////////////
				if(demarcFlag==1) {
					if(slope<=enterDemarcThresh) {
						enterDemarcPos = Point(step,fd.pt.y);
					}
					else if(slope>=exitDemarcThresh) {
						if(enterDemarcPos.x==-1 && enterDemarcPos.y==-1)
							enterDemarcPos = Point(fd.pt.x,fd.pt.y);
						exitDemarcPos = Point(step,fd.pt.y);
					}
				}
				/*
				printf("Flag: %d\n",demarcFlag);
				printf("%d,%d\n",fd.pt.x,fd.pt.y);
				printf("enterDemarcPos - %d,%d\n",enterDemarcPos.x,enterDemarcPos.y);
				printf("exitDemarcPos - %d,%d\n",exitDemarcPos.x,exitDemarcPos.y);
				*/
				if(enterDemarcPos.x!=-1 && enterDemarcPos.y!=-1) {
					if(fd.pt.x>=enterDemarcPos.x && fd.pt.y>=enterDemarcPos.y) {
						if(exitDemarcPos.x==-1 && exitDemarcPos.y==-1) {
							for(unsigned int k=j; k<=step; k++) {
								oldPix = c.getMainColor(fd.colorVec.at(i).at(k));
								oldShade = sh.extractShade(fd.colorVec.at(i).at(k));
								newShade = shade;
								newPix = "Pink";
								//pos1 = fd.colorVec.at(i).at(k).find(oldPix);
								//pos2 = fd.colorVec.at(i).at(k).find(oldShade);
								//fd.colorVec.at(i).at(k).replace(pos1,oldPix.length(),newPix);
								//fd.colorVec.at(i).at(k).replace(pos2,oldShade.length(),newShade);
								fd.colorVec.at(i).at(k) = newShade+newPix;
							}
							flag=true;
						}
						else if(fd.pt.x<=exitDemarcPos.x && fd.pt.y<=exitDemarcPos.y) {
							for(unsigned int k=j; k<=step; k++) {
								oldPix = c.getMainColor(fd.colorVec.at(i).at(k));
								oldShade = sh.extractShade(fd.colorVec.at(i).at(k));
								newShade = shade;
								newPix = "Pink";
								//pos1 = fd.colorVec.at(i).at(k).find(oldPix);
								//pos2 = fd.colorVec.at(i).at(k).find(oldShade);
								//fd.colorVec.at(i).at(k).replace(pos1,oldPix.length(),newPix);
								//fd.colorVec.at(i).at(k).replace(pos2,oldShade.length(),newShade);
								fd.colorVec.at(i).at(k) = newShade+newPix;
							}
							flag=true;
						}
						else {
							enterDemarcPos = Point(-1,-1);
							exitDemarcPos = Point(-1,-1);
							demarcFlag=0;
						}
					}
				}
				j=step;
			}// end if color
		}//end for j
	}//end for i

	if(flag==true) return ruleNum;
	return 0;
}

/** rule 6 - Assign Dark Grey **/
double rule6(String& pix, String& newPix, String& newShade) {
	double ruleNum = 6;
	bool flag = false;
	Rgb rgb;
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
	Hsl hsl;
	Rgb rgb;
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
	Rgb rgb;
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
	ruleNumVec.push_back(rule3(fd,newPix,newShade));
	ruleNumVec.push_back(rule6(pix,newPix,newShade));
	ruleNumVec.push_back(rule9(fd,newPix));
	ruleNumVec.push_back(rule8(fd,newPix,loc));
	//ruleNumVec.push_back(rule5(fd,newPix,newShade));
	ruleNumVec.push_back(rule7(pix,newPix));

	if(ruleNumVec.size()>0) {
		for(unsigned int i=0; i<ruleNumVec.size(); i++) {
			if(ruleNumVec.at(i)!=0) {
				ruleNo.push_back(ruleNumVec.at(i));
				flag=true;
			}
		}
	}
	deque<double>().swap(ruleNumVec);
	shade = newShade;
	pix = newPix;
	return flag;
}
