/*
 * contrast.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: jason
 */

#include "contrast.h"

//uses calcColorfulness to calculate colorfulness
double contrast::calcColorfulness(double hue, String color) {
	rgb rgb;
	double colorLevel[mainColors.size()];
	double cHue[mainColors.size()];
	double colorfn[mainColors.size()];
	fill_n(colorLevel,mainColors.size(),0);
	fill_n(cHue,mainColors.size(),0);
	fill_n(colorfn,mainColors.size(),0);
	double totalLevel=0;
	double totalColorfn=0;
	deque<int> index;
	deque<double> vec,vec2;
	for(unsigned int i=0; i<mainColors.size(); i++)	{
		cHue[i] = hue;
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
		colorfn[index.at(i)] = colorLevel[index.at(i)] * cHue[index.at(i)];
		if(mainColors.at(index.at(i))!="Gray") {
			colorfn[index.at(i)] *= (colorLevel[index.at(i)]/totalLevel);
		}
		totalColorfn += colorfn[index.at(i)];
	}
	return totalColorfn;
}

//calculates contrast between two colors
double contrast::calcContrast(double hue1, double hue2, String color1, String color2) {
	Color c;
	double colorfn1=0, colorfn2=0;
	double contrast=0;
	colorfn1 = calcColorfulness(hue1,color1);
	colorfn2 = calcColorfulness(hue2,color2);
	//printf("Colorfn1: %f\n",colorfn1);
	//printf("Colorfn2: %f\n",colorfn2);
	contrast = colorfn2-colorfn1;
	if(c.isSameColor(color1,color2))
		contrast *= 4;

	return contrast;
}
//calculates contrast angle between two colors
double contrast::getContrastAngle(double hue1, double hue2, String color1, String color2) {
	double colorfn1=0, colorfn2=0;
	double angleContrast = 0;
	colorfn1 = calcColorfulness(hue1,color1);
	colorfn2 = calcColorfulness(hue2,color2);
	angleContrast = calcContrastAngle(hue1, hue2, colorfn1, colorfn2);
	return angleContrast;
}
//change between calcContrast and calcContrast2 for diff options
void contrast::calcContrastFromMatrix(deque< deque<String> > &windowVec, deque< deque<double> > &hueVec,String name) {
	double contrast=0;
	double feature=0;
	String color1,color2;
	double hue1,hue2;
	deque<double> vec;
	deque< deque<double> > vec2;
	deque<double> fVec1;
	deque< deque<double> > fVec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-2); j++) {
			color1 = windowVec.at(i).at(j);
			color2 = windowVec.at(i).at(j+1);
			if(color1.find("Black")==string::npos && color2.find("Black")==string::npos) {
				hue1 = hueVec.at(i).at(j);
				hue2 = hueVec.at(i).at(j+1);
				contrast = calcContrast(hue1, hue2, color1, color2);
				feature = (0.75 * feature) + (0.25 * contrast);
				vec.push_back(contrast);
				fVec1.push_back(feature);
			}
		}
		if(fVec1.size()!=0)
			fVec2.push_back(fVec1);
		if(vec.size()!=0)
			vec2.push_back(vec);
		vec.clear();
		fVec1.clear();
		feature=0;
	}
	writeSeq2File(vec2,name);
	String filename = name + "Feature";
	writeSeq2File(fVec2,filename);
	deque<double>().swap(vec);
	deque< deque<double> >().swap(vec2);
}

void contrast::colorfulnessMatrix1x1(Mat &img, String name) {
	rgb rgb;
	hsl hsl;
	String filename = name + "Colorfn";
	contrast con;
	Color c;
	double colorfn;
	double hue;
	deque<double> clrfn;
	deque< deque<double> > clrfnVec;
	int ind=0;
	double dist=0;
	String pix;
	int r,g,b;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			hsl.rgb2hsl(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.calcColor2(r,g,b);
				if(pix=="OTHER") {
					pix = rgb.pushColor(r,g,b,dist,ind);
					if(dist>4)
						pix = c.reassignLevels(pix,r,g,b);
				}
			}
			hue = (hsl.getHue()+180)%360;
			hue /=360;
			colorfn = con.calcColorfulness(hue,pix);
			clrfn.push_back(colorfn);
		}
		clrfnVec.push_back(clrfn);
		clrfn.clear();
	}
	writeSeq2File(clrfnVec,filename);
}
//used for calculating 2x2 colorfulness
void contrast::calcColorfulnessMatrix(deque< deque<String> > &windowVec, deque< deque<double> > &hueVec,String name) {
	String file;
	double colorfn=0;
	String color;
	double hue;
	deque<double> vec;
	deque< deque<double> > vec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-1); j++) {
			color = windowVec.at(i).at(j);
			hue = hueVec.at(i).at(j);
			colorfn = calcColorfulness(hue,color);
			vec.push_back(colorfn);
		}
		vec2.push_back(vec);
		vec.clear();
	}
	file = name + "Colorfn2x2";
	writeSeq2File(vec2,file);
	deque<double>().swap(vec);
	deque< deque<double> >().swap(vec2);
}

void contrast::colorfulMatrix(Mat img, Size size, String name)
	{
		rgb rgb;
		hsl hsl;
		contrast con;
		Color colorObj;
		double matchingScans = (size.width*size.height)/2;
		deque<String> pixelColorWindow;
		deque<String> colorWindow;
		deque< deque<String> > windowVec;
		deque<int> index;
		deque<int> hueVals;
		deque<double> hueAvg;
		deque< deque<double> > hueVec;
		int mainColorIndex[mainColors.size()];
		double mainColorLevels[mainColors.size()];
		double mainColorLevelAvg[mainColors.size()];
		String pix;
		int ind=0;
		double dist=0;
		int b,g,r;
		int row=0, col=0;
		fill_n(mainColorIndex,mainColors.size(),0);
		fill_n(mainColorLevelAvg,mainColors.size(),0);
		while(row<=(img.rows-size.height))
		{
			while(col<=(img.cols-size.width))
			{
				if(col==0)
				{
					for(int x=col; x<(col+size.width); x++)
					{
						for(int y=row; y<(row+size.height); y++)
						{
							b = img.at<Vec3b>(y,x)[0];
							g = img.at<Vec3b>(y,x)[1];
							r = img.at<Vec3b>(y,x)[2];
							pix = rgb.checkBlack(r,g,b);
							hsl.rgb2hsl(r,g,b);
							hueVals.push_back(hsl.getHue());
							if(pix=="OTHER")
							{
								pix = rgb.calcColor(r,g,b);
								if(pix=="OTHER") {
									pix = rgb.pushColor(r,g,b,dist,ind);
									if(dist>4)
										pix = colorObj.reassignLevels(pix,r,g,b);
								}
							}
							pixelColorWindow.push_back(pix);
						}
					}
				}
				else
				{
					for(int y=row; y<(row+size.height); y++)
					{
						b = img.at<Vec3b>(y,col+(size.width-1))[0];
						g = img.at<Vec3b>(y,col+(size.width-1))[1];
						r = img.at<Vec3b>(y,col+(size.width-1))[2];
						pix = rgb.checkBlack(r,g,b);
						hsl.rgb2hsl(r,g,b);
						hueVals.pop_front();
						hueVals.push_back(hsl.getHue());
						if(pix=="OTHER")
						{
							pix = rgb.calcColor2(r,g,b);
							if(pix=="OTHER") {
								pix = rgb.pushColor(r,g,b,dist,ind);
								if(dist>4)
									pix = colorObj.reassignLevels(pix,r,g,b);
							}
						}
						pixelColorWindow.pop_front();
						pixelColorWindow.push_back(pix);
					}
				}
				hueAvg.push_back(hsl.calcHueAvg(hueVals));
				for(unsigned int i=0; i<pixelColorWindow.size(); i++)
				{
					for(unsigned int j=0; j<mainColors.size(); j++)
					{
						if(colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j))!=0)
							mainColorIndex[j]++;
					}
				}
				for(unsigned int j=0; j<mainColors.size(); j++)
				{
					if(mainColorIndex[j]>=round(matchingScans))
					{
						index.push_back(j);
					}
				}
				for(unsigned int i=0; i<pixelColorWindow.size(); i++)
				{
					for(unsigned int j=0; j<index.size(); j++)
					{
						mainColorLevels[index.at(j)] = rgb.getColorLevel(pixelColorWindow.at(i),
															mainColors.at(index.at(j)));
						mainColorLevelAvg[index.at(j)] += mainColorLevels[index.at(j)];
					}
				}
				for(unsigned int i=0; i <index.size(); i++)
				{
					mainColorLevelAvg[index.at(i)] /= mainColorIndex[index.at(i)];
				}
				if(index.size()!=0)
				{
					pix.clear();
					for(unsigned int i=0; i<index.size(); i++)
					{
						if(mainColors.at(index[i])=="Black") {
							pix = mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
							break;
						}
						else
							pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
					}
					colorWindow.push_back(pix);
				}
				else
				{
					colorWindow.push_back("NZ");
				}
				fill_n(mainColorIndex,mainColors.size(),0);
				fill_n(mainColorLevelAvg,mainColors.size(),0);
				index.clear();
				++col;
			}//end while col
			hueVec.push_back(hueAvg);
			windowVec.push_back(colorWindow);
			hueAvg.clear();
			hueVals.clear();
			colorWindow.clear();
			pixelColorWindow.clear();
			col=0; ++row;
		}//end while row
		//writeSeq2File(windowVec,name);
		con.calcColorfulnessMatrix(windowVec,hueVec,name);
		deque<String>().swap(pixelColorWindow);
		deque<String>().swap(colorWindow);
		deque< deque<String> >().swap(windowVec);
		deque<int>().swap(hueVals);
		deque<double>().swap(hueAvg);
		deque< deque<double> >().swap(hueVec);
		deque<int>().swap(index);
	}

double contrast::calcContrastAngle(double hue1, double hue2, double colorfn1, double colorfn2) {
	const double c = 65.0;
	double fColor1 = c * hue1;
	double fColor2 = c * hue2;
	double angle=0;
	double top=0, bottom=0;
	double root = 1/2.5;
	top = abs((fColor1*colorfn2) - (colorfn1*fColor2));
	bottom = sqrt(pow(fColor1,2)+pow(colorfn1,2)) * sqrt(pow(fColor2,2)+pow(colorfn2,2));
	angle = top/bottom;
	angle = pow(angle,root);
	return angle;
}

void contrast::shadeOfFeature(deque<double> &feature) {
	min = feature.at(0);
	max = feature.at(1);
	for(unsigned int i=0; i<feature.size(); i++) {
		if(feature.at(i)>max) {
			max = feature.at(i);
		}
		if(feature.at(i)<min) {
			min = feature.at(i);
		}
	}
	range = max - min;
}

String contrast::getShade(double feature) {
	String shade;
	double range1 = min+(range/4);
	double range2 = min+(range*3)/4;
	double light[] = {min,range1};
	double reg[] = {range1,range2};
	double dark[] = {range2,max};
	if(feature>=light[0] && feature<=light[1])
		shade = "Light";
	else if(feature>=reg[0] && feature<=reg[1])
		shade = "";
	else if(feature>=dark[0] && feature<=dark[1])
		shade = "Dark";

	return shade;
}

void contrast::writeMainColorMatrix(deque< deque<String> > &windowVec, String name) {
	double contrast=0;
	double feature=0;
	int flag=0;
	Point pt; //pointer to hold x,y of color window
	String color1,color2;
	deque<double> vec;
	deque< deque<double> > vec2;
	deque<double> fVec1;
	deque< deque<double> > fVec2;
	deque<Point> ptVec1; //pointerVec to hold all pointers
	deque <deque<Point> > ptVec2;
	for(unsigned int i=0; i<windowVec.size(); i++) {
		for(unsigned int j=0; j<(windowVec.at(i).size()-2); j++) {
			color1 = windowVec.at(i).at(j);
			color2 = windowVec.at(i).at(j+1);
			if(color1.find("Black")==string::npos && color2.find("Black")==string::npos) {
				pt.x = j;
				pt.y = i;
				contrast = calcContrast(0, 0, color1, color2);
				feature = (0.75 * feature) + (0.25 * contrast);
				vec.push_back(contrast);
				fVec1.push_back(feature);
				ptVec1.push_back(pt);
				flag=1;
			}
		}
		if(flag==1) {
			ptVec2.push_back(ptVec1);
			fVec2.push_back(fVec1);
			vec2.push_back(vec);
		}
		flag=0;
		ptVec1.clear();
		vec.clear();
		fVec1.clear();
		feature=0;
	}
	Color c;
	int x=0,y=0;
	String pix;
	String shade;
	deque<String> strVec;
	deque< deque<String> > strVec2;
	for(unsigned int i=0; i<ptVec2.size(); i++) {
		shadeOfFeature(fVec2.at(i));
		for(unsigned int j=0; j<ptVec2.at(i).size(); j++) {
			x = ptVec2.at(i).at(j).x;
			y = ptVec2.at(i).at(j).y;
			pix = windowVec.at(y).at(x);
			pix = c.getMainColor(pix);
			shade = getShade(fVec2.at(i).at(j));
			if(pix!="Black" && pix!="White")
				pix = shade + pix;
			pix += "("+toString(x+1) + ";" + toString(y+1)+")";
			strVec.push_back(pix);
		}
		strVec2.push_back(strVec);
		strVec.clear();
	}
	String filename = name +"MainColors";
	writeSeq2File(strVec2,filename);
	deque<double>().swap(vec);
	deque<double>().swap(fVec1);
	deque< deque<double> >().swap(vec2);
	deque< deque<double> >().swap(fVec2);
	deque<String>().swap(strVec);
	deque< deque<String> >().swap(strVec2);
	deque<Point>().swap(ptVec1);
	deque< deque<Point> >().swap(ptVec2);
}

deque< deque<double> > contrast::calcContrastFromMatrix(deque< deque<double> > &intensityVec) {
	Intensity in;
	double contrast=0, intensity=0;
	double minIntensity = in.getMinIntensity();
	double maxIntensity = in.getMaxIntensity();
	double rangeIntensity = maxIntensity-minIntensity;
	double flag=0;
	deque<double> vec1;
	deque< deque<double> > vec2;
	for(unsigned int i=0; i<intensityVec.size(); i++) {
		for(unsigned int j=0; j<intensityVec.at(i).size(); j++) {
			intensity = intensityVec.at(i).at(j);
			intensity = (intensity*rangeIntensity)+minIntensity;
			if(intensity<900 && flag!=0) {//no black color
				contrast = intensityVec.at(i).at(j) - intensityVec.at(i).at(j-1);
			}
			if(intensity<900 && flag==0) {
				contrast=0;
				flag=1;
			}
			if(intensity>=900 && flag!=0) { //no black color in between other colors
				contrast=0;
				flag=0;
			}
			vec1.push_back(contrast);
		}
		flag=0;
		contrast=0;
		vec2.push_back(vec1);
		vec1.clear();
	}
	return vec2;
}

deque< deque<double> > contrast::calcCumulativeContrast(deque< deque<double> > &vec) {
	double contrast=0;
	deque<double>	contrastVec1;
	deque< deque<double> > contrastVec2;
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int j=0; j<vec.at(i).size(); j++) {
			contrast += vec.at(i).at(j);
			contrastVec1.push_back(contrast);
		}
		contrastVec2.push_back(contrastVec1);
		contrastVec1.clear();
		contrast=0;
	}
	return contrastVec2;
}

void contrast::writeCumulativeContrast(deque< deque<double> > &vec, String name) {
	deque< deque<double> > conVec;
	conVec = calcCumulativeContrast(vec);
	String filename = name + "CumulContrast";
	writeSeq2File(conVec,filename);
	deque< deque<double> >().swap(conVec);
}
