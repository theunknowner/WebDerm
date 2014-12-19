/*
 * entropyold.cpp
 *
 *  Created on: Dec 16, 2014
 *      Author: jason
 */

#include "entropy.h"

inline double eyeF_fn1(double Y, double a, double b, double p)  {
	double result=0;
	result = 1.0/(1+a*exp(-b*pow(Y,p)));
	return result;
}

String combineShadesOld(String shade) {
	if(shade.find("Dark")!=string::npos) return "Dark";
	if(shade.find("High")!=string::npos) return "High";
	if(shade.find("Low")!=string::npos) return "Low";
	if(shade.find("Light")!=string::npos) return "Light";
	return shade;
}

void Entropy::eyeFnOld(FileData &fd, Size ksize,String targetColor,String targetShade) {
	int height = fd.colorVec.size()/ksize.height;
	int width = fd.colorVec.at(0).size()/ksize.width;
	int innerHeight = allColors.size();
	int innerWidth = g_Shades2.size();
	deque<deque<deque<deque<double> > > > ratio(height,deque<deque<deque<double> > >(width,deque<deque<double> >(allColors.size(),deque<double>(g_Shades2.size(),0))));
	deque<deque<deque<deque<double> > > > smoothRatio(height,deque<deque<deque<double> > >(width,deque<deque<double> >(allColors.size(),deque<double>(g_Shades2.size(),0))));
	Rgb rgb;
	Hsl hsl;
	Color c;
	Shades sh;
	Functions fn;
	String color,shade,pix;
	double h,s,l;
	int shadeIndex,colorIndex;
	int cellSize = ksize.height*ksize.width;
	deque< deque<double> > pShadeColor(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > fnEye(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > dnEye(allColors.size(),deque<double>(g_Shades2.size(),0));
	double pTotal=0;
	unsigned int row=0, col=0;
	int i=0,j=0, maxRow=0, maxCol=0;
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
						try {
							pix = fd.colorVec.at(i).at(j);
							shade = sh.extractShade(pix);
							color = c.getMainColor(pix);
							color = c.optimizeColor2(color);
							/**temporary testing**/
							shade = combineShadesOld(shade);
							shade = "Low";
							color = c.combineColors(color);
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
							printf("ColorVec.Size: %lu\n",fd.colorVec.size());
							printf("HslMat.Size: %lu\n",fd.hslMat.size());
							printf("Point(%d,%d)\n",j,i);
							printf("Shade: %s\n",shade.c_str());
							printf("ShadeIndex: %d\n",shadeIndex);
							printf("Color: %s\n",color.c_str());
							printf("ColorIndex: %d\n",colorIndex);
							printf("pShadeColor.Size: %lu\n",pShadeColor.size());
							printf("pShadeColor(%d,%d)\n",i,j);
							exit(1);
						}
					}
				}
				for(unsigned int colorRow=0; colorRow<allColors.size(); colorRow++) {
					for(unsigned int shadeCol=0; shadeCol<g_Shades2.size(); shadeCol++) {
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
				pShadeColor.resize(allColors.size(),deque<double>(g_Shades2.size(),0));
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
	deque< deque<int> > cellCount(allColors.size(),deque<int>(g_Shades2.size(),0));
	deque< deque<int> > targetCellCount(allColors.size(),deque<int>(g_Shades2.size(),0));
	int binSize = 21;
	deque< deque<double> > stdevRatio(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > totalBins(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque<deque<deque<deque<double> > > > densityBin(allColors.size(),deque<deque<deque<double> > >(g_Shades2.size(),deque<deque<double> >(binSize,deque<double>(1,-1))));
	int binNum = 0;
	deque<deque<deque<Point> > > ratioPtsList(allColors.size(), deque<deque<Point> >(g_Shades2.size(),deque<Point>(1,Point(-1,-1))));
	deque<deque<deque<double> > > smoothRatioSingleList(allColors.size(), deque<deque<double> >(g_Shades2.size(),deque<double>(1,-1)));
	deque<deque<deque<deque<double> > > > smoothRatioOutlierRm(height,deque<deque<deque<double> > >(width,deque<deque<double> >(allColors.size(),deque<double>(g_Shades2.size(),0))));
	deque<deque<deque<deque<double> > > > ratioOutlierRm(height,deque<deque<deque<double> > >(width,deque<deque<double> >(allColors.size(),deque<double>(g_Shades2.size(),0))));
	int x1=0,y1=0,minRow=0,minCol=0;
	double count=0, min=0.03;
	double outlierThresh = 0.05;
	while(y1<height) {
		minRow=maxRow=y1;
		if((minRow-1)>=0) minRow-=1;
		if((maxRow+1)<height) maxRow+=1;
		while(x1<width) {
			minCol=maxCol=x1;
			if((minCol-1)>=0) minCol-=1;
			if((maxCol+1)<width) maxCol+=1;
			for(int a=minRow; a<=maxRow; a++)  {
				for(int b=minCol; b<=maxCol; b++)	 {
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
						if(smoothRatioSingleList.at(c).at(d).size()>0) {
							if(smoothRatioSingleList.at(c).at(d).at(0)==-1) {
								smoothRatioSingleList.at(c).at(d).pop_front();
								ratioPtsList.at(c).at(d).pop_front();
							}
						}
						smoothRatioSingleList.at(c).at(d).push_back(smoothRatio[y1][x1][c][d]);
						ratioPtsList.at(c).at(d).push_back(Point(x1,y1));

						cellCount.at(c).at(d)++;
						fnEye.at(c).at(d) += smoothRatio[y1][x1][c][d];
						dnEye.at(c).at(d) = ((cellCount.at(c).at(d)-1)*dnEye.at(c).at(d)+smoothRatio[y1][x1][c][d])/cellCount.at(c).at(d);
						/**to calc entropy of density**/
						binNum = floor((smoothRatio[y1][x1][c][d]*100)/5); //should I round it?
						if(densityBin.at(c).at(d).at(binNum).size()==1) {
							if(densityBin.at(c).at(d).at(binNum).at(0)==-1) {
								deque<double>().swap(densityBin.at(c).at(d).at(binNum));
							}
						}
						densityBin.at(c).at(d).at(binNum).push_back(smoothRatio[y1][x1][c][d]);
						++totalBins.at(c).at(d);
						if(targetColor!="" && targetShade!="") {
							int index = rgb.getColorIndex(targetColor);
							int shadeIndex = sh.getShadeIndex2(targetShade);
							if(c==index && d==shadeIndex) {
								//pt.at(y1).at(x1) = 1;
								targetCellCount.at(c).at(d) = cellCount.at(c).at(d);
							}
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
			if(smoothRatioSingleList.at(i).at(j).size()>0) {
				if(smoothRatioSingleList.at(i).at(j).at(0)!=-1) {
					if(sortedList.size()==0) {
						sortedList.push_back(smoothRatioSingleList.at(i).at(j).at(0));
						sortedPtsList.push_back(ratioPtsList.at(i).at(j).at(0));
					}
					for(unsigned int d=1; d<smoothRatioSingleList.at(i).at(j).size(); d++) {
						for(unsigned int e=0; e<sortedList.size(); e++) {
							if((sortedList.size()-e)<=1) {
								if(smoothRatioSingleList.at(i).at(j).at(d)>=sortedList.at(e)) {
									sortedList.push_back(smoothRatioSingleList.at(i).at(j).at(d));
									sortedPtsList.push_back(ratioPtsList.at(i).at(j).at(d));
									break;
								}
								else {
									sortedList.push_front(smoothRatioSingleList.at(i).at(j).at(d));
									sortedPtsList.push_front(ratioPtsList.at(i).at(j).at(d));
									break;
								}
							}
							else {
								if(smoothRatioSingleList.at(i).at(j).at(d)>=sortedList.at(e) && smoothRatioSingleList.at(i).at(j).at(d)<=sortedList.at(e+1)) {
									sortedList.insert(sortedList.begin()+e+1,smoothRatioSingleList.at(i).at(j).at(d));
									sortedPtsList.insert(sortedPtsList.begin()+e+1,ratioPtsList.at(i).at(j).at(d));
									break;
								}
								if(smoothRatioSingleList.at(i).at(j).at(d)<=sortedList.at(0)) {
									sortedList.push_front(smoothRatioSingleList.at(i).at(j).at(d));
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
	smoothRatioSingleList.clear();
	smoothRatioSingleList.resize(allColors.size(),deque<deque<double> >(g_Shades2.size(),deque<double>(1,-1)));
	x1=0,y1=0,minRow=0,minCol=0,maxRow=0,maxCol=0,count=0;
	while(y1<height) {
		minRow=maxRow=y1;
		if((minRow-1)>=0) minRow-=1;
		if((maxRow+1)<height) maxRow+=1;
		while(x1<width) {
			minCol=maxCol=x1;
			if((minCol-1)>=0) minCol-=1;
			if((maxCol+1)<width) maxCol+=1;
			for(int a=minRow; a<=maxRow; a++)  {
				for(int b=minCol; b<=maxCol; b++)	 {
					for(int c=0; c<innerHeight; c++) {
						for(int d=0; d<innerWidth; d++) {
							if(ratioOutlierRm[y1][x1][c][d]>min) {
								smoothRatioOutlierRm[y1][x1][c][d] += ratioOutlierRm[a][b][c][d];
							}
						}
					}
					++count;
				}
			}
			for(int c=0; c<innerHeight; c++) {
				for(int d=0; d<innerWidth; d++) {
					smoothRatioOutlierRm[y1][x1][c][d] /=count;
					if(ratioOutlierRm[y1][x1][c][d]>min) {
						if(smoothRatioSingleList.at(c).at(d).size()>0) {
							if(smoothRatioSingleList.at(c).at(d).at(0)==-1) {
								smoothRatioSingleList.at(c).at(d).pop_front();
							}
						}
						smoothRatioSingleList.at(c).at(d).push_back(smoothRatioOutlierRm[y1][x1][c][d]);
					}
				}
			}
			count=0;
			++x1;
		}
		x1=0;
		++y1;
	}
	for(int c=0; c<innerHeight; c++) {
		for (int d=0; d<innerWidth; d++) {
			if(smoothRatioSingleList.at(c).at(d).size()>0) {
				if(smoothRatioSingleList.at(c).at(d).at(0)!=-1) {
					stdevRatio.at(c).at(d) = standardDev(smoothRatioSingleList.at(c).at(d));
				}
			}
		}
	}

	String strSize = toString(ksize.width)+"x"+toString(ksize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	/*
	String filename = path+fd.filename+ "_"+ file_ksize+"_EyeFnCombined_"+strSize+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	String filename2 = path+fd.filename+ "_"+ file_ksize+"_EyeDensityCombined_"+strSize+".csv";
	FILE * fp2;
	fp2 = fopen(filename2.c_str(),"w");
	String filename3 = path+fd.filename+ "_"+ file_ksize+"_StdevCombined_"+strSize+".csv";
	FILE * fp3;
	fp3 = fopen(filename3.c_str(),"w");
	for(unsigned int i=0; i<g_Shades2.size(); i++) {
		fprintf(fp,",%s",g_Shades2.at(i).c_str());
		fprintf(fp2,",%s",g_Shades2.at(i).c_str());
		fprintf(fp3,",%s",g_Shades2.at(i).c_str());
	}
	fprintf(fp,"\n");
	fprintf(fp2,"\n");
	fprintf(fp3,"\n");
	for(unsigned int i=0; i<fnEye.size(); i++)  {
		fprintf(fp,"%s,",allColors.at(i).c_str());
		fprintf(fp2,"%s,",allColors.at(i).c_str());
		fprintf(fp3,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<fnEye.at(i).size(); j++)  {
			if(j<fnEye.at(i).size()-1) {
				fprintf(fp,"%f,", fnEye.at(i).at(j));
				fprintf(fp2,"%f,", dnEye.at(i).at(j));
				fprintf(fp3,"%f,", stdevRatio.at(i).at(j));
			}
			else {
				fprintf(fp,"%f\n", fnEye.at(i).at(j));
				fprintf(fp2,"%f\n", dnEye.at(i).at(j));
				fprintf(fp3,"%f\n", stdevRatio.at(i).at(j));
			}
		}
	}
	 */

	if(targetColor=="" && targetShade=="") {
		cout << fd.filename  << ": Writing YSV files..." << flush;
		String filename4 = fd.filename+ "_"+ file_ksize+"_YSV_Combined"+strSize+".csv";
		String shadeColor, shade, color;
		FILE * fp4;
		fp4 = fopen(filename4.c_str(),"w");
		fprintf(fp4,",Y,S,V\n");
		for(unsigned int j=0; j<g_Shades2.size(); j++) {
			for(unsigned int i=0; i<allColors.size(); i++) {
				if(j==2) {
					shadeColor = g_Shades2.at(j)+allColors.at(i);
					fprintf(fp4,"%s,%f,%f,%f\n",shadeColor.c_str(),fnEye.at(i).at(j),dnEye.at(i).at(j),stdevRatio.at(i).at(j));
				}
			}
		}
		cout << "Done!" << endl;
		//fclose(fp);
		//fclose(fp2);
		//fclose(fp3);
		fclose(fp4);
	}
}