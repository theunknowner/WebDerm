/*
 * Entropy.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#include "entropy.h"

deque<deque< deque< deque<double> > > > vec;
deque<deque< deque< deque<double> > > > vec2;
deque<deque< deque< deque<int> > > > gTargetCellCount;
double vecTotal[5] = {0};

deque<deque<deque<deque<double> > > > gRatio;
deque<deque<deque< deque<double> > > > gSmoothRatio;
deque<deque<deque< deque<double> > > > gSmoothRatioRm;
deque<deque<deque< deque<double> > > > gRatioRm;

String combineShades(String shade) {
	if(shade.find("Dark2")!=string::npos || shade.find("Dark1")!=string::npos)
		return "Dark2";
	if(shade.find("Dark3")!=string::npos) return "Dark3";
	if(shade.find("High")!=string::npos) return "High";
	if(shade.find("Low")!=string::npos) return "Low";
	if(shade.find("Light")!=string::npos) return "Light";
	return shade;
}

void Entropy::eyeFn(FileData &fd, Size ksize,String targetColor,String targetShade) {
	int height = fd.colorVec.size()/ksize.height;
	int width = fd.colorVec.at(0).size()/ksize.width;
	int innerHeight = allColors.size();
	int innerWidth = g_Shades2.size();
	vec = createDeque4D(height,width,innerHeight,innerWidth,0.);
	vec2 = createDeque4D(height,width,innerHeight,innerWidth,0.);
	gTargetCellCount = createDeque4D(height,width,innerHeight,innerWidth,0);
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
	this->totalPopulation.resize(allColors.size(),deque<double>(g_Shades2.size(),0));
	this->populationDensity.resize(allColors.size(),deque<double>(g_Shades2.size(),0));
	this->densityVariation.resize(allColors.size(),deque<double>(g_Shades2.size(),0));
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
							shade = combineShades(shade);
							if(shade.find("Dark")==string::npos)
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
						totalPopulation.at(c).at(d) += smoothRatio[y1][x1][c][d];
						populationDensity.at(c).at(d) = ((cellCount.at(c).at(d)-1)*populationDensity.at(c).at(d)+smoothRatio[y1][x1][c][d])/cellCount.at(c).at(d);
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
					densityVariation.at(c).at(d) = standardDev(smoothRatioSingleList.at(c).at(d));
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
	String filename4 = filename+ "_"+ file_ksize+"_YSV_Combined"+strSize+".csv";
	String shadeColor, shade, color;
	FILE * fp4;
	fp4 = fopen(filename4.c_str(),"w");
	fprintf(fp4,",Y,S,V,T1,T2\n");
	for(unsigned int j=0; j<g_Shades2.size(); j++) {
		for(unsigned int i=0; i<allColors.size(); i++) {
			if(i==0 && j==0) {
				if(j==0 || j==1 || j==3) {
					shadeColor = g_Shades2.at(j)+allColors.at(i);
					fprintf(fp4,"%s,%f,%f,%f,",shadeColor.c_str(),this->totalPopulation.at(i).at(j),this->populationDensity.at(i).at(j),this->densityVariation.at(i).at(j));
					fprintf(fp4,"%f,%f\n",this->shapeMetric.at(0),this->shapeMetric.at(1));
				}
			}
			else {
				if(j==0 || j==1 || j==3) {
					shadeColor = g_Shades2.at(j)+allColors.at(i);
					fprintf(fp4,"%s,%f,%f,%f\n",shadeColor.c_str(),this->totalPopulation.at(i).at(j),this->populationDensity.at(i).at(j),this->densityVariation.at(i).at(j));
				}
			}
		}
	}
	cout << "Done!" << endl;
	fclose(fp4);
}

void Entropy::shapeFn(FileData &fd) {
	ShapeMorph sm;
	Mat img2, img3, img4;
	img2 = sm.findShapes(fd.getImage());
	img3 = sm.detectHeat(img2, Size(11,11));
	img4 = sm.connectImage(img3,Size(21,21),9.0);
	vector<Mat> featureVec = sm.liquidExtraction(img4);
	TestML ml;
	CvANN_MLP ann;
	ann.load("/home/jason/Desktop/workspace/Samples/Training/param.xml");
	Mat layers = ann.get_layer_sizes();
	int sampleSize = featureVec.size();
	int inputSize = layers.at<int>(0,0);
	int outputSize = layers.at<int>(0,2);
	int hiddenNodes = layers.at<int>(0,1);
	ml.setLayerParams(inputSize,hiddenNodes,outputSize);
	vector<Mat> sampleVec;
	Mat sample;
	for(int i=0; i<sampleSize; i++) {
		sample = ml.prepareImage(featureVec.at(i));
		sampleVec.push_back(sample);
		sample.release();
	}
	Mat results;
	Mat sample_set = ml.prepareMatSamples(sampleVec);
	ann.predict(sample_set,results);

	double circleThresh=0.60, randomThresh=-0.40;
	deque<int> circleVec, randomVec;
	for(int i=0; i<results.rows; i++) {
		if(results.at<float>(i,0)>circleThresh && results.at<float>(i,1)<=randomThresh) {
			circleVec.push_back(i);
		}
		if(results.at<float>(i,0)<=circleThresh && results.at<float>(i,1)>randomThresh) {
			randomVec.push_back(i);
		}
		//printf("Sample: %d, ",i+1);
		//cout << results.row(i) << endl;
	}
	//printf("Circles: %lu\n", circleVec.size());
	//printf("Randoms: %lu\n", randomVec.size());
	double avgT[results.cols];
	fill_n(avgT,results.cols,0.);
	this->shapeMetric.clear();
	if(circleVec.size()>=randomVec.size()) {
		for(unsigned int i=0; i<circleVec.size(); i++) {
			for(int j=0; j<results.cols; j++) {
				avgT[j] += results.at<float>(circleVec.at(i),j);
			}
		}
		for(int j=0; j<results.cols; j++) {
			avgT[j] /= circleVec.size();
			this->shapeMetric.push_back(avgT[j]);
		}
	}
	if(circleVec.size()<randomVec.size()) {
		for(unsigned int i=0; i<randomVec.size(); i++) {
			for(int j=0; j<results.cols; j++) {
				avgT[j] += results.at<float>(randomVec.at(i),j);
			}
		}
		for(int j=0; j<results.cols; j++) {
			avgT[j] /= randomVec.size();
			this->shapeMetric.push_back(avgT[j]);
		}
	}
}
