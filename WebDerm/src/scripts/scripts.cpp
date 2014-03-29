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
	String pix;
	vector<String> vecColor;
	int colorLevel=0;
	int grayLevel=0;
	int mainColorIndex[mainColors.size()];
	fill_n(mainColorIndex,mainColors.size(),0);
	fprintf(fp,"Color,Rmin,Rmax,Gmin,Gmax,Bmin,Bmax,rMin,rMax,gMin,gMax,bMin,bMax\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		colorLevel = rgb.calcColorLevel(r,g,b);
		grayLevel = rgb.calcGrayLevel(r,g,b);
		color.extractColorFromString(rgbColors.at(i),vecColor);
		for(unsigned int j=0; j<vecColor.size(); j++)
		{
			if(vecColor.size()==1) {
				pix = pix + vecColor.at(j) + toString(colorLevel);
			} else if(vecColor.size()==2) {
				if(j==0) {
					pix = pix + vecColor.at(j) + toString(grayLevel);
				}
				else {
					pix = pix + vecColor.at(j) + toString(colorLevel);
				}
			} else if(vecColor.size()==3) {
				if(j==0) {
					pix = pix + vecColor.at(j) + toString(grayLevel);
				}
				else {
					colorLevel = rgb.getColorLevel(rgbColors.at(i),vecColor.at(j));
					pix = pix + vecColor.at(j) + toString(colorLevel);
				}
			}
		}
			fprintf(fp,"%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
					pix.c_str(),absThresh[i][0],absThresh[i][1],absThresh[i][2],absThresh[i][3],
					absThresh[i][4],absThresh[i][5],normThresh[i][0],normThresh[i][1],
					normThresh[i][2],normThresh[i][3],normThresh[i][4],normThresh[i][5]);
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
	int grayLevel=0;
	int colorLevel=0;
	double s,l;
	fprintf(fp,"Color,S,Slvl,L,Lvl\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		hsl.rgb2hsl(r,g,b);
		s = hsl.getSat();
		l = hsl.getLum();
		s = roundDecimal(s,2);
		l = roundDecimal(l,2);
		grayLevel = rgb.calcGrayLevel(r,g,b);
		colorLevel = rgb.calcColorLevel(r,g,b);
		fprintf(fp,"%s,%f,%d,%f,%d\n", rgbColors.at(i).c_str(), s,grayLevel,l,colorLevel);
	}
}

void outputMeanScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/mean.csv", "w");
	double r,g,b;
	fprintf(fp,"Color,R,G,B\n");
	for(unsigned int i=0; i<rgbColors.size(); i++)
	{
		r = absMeanThresh.at(i).at(0);
		g = absMeanThresh.at(i).at(1);
		b = absMeanThresh.at(i).at(2);
		fprintf(fp,"%s,%f,%f,%f\n",rgbColors.at(i).c_str(),r,g,b);
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
void addNewColors(Mat &img, Point pt1, Point pt2) {
	rgb rgb;
	hsl hsl;
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/newColors.csv","w");
	String pix;
	String color1 = "Gray";
	vector<int> rgbVals;
	int r,g,b, hue;
	int grayLevel=0, colorLevel=0;
	double normR, normG, normB;
	for(int row=pt1.y; row<pt2.y; row++) {
		for(int col=pt1.x; col<pt2.x; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			rgbVals.push_back(r); rgbVals.push_back(g); rgbVals.push_back(b);
			hsl.rgb2hsl(r,g,b);
			hue = hsl.getHue();
			grayLevel = rgb.calcGrayLevel(r,g,b);
			colorLevel = rgb.calcColorLevel(r,g,b);
			for(unsigned int i=0; i<hueThresh.size(); i++) {
				if(hue>=hueThresh.at(i).at(0) && hue<=hueThresh.at(i).at(1)) {
					pix = color1 + toString(grayLevel) + hslColors.at(i) + toString(colorLevel);
					normR = (double)r/(r+g+b); normG = (double)g/(r+g+b); normB = (double)b/(r+g+b);
					fprintf(fp,"%s,%d,%d,%d,%f,%f,%f\n",pix.c_str(),r,g,b,
							normR,normG,normB);
				}
			}
		}
	}
}

void getThresholdColors() {
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/newColors2.csv","w");
	for(unsigned int i=0; i<absMeanThresh.size(); i++) {
		fprintf(fp,"%s,",rgbColors.at(i).c_str());
		for(unsigned int j=0; j<absMeanThresh.at(i).size(); j++) {
			fprintf(fp,"%f,",absMeanThresh.at(i).at(j));
		}
		for(unsigned int j=0; j<normMeanThresh.at(i).size(); j++) {
			fprintf(fp,"%f,",normMeanThresh.at(i).at(j));
		}
		fprintf(fp,"\n");
	}
}

