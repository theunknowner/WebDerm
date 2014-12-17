#include "/home/jason/git/WebDerm/WebDerm/src/Color/color.h"

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
	//int shadeCount = sh.getShadeCount();
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
	Rgb rgb;
	deque<String> colorVec;
	String pix2;
	extractColorFromString(pix, colorVec);
	double grayLevel = rgb.calcGrayLevel(r,g,b);
	double colorLevel = rgb.calcColorLevel(r,g,b);
	double grayLumLevel = rgb.calcGrayLumLevel(r,g,b);
	for(unsigned int i=0; i<colorVec.size(); i++) {
		if(colorVec.at(i)=="Gray")
			pix2 += colorVec.at(i) + toString(grayLumLevel);
		else
			pix2 += colorVec.at(i) + toString(colorLevel);
	}
	colorVec.clear();
	colorVec.shrink_to_fit();
	return toString(grayLevel) + pix2;
}

//gets color and recalculates gray and color levels
String Color::reassignLevels(String color, double h, double s, double l) {
	Rgb rgb;
	Hsl hsl;
	deque<String> colorVec;
	String pix2;
	extractColorFromString(color, colorVec);
	int *RGB = hsl.hsl2rgb(h,s,l);
	double grayLevel = rgb.calcGrayLevel(RGB[0],RGB[1],RGB[2]);
	double colorLevel = rgb.calcColorLevel(RGB[0],RGB[1],RGB[2]);
	double grayLumLevel = rgb.calcGrayLumLevel(RGB[0],RGB[1],RGB[2]);
	for(unsigned int i=0; i<colorVec.size(); i++) {
		if(colorVec.at(i)=="Gray")
			pix2 += colorVec.at(i) + toString(grayLumLevel);
		else
			pix2 += colorVec.at(i) + toString(colorLevel);
	}
	colorVec.clear();
	colorVec.shrink_to_fit();
	return toString(grayLevel) + pix2;
}

String Color::getMainColor(String color) {
	String pix="";
	int flag=0;
	try {
		for(unsigned int i=0; i<mainColors.size(); i++) {
			if(color.find(mainColors.at(i))!=string::npos && mainColors.at(i)!="Gray") {
				pix += mainColors.at(i);
				flag=1;
			}
		}
		if(flag==0 && color.find("Gray")!=string::npos)
			return "Grey";
	}
	catch(const std::out_of_range &oor) {
		printf("Color::getMainColor() out of range!\n");
		exit(1);
	}
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

void Color::output2ImageColor(deque< deque<String> > &window, Size size, String name) {
	String filename = "Thresholds/output-shades.csv";
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
		Hsl hsl;
		Shades sh;
		String shade, color;
		int shadeLevel;
		double lumIncThresh = 0.05;
		Mat img = img.zeros(Size(window.at(0).size(),window.size()),16);
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
							if(shade.find("Dark")!=string::npos) lumIncThresh = 0.08;
							if(shade.find("High")!=string::npos) lumIncThresh = 0.06;
							if(shade.find("Low")!=string::npos) lumIncThresh = 0.05;
							if(shade.find("Light")!=string::npos) lumIncThresh = 0.04;
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
		String str = toString(size.width)+"x"+toString(size.height);
		String file = name + "_outputShades_"+str+".png";
		imwrite(file,img);
		img.release();
		fs.close();
		vec.clear();
		vec.shrink_to_fit();
		colorVec.clear();
		colorVec.shrink_to_fit();
		thresh1.clear();
		thresh1.shrink_to_fit();
		values.clear();
		values.shrink_to_fit();
	}
	else {
		printf("Error opening %s\n",filename.c_str());
	}
}

String Color::fixColors(String pix, double r, double g, double b) {
	Rgb rgb;
	String color=pix;
	double colorLevel = rgb.calcColorLevel(r,g,b);
	if(getMainColor(pix)=="Grey")
		color = "Grey" + toString(colorLevel);
	if(containsColor(pix,"Gray") && containsColor(pix,"Grey"))
		color = "Grey" + toString(colorLevel);
	return color;
}

// amt is between 0.0-1.0 in 0=HSL / 1=RGB form
Mat Color::changeImageBrightness(Mat &img, double amt, int type) {
	Hsl hsl;
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
				if(type==1) {
					r = (int)round(r*amt);
					g = (int)round(g*amt);
					b = (int)round(b*amt);
				}
				HSL = hsl.rgb2hsl(r,g,b);
				if(type==0) {
					HSL[2] += amt;
					if(HSL[2]>1.0) HSL[2] = 1.0;
					if(HSL[2]<0) HSL[2] = 0;
				}
				RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
				img2.at<Vec3b>(i,j)[2] = RGB[0];
				img2.at<Vec3b>(i,j)[1] = RGB[1];
				img2.at<Vec3b>(i,j)[0] = RGB[2];
			}
		}
	}
	return img2;
}

Mat Color::changeImageSaturation(Mat img, double amt) {
	Mat img2 = img.clone();
	Hsl hsl;
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
				HSL[1] += amt;
				if(HSL[1]>1.0) HSL[1] = 1.0;
				if(HSL[1]<0) HSL[1] = 0;
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
	if(containsAllColor(toString(3),color.c_str(),"Pink","Purple")) {
		color = "Violet";
	}
	if(containsAllColor(toString(3),color.c_str(),"Purple","Violet")) {
		color = "Purple";
	}
	if(color=="RedPink")
		color = "PinkRed";
	if(countColors(color)>2) {
		if(color!="GreyBrownPink" && color!="GreyPinkViolet")  {
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
	}

	return color;
}

String Color::optimizeColor2(String pix) {
	String color = pix;
	int count=0;
	if(color=="RedPink")
		color = "PinkRed";
	if(pix.find("White")!=string::npos)
		color = "White";
	if(pix.find("Black")!=string::npos)
		color = "Black";
	if(countColors(color)>2) {
		if(color!="GreyBrownPink" && color!="GreyPinkViolet")  {
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
	Hsl hsl;
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

void Color::imgRgb2Gray(Mat &src, Mat &dst) {
	Rgb rgb;
	int relLum=0;
	int r,g,b;
	dst = dst.zeros(src.size(), CV_8U);
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			r = src.at<Vec3b>(i,j)[2];
			g = src.at<Vec3b>(i,j)[1];
			b = src.at<Vec3b>(i,j)[0];
			relLum = rgb.calcPerceivedBrightness(r,g,b);
			dst.at<uchar>(i,j) = relLum;
		}
	}
}

int* Color::changeRgbRelLum(double r, double g, double b, double amt) {
	Hsl hsl;
	static double RGB[3];
	static int results[3];
	double cR = 0.299, cG = 0.587, cB = 0.114;
	double incR = cR*amt;
	double incG = cG*amt;
	double incB = cB*amt;
	RGB[0]= cR*r; RGB[1] = cG*g; RGB[2] = cB*b;
	RGB[0] += incR; RGB[1] += incG; RGB[2] += incB;
	RGB[0] /= cR; RGB[1] /= cG; RGB[2] /= cB;

	double *HSL = hsl.rgb2hsl(r,g,b);
	double *HSL2 = hsl.rgb2hsl(RGB[0],RGB[1],RGB[2]);
	int *RGB2 = hsl.hsl2rgb(HSL[0],HSL[1],HSL2[2]);

	results[0] = RGB2[0];
	results[1] = RGB2[1];
	results[2] = RGB2[2];

	return results;
}

String Color::combineColors(String color)  {
	if(color.find("Grey")!=string::npos && color.find("Yellow")!=string::npos)
		return "Yellow";
	if(color.find("Grey")!=string::npos && color.find("Brown")!=string::npos)
		return "Brown";
	if(color.find("Grey")!=string::npos && color.find("Green")!=string::npos)
		return "Green";
	if(color.find("Grey")!=string::npos && color.find("Blue")!=string::npos)
		return "Blue";
	if(color.find("Purple")!=string::npos)
		return "Purple";
	if(color.find("Violet")!=string::npos)
		return "Violet";
	if(color.find("Pink")!=string::npos)
		if(color.find("Brown")==string::npos && color.find("Red")==string::npos)
			return "Pink";
	if(color.find("Brown")!=string::npos && color.find("Orange")!=string::npos)
		color = "Brown";
	return color;
}

Mat Color::output2ImageTargetColor(deque< deque<String> > &window, Size size, String name, String colorTarget, String shadeTarget) {
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
		Hsl hsl;
		Shades sh;
		String shade, color;
		int shadeLevel;
		double lumIncThresh = 0.05;
		Mat img = img.zeros(Size(window.at(0).size(),window.size()),16);
		double* HSL;
		int *RGB;
		for(unsigned int i=0; i<window.size(); i++) {
			for(unsigned int j=0; j<window.at(i).size(); j++) {
				shade = extractShade(window.at(i).at(j));
				shadeLevel = sh.extractShadeLevel(shade);
				color = getMainColor(window.at(i).at(j));
				color = optimizeColor(color);
				color = combineColors(color);

				if(shade.find("Zero")!=string::npos) {
					img.at<Vec3b>(i,j)[2] = 0;
					img.at<Vec3b>(i,j)[1] = 0;
					img.at<Vec3b>(i,j)[0] = 0;
					HSL = hsl.rgb2hsl(0,0,0);
				}
				else if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos) {
					img.at<Vec3b>(i,j)[2] = 35;
					img.at<Vec3b>(i,j)[1] = 35;
					img.at<Vec3b>(i,j)[0] = 35;
					HSL = hsl.rgb2hsl(35,35,35);
				}
				else if(shade=="White" || color.find("White")!=string::npos) {
					img.at<Vec3b>(i,j)[2] = 255;
					img.at<Vec3b>(i,j)[1] = 255;
					img.at<Vec3b>(i,j)[0] = 255;
					HSL = hsl.rgb2hsl(255,255,255);
				}
				else {
					for(unsigned int k=0; k<colorVec.size(); k++) {
						if(shade.find(shadeVec.at(k))!=string::npos && color==colorVec.at(k)) {
							int r = values.at(k).at(0);
							int g = values.at(k).at(1);
							int b = values.at(k).at(2);
							HSL = hsl.rgb2hsl(r,g,b);
							if(shade.find("Dark")!=string::npos) lumIncThresh = 0.08;
							if(shade.find("High")!=string::npos) lumIncThresh = 0.06;
							if(shade.find("Low")!=string::npos) lumIncThresh = 0.05;
							if(shade.find("Light")!=string::npos) lumIncThresh = 0.04;
							HSL[2] += (shadeLevel*lumIncThresh)-lumIncThresh;
							if(HSL[2]>1) HSL[2]=1;
							break;
						}
					}
				}
				if(color==colorTarget && shade.find(shadeTarget)!=string::npos) {
					RGB = hsl.hsl2rgb(HSL[0],HSL[1],HSL[2]);
					img.at<Vec3b>(i,j)[2] = RGB[0];
					img.at<Vec3b>(i,j)[1] = RGB[1];
					img.at<Vec3b>(i,j)[0] = RGB[2];
				}
				else {
					RGB = hsl.hsl2rgb(HSL[0],0.20,0.75);
					img.at<Vec3b>(i,j)[2] = RGB[0];
					img.at<Vec3b>(i,j)[1] = RGB[1];
					img.at<Vec3b>(i,j)[0] = RGB[2];
				}

			}
		}
		//namedWindow(name+"TargetColorOutput", CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
		//imshow(name+"TargetColorOutput",img);
		fs.close();
		deque<String>().swap(vec);
		deque<String>().swap(colorVec);
		deque<int>().swap(thresh1);
		deque< deque<int> >().swap(values);
		return img;
	}
	else {
		printf("Output2ImageTargetColor: Cannot open image!\n");
		exit(1);
	}
}

Mat Color::correctGamma( Mat& img, double gamma ) {
	double inverse_gamma = 1.0 / gamma;

	Mat lut_matrix(1, 256, CV_8UC1 );
	uchar * ptr = lut_matrix.ptr();
	for( int i = 0; i < 256; i++ )
		ptr[i] = (int)( pow( (double) i / 255.0, inverse_gamma ) * 255.0 );

	Mat result;
	LUT( img, lut_matrix, result );

	return result;
}

Mat Color::shadeCorrection(Mat img) {
	Hsv hsv;
	double * HSV;
	Mat coef = Mat::zeros(6,6,CV_64F);
	double mul[6] = {1};
	Mat totalV = Mat::zeros(6,1,CV_64F);
	int r,g,b;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			if(r>20 && g>20 && b>20) {
				HSV = hsv.rgb2hsv(r,g,b);
				HSV[2] = roundDecimal(HSV[2],2);
				totalV.at<double>(0,0) += HSV[2];
				totalV.at<double>(1,0) += j * HSV[2];
				totalV.at<double>(2,0) += i * HSV[2];
				totalV.at<double>(3,0) += j * i * HSV[2];
				totalV.at<double>(4,0) += pow(j,2) * HSV[2];
				totalV.at<double>(5,0) += pow(i,2) * HSV[2];
				mul[1] = j+1;
				mul[2] = i+1;
				mul[3] = (i+1)*(j+1);
				mul[4] = pow(j+1,2);
				mul[5] = pow(i+1,2);
				for(int m=0; m<6; m++) {
					for(int k=0; k<6; k++) {
						coef.at<double>(m,k) += mul[k] * mul[m];
					}
				}
			}
		}
	}
	Mat img2 = img.clone();
	int * RGB;
	double z=0;
	invert(coef,coef);
	Mat product = coef * totalV;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			if(r>20 && g>20 && b>20) {
				HSV = hsv.rgb2hsv(r,g,b);
				mul[1] = j+1;
				mul[2] = i+1;
				mul[3] = (i+1)*(j+1);
				mul[4] = pow(j+1,2);
				mul[5] = pow(i+1,2);
				for(int k=0; k<6; k++) {
					z += product.at<double>(k,0) * mul[k];
				}
				HSV[2] /= z;
				if(HSV[2]>1) HSV[2] = 1;
				RGB = hsv.hsv2rgb(HSV[0],HSV[1],HSV[2]);
				img2.at<Vec3b>(i,j)[2] = RGB[0];
				img2.at<Vec3b>(i,j)[1] = RGB[1];
				img2.at<Vec3b>(i,j)[0] = RGB[2];
				z=0;
			}
		}
	}
	coef.release();
	totalV.release();
	product.release();
	return img2;
}
