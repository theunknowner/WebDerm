/*
 * scripts.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: jason
 */

#include "scripts.h"

void colorThreshNamingScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorThresh.csv","w");
	hsl hsl;
	rgb rgb;
	Color color;
	double r,g,b;
	double lum=0;
	String pix;
	vector<String> vecColor;
	int colorLevel=0;
	double grayLevel=0;
	int mainColorIndex[mainColors.size()];
	fill_n(mainColorIndex,mainColors.size(),0);
	fprintf(fp,"Color,R,G,B,NormR,NormG,NormB\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		lum = roundDecimal(hsl.getLum(),2);
		colorLevel = rgb.calcColorLevel(r,g,b);
		grayLevel = rgb.calcGrayLevel(r,g,b);
		grayLevel = rgb.calcGrayLevel2(grayLevel,lum);
		color.extractColorFromString(rgbColors.at(i),vecColor);
		for(unsigned int j=0; j<vecColor.size(); j++)
		{
			if(vecColor.at(j)=="Gray" && grayLevel!=0) {
				pix += "Gray" + toString(grayLevel);
			}
			else {
				pix += vecColor.at(j) + toString(colorLevel);
			}
		}
			fprintf(fp,"%s,%f,%f,%f,%f,%f,%f\n",
					pix.c_str(),absMeanThresh[i][0],absMeanThresh[i][1],absMeanThresh[i][2],
					normMeanThresh[i][0],normMeanThresh[i][1],normMeanThresh[i][2]);
			pix.clear(); vecColor.clear();
	}
}

void colorThreshScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorThresh2.csv","w");
	hsl hsl;
	rgb rgb;
	double r,g,b;
	String pix;
	double grayLevel=0;
	int colorLevel=0;
	int hue;
	double sat,lum;
	fprintf(fp,"Color,H,S,L,SatLvl,LumLvl\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		hue = hsl.getHue();
		sat =roundDecimal(hsl.getSat(),2);
		lum = roundDecimal(hsl.getLum(),2);
		grayLevel = rgb.calcGrayLevel(r,g,b);
		colorLevel = rgb.calcColorLevel(r,g,b);
		fprintf(fp,"%s,%d,%f,%f,%f,%d\n", rgbColors.at(i).c_str(),hue,sat,lum,grayLevel,colorLevel);
	}
}

void sortColorThreshold(String color1, String color2) {
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/sortColor.csv","w");
	fprintf(fp,"Color,Hue,Sat,Lum,R,G,B,Index\n");
	hsl hsl;
	String pix;
	int arr[10] = {0,6,7,13,339,345,346,352,353,359};
	int r,g,b;
	int hue;
	double sat,lum;
	for(int i=0; i<10; i+=2) {
		for(int j=1; j<=16; j++) {
			for(double k=1; k<=9; k+=0.5) {
				pix = color1 + toString(k) + color2 + toString(j);
				for(unsigned int m=0; m<rgbColors.size(); m++) {
					r = absMeanThresh.at(m).at(0);
					g = absMeanThresh.at(m).at(1);
					b = absMeanThresh.at(m).at(2);
					hsl.rgb2hsl(r,g,b);
					hue = hsl.getHue();
					if(hue>=arr[i] && hue<=arr[i+1] && pix==rgbColors.at(m)) {
						sat = roundDecimal(hsl.getSat(),2);
						lum = roundDecimal(hsl.getLum(),2);
						fprintf(fp,"%s,%d,%f,%f,%d,%d,%d,%d\n",pix.c_str(),hue,sat,lum,r,g,b,m+2);
					}
				}
			}
		}
	}
}

void outputFarRGBScript(Mat &img, String name)
{
	Color color;
	String filename = path+name+"Colors.txt";
	FILE * fp;
	FILE * fs;
	fp = fopen(filename.c_str(),"w");
	fs = fopen("/home/jason/Desktop/workspace/colorThresh.csv","w");
	rgb rgb;
	int r,g,b;
	int ind=0;
	double dist=0;
	double thresh=8.0;
	double min1=0, min2=0;
	int lumLevel=0;
	String pix, pix2;
	vector<int> rgbVals;
	vector< vector<int> > rgbList;
	vector<String> vecColor;
	vector<Point> points;
	fprintf(fs,"Color,Rmin,Rmax,Gmin,Gmax,Bmin,Bmax,rMin,rMax,gMin,gMax,bMin,bMax\n");
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER")
			{
				pix = rgb.pushColor(r,g,b,dist,ind);
				if(rgb.checkAbsDist(dist,thresh)) {
					if(rgbList.size()==0) {
						points.push_back(Point(col+1,row+1));
						rgbVals.push_back(r); rgbVals.push_back(g); rgbVals.push_back(b);
						rgbList.push_back(rgbVals);
						rgbVals.clear();
					}
					else {
						for(unsigned int i=0; i<rgbList.size(); i++) {
							dist = rgb.absEucDist(r,g,b,rgbList.at(i));
							if(dist<=thresh) {
								break;
							}
							if(i==(rgbList.size()-1)) { //if no match add to list
								points.push_back(Point(col+1,row+1));
								rgbVals.push_back(r); rgbVals.push_back(g); rgbVals.push_back(b);
								rgbList.push_back(rgbVals);
								rgbVals.clear();
							}
						}
					}
				}
			}
		}
	}
	for(unsigned int i=0; i<rgbList.size(); i++) {
		r = rgbList.at(i).at(0);
		g = rgbList.at(i).at(1);
		b = rgbList.at(i).at(2);
		pix = outputCorrelationRGB(r,g,b,min1);
		pix2 = outputCorrelationRGBnorm(r,g,b,min2);
		if(min1<=min2)
			color.extractColorFromString(pix, vecColor);
		else
			color.extractColorFromString(pix2,vecColor);

		lumLevel = rgb.calcColorLevel(r,g,b);
		pix.clear();
		for(unsigned int i=0; i<vecColor.size(); i++) {
			if(vecColor.size()==1 && vecColor.at(i)!="Gray") {
				pix = pix + "Gray" + toString(lumLevel) + vecColor.at(i) + toString(lumLevel);
			}
			else {
				pix = pix + vecColor.at(i) + toString(lumLevel);
			}
		}
		fprintf(fp,"%d,%d,%d - (%d,%d) - %s\n",r,g,b,points.at(i).x,points.at(i).y,pix.c_str());
		vecColor.clear();
	}
}

String outputCorrelationRGB(int r, int g, int b, double &m)
{
	double vec1[3] = {(double)r,(double)g,(double)b};
	double dist=0;
	int ind=0;
	double min=10;
	String pix;
	for(unsigned int i=0; i<absMeanThresh.size(); i++)
	{
		double vec2[3] = {absMeanThresh.at(i).at(0),absMeanThresh.at(i).at(1),
				absMeanThresh.at(i).at(2)};
		dist = correlationDist(vec1,vec2);
		if(dist!=-1)
		{
			if(dist<min)
			{
				min = dist;
				ind = i;
			}
			pix = rgbColors.at(ind);
		}
	}
	//cout << min <<endl;
	//cout << ind +2 << endl;
	m = min;
	return pix;
}

String outputCorrelationRGBnorm(int r, int g, int b, double &m)
{
	double vec1[3] = {(double)r,(double)g,(double)b};
	double dist=0;
	int ind=0;
	double min=10;
	String pix;
	for(unsigned int i=0; i<normMeanThresh.size(); i++)
	{
		double vec2[3] = {normMeanThresh.at(i).at(0),normMeanThresh.at(i).at(1),
				normMeanThresh.at(i).at(2)};
		dist = correlationDist(vec1,vec2);
		if(dist!=-1)
		{
			if(dist<min)
			{
				min = dist;
				ind = i;
			}
			pix = rgbColors.at(ind);
		}
	}
	//cout << min <<endl;
	//cout << ind +2 << endl;
	m=min;
	return pix;
}

//compares two spreadsheets
void spreadsheetCompareScript()
{
	String filename;
	String filename2;
	String input;
	cout << "Enter filename of 1st:";
	cin >> input;
	filename = input;
	cout << "Enter filename of 2nd:";
	cin >> input;
	filename2 = input;
	fstream fs(filename.c_str());
	fstream fs2(filename2.c_str());
	if(fs.is_open() && fs2.is_open())
	{
		vector< vector<String> > matVec1;
		vector <vector<String> > matVec2;
		vector<String> vec;
		while(getline(fs,input))
		{
			getSubstr(input,',',vec);
			matVec1.push_back(vec);
			vec.clear();
		}
		while(getline(fs2,input))
		{
			getSubstr(input,',',vec);
			matVec2.push_back(vec);
			vec.clear();
		}
		fs.close(); fs2.close();
		FILE * fp;
		fp = fopen("/home/jason/Desktop/workspace/compare.txt","w");
		for(unsigned int i=0; i<matVec1.size(); i++)
		{
			for(unsigned int j=0; j<matVec1.at(i).size(); j++)
			{
				if(matVec1.at(i).at(j)!=matVec2.at(i).at(j))
				{
					fprintf(fp,"%d,%d)\n", j+1,i+1);
				}
			}
		}
		vector< vector<String> >().swap(matVec1);
		vector< vector<String> >().swap(matVec2);
		vector<String>().swap(vec);
	}
	else
	{
		cout << "File not found!" << endl;
	}
}

//prints out rgb vals and location of matching index
void outputPixLoc(Mat img,int index)
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/pixLoc.txt", "w");
	rgb rgb;
	int r,g,b;
	String pix;
	int ind=0;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.pushColor2(r,g,b,ind);
			}
			if(ind==(index-2)) {
				fprintf(fp,"%d,%d,%d - (%d,%d)\n", r,g,b,col+1,row+1);
			}
		}
	}
}

void showPushColorOnImage(Mat &img, int index) {
	rgb rgb;
	int r,g,b;
	int ind=0;
	int count=0;
	double dist=0;
	String pix;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.pushColor(r,g,b,dist,ind);
				if(ind+2==index) {
					img.at<Vec3b>(row,col) = Vec3b(0,0,255);
					++count;
				}
			}
		}
	}
	cout << count << endl;
	imshow("Mask",img);
	waitKey(0);
}

//needs manual changes for the name of color
void addNewColors(Mat &img, Point pt1, Point pt2,String color1, String color2) {
	rgb rgb;
	hsl hsl;
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/newColors.csv","w");
	String pix;
	vector<double> rgbVals;
	vector< vector<double> > rgbVec;
	int r,g,b, hue,flag=0;
	double lum,sat;
	double grayLevel=0, colorLevel=0;
	double normR, normG, normB;
	double dist=0;
	for(int row=(pt1.y-1); row<=(pt2.y-1); row++) {
		for(int col=(pt1.x-1); col<=(pt2.x-1); col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			for(unsigned int i=0; i<absMeanThresh.size(); i++) {
				dist = rgb.absEucDist(r,g,b,absMeanThresh.at(i));
				if(dist<=3) {
					flag=1;
					break;
				}
			}
			if(rgbVec.size()==0 && flag==0) {
				rgbVals.push_back(r); rgbVals.push_back(g); rgbVals.push_back(b);
				rgbVec.push_back(rgbVals);
				rgbVals.clear();
			}
			if(flag==0) {
				for(unsigned int i=0; i<rgbVec.size(); i++) {
					dist = rgb.absEucDist(r,g,b,rgbVec.at(i));
					if(dist<=3) {
						flag=1;
						break;
					}
				}
			}
			if(flag==0) {
				rgbVals.push_back(r); rgbVals.push_back(g); rgbVals.push_back(b);
				rgbVec.push_back(rgbVals);
				rgbVals.clear();
			}
			flag=0;
		}
	}
	if(rgbVec.size()>0) {
		for(unsigned int j=0; j<rgbVec.size(); j++) {
			r = rgbVec.at(j).at(0);
			g = rgbVec.at(j).at(1);
			b = rgbVec.at(j).at(2);
			hsl.rgb2hsl(r,g,b);
			hue = hsl.getHue();
			lum = roundDecimal(hsl.getLum(),2);
			sat = roundDecimal(hsl.getSat(),2);
			grayLevel = rgb.calcGrayLevel(r,g,b);
			colorLevel = rgb.calcColorLevel(r,g,b);
			for(unsigned int i=0; i<hueThresh.size(); i++) {
				if(hue>=hueThresh.at(i).at(0) && hue<=hueThresh.at(i).at(1)) {
					if(sat>=satThresh.at(i).at(0) && sat<=satThresh.at(i).at(1)) {
						if(lum>=lumThresh.at(i).at(0) && lum<=lumThresh.at(i).at(1)) {
							pix = hslColors.at(i);
							if(pix=="White" || pix=="Black") break;
							if(pix=="Gray")	{
								pix += toString(grayLevel);
								break;
							}
							if(grayLevel==0) {
								pix = hslColors.at(i) + toString(colorLevel);
							}
							else {
								pix = color1 + toString(grayLevel) + hslColors.at(i) + toString(colorLevel);
							}
							break;
						}
					}
				}
				else {
					pix = color1 + toString(grayLevel) + color2 + toString(colorLevel);
				}
			}
			normR = (double)r/(r+g+b); normG = (double)g/(r+g+b); normB = (double)b/(r+g+b);
			fprintf(fp,"%s,%d,%d,%d,%f,%f,%f,%d,%f,%f\n",pix.c_str(),r,g,b,
					normR,normG,normB,hue,sat,lum);
			pix.clear();
		}
	}
	String min = minFormulaR1C1(1,rgbVec.size());
	String max = maxFormulaR1C1(1,rgbVec.size());
	fprintf(fp,",,,,,,,%s,%s,%s\n",min.c_str(),min.c_str(),min.c_str());
	fprintf(fp,",,,,,,,%s,%s,%s\n",max.c_str(),max.c_str(),max.c_str());
}
