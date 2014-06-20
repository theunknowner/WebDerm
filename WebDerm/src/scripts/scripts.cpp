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
	deque<String> vecColor;
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
		colorLevel = rgb.calcColorLevel2(r,g,b);
		//grayLevel = rgb.calcGrayLevel(r,g,b);
		grayLevel = rgb.calcGrayLumLevel(r,g,b);
		color.extractColorFromString(rgbColors.at(i),vecColor);
		for(unsigned int j=0; j<vecColor.size(); j++)
		{
			if(vecColor.at(j)=="Gray" && grayLevel!=0) {
				pix += "Gray" + toString(grayLevel);
			}
			else {
				if(vecColor.size()==3) {
					int tempLvl = (colorLevel/2);
					if(j==1) {
						tempLvl = colorLevel - tempLvl;
						pix += vecColor.at(j) + toString(tempLvl);
					}
					if(j==2) {
						pix += vecColor.at(j) + toString(tempLvl);
					}
				}
				else
					pix += vecColor.at(j) + toString(colorLevel);
			}
		}
			fprintf(fp,"%s,%f,%f,%f,%f,%f,%f\n",
					pix.c_str(),absMeanThresh[i][0],absMeanThresh[i][1],absMeanThresh[i][2],
					normMeanThresh[i][0],normMeanThresh[i][1],normMeanThresh[i][2]);
			pix.clear(); vecColor.clear();
	}
}

void colorThreshRenamingScript()
{
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/colorThresh.csv","w");
	hsl hsl;
	rgb rgb;
	Color color;
	double r,g,b;
	double hue=0;
	String pix;
	deque<String> vecColor;
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
		hue = hsl.getHue();
		colorLevel = rgb.calcColorLevel2(r,g,b);
		grayLevel = rgb.calcGrayLumLevel(r,g,b);
		color.extractColorFromString(rgbColors.at(i),vecColor);
		for(unsigned int j=0; j<vecColor.size(); j++)
		{
			if(vecColor.at(j)=="Gray" && vecColor.size()==1) {
				for(unsigned int k=0; k<hueThresh.size(); k++) {
					if(hue>=hueThresh.at(k).at(0) && hue<=hueThresh.at(k).at(1)) {
						pix = "Gray" + toString(grayLevel) + hslColors.at(k) + toString(colorLevel);
					}
				}
			}
			else if(vecColor.at(j)=="Gray") {
				pix += "Gray" + toString(grayLevel);
			}
			else {
				pix += vecColor.at(j) + toString(colorLevel);
			}
		}
		//if(grayLevel>=93) pix = "Grey" + toString(colorLevel);
		if(vecColor.at(0)=="Black") pix = "Black" + toString(colorLevel);
			fprintf(fp,"%s,%f,%f,%f,%f,%f,%f\n",
					pix.c_str(),absMeanThresh[i][0],absMeanThresh[i][1],absMeanThresh[i][2],
					normMeanThresh[i][0],normMeanThresh[i][1],normMeanThresh[i][2]);
			pix.clear(); vecColor.clear();
	}
	fclose(fp);
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
	fclose(fp);
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
	deque<int> rgbVals;
	deque< deque<int> > rgbList;
	deque<String> vecColor;
	deque<Point> points;
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
		for(unsigned int j=0; j<vecColor.size(); j++) {
			if(vecColor.size()==1 && vecColor.at(j)!="Gray") {
				pix = pix + "Gray" + toString(lumLevel) + vecColor.at(j) + toString(lumLevel);
			}
			else {
				pix = pix + vecColor.at(j) + toString(lumLevel);
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
		deque< deque<String> > matVec1;
		deque <deque<String> > matVec2;
		deque<String> vec;
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
		deque< deque<String> >().swap(matVec1);
		deque< deque<String> >().swap(matVec2);
		deque<String>().swap(vec);
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
	double dist=0;
	for(int row=0; row<img.rows; row++) {
		for(int col=0; col<img.cols; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.pushColor(r,g,b,ind,dist);
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
	deque<double> rgbVals;
	deque< deque<double> > rgbVec;
	int r,g,b, hue,flag=0;
	double lum,sat;
	double grayLumLevel=0, colorLevel=0,grayLevel=0;
	double normR, normG, normB;
	double dist=0;
	for(int row=(pt1.y-1); row<pt2.y; row++) {
		for(int col=(pt1.x-1); col<pt2.x; col++) {
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			for(unsigned int i=0; i<absMeanThresh.size(); i++) {
				dist = rgb.absEucDist(r,g,b,absMeanThresh.at(i));
				if(dist<=2) {
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
					if(dist<=2) {
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
			grayLumLevel = rgb.calcGrayLumLevel(r,g,b);
			colorLevel = rgb.calcColorLevel2(r,g,b);
			grayLevel = rgb.calcGrayLevel(r,g,b);
			for(unsigned int i=0; i<hueThresh.size(); i++) {
				if(hue>=hueThresh.at(i).at(0) && hue<=hueThresh.at(i).at(1)) {
					if(sat>=satThresh.at(i).at(0) && sat<=satThresh.at(i).at(1)) {
						if(lum>=lumThresh.at(i).at(0) && lum<=lumThresh.at(i).at(1)) {
							pix = hslColors.at(i);
							if(pix=="White" || pix=="Black") break;
							if(pix=="Gray")	{
								pix += toString(grayLumLevel);
								break;
							}
							if(grayLevel==0) {
								pix = hslColors.at(i) + toString(colorLevel);
							}
							else {
								pix = color1 + toString(grayLumLevel) + hslColors.at(i) + toString(colorLevel);
							}
							break;
						}
					}
				}
				else {
					if(color2=="Grey")
						pix = color2 + toString(colorLevel);
					else
						pix = color1 + toString(grayLumLevel) + color2 + toString(colorLevel);
				}
			}
			normR = (double)r/(r+g+b); normG = (double)g/(r+g+b); normB = (double)b/(r+g+b);
			fprintf(fp,"%s,%d,%d,%d,%f,%f,%f,%d,%f,%f\n",pix.c_str(),r,g,b,
					normR,normG,normB,hue,sat,lum);
			pix.clear();
		}
	}
	fclose(fp);
}

//generates a csv file with info of specified pixel-location
void generateColorRegionTable(Mat &img, Point pt, Size size) {
	int ptY = pt.y-1;
	int ptX = pt.x-1;
	int width = size.width;
	int height = size.height;
	int r,g,b;
	double h;
	String pix,pix2;
	double dist = -1.0;
	int ind=0;
	double lum=0, grayLevel=0, colorLevel=0, grayLumLevel=0;
	double ratio=0, ratio2=0;
	rgb rgb;
	hsl hsl;
	Color c;
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/ColorRegionTable.csv","w");
	fprintf(fp,"Coord,Color,RGB,Hue,Dist Push,GrayLevel,GrayLumLevel,ColorLevel,Darkness,GL/CL,GLL/CL\n");
	for(int y=ptY; y<ptY+height; y++) {
		for(int x=ptX; x<ptX+width; x++) {
			r = img.at<Vec3b>(y,x)[2];
			g = img.at<Vec3b>(y,x)[1];
			b = img.at<Vec3b>(y,x)[0];
			hsl.rgb2hsl(r,g,b);
			h = hsl.getHue();
			pix = rgb.calcColor(r,g,b,dist,ind);
			pix2 = c.getMainColor(pix);
			dist = roundDecimal(dist,1);
			lum = rgb.calcPerceivedBrightness(r,g,b);
			lum = round(lum);
			grayLumLevel = rgb.calcGrayLumLevel(r,g,b);
			grayLevel = rgb.calcGrayLevel(r,g,b);
			//if(pix2=="Grey") grayLevel = 100;
			colorLevel = rgb.calcColorLevel2(r,g,b);
			ratio2 = grayLumLevel/colorLevel;
			ratio = grayLevel/colorLevel;
			fprintf(fp,"(%d;%d),%s,(%d;%d;%d),%f,%f,%f,%f,%f,%f,%f,%f\n",x+1,y+1,pix.c_str(),r,g,b,h,
					dist,grayLevel,grayLumLevel,colorLevel,lum,ratio,ratio2);
			dist = -1.0;
		}
	}
	fclose(fp);
}

//generates a csv file with info of specified pixel-location
void generateColorRegionTable(Mat &img, Point pt, Point pt2) {
	int ptY = pt.y-1;
	int ptX = pt.x-1;
	int r,g,b;
	double h;
	String pix,pix2;
	double dist = -1.0;
	int ind=0;
	double lum=0, grayLevel=0, colorLevel=0, grayLumLevel=0;
	double ratio=0, ratio2=0;
	rgb rgb;
	hsl hsl;
	Color c;
	FILE * fp;
	fp = fopen("/home/jason/Desktop/workspace/ColorRegionTable.csv","w");
	fprintf(fp,"Coord,Color,RGB,Hue,Dist Push,GrayLevel,GrayLumLevel,ColorLevel,Darkness,GL/CL,GLL/CL\n");
	for(int y=ptY; y<pt2.y; y++) {
		for(int x=ptX; x<pt2.x; x++) {
			r = img.at<Vec3b>(y,x)[2];
			g = img.at<Vec3b>(y,x)[1];
			b = img.at<Vec3b>(y,x)[0];
			hsl.rgb2hsl(r,g,b);
			h = hsl.getHue();
			pix = rgb.calcColor(r,g,b,dist,ind);
			pix2 = c.getMainColor(pix);
			dist = roundDecimal(dist,1);
			lum = rgb.calcPerceivedBrightness(r,g,b);
			lum = round(lum);
			grayLumLevel = rgb.calcGrayLumLevel(r,g,b);
			grayLevel = rgb.calcGrayLevel(r,g,b);
			//if(pix2=="Grey") grayLevel = 100;
			colorLevel = rgb.calcColorLevel2(r,g,b);
			ratio2 = grayLumLevel/colorLevel;
			ratio = grayLevel/colorLevel;
			fprintf(fp,"(%d;%d),%s,(%d;%d;%d),%f,%f,%f,%f,%f,%f,%f,%f\n",x+1,y+1,pix.c_str(),r,g,b,h,
					dist,grayLevel,grayLumLevel,colorLevel,lum,ratio,ratio2);
			dist = -1.0;
		}
	}
	fclose(fp);
}

double checkEucDist(int r,int g, int b) {
	String folderName = path+"Thresholds/";
	String filename = folderName+"color-thresholds.csv";
	fstream fsThresh(filename.c_str());
	double dist=-1;
	if(fsThresh.is_open()) {
		rgb rgb;
		String temp;
		deque<String> vec;
		deque<double> thresh;
		int index=0,minIndex=0;
		double distPrev=500;
		getline(fsThresh,temp);
		while(getline(fsThresh,temp)) {
			index++;
			getSubstr(temp,',',vec);
			for(unsigned int i=0; i<vec.size(); i++) {
				if(i>=1 && i<=3) {
					thresh.push_back(atof(vec.at(i).c_str()));
				}
			}
			dist = rgb.absEucDist(r,g,b,thresh);
			if(dist<distPrev) {
				distPrev = dist;
				minIndex=index;
			}
			thresh.clear();
			vec.clear();
			if(dist<8) break;
		}
		cout << minIndex+1 << endl;
		fsThresh.close();
		deque<String>().swap(vec);
		deque<double>().swap(thresh);
	}
	else
		cout << "Importing color-thresholds.csv failed!" << endl;

	return dist;
}

void checkColorsFromList(Mat &img, Point pt1, Point pt2) {
	rgb rgb;
	hsl hsl;
	Color c;
	int r,g,b;
	double *HSL;
	String pix;
	double dist=0;
	int ind=0,ind2=0;
	FILE *fp;
	fp = fopen("/home/jason/Desktop/workspace/checkColors.csv","w");
	fprintf(fp,"R,G,B,H,S,L,CalcColorIndex,Color,Dist,Index\n");
	for(int i=(pt1.y-1); i<pt2.y; i++) {
		for(int j=(pt1.x-1); j<pt2.x; j++) {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			pix = rgb.pushColor(r,g,b,dist,ind);
			pix = c.reassignLevels(pix,r,g,b);
			rgb.calcColor2(r,g,b,ind2);
			HSL = hsl.rgb2hsl(r,g,b);
			HSL[1] = roundDecimal(HSL[1],2);
			HSL[2] = roundDecimal(HSL[2],2);
			fprintf(fp,"%d,%d,%d,%f,%f,%f,%d,%s,%f,%d\n",r,g,b,HSL[0],HSL[1],HSL[2],
					ind2,pix.c_str(),dist,ind+2);
		}
	}
	fclose(fp);
}
