/*
 * Entropy.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#include "entropy.h"
#include "../FileData/filedata.h"
#include "../functions.h"
#include "../Algorithms/write.h"
#include "../Algorithms/stddev.h"
#include "../Shape/shapemorph.h"
#include "../neuralnetworks/testml.h"
#include "../Shape/shapecolor.h"
#include "../Shades/shades.h"
#include "../rgb/rgb.h"
#include "../hsl/hsl.h"
#include "../Color/color.h"

using namespace ip;

deque<deque< deque< deque<double> > > > vec;
deque<deque< deque< deque<double> > > > vec2;
deque<deque< deque< deque<int> > > > gTargetCellCount;
double vecTotal[5] = {0};

deque<deque<deque<deque<double> > > > gRatio;
deque<deque<deque< deque<double> > > > gSmoothRatio;
deque<deque<deque< deque<double> > > > gSmoothRatioRm;
deque<deque<deque< deque<double> > > > gRatioRm;

void Entropy::eyeFn(FileData &fd, Size ksize, Mat map, String targetColor,String targetShade) {
	int height = fd.colorVec.size()/ksize.height;
	int width = fd.colorVec.at(0).size()/ksize.width;
	int innerHeight = Rgb::allColors.size();
	int innerWidth = Shades::g_Shades2.size();
	vec = createDeque4D(height,width,innerHeight,innerWidth,0.);
	vec2 = createDeque4D(height,width,innerHeight,innerWidth,0.);
	gTargetCellCount = createDeque4D(height,width,innerHeight,innerWidth,0);
	deque<deque<deque<deque<double> > > > ratio(height,deque<deque<deque<double> > >(width,deque<deque<double> >(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0))));
	deque<deque<deque<deque<double> > > > smoothRatio(height,deque<deque<deque<double> > >(width,deque<deque<double> >(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0))));
	Rgb rgb;
	Hsl hsl;
	Color c;
	Shades sh;
	Functions fn;
	String color,shade,pix;
	double h,s,l;
	int shadeIndex,colorIndex;
	int cellSize = ksize.height*ksize.width;
	deque< deque<double> > pShadeColor(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0));
	this->totalPopulation.resize(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0));
	this->populationDensity.resize(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0));
	this->densityVariation.resize(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0));
	double pTotal=0;
	unsigned int row=0, col=0;
	int i=0,j=0, maxRow=0, maxCol=0;
	if(map.empty()) map = map.ones(fd.colorVec.size(),fd.colorVec.at(0).size(),CV_8U);
	try {
		while(row<=(fd.colorVec.size()-ksize.height)) {
			while(col<=(fd.colorVec.at(row).size()-ksize.width)) {
				if((row+ksize.height+1)>fd.colorVec.size())
					maxRow = fd.colorVec.size();
				else
					maxRow = row+ksize.height;
				if((col+ksize.width+1)>fd.colorVec.at(row).size())
					maxCol = fd.colorVec.at(row).size();
				else
					maxCol = col+ksize.width;

				for(i=row; i<maxRow; i++) {
					for(j=col; j<maxCol; j++) {
						if(map.at<uchar>(i,j)>0) {
							try {
								pix = fd.colorVec.at(i).at(j);
								shade = sh.extractShade(pix);
								color = c.getMainColor(pix);
								color = c.optimizeColor2(color);
								/**temporary testing**/
								if(shade=="Dark1" && color=="Grey")
									color = "Black";
								shade = sh.combineShades(shade);
								color = c.combineColors(color);
								if(color=="Violet" || color =="Purple")
									shade = "High";
								if(fd.filename.find("acne")!=string::npos)  {
									if(color=="Violet")  {
										int index=-1;
										h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
										s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
										l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
										hsl.getHslColor(h,s,l,index);
										color = hsl.getHslColor(index+1);
										//color = "BrownRed";
									}
								}
								if(fd.filename.find("psoriasis")!=string::npos)  {
									if(color=="Violet")  {
										int index=-1;
										h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
										s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
										l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
										hsl.getHslColor(h,s,l,index);
										color = hsl.getHslColor(index+1);
										//color = "BrownRed";
									}
								}
								if(fd.filename.find("herpes")!=string::npos)  {
									if(color=="Violet")  {
										int index=-1;
										h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
										s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
										l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
										hsl.getHslColor(h,s,l,index);
										color = hsl.getHslColor(index+1);
										//color = "BrownRed";
									}
								}
								if(fd.filename.find("herpes5")!=string::npos) {
									if(color=="BrownOrange")
										color = "BrownPink";
								}
								/************************************/
								shadeIndex = sh.getShadeIndex2(shade);
								if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)
									color = "Black";
								else if(shade=="White" || color.find("White")!=string::npos)
									color = "White";
								colorIndex = rgb.getColorIndex(color);
								if(color!="Zero" || colorIndex>=0)
									++pShadeColor.at(colorIndex).at(shadeIndex);
							}
							catch(const std::out_of_range& oor) {
								printf("Entropy::eyeFn() out of range 1!\n");
								printf("ColorVec.Size: %lu\n",fd.colorVec.size());
								printf("HslMat.Size: %lu\n",fd.hslMat.size());
								printf("Point(%d,%d)\n",j,i);
								printf("Shade: %s\n",shade.c_str());
								printf("ShadeIndex: %d\n",shadeIndex);
								printf("Color: %s\n",color.c_str());
								printf("ColorIndex: %d\n",colorIndex);
								printf("pShadeColor.Size: %lu\n",pShadeColor.size());
								printf("pShadeColor(%d,%d)\n",j,i);
								exit(1);
							}
						}
					}
				}
				for(unsigned int colorRow=0; colorRow<Rgb::allColors.size(); colorRow++) {
					for(unsigned int shadeCol=0; shadeCol<Shades::g_Shades2.size(); shadeCol++) {
						try {
							pTotal = pShadeColor.at(colorRow).at(shadeCol)/cellSize;
							ratio[row/ksize.height][col/ksize.width][colorRow][shadeCol] = pTotal;
						}
						catch (const std::out_of_range &oor) {
							printf("pShadeColor.Rows: %lu\n", pShadeColor.size());
							printf("pShadeColor(%d,%d)\n",colorRow,shadeCol);
							printf("pEntropy(%d,%d)\n",colorRow,shadeCol);
							exit(1);
						}
					}
				}
				col+=ksize.width;
				pShadeColor.clear();
				pShadeColor.resize(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0));
			}
			row += ksize.height;
			col=0;
		}
	}
	catch (const std::out_of_range &oor) {
		printf("colorVec.Rows: %lu\n",fd.colorVec.size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}

	deque< deque<int> > cellCount(Rgb::allColors.size(),deque<int>(Shades::g_Shades2.size(),0));
	deque< deque<int> > targetCellCount(Rgb::allColors.size(),deque<int>(Shades::g_Shades2.size(),0));
	deque<deque<deque<Point> > > ratioPtsList(Rgb::allColors.size(), deque<deque<Point> >(Shades::g_Shades2.size(),deque<Point>(1,Point(-1,-1))));
	deque<deque<deque<double> > > ratioSingleList(Rgb::allColors.size(), deque<deque<double> >(Shades::g_Shades2.size(),deque<double>(1,-1)));
	deque<deque<deque<deque<double> > > > smoothRatioOutlierRm(height,deque<deque<deque<double> > >(width,deque<deque<double> >(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0))));
	deque<deque<deque<deque<double> > > > ratioOutlierRm(height,deque<deque<deque<double> > >(width,deque<deque<double> >(Rgb::allColors.size(),deque<double>(Shades::g_Shades2.size(),0))));
	int x1=0,y1=0,minRow=0,minCol=0;
	double count=0, min=0.03;
	double outlierThresh = 0.05;
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			for(int c=0; c<innerHeight; c++) {
				for(int d=0; d<innerWidth; d++) {
					if(ratio[y][x][c][d]>min) {
						/* totalPopulation (Y) = sum of density (Si) */
						totalPopulation.at(c).at(d) += ratio[y][x][c][d];
					}
				}
			}
		}
	}
	while(y1<height) {
		minRow=maxRow=y1;
		if((minRow-1)>=0) minRow-=1;
		if((maxRow+1)<height) maxRow+=1;
		while(x1<width) {
			minCol=maxCol=x1;
			if((minCol-1)>=0) minCol-=1;
			if((maxCol+1)<width) maxCol+=1;
			for(int a=minRow; a<=maxRow; a++)  {
				for(int b=minCol; b<=maxCol; b++)	{
					for(int c=0; c<innerHeight; c++) {
						for(int d=0; d<innerWidth; d++) {
							if(ratio[y1][x1][c][d]>min) {
								smoothRatio[y1][x1][c][d] += ratio[a][b][c][d];
							}
						}
					}
					++count;
				}
			}
			for(int c=0; c<innerHeight; c++) {
				for(int d=0; d<innerWidth; d++) {
					smoothRatio[y1][x1][c][d] /=count;
					if(ratio[y1][x1][c][d]>min) {
						if(ratioSingleList.at(c).at(d).size()>0) {
							if(ratioSingleList.at(c).at(d).at(0)==-1) {
								ratioSingleList.at(c).at(d).pop_front();
								ratioPtsList.at(c).at(d).pop_front();
							}
						}
						ratioSingleList.at(c).at(d).push_back(ratio[y1][x1][c][d]);
						ratioPtsList.at(c).at(d).push_back(Point(x1,y1));

						cellCount.at(c).at(d)++;
						/* density(S) = sum( (Si^2) / Y ) */
						if(ratio[y1][x1][c][d]>min)
							populationDensity.at(c).at(d) += (ratio[y1][x1][c][d] * ratio[y1][x1][c][d] / totalPopulation.at(c).at(d));
						if(targetColor!="" && targetShade!="") {
							int index = rgb.getColorIndex(targetColor);
							int shadeIndex = sh.getShadeIndex2(targetShade);
							if(c==index && d==shadeIndex) {
								//pt.at(y1).at(x1) = 1;
								targetCellCount.at(c).at(d) = cellCount.at(c).at(d);
							}
						}
					}
					if(targetColor!="" && targetShade!="") {
						int index = rgb.getColorIndex(targetColor);
						int shadeIndex = sh.getShadeIndex2(targetShade);
						if(c==index && d==shadeIndex) {
							vec[y1][x1][c][d] = totalPopulation.at(c).at(d);
							vec2[y1][x1][c][d] = populationDensity.at(c).at(d);
							gTargetCellCount[y1][x1][c][d] = targetCellCount.at(c).at(d);
						}
					}
				}
			}
			count=0;
			++x1;
		}
		x1=0;
		++y1;
	}
	//start of standard dev (V)
	ratioOutlierRm = ratio;
	deque<double> sortedList;
	deque<Point> sortedPtsList;
	for(i=0; i<innerHeight; i++) {
		for (j=0; j<innerWidth; j++) {
			if(ratioSingleList.at(i).at(j).size()>0) {
				if(ratioSingleList.at(i).at(j).at(0)!=-1) {
					if(sortedList.size()==0) {
						sortedList.push_back(ratioSingleList.at(i).at(j).at(0));
						sortedPtsList.push_back(ratioPtsList.at(i).at(j).at(0));
					}
					for(unsigned int d=1; d<ratioSingleList.at(i).at(j).size(); d++) {
						for(unsigned int e=0; e<sortedList.size(); e++) {
							if((sortedList.size()-e)<=1) {
								if(ratioSingleList.at(i).at(j).at(d)>=sortedList.at(e)) {
									sortedList.push_back(ratioSingleList.at(i).at(j).at(d));
									sortedPtsList.push_back(ratioPtsList.at(i).at(j).at(d));
									break;
								}
								else {
									sortedList.push_front(ratioSingleList.at(i).at(j).at(d));
									sortedPtsList.push_front(ratioPtsList.at(i).at(j).at(d));
									break;
								}
							}
							else {
								if(ratioSingleList.at(i).at(j).at(d)>=sortedList.at(e) && ratioSingleList.at(i).at(j).at(d)<=sortedList.at(e+1)) {
									sortedList.insert(sortedList.begin()+e+1,ratioSingleList.at(i).at(j).at(d));
									sortedPtsList.insert(sortedPtsList.begin()+e+1,ratioPtsList.at(i).at(j).at(d));
									break;
								}
								if(ratioSingleList.at(i).at(j).at(d)<=sortedList.at(0)) {
									sortedList.push_front(ratioSingleList.at(i).at(j).at(d));
									sortedPtsList.push_front(ratioPtsList.at(i).at(j).at(d));
									break;
								}
							}
						}
					}
					double t = sortedList.size()*outlierThresh;
					t = round(t);
					for(int d=0; d<t; d++) {
						ratioOutlierRm[sortedPtsList.front().y][sortedPtsList.front().x][i][j]=0;
						ratioOutlierRm[sortedPtsList.back().y][sortedPtsList.back().x][i][j]=0;
						sortedPtsList.pop_back();
						sortedPtsList.pop_front();
						sortedList.pop_back();
						sortedList.pop_front();
					}
					sortedList.clear();
					sortedPtsList.clear();
					deque<double>().swap(sortedList);
					deque<Point>().swap(sortedPtsList);
				}
			}
		}//end for innerWidth
	}//end for innerHeight
	ratioSingleList.clear();
	ratioSingleList.resize(Rgb::allColors.size(),deque<deque<double> >(Shades::g_Shades2.size(),deque<double>(1,-1)));
	x1=0;y1=0;minRow=0;minCol=0;maxRow=0;maxCol=0;
	while(y1<height) {
		while(x1<width) {
			for(int c=0; c<innerHeight; c++) {
				for(int d=0; d<innerWidth; d++) {
					if(ratioOutlierRm[y1][x1][c][d]>min) {
						if(ratioSingleList.at(c).at(d).size()>0) {
							if(ratioSingleList.at(c).at(d).at(0)==-1) {
								ratioSingleList.at(c).at(d).pop_front();
							}
						}
						ratioSingleList.at(c).at(d).push_back(ratioOutlierRm[y1][x1][c][d]);
					}
				}
			}
			++x1;
		}
		x1=0;
		++y1;
	}
	for(int c=0; c<innerHeight; c++) {
		for (int d=0; d<innerWidth; d++) {
			if(ratioSingleList.at(c).at(d).size()>0) {
				if(ratioSingleList.at(c).at(d).at(0)!=-1) {
					densityVariation.at(c).at(d) = Algos::standardDev(ratioSingleList.at(c).at(d));
				}
			}
		}
	}
	if(targetColor=="" && targetShade=="") {
		this->writeEntropyFile(fd.filename,fd);
	}
	gRatio = ratio;
	gSmoothRatio = smoothRatio;
	gSmoothRatioRm = smoothRatioOutlierRm;
	gRatioRm = ratioOutlierRm;
	this->totalPopulation.clear();
	this->populationDensity.clear();
	this->densityVariation.clear();
}

Mat Entropy::showEyeFnSquares(Mat img, Size ksize, String targetColor, String targetShade)  {
	Mat dst = img.clone();
	Point end;
	String dark,high,low,light,white;
	String low2;
	String data,data2,data3;
	Rgb rgb;
	Shades sh;
	int indexColor = rgb.getColorIndex(targetColor);
	int indexShade = sh.getShadeIndex2(targetShade);
	resize(dst,dst,Size(700,700));
	ksize = Size(50,50);
	for(int i=0; i<dst.rows; i+=ksize.height)  {
		for(int j=0; j<dst.cols; j+=ksize.width)  {
			if((i+ksize.width)>dst.cols) end = Point(dst.cols-1,dst.rows-1);
			else end = Point(j+ksize.width,i+ksize.height);
			//data = toString(roundDecimal(vec[i/ksize.height][j/ksize.width][2],3));
			data3 = toString(roundDecimal(gTargetCellCount[i/ksize.height][j/ksize.width][indexColor][indexShade],3));
			data2 = toString(roundDecimal(vec2[i/ksize.height][j/ksize.width][indexColor][indexShade],3));
			//dark = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][0],3));
			//high = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][1],3));
			low2 = toString(roundDecimal(gRatio[i/ksize.height][j/ksize.width][indexColor][indexShade],3));
			low = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][indexShade],3));
			//data = toString(roundDecimal(gRatioRm[i/ksize.height][j/ksize.width][indexColor][2],3));
			//data2 = toString(roundDecimal(gSmoothRatioRm[i/ksize.height][j/ksize.width][indexColor][2],3));
			//light = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][3],3));
			//white = toString(vec[i/ksize.height][j/ksize.width][4]);
			//putText(dst,dark,Point(j+5,i+10),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			//putText(dst,high,Point(j+5,i+20),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));

			//putText(dst,data3,Point(j+5,i+10),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			//putText(dst,data2,Point(j+5,i+20),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,low2,Point(j+5,i+30),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,low,Point(j+5,i+40),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));

			//putText(dst,light,Point(j+5,i+40),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			rectangle(dst,Point(j,i),Point(end.x,end.y),Scalar(0,0,255));
		}
	}
	//namedWindow("output",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//imshow("output",dst);
	//waitKey(0);
	return dst;
}

void Entropy::writeEntropyFile(String filename, FileData &fd) {
	String strSize = toString(this->entSize.width)+"x"+toString(this->entSize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	cout << filename  << ": Writing YSV files..." << flush;
	String filename4 = filename+ "-"+ file_ksize+"-YSV-Combined"+strSize+".csv";
	String shadeColor, shade, color;
	FILE * fp4;
	fp4 = fopen(filename4.c_str(),"w");
	fprintf(fp4,",Y,S,V,T\n");
	for(unsigned int j=0; j<Shades::g_Shades2.size(); j++) {
		for(unsigned int i=0; i<Rgb::allColors.size(); i++) {
			if(i==0 && j==0) {
				if(j>=0 && j<=3) {
					shadeColor = Shades::g_Shades2.at(j)+Rgb::allColors.at(i);
					fprintf(fp4,"%s,%f,%f,%f,",shadeColor.c_str(),this->totalPopulation.at(i).at(j),this->populationDensity.at(i).at(j),this->densityVariation.at(i).at(j));
					for(unsigned int k=0; k<this->shapeMetric.size();k++) {
						fprintf(fp4,"%f,",this->shapeMetric.at(k));
					}
					fprintf(fp4,"\n");
				}
			}
			else {
				if(j>=0 && j<=3) {
					shadeColor = Shades::g_Shades2.at(j)+Rgb::allColors.at(i);
					fprintf(fp4,"%s,%f,%f,%f\n",shadeColor.c_str(),this->totalPopulation.at(i).at(j),this->populationDensity.at(i).at(j),this->densityVariation.at(i).at(j));
				}
			}
		}
	}
	cout << "Done!" << endl;
	if(fp4!=NULL)
		fclose(fp4);

}

void Entropy::shapeFn(FileData &fd) {
	ShapeMorph sm;
	Mat img1 = fd.getImage();
	vector<Mat> matVec = sm.lumFilter1(img1);
	vector<Mat> matVec2 = sm.lumFilter2(img1);
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	vector<Mat> sampleVec, sampleVec2;
	//crops & fixes up the binary sample set
	for(unsigned int i=0; i<matVec.size(); i++) {
		Mat sample = ml.prepareImage(matVec.at(i),Size(20,20));
		Mat sample2 = ml.prepareImage(matVec2.at(i),Size(20,20));
		sampleVec.push_back(sample);
		sampleVec2.push_back(sample2);
		sample.release();
		sample2.release();
	}
	Mat results = ml.runANN(param,sampleVec);
	Mat results2 = ml.runANN(param,sampleVec2);
	//gets the index of the largest output for each sample
	deque<int> largestOutputVec;
	double max=results.at<float>(0,0);
	double max2=results2.at<float>(0,0);
	int idx=0, idx2=0;
	for(int i=0; i<results.rows; i++) {
		for(int j=0; j<results.cols; j++) {
			try {
				if(results.at<float>(i,j)>max) {
					max = results.at<float>(i,j);
					idx = j;
				}
				if(results2.at<float>(i,j)>max2) {
					max2 = results2.at<float>(i,j);
					idx2 = j;
				}
			}
			catch(const std::out_of_range &oor) {
				printf("Catch #1: Entropy::shapeFn() out of range!\n");
				exit(1);
			}
		}
		largestOutputVec.push_back(idx);
		largestOutputVec.push_back(idx2);
	}
	//count circle or random base on largest output
	deque<int> shapeVec(largestOutputVec.size(),0);
	for(unsigned int i=0; i<largestOutputVec.size(); i++) {
		if(largestOutputVec.at(i)<(results.cols-1))
			shapeVec.at(i) = 1;
		else
			shapeVec.at(i) = 0;
	}

	// this this the scale from 0->1 for chances of circle
	// 0=High random, 0.33=Low circle, 0.66=Med circle, 1=High circle
	this->shapeMetric.clear();
	if(shapeVec.at(0)==1 && shapeVec.at(1)==1)
		this->shapeMetric.push_back(1.0);
	if(shapeVec.at(0)==1 && shapeVec.at(1)==0)
		this->shapeMetric.push_back(0.66);
	if(shapeVec.at(0)==0 && shapeVec.at(1)==1)
		this->shapeMetric.push_back(0.33);
	if(shapeVec.at(0)==0 && shapeVec.at(1)==0)
		this->shapeMetric.push_back(0.0);
}

void Entropy::shapeFn2(FileData &fd) {
	ShapeMorph sm;
	ShapeColor sc;
	Mat img = fd.getImage();
	Mat imgGray;
	cvtColor(img,imgGray,CV_BGR2GRAY);
	Mat src = sm.prepareImage(imgGray);
	Mat mapOfNonNoise = sm.removeNoiseOnBoundary(src);
	Mat map = sc.getShapeUsingColor(img,mapOfNonNoise);
	Mat maskEmax = sc.removeRunningLines(map,Size(3,1));
	Mat maskLC;
	src.copyTo(maskLC,mapOfNonNoise);
	maskLC = sc.filterKneePt(maskLC);
	src.copyTo(maskLC,maskEmax);
	Mat img2 = maskLC * 255;
	img2 = sm.densityConnector(img2,0.9999);
	vector<Mat> islands = sm.liquidFeatureExtraction(img2,0.0,1);
	Mat img3 = sm.haloTransform(islands.at(0));
	img3.convertTo(img3,CV_8U);
	img3 *= 255;
	TestML ml;
	String param = "/home/jason/git/Samples/Samples/param.xml";
	vector<Mat> sampleVec;
	Mat results = ml.runANN(param,sampleVec);
	//gets the index of the largest output for each sample
	double max=results.at<float>(0,0);
	int idx=0;
	for(int i=0; i<results.rows; i++) {
		for(int j=0; j<results.cols; j++) {
			try {
				if(results.at<float>(i,j)>max) {
					max = results.at<float>(i,j);
					idx = j;
				}
			}
			catch(const std::out_of_range &oor) {
				printf("Catch #1: Entropy::shapeFn() out of range!\n");
				exit(1);
			}
		}
	}
	//var idx holds the largest shape
}

void Entropy::setDebugMode(bool mode) {
	this->debugMode = mode;
}

bool Entropy::isDebugModeOn() {
	return this->debugMode;
}
