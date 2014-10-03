/*
 * hysteresis.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hysteresis.h"

void hysteresis(FileData &fd) {
	hysteresis3(fd.matImage,fd.ksize,fd.filename,fd);
}

//hysteresis moving 1 col/row at a time
void hysteresis(Mat img, Size size, String name, FileData &fd)
{
	Rgb rgb;
	Hsl hsl;
	Color colorObj;
	int dimension = size.width*size.height;
	int matchingScans = dimension/2;
	int windowFlags[dimension];
	deque< deque<String> > windowColors(img.rows, deque<String>(img.cols,""));
	deque<String> pixelColorWindow;
	deque<String> colorWindow;
	deque<int> index;
	deque<double> hueWindow;
	deque<double> satWindow;
	deque<double> lumWindow;
	deque<String> hslVec;
	int mainColorIndex[mainColors.size()];
	double mainColorLevels[mainColors.size()];
	double mainColorLevelAvg[mainColors.size()];
	String pix;
	int ind=0;
	double dist=0;
	double grayLevel=0;
	int b=0,g=0,r=0;
	double* HSL;
	int hue=0;
	double hslAvg[3]={0};
	int row=0, col=0, maxRow=0, maxCol=0;
	int count=0; //stores the amount of pixels hit
	fill_n(windowFlags,dimension,0);
	fill_n(mainColorIndex,mainColors.size(),0);
	fill_n(mainColorLevelAvg,mainColors.size(),0);
	int rowDiff=0;
	while(row<img.rows)  {
		while(col<img.cols)  {
			rowDiff = (row+size.height) - img.rows;
			if(rowDiff>0) {
				size.height -= rowDiff;
				dimension = size.height * size.width;
				matchingScans = round(dimension/2.);
			}
			maxRow = row+size.height;

			if((col+size.width)>img.cols)  maxCol= img.cols;
			else  maxCol = col+size.width;

			if(pixelColorWindow.size()==0)  {
				for(int x=col; x<(maxCol); x++)  {
					for(int y=row; y<(maxRow); y++)  {
						b = img.at<Vec3b>(y,x)[0];
						g = img.at<Vec3b>(y,x)[1];
						r = img.at<Vec3b>(y,x)[2];
						try {
							if(windowColors.at(y).at(x)=="") {
								pix = rgb.checkBlack(r,g,b);
								if(pix=="OTHER") {
									pix = rgb.calcColor(r,g,b,dist,ind);
									//if(dist>10)
									//fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
								}
								windowColors.at(y).at(x) = pix;
							}
						} catch (const std::out_of_range &oor) {
							printf("1\n");
							printf("windowColors.Size: %lu\n", windowColors.size());
							printf("ColRow(%d,%d)\n",col,row);
							printf("Point(%d,%d)\n",x,y);
							exit(1);
						}
						HSL = hsl.rgb2hsl(r,g,b);
						HSL[1] = roundDecimal(HSL[1],2);
						HSL[2] = roundDecimal(HSL[2],2);
						hueWindow.push_back(HSL[0]);
						satWindow.push_back(HSL[1]);
						lumWindow.push_back(HSL[2]);
						pixelColorWindow.push_back(windowColors.at(y).at(x));
					}
				}
			}
			else  {
				int x = 0;
				if((col+size.width-1)>=img.cols)
					x = col;
				else
					x = maxCol-1;
				for(int y=row; y<(maxRow); y++)  {
					b = img.at<Vec3b>(y,x)[0];
					g = img.at<Vec3b>(y,x)[1];
					r = img.at<Vec3b>(y,x)[2];
					try  {
						if(windowColors.at(y).at(x)=="")  {
							pix = rgb.checkBlack(r,g,b);
							if(pix=="OTHER") {
								pix = rgb.calcColor(r,g,b,dist,ind);
								//if(dist>10)
								//fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
							}
							windowColors.at(y).at(x) = pix;
						}
					} catch (const std::out_of_range &oor) {
						printf("2\n");
						printf("windowColors.Size: %lu\n", windowColors.size());
						printf("ColRow(%d,%d)\n",col,row);
						printf("Point(%d,%d)\n",x,y);
						exit(1);
					}
					HSL = hsl.rgb2hsl(r,g,b);
					HSL[1] = roundDecimal(HSL[1],2);
					HSL[2] = roundDecimal(HSL[2],2);
					hueWindow.pop_front();
					satWindow.pop_front();
					lumWindow.pop_front();
					hueWindow.push_back(HSL[0]);
					satWindow.push_back(HSL[1]);
					lumWindow.push_back(HSL[2]);
					pixelColorWindow.pop_front();
					pixelColorWindow.push_back(windowColors.at(y).at(x));
				}
			}
			for(unsigned int i=0; i<pixelColorWindow.size(); i++)  {
				for(unsigned int j=0; j<mainColors.size(); j++)  {
					if(colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j))!=0)
						mainColorIndex[j]++;
				}
			}
			for(unsigned int j=0; j<mainColors.size(); j++)  {
				if(mainColorIndex[j]>=matchingScans)  {
					index.push_back(j);
				}
			}
			double h = 0, s = 0, l = 0;
			if(index.size()!=0) {
				for(unsigned int i=0; i<pixelColorWindow.size(); i++)  {
					for(unsigned int j=0; j<index.size(); j++)  {
						mainColorLevels[index.at(j)] = rgb.getColorLevel(pixelColorWindow.at(i),
								mainColors.at(index.at(j)));
						mainColorLevelAvg[index.at(j)] += mainColorLevels[index.at(j)];
						if(pixelColorWindow.at(i).find(mainColors.at(index.at(j)))!=string::npos) {
							windowFlags[i] = 1;
						}
					}
				}
				for(int i=0; i<dimension; i++) {
					if(windowFlags[i]==true) {
						grayLevel += rgb.getGrayLevel1(pixelColorWindow.at(i));
						hue =  hueWindow.at(i) + 180;
						if(hue>=360) hue %= 360;
						hslAvg[0] += hue;
						hslAvg[1] += satWindow.at(i);
						hslAvg[2] += lumWindow.at(i);
						count++;
					}
				}
				for(unsigned int i=0; i <index.size(); i++)
					mainColorLevelAvg[index.at(i)] /= mainColorIndex[index.at(i)];

				pix.clear();
				for(unsigned int i=0; i<index.size(); i++)  {
					if(mainColors.at(index[i])=="Zero") {
						pix = mainColors.at(index[i]);
						break;
					}
					else if(mainColors.at(index[i])=="Black" || mainColors.at(index[i])=="White") {
						pix = mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
						break;
					}
					else
						pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
				}
				grayLevel = round(grayLevel/count);
				hslAvg[0] = round(hslAvg[0]/count);
				hslAvg[1] = roundDecimal(hslAvg[1]/count,2);
				hslAvg[2] = roundDecimal(hslAvg[2]/count,2);
				hslAvg[0] -= 180;
				if(hslAvg[0]<0) hslAvg[0] += 360;
				h = hslAvg[0];
				s = hslAvg[1];
				l = hslAvg[2];
				pix = colorObj.fixColors(pix,r,g,b);
				if(pix!="Zero")
					pix = toString(grayLevel) + pix;
				else {
					h=0;s=0;l=0;
				}
				colorWindow.push_back(pix);
			}
			else {
				colorWindow.push_back("Zero");
			}
			String hslStr = toString(h)+";"+toString(s)+";"+toString(l);
			hslVec.push_back(hslStr);
			grayLevel=0;
			hslAvg[0] = 0;
			hslAvg[1] = 0;
			hslAvg[2] = 0;
			count=0;
			fill_n(windowFlags,dimension,0);
			fill_n(mainColorIndex,mainColors.size(),0);
			fill_n(mainColorLevelAvg,mainColors.size(),0);
			index.clear();
			/* remove window clears when using FIFO
				hueWindow.clear();
				satWindow.clear();
				lumWindow.clear();
				pixelColorWindow.clear();*/
			col++;
		}//end while col
		fd.hslMat.push_back(hslVec);
		fd.windowVec.push_back(colorWindow);
		colorWindow.clear();
		pixelColorWindow.clear();
		hueWindow.clear();
		satWindow.clear();
		lumWindow.clear();
		hslVec.clear();

		col=0; row++;
	}//end while row
	Intensity in;
	fd.colorVec = in.calcMainColorMatrix(fd.matImage, fd.windowVec, fd.hslMat, fd.filename, fd);
	deque<String>().swap(pixelColorWindow);
	deque<String>().swap(colorWindow);
	deque<int>().swap(index);
	deque<String>().swap(hslVec);
	deque<double>().swap(hueWindow);
	deque<double>().swap(satWindow);
	deque<double>().swap(lumWindow);
}

void hysteresis2(Mat img, Size size, String name, FileData &fd) {
	Mat img2;
	blur(img,img2,size);
	Rgb rgb;
	Hsl hsl;
	int r,g,b;
	double *HSL;
	String pix;
	deque<String> colorWindow;
	deque<String> hslVec;
	for(int i=0; i<img.rows; i++)  {
		for(int j=0; j<img.cols; j++)  {
			r = img.at<Vec3b>(i,j)[2];
			g = img.at<Vec3b>(i,j)[1];
			b = img.at<Vec3b>(i,j)[0];
			HSL = hsl.rgb2hsl(r,g,b);
			HSL[1] = roundDecimal(HSL[1],2);
			HSL[2] = roundDecimal(HSL[2],2);
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER") {
				pix = rgb.calcColor(r,g,b);
			}
			colorWindow.push_back(pix);
			String hslStr = toString(HSL[0])+";"+toString(HSL[1])+";"+toString(HSL[2]);
			hslVec.push_back(hslStr);
		}
		fd.windowVec.push_back(colorWindow);
		fd.hslMat.push_back(hslVec);
		colorWindow.clear();
		hslVec.clear();
	}
	Intensity in;
	fd.colorVec = in.calcMainColorMatrix(fd.matImage, fd.windowVec, fd.hslMat, fd.filename, fd);
	deque<String>().swap(colorWindow);
	deque<String>().swap(hslVec);
}

void hysteresis3(Mat img, Size size, String name, FileData &fd) {
	Rgb rgb;
	Hsl hsl;
	Color colorObj;
	int dimension = size.width*size.height;
	int matchingScans = dimension/2;
	int windowFlags[dimension];
	deque<String> pixelColorWindow;
	deque<int> index;
	deque<double> hueWindow;
	deque<double> satWindow;
	deque<double> lumWindow;
	deque<String> hslVec;
	int mainColorIndex[mainColors.size()];
	double mainColorLevels[mainColors.size()];
	double mainColorLevelAvg[mainColors.size()];
	String pix;
	int ind=0;
	double dist=0;
	double grayLevel=0;
	int b=0,g=0,r=0;
	double* HSL;
	int hue=0;
	double hslAvg[3]={0};
	int row=0, col=0, maxRow=0, maxCol=0;
	int count=0; //stores the amount of pixels hit
	fill_n(windowFlags,dimension,0);
	fill_n(mainColorIndex,mainColors.size(),0);
	fill_n(mainColorLevelAvg,mainColors.size(),0);
	init_2D_Deque(fd.windowVec,img.cols,img.rows,"");
	int rowDiff=0, colDiff=0;
	while(row<img.rows)  {
		while(col<img.cols)  {
			rowDiff = (row+size.height) - img.rows;
			if(rowDiff>0) {
				size.height -= rowDiff;
				dimension = size.height * size.width;
				matchingScans = round(dimension/2.);
			}
			maxRow = row+size.height;

			colDiff = (col+size.height) - img.cols;
			if(colDiff>0) {
				size.width -= colDiff;
				dimension = size.height * size.width;
				matchingScans = round(dimension/2.);
			}
			maxCol = col+size.width;

			for(int x=col; x<(maxCol); x++)  {
				for(int y=row; y<(maxRow); y++)  {
					b = img.at<Vec3b>(y,x)[0];
					g = img.at<Vec3b>(y,x)[1];
					r = img.at<Vec3b>(y,x)[2];

					pix = rgb.checkBlack(r,g,b);
					if(pix=="OTHER") {
						pix = rgb.calcColor(r,g,b,dist,ind);
						//if(dist>10)
						//fprintf(fp,"%s,%f,%d,%d,%d\n",pix.c_str(),dist,r,g,b);
					}
				}
				HSL = hsl.rgb2hsl(r,g,b);
				HSL[1] = roundDecimal(HSL[1],2);
				HSL[2] = roundDecimal(HSL[2],2);
				hueWindow.push_back(HSL[0]);
				satWindow.push_back(HSL[1]);
				lumWindow.push_back(HSL[2]);
				pixelColorWindow.push_back(pix);
			}
			for(unsigned int i=0; i<pixelColorWindow.size(); i++)  {
				for(unsigned int j=0; j<mainColors.size(); j++)  {
					if(colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j))!=0)
						mainColorIndex[j]++;
				}
			}
			for(unsigned int j=0; j<mainColors.size(); j++)  {
				if(mainColorIndex[j]>=matchingScans)  {
					index.push_back(j);
				}
			}
			double h = 0, s = 0, l = 0;
			if(index.size()!=0) {
				for(unsigned int i=0; i<pixelColorWindow.size(); i++)  {
					for(unsigned int j=0; j<index.size(); j++)  {
						mainColorLevels[index.at(j)] = rgb.getColorLevel(pixelColorWindow.at(i),
								mainColors.at(index.at(j)));
						mainColorLevelAvg[index.at(j)] += mainColorLevels[index.at(j)];
						if(pixelColorWindow.at(i).find(mainColors.at(index.at(j)))!=string::npos) {
							windowFlags[i] = 1;
						}
					}
				}
				for(int i=0; i<dimension; i++) {
					if(windowFlags[i]==true) {
						grayLevel += rgb.getGrayLevel1(pixelColorWindow.at(i));
						hue =  hueWindow.at(i) + 180;
						if(hue>=360) hue %= 360;
						hslAvg[0] += hue;
						hslAvg[1] += satWindow.at(i);
						hslAvg[2] += lumWindow.at(i);
						count++;
					}
				}
				for(unsigned int i=0; i <index.size(); i++)
					mainColorLevelAvg[index.at(i)] /= mainColorIndex[index.at(i)];

				pix.clear();
				for(unsigned int i=0; i<index.size(); i++)  {
					if(mainColors.at(index[i])=="Zero") {
						pix = mainColors.at(index[i]);
						break;
					}
					else if(mainColors.at(index[i])=="Black" || mainColors.at(index[i])=="White") {
						pix = mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
						break;
					}
					else
						pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
				}
				grayLevel = round(grayLevel/count);
				hslAvg[0] = round(hslAvg[0]/count);
				hslAvg[1] = roundDecimal(hslAvg[1]/count,2);
				hslAvg[2] = roundDecimal(hslAvg[2]/count,2);
				hslAvg[0] -= 180;
				if(hslAvg[0]<0) hslAvg[0] += 360;
				h = hslAvg[0];
				s = hslAvg[1];
				l = hslAvg[2];
				pix = colorObj.fixColors(pix,r,g,b);
				if(pix!="Zero")
					pix = toString(grayLevel) + pix;
				else {
					h=0;s=0;l=0;
				}
				for(int x=col; x<(maxCol); x++)  {
					for(int y=row; y<(maxRow); y++)  {
						fd.windowVec.at(y).at(x) = pix;
					}
				}
			}
			else {
				for(int x=col; x<(maxCol); x++)  {
					for(int y=row; y<(maxRow); y++)  {
						fd.windowVec.at(y).at(x) = "Zero";
					}
				}
			}
			String hslStr = toString(h)+";"+toString(s)+";"+toString(l);
			hslVec.push_back(hslStr);
			grayLevel=0;
			hslAvg[0] = 0;
			hslAvg[1] = 0;
			hslAvg[2] = 0;
			count=0;
			fill_n(windowFlags,dimension,0);
			fill_n(mainColorIndex,mainColors.size(),0);
			fill_n(mainColorLevelAvg,mainColors.size(),0);
			index.clear();
			pixelColorWindow.clear();
			/* remove window clears when using FIFO
				hueWindow.clear();
				satWindow.clear();
				lumWindow.clear();
				pixelColorWindow.clear();*/
			col+=size.width;
		}//end while col
		fd.hslMat.push_back(hslVec);
		pixelColorWindow.clear();
		hueWindow.clear();
		satWindow.clear();
		lumWindow.clear();
		hslVec.clear();

		col=0; row+=size.height;
	}//end while row
	Intensity in;
	fd.colorVec = in.calcMainColorMatrix(fd.matImage, fd.windowVec, fd.hslMat, fd.filename, fd);
	deque<String>().swap(pixelColorWindow);
	deque<int>().swap(index);
	deque<String>().swap(hslVec);
	deque<double>().swap(hueWindow);
	deque<double>().swap(satWindow);
	deque<double>().swap(lumWindow);
}
