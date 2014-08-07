/*
 * color.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: jason
 */

#include "color.h"

//check if color1 contains color2
bool Color::containsColor(String color1, String color2)
{
	if(color1.find(color2)!=string::npos)
		return true;
	else
		return false;
}

/** argNum as first input followed by color1 and colors to find  **/
bool Color::containsColor(String argNum, ...) {
	int arg = atoi(argNum.c_str());
	va_list vl;
	va_start(vl,argNum);
	String color1 = va_arg(vl,char *);
	String color2;
	for(int i=1; i<arg; i++) {
		color2 = va_arg(vl,char *);
		if(color1.find(color2)!=string::npos)
		return true;
	}
	va_end(vl);
	return false;
}

/** argNum as first input followed by color1 and colors to find  **/
bool Color::containsAllColor(String argNum, ...) {
	int arg = atoi(argNum.c_str());
	va_list vl;
	va_start(vl,argNum);
	String color1 = va_arg(vl,char *);
	String color2;
	for(int i=1; i<arg; i++) {
		color2 = va_arg(vl,char *);
		if(color1.find(color2)==string::npos)
			return false;
	}

	va_end(vl);
	return true;
}

//returns the amount of time pixels contain main color
int Color::containsMainColor(String color, String mColor)
{
	int count=0;
	size_t pos=0;
	while((pos=color.find(mColor,pos))!=string::npos)
	{
		++count;
		pos += mColor.length();
	}
	return count;
}

bool Color::isSameColor(String color1, String color2)
{
	deque<String> vec1, vec2;
	extractColorFromString(color1,vec1);
	extractColorFromString(color2, vec2);
	if(vec1.size()!=vec2.size()) return false;
	if(vec1.size()==1 && vec2.size()==1) {
		if(vec1.at(0)==vec2.at(0)) return true;
	}
	if(vec1.size()>=3 || vec2.size()>=3) {
		if(vec1.size()!=vec2.size())
			return false;
	}
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec2.size(); j++) {
			if(vec1.at(i)!="Gray" && vec2.at(j)!="Gray") {
				if(vec1.at(i)==vec2.at(j))
					return true;
			}
		}
	}
	return false;
}

void Color::extractColorFromString(String color, deque<String> &vecColor) {
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(containsMainColor(color,mainColors.at(i))) {
			vecColor.push_back(mainColors.at(i));
		}
	}
}

String Color::extractShade(String pix) {
	Shades sh;
	int shadeCount = sh.getShadeCount();
	String shade = "";
	if(pix=="Zero") return pix;
	//if(pix.find("Gray")!=string::npos) return "Gray";
	for(int i=0; i<shadeCount; i++) {
		shade = sh.getShade(i);
		if(pix.find(shade)!=string::npos)
			break;
	}
	return shade;
}
//returns the amount of different colors in the string
int Color::countColors(String color) {
	int count=0;
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color.find(mainColors.at(i))!=string::npos) {
			++count;
		}
	}
	return count;
}

//gets color and recalculates gray and color levels
String Color::reassignLevels(String pix, int r, int g, int b) {
	rgb rgb;
	deque<String> colorVec;
	String pix2;
	//double grayLevel=0;
	double colorLevel=0;
	extractColorFromString(pix, colorVec);
	//grayLevel = rgb.calcGrayLevel(r,g,b);
	colorLevel = rgb.calcColorLevel(r,g,b);
	double grayLumLevel = rgb.calcGrayLumLevel(r,g,b);
	for(unsigned int i=0; i<colorVec.size(); i++) {
		if(colorVec.at(i)=="Gray")
			pix2 += colorVec.at(i) + toString(grayLumLevel);
		else
			pix2 += colorVec.at(i) + toString(colorLevel);
	}
	return pix2;
}

String Color::getMainColor(String color) {
	String pix="";
	int flag=0;
	for(unsigned int i=0; i<mainColors.size(); i++) {
		if(color.find(mainColors.at(i))!=string::npos && mainColors.at(i)!="Gray") {
			pix += mainColors.at(i);
			flag=1;
		}
	}
	if(flag==0 && color.find("Gray")!=string::npos)
		return "Grey";
	return pix;
}

void Color::output2ImageGray(deque< deque<String> > &input, String name) {
	Mat img = img.zeros(Size(700,700),CV_8U);
	for(unsigned int y=0; y<input.size(); y++) {
		for(unsigned int x=0; x<input.at(y).size(); x++) {
			if(input.at(y).at(x)=="Black")
				img.at<uchar>(y,x) = 0;
			if(input.at(y).at(x)=="Dark")
				img.at<uchar>(y,x) = 60;
			if(input.at(y).at(x)=="High")
				img.at<uchar>(y,x) = 110;
			if(input.at(y).at(x)=="Low")
				img.at<uchar>(y,x) = 155;
			if(input.at(y).at(x)=="Light")
				img.at<uchar>(y,x) = 185;
			if(input.at(y).at(x)=="White")
				img.at<uchar>(y,x) = 255;
		}
	}
	String str = "5shadesBW2x2" + name + ".png";
	imwrite(str,img);
	img.release();
}

void Color::output2ImageColor(deque< deque<String> > &window, String name) {
	String filename = path+"Thresholds/output-shades.csv";
	fstream fs(filename.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		deque<String> colorVec;
		deque<String> shadeVec;
		deque<int>	thresh1;
		deque< deque<int> > values;
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i==0) shadeVec.push_back(vec.at(i));
				if(i==1) colorVec.push_back(vec.at(i));
				if(i>1) thresh1.push_back(atoi(vec.at(i).c_str()));
			}
			values.push_back(thresh1);
			thresh1.clear();
			vec.clear();
		}
		hsl hsl;
		Color c;
		Shades sh;
		String shade, color;
		int shadeLevel;
		double lumIncThresh = 0.05;
		Size size(window.at(0).size(),window.size());
		Mat img = img.zeros(size,16);
		double* HSL;
		int *RGB;
		for(unsigned int i=0; i<window.size(); i++) {
			for(unsigned int j=0; j<window.at(i).size(); j++) {
				shade = extractShade(window.at(i).at(j));
				shadeLevel = sh.extractShadeLevel(shade);
				color = getMainColor(window.at(i).at(j));
				color = optimizeColor(color);
				if(shade.find("Zero")!=string::npos) {
					img.at<Vec3b>(i,j)[2] = 0;
					img.at<Vec3b>(i,j)[1] = 0;
					img.at<Vec3b>(i,j)[0] = 0;
				}
				else if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos) {
					img.at<Vec3b>(i,j)[2] = 35;
					img.at<Vec3b>(i,j)[1] = 35;
					img.at<Vec3b>(i,j)[0] = 35;
				}
				else if(shade=="White" || color.find("White")!=string::npos) {
					img.at<Vec3b>(i,j)[2] = 255;
					img.at<Vec3b>(i,j)[1] = 255;
					img.at<Vec3b>(i,j)[0] = 255;
				}
				else {
					for(unsigned int k=0; k<colorVec.size(); k++) {
						if(shade.find(shadeVec.at(k))!=string::npos && color==colorVec.at(k)) {
							int r = values.at(k).at(0);
							int g = values.at(k).at(1);
							int b = values.at(k).at(2);
							HSL = hsl.rgb2hsl(r,g,b);
							HSL[2] += (shadeLevel*lumIncThresh)-lumIncThresh;
							if(HSL[2]>1) HSL[2]=1;
							RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
							img.at<Vec3b>(i,j)[2] = RGB[0];
							img.at<Vec3b>(i,j)[1] = RGB[1];
							img.at<Vec3b>(i,j)[0] = RGB[2];
							break;
						}
					}
				}
			}
		}
		String file = "outputShades2x2" + name + ".png";
		//resize(img,img,Size(700,700),INTER_CUBIC);
		imwrite(file,img);
		img.release();
		fs.close();
		deque<String>().swap(vec);
		deque<String>().swap(colorVec);
		deque<int>().swap(thresh1);
		deque< deque<int> >().swap(values);
	}
}

String Color::fixColors(String pix, double r, double g, double b) {
	rgb rgb;
	String color=pix;
	double colorLevel = rgb.calcColorLevel(r,g,b);
	if(getMainColor(pix)=="Grey")
		color = "Grey" + toString(colorLevel);
	if(containsColor(pix,"Gray") && containsColor(pix,"Grey"))
		color = "Grey" + toString(colorLevel);
	return color;
}

Mat Color::changeImageBrightness(Mat &img, double amt) {
	hsl hsl;
	Mat img2 = img.clone();
	int r,g,b;
	double *HSL;
	int *RGB;
	for(int i=0; i<img2.rows; i++) {
		for(int j=0; j<img2.cols; j++) {
			r = img2.at<Vec3b>(i,j)[2];
			g = img2.at<Vec3b>(i,j)[1];
			b = img2.at<Vec3b>(i,j)[0];
			if(r!=0 && g!=0 && b!=0) {
				HSL = hsl.rgb2hsl(r,g,b);
				HSL[2] += amt;
				if(HSL[2]>1.0) HSL[2] = 1.0;
				if(HSL[2]<0) HSL[2] = 0;
				RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
				img2.at<Vec3b>(i,j)[2] = RGB[0];
				img2.at<Vec3b>(i,j)[1] = RGB[1];
				img2.at<Vec3b>(i,j)[0] = RGB[2];
			}
		}
	}
	return img2;
}

void Color::changeContrast(double &r, double &g, double &b, double alpha, double beta) {
	double contrast[3];
	contrast[2] = round((alpha * r) + beta);
	contrast[1] = round((alpha * g) + beta);
	contrast[0] = round((alpha * b) + beta);
	for(int k=0; k<3; k++) {
		if(contrast[k]>255) contrast[k] = 255;
		if(contrast[k]<0) contrast[k] = 0;
	}
	r = contrast[2];
	g = contrast[1];
	b = contrast[0];
}

String Color::optimizeColor(String pix) {
	String color = pix;
	int count=0;
	if(containsAllColor(toString(3),color.c_str(),"Violet","Pink")) {
		color = removeColor(color,"Violet");
	}
	if(color.find("Violet")!=string::npos) {
		color = replaceColor(color,"Violet","Pink");
	}
	if(containsAllColor(toString(3),color.c_str(),"Pink","Purple")) {
		color = removeColor(color,"Purple");
	}
	if(countColors(color)>2 && color.find("Grey")!=string::npos) {
		color = removeColor(color,"Grey");
	}
	if(countColors(color)>2) {
		color.clear();
		for(unsigned int i=0; i<mainColors.size(); i++) {
			if(pix.find(mainColors.at(i))!=string::npos) {
				color += mainColors.at(i);
				count++;
			}
			if(count==2)
				break;
		}
	}

	return color;
}

double* Color::extractHSL(String inputString) {
	Functions fn;
	int beginPos=0, endPos=0;
	String substr;
	for(unsigned int i=0; i<inputString.length(); i++) {
		if(inputString[i]=='(')
			beginPos=i;
		if(inputString[i]==')') {
			endPos=i;
			break;
		}
	}
	substr = inputString.substr(beginPos+1,endPos-1);
	deque<double> vec;
	fn.getDelimitedValuesFromString(substr,';',vec);
	static double result[3];
	for(unsigned int i=0; i<vec.size(); i++) {
		result[i] = vec.at(i);
	}
	vec.clear();
	deque<double>().swap(vec);
	return result;
}

String Color::removeColor(String color, String colorRemove) {
	size_t pos = 0;
	String str;
	pos = color.find(colorRemove);
	if(pos!=string::npos)
		str = color.erase(pos,colorRemove.length());

	return str;
}

String Color::replaceColor(String color, String colorTarget, String colorReplace) {
	size_t pos=0;
	String str;
	pos = color.find(colorTarget);
	if(pos!=string::npos)
		str = color.replace(pos,colorTarget.length(),colorReplace);

	return str;
}

Mat Color::changeImageContrast(Mat img, double alpha, double beta) {
	Mat img2 = img.clone();
	double r,g,b;
	double contrast[3];
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			contrast[2] = round((alpha * r) + beta);
			contrast[1] = round((alpha * g) + beta);
			contrast[0] = round((alpha * b) + beta);
			for(int k=0; k<img.channels(); k++) {
				if(contrast[k]>255) contrast[k] = 255;
				if(contrast[k]<0) contrast[k] = 0;
				img2.at<Vec3b>(i,j)[k] = contrast[k];
			}
		}
	}
	return img2;
}

void Color::avgImageLuminance(Mat &src) {
	hsl hsl;
	Mat dst = src.clone();
	int satArr[101] = {0};
	int lumArr[101] = {0};
	int r,g,b;
	int sat, lum;
	double h,s,l;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			r = src.at<Vec3b>(i,j)[2];
			g = src.at<Vec3b>(i,j)[1];
			b = src.at<Vec3b>(i,j)[0];
			hsl.rgb2hsl(r,g,b);
			s = roundDecimal(hsl.getSat(),2);
			l = roundDecimal(hsl.getLum(),2);
			sat = s*100;
			lum = l*100;
			satArr[sat]++;
			if(lum>0)
			lumArr[lum]++;
		}
	}
	double avgSat=0;
	double avgLum=0;
	double totalLum=0;
	double countLum=0;
	double totalSat=0;
	double countSat=0;
	for(int i=0; i<101; i++) {
		if(lumArr[i]>0) {
			totalLum += (i/100.) * lumArr[i];
			countLum += lumArr[i];
		}
		totalSat += (i/100.) * satArr[i];
		countSat += satArr[i];
	}
	avgLum = roundDecimal(totalLum/countLum,2);
	avgSat = roundDecimal(totalSat/countSat,2);
	int *RGB;
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			r = src.at<Vec3b>(i,j)[2];
			g = src.at<Vec3b>(i,j)[1];
			b = src.at<Vec3b>(i,j)[0];
			hsl.rgb2hsl(r,g,b);
			h = hsl.getHue();
			s = roundDecimal(hsl.getSat(),2);
			l = roundDecimal(hsl.getLum(),2);
			if(l>0) {
				RGB = hsl.hsl2rgb(h,avgSat,avgLum);
				dst.at<Vec3b>(i,j)[2] = RGB[0];
				dst.at<Vec3b>(i,j)[1] = RGB[1];
				dst.at<Vec3b>(i,j)[0] = RGB[2];
			}
		}
	}
	imshow("img",src);
	imshow("DST",dst);
	waitKey(0);
}
