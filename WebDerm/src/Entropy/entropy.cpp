/*
 * Entropy.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#include "entropy.h"

deque< deque<double> > Entropy::outputEntropy(FileData &fd, Size ksize) {
	String shade, color, pix;
	int shadeIndex=0, colorIndex=0;
	Shades sh;
	Rgb rgb;
	Color c;
	init_2D_Deque(fd.shadeColorCount,g_Shades.size(), allColors.size(),0);
	for(unsigned int i=0; i<fd.colorVec.size(); i++) {
		for(unsigned int j=0; j<fd.colorVec.at(i).size(); j++) {
			try {
				pix = fd.colorVec.at(i).at(j);
				shade = sh.extractShade(pix);
				/**temporary testing**
				if(shade.find("Dark")!=string::npos) shade = "Dark";
				if(shade.find("High")!=string::npos) shade = "High";
				if(shade.find("Low")!=string::npos) shade = "Low";
				if(shade.find("Light")!=string::npos) shade = "Light";
				/************************************/
				shadeIndex = sh.getShadeIndex(shade);
				color = c.getMainColor(pix);
				color = c.optimizeColor2(color);
				if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)  {
					color = "Black";
				}
				else if(shade=="White" || color.find("White")!=string::npos)
					color = "White";
				colorIndex = rgb.getColorIndex(color);
				if(color!="Zero" || colorIndex>=0)
					++fd.shadeColorCount.at(colorIndex).at(shadeIndex);
			}
			catch(const std::out_of_range& oor) {
				printf("Point(%d,%d)\n",j,i);
				printf("Shade: %s\n",shade.c_str());
				printf("ShadeIndex: %d\n",shadeIndex);
				printf("Color: %s\n",color.c_str());
				printf("ColorIndex: %d\n",colorIndex);
				exit(1);
			}
		}
	}
	deque< deque<double> > pShadeColor(allColors.size(),deque<double>(g_Shades.size(),0));
	deque< deque<double> > pEntropy(allColors.size(),deque<double>(g_Shades.size(),0));
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
							/**temporary testing**
							if(shade.find("Dark")!=string::npos) shade = "Dark";
							if(shade.find("High")!=string::npos) shade = "High";
							if(shade.find("Low")!=string::npos) shade = "Low";
							if(shade.find("Light")!=string::npos) shade = "Light";
		/************************************/
							shadeIndex = sh.getShadeIndex(shade);
							color = c.getMainColor(pix);
							color = c.optimizeColor2(color);
							if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)
								color = "Black";
							else if(shade=="White" || color.find("White")!=string::npos)
								color = "White";
							colorIndex = rgb.getColorIndex(color);
							if(color!="Zero" || colorIndex>=0)
								++pShadeColor.at(colorIndex).at(shadeIndex);
						}
						catch(const std::out_of_range& oor) {
							printf("ColorVec.Size: %lux%lu\n",fd.colorVec.size(),fd.colorVec.at(i).size());
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
					for(unsigned int shadeCol=0; shadeCol<g_Shades.size(); shadeCol++) {
						try {
							if(pShadeColor.at(colorRow).at(shadeCol)!=0) {
								pTotal = pShadeColor.at(colorRow).at(shadeCol)/fd.shadeColorCount.at(colorRow).at(shadeCol);
								pEntropy.at(colorRow).at(shadeCol) += (-pTotal * log2(pTotal));
							}
						}
						catch (const std::out_of_range &oor) {
							printf("pShadeColor.Rows: %lu\n", pShadeColor.size());
							printf("pShadeColor.Cols: %lu\n", pShadeColor.at(colorRow).size());
							printf("pShadeColor(%d,%d)\n",colorRow,shadeCol);
							printf("pEntropy(%d,%d)\n",colorRow,shadeCol);
							exit(1);
						}
					}
				}
				col+=ksize.width;
				pShadeColor.clear();
				pShadeColor.resize(allColors.size(),deque<double>(g_Shades.size(),0));
			}
			row += ksize.height;
			col=0;
		}
	}
	catch (const std::out_of_range &oor) {
		printf("colorVec.Rows: %lu\n",fd.colorVec.size());
		printf("colorVec.Cols: %lu\n",fd.colorVec.at(row).size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}
	String strSize = toString(ksize.width)+"x"+toString(ksize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	String filename = path+fd.filename+ "_"+ file_ksize+"_Entropy_"+strSize+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<g_Shades.size(); i++) {
		fprintf(fp,",%s",g_Shades.at(i).c_str());
	}
	fprintf(fp,"\n");
	for(unsigned int i=0; i<pEntropy.size(); i++)
	{
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<pEntropy.at(i).size(); j++)
		{
			if(j<pEntropy.at(i).size()-1)
				fprintf(fp,"%f,", pEntropy.at(i).at(j));
			else
				fprintf(fp,"%f\n", pEntropy.at(i).at(j));
		}
	}
	fclose(fp);
	return pEntropy;
}

String combineShades(String shade) {
	if(shade.find("Dark")!=string::npos) return "Dark";
	if(shade.find("High")!=string::npos) return "High";
	if(shade.find("Low")!=string::npos) return "Low";
	if(shade.find("Light")!=string::npos) return "Light";
	return shade;
}

deque< deque< deque<double> > > vec;
deque< deque< deque<double> > > vec2;
deque< deque< deque<int> > > gTargetCellCount;
double vecTotal[5] = {0};

deque<deque<deque<deque<double> > > > gRatio;
deque<deque<deque< deque<double> > > > gSmoothRatio;
deque<deque<deque< deque<double> > > > gSmoothRatioRm;
deque<deque<deque< deque<double> > > > gRatioRm;

deque< deque<double> > Entropy::outputCombinedEntropy(FileData &fd, Size ksize) {
	String shade, color, pix;
	int shadeIndex=0, colorIndex=0;
	Shades sh;
	Rgb rgb;
	Color c;
	Hsl hsl;
	Functions fn;
	double h,s,l;
	init_3D_Deque(vec,fd.colorVec.size()/ksize.height,fd.colorVec.at(0).size()/ksize.width,g_Shades2.size());
	init_3D_Deque(vec2,fd.colorVec.size()/ksize.height,fd.colorVec.at(0).size()/ksize.width,g_Shades2.size());
	init_2D_Deque(fd.shadeColorCount,g_Shades2.size(), allColors.size(),0);
	for(unsigned int i=0; i<fd.colorVec.size(); i++) {
		for(unsigned int j=0; j<fd.colorVec.at(i).size(); j++) {
			try  {
				pix = fd.colorVec.at(i).at(j);
				shade = sh.extractShade(pix);
				color = c.getMainColor(pix);
				color = c.optimizeColor2(color);
				/**temporary testing**/
				shade = combineShades(shade);
				color = c.combineColors(color);
				if(fd.filename.find("acne")!=string::npos)  {
					if(color=="Violet")  {
						int index=-1;
						h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
						s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
						l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
						color = hsl.getHslColor(h,s,l,index);
						color = hsl.getHslColor(index+1);
						//color = "BrownRed";
					}
				}
				if(fd.filename.find("Psoriasis")!=string::npos)  {
					if(color=="Violet")  {
						int index=-1;
						h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
						s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
						l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
						color = hsl.getHslColor(h,s,l,index);
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
				if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)  {
					color = "Black";
				}
				else if(shade=="White" || color.find("White")!=string::npos)
					color = "White";
				colorIndex = rgb.getColorIndex(color);
				if(color!="Zero" || colorIndex>=0)
					++fd.shadeColorCount.at(colorIndex).at(shadeIndex);
			}
			catch(const std::out_of_range& oor) {
				printf("Point(%d,%d)\n",j,i);
				printf("Shade: %s\n",shade.c_str());
				printf("ShadeIndex: %d\n",shadeIndex);
				printf("Color: %s\n",color.c_str());
				printf("ColorIndex: %d\n",colorIndex);
				exit(1);
			}
		}
	}
	deque< deque<double> > pShadeColor(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > pEntropy(allColors.size(),deque<double>(g_Shades2.size(),0));
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
							if(fd.filename.find("Psoriasis")!=string::npos)  {
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
							printf("ColorVec.Size: %lux%lu\n",fd.colorVec.size(),fd.colorVec.at(i).size());
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
							if(pShadeColor.at(colorRow).at(shadeCol)>5 && fd.shadeColorCount.at(colorRow).at(shadeCol)>100) {
								pTotal = pShadeColor.at(colorRow).at(shadeCol)/fd.shadeColorCount.at(colorRow).at(shadeCol);
								pTotal = -pTotal * log2(pTotal);
								/***Test Code***/
								if(allColors.at(colorRow)=="Pink") {
									vec[row/ksize.height][col/ksize.width][shadeCol] = pShadeColor.at(colorRow).at(shadeCol);
									vec2[row/ksize.height][col/ksize.width][shadeCol] = pTotal;
									vecTotal[shadeCol] = fd.shadeColorCount.at(colorRow).at(shadeCol);
								}
								/*************/
								pEntropy.at(colorRow).at(shadeCol) += pTotal;
							}
						}
						catch (const std::out_of_range &oor) {
							printf("pShadeColor.Rows: %lu\n", pShadeColor.size());
							printf("pShadeColor.Cols: %lu\n", pShadeColor.at(colorRow).size());
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
		printf("colorVec.Cols: %lu\n",fd.colorVec.at(row).size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}
	double total=0;
	//Graph gr;
	//gr.graph(pShadeColor,g_Shades2,allColors,fd.filename+"pShadeColors");
	String strSize = toString(ksize.width)+"x"+toString(ksize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	String filename = path+fd.filename+ "_"+ file_ksize+"_EntropyCombined_"+strSize+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<g_Shades2.size(); i++) {
		fprintf(fp,",%s",g_Shades2.at(i).c_str());
	}
	fprintf(fp,"\n");
	for(unsigned int i=0; i<pEntropy.size(); i++)  {
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<pEntropy.at(i).size(); j++)  {
			total += pEntropy.at(i).at(j);
			if(j<pEntropy.at(i).size()-1)
				fprintf(fp,"%f,", pEntropy.at(i).at(j));
			else
				fprintf(fp,"%f,%f\n", pEntropy.at(i).at(j),total);
		}
		total=0;
	}
	fclose(fp);
	return pEntropy;
}

void Entropy::outputShiftedEntropy(FileData &fd1, FileData &fd2, Size ksize, deque<int> &colorShiftAmt)  {
	String shade, shade2, color,color2, pix,pix2;
	int shadeIndex=0, colorIndex=0;
	int shadeIndex2=0, colorIndex2=0;
	Shades sh;
	Rgb rgb;
	Color c;
	fd1.shadeColorCount.clear();
	deque< deque<int> >().swap(fd1.shadeColorCount);
	init_2D_Deque(fd1.shadeColorCount,g_Shades2.size(), allColors.size(),0);
	fd2.shadeColorCount.clear();
	deque< deque<int> >().swap(fd2.shadeColorCount);
	init_2D_Deque(fd2.shadeColorCount,g_Shades2.size(), allColors.size(),0);
	for(unsigned int i=0; i<fd1.colorVec.size(); i++) {
		for(unsigned int j=0; j<fd1.colorVec.at(i).size(); j++) {
			try {
				pix = fd1.colorVec.at(i).at(j);
				pix2 = fd2.colorVec.at(i).at(j);
				color = c.getMainColor(pix);
				color2 = c.getMainColor(pix2);
				color = c.optimizeColor2(color);
				color2 = c.optimizeColor2(color2);
				shade = sh.extractShade(pix);
				shade2 = sh.extractShade(pix2);
				/**temporary testing**/
				shadeIndex = sh.getShadeIndex(shade);
				shadeIndex2 = sh.getShadeIndex(shade2);
				for(unsigned int k=0; k<colorShiftAmt.size(); k++) {
					if(color==allColors.at(k)) {
						if(colorShiftAmt.at(k)>0)  {
							shadeIndex -= colorShiftAmt.at(k);
						}
					}
					if(color2==allColors.at(k)) {
						if(colorShiftAmt.at(k)<0)  {
							shadeIndex2 -= abs(colorShiftAmt.at(k));
						}
					}
				}
				shade = sh.getShade(shadeIndex);
				shade2 = sh.getShade(shadeIndex2);
				if(shade.find("Dark")!=string::npos) shade = "Dark";
				if(shade.find("High")!=string::npos) shade = "High";
				if(shade.find("Low")!=string::npos) shade = "Low";
				if(shade.find("Light")!=string::npos) shade = "Light";
				if(shade2.find("Dark")!=string::npos) shade2 = "Dark";
				if(shade2.find("High")!=string::npos) shade2 = "High";
				if(shade2.find("Low")!=string::npos) shade2 = "Low";
				if(shade2.find("Light")!=string::npos) shade2 = "Light";
				/************************************/
				shadeIndex = sh.getShadeIndex2(shade);
				shadeIndex2 = sh.getShadeIndex2(shade2);
				if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)  {
					color = "Black";
					shade = "Dark";
				}
				else if(shade=="White" || color.find("White")!=string::npos)
					color = "White";
				colorIndex = rgb.getColorIndex(color);

				if(color!="Zero" || colorIndex>=0)
					++fd1.shadeColorCount.at(colorIndex).at(shadeIndex);

				if(shade2.find("Black")!=string::npos || color2.find("Black")!=string::npos)  {
					color2 = "Black";
					shade = "Dark";
				}
				else if(shade2=="White" || color2.find("White")!=string::npos)
					color2 = "White";
				colorIndex2 = rgb.getColorIndex(color2);

				if(color2!="Zero" || colorIndex2>=0)
					++fd2.shadeColorCount.at(colorIndex2).at(shadeIndex2);
			}
			catch(const std::out_of_range& oor) {
				printf("Entropy::outputShiftedEntropy() out of range!\n");
				printf("Point(%d,%d)\n",j,i);
				printf("Shade: %s\n",shade.c_str());
				printf("ShadeIndex: %d\n",shadeIndex);
				printf("Color: %s\n",color.c_str());
				printf("ColorIndex: %d\n",colorIndex);
				exit(1);
			}
		}
	}
	deque< deque<double> > pShadeColor(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > pEntropy(allColors.size(),deque<double>(g_Shades2.size(),0));

	deque< deque<double> > pShadeColor2(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > pEntropy2(allColors.size(),deque<double>(g_Shades2.size(),0));
	double pTotal=0;
	double pTotal2=0;
	unsigned int row=0, col=0;
	int i=0,j=0, maxRow=0, maxCol=0;
	try {
		while(row<=(fd1.colorVec.size()-ksize.height)) {
			while(col<=(fd1.colorVec.at(row).size()-ksize.width)) {
				if((row+ksize.height+1)>fd1.colorVec.size())
					maxRow = fd1.colorVec.size();
				else
					maxRow = row+ksize.height;
				if((col+ksize.width+1)>fd1.colorVec.at(row).size())
					maxCol = fd1.colorVec.at(row).size();
				else
					maxCol = col+ksize.width;

				for(i=row; i<maxRow; i++) {
					for(j=col; j<maxCol; j++) {
						try {
							pix = fd1.colorVec.at(i).at(j);
							pix2 = fd2.colorVec.at(i).at(j);
							color = c.getMainColor(pix);
							color = c.optimizeColor2(color);
							color2 = c.getMainColor(pix2);
							color2 = c.optimizeColor2(color2);
							shade = sh.extractShade(pix);
							shade2 = sh.extractShade(pix2);
							/**temporary testing**/
							shadeIndex = sh.getShadeIndex(shade);
							shadeIndex2 = sh.getShadeIndex(shade2);
							for(unsigned int k=0; k<colorShiftAmt.size(); k++) {
								if(color==allColors.at(k)) {
									if(colorShiftAmt.at(k)>0)  {
										shadeIndex -= colorShiftAmt.at(k);
									}
								}
								if(color2==allColors.at(k)) {
									if(colorShiftAmt.at(k)<0)  {
										shadeIndex2 -= abs(colorShiftAmt.at(k));
									}
								}
							}
							shade = sh.getShade(shadeIndex);
							shade2 = sh.getShade(shadeIndex2);
							if(shade.find("Dark")!=string::npos) shade = "Dark";
							if(shade.find("High")!=string::npos) shade = "High";
							if(shade.find("Low")!=string::npos) shade = "Low";
							if(shade.find("Light")!=string::npos) shade = "Light";
							if(shade2.find("Dark")!=string::npos) shade2 = "Dark";
							if(shade2.find("High")!=string::npos) shade2 = "High";
							if(shade2.find("Low")!=string::npos) shade2 = "Low";
							if(shade2.find("Light")!=string::npos) shade2 = "Light";
							/************************************/
							shadeIndex = sh.getShadeIndex2(shade);
							shadeIndex2 = sh.getShadeIndex2(shade2);
							if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)  {
								color = "Black";
							}
							else if(shade=="White" || color.find("White")!=string::npos)
								color = "White";
							colorIndex = rgb.getColorIndex(color);
							if(color!="Zero" || colorIndex>=0)
								++pShadeColor.at(colorIndex).at(shadeIndex);

							if(shade2.find("Black")!=string::npos || color2.find("Black")!=string::npos)
								color2 = "Black";
							else if(shade2=="White" || color2.find("White")!=string::npos)
								color2 = "White";
							colorIndex2 = rgb.getColorIndex(color2);

							if(color2!="Zero" || colorIndex2>=0)
								++pShadeColor2.at(colorIndex2).at(shadeIndex2);
						}
						catch(const std::out_of_range& oor) {
							printf("Entropy::outputShiftedEntropy() out of range!\n");
							printf("ColorVec.Size: %lux%lu\n",fd1.colorVec.size(),fd1.colorVec.at(i).size());
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
							if(pShadeColor.at(colorRow).at(shadeCol)!=0 && fd1.shadeColorCount.at(colorRow).at(shadeCol)!=0) {
								pTotal = pShadeColor.at(colorRow).at(shadeCol)/fd1.shadeColorCount.at(colorRow).at(shadeCol);
								pEntropy.at(colorRow).at(shadeCol) += (-pTotal * log2(pTotal));
							}
							if(pShadeColor2.at(colorRow).at(shadeCol)!=0 && fd2.shadeColorCount.at(colorRow).at(shadeCol)!=0) {
								pTotal2 = pShadeColor2.at(colorRow).at(shadeCol)/fd2.shadeColorCount.at(colorRow).at(shadeCol);
								pEntropy2.at(colorRow).at(shadeCol) += (-pTotal2 * log2(pTotal2));
								//if(colorRow==6 && shadeCol==0) {
								//	printf("%.0f/%d=%f\n",pShadeColor2.at(colorRow).at(shadeCol),fd2.shadeColorCount.at(colorRow).at(shadeCol),pTotal2);
								//}
							}
						}
						catch (const std::out_of_range &oor) {
							printf("Entropy::outputShiftedEntropy() out of range!\n");
							printf("pShadeColor.Rows: %lu\n", pShadeColor.size());
							printf("pShadeColor.Cols: %lu\n", pShadeColor.at(colorRow).size());
							printf("pShadeColor(%d,%d)\n",colorRow,shadeCol);
							printf("pEntropy(%d,%d)\n",colorRow,shadeCol);
							exit(1);
						}
					}
				}
				col+=ksize.width;
				pShadeColor.clear();
				pShadeColor.resize(allColors.size(),deque<double>(g_Shades2.size(),0));

				pShadeColor2.clear();
				pShadeColor2.resize(allColors.size(),deque<double>(g_Shades2.size(),0));
			}
			row += ksize.height;
			col=0;
		}
	}
	catch (const std::out_of_range &oor) {
		printf("Entropy::outputShiftedEntropy() out of range!\n");
		printf("colorVec.Rows: %lu\n",fd1.colorVec.size());
		printf("colorVec.Cols: %lu\n",fd1.colorVec.at(row).size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}
	double total=0;
	double total2=0;
	String strSize = toString(fd1.ksize.width)+"x"+toString(fd1.ksize.height);
	String filename = path+fd1.filename;
	String filename2 = path+fd2.filename;
	filename += "_" + fd2.filename + "_EntropyShifted_"+strSize+".csv";
	filename2 += "_" + fd1.filename + "_EntropyShifted_"+strSize+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	FILE * fp2;
	fp2 = fopen(filename2.c_str(),"w");
	for(unsigned int i=0; i<g_Shades2.size(); i++) {
		fprintf(fp,",%s",g_Shades2.at(i).c_str());
		fprintf(fp2,",%s",g_Shades2.at(i).c_str());
	}
	fprintf(fp,"\n");
	fprintf(fp2,"\n");
	for(unsigned int i=0; i<pEntropy.size(); i++)
	{
		fprintf(fp,"%s,",allColors.at(i).c_str());
		fprintf(fp2,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<pEntropy.at(i).size(); j++)
		{
			total += pEntropy.at(i).at(j);
			total2 += pEntropy2.at(i).at(j);
			if(j<pEntropy.at(i).size()-1)
				fprintf(fp,"%f,", pEntropy.at(i).at(j));
			else
				fprintf(fp,"%f,%f\n", pEntropy.at(i).at(j),total);

			if(j<pEntropy2.at(i).size()-1)
				fprintf(fp2,"%f,", pEntropy2.at(i).at(j));
			else
				fprintf(fp2,"%f,%f\n", pEntropy2.at(i).at(j),total2);
		}
		total=0;
		total2=0;
	}
	fclose(fp);
	fclose(fp2);
}

deque<int> Entropy::calcShadeShift(deque< deque<double> > &vec1, deque< deque<double> > &vec2) {
	deque<int> colorShiftAmt;
	int flag=0;
	int shift=0;
	int vec1Begin=-1, vec2Begin=-1;
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec1.at(i).size(); j++) {
			if(vec1.at(i).at(j)!=0 && vec1Begin==-1) vec1Begin = j;
			if(vec2.at(i).at(j)!=0 && vec2Begin==-1) vec2Begin = j;
			if(vec1Begin!=-1 && vec2Begin!=-1)  {
				flag=1;
				break;
			}
		}
		if(flag==1)
			shift = vec1Begin - vec2Begin;
		else
			shift = 0;
		colorShiftAmt.push_back(shift);
		flag=0;
		vec1Begin = -1;
		vec2Begin = -1;
	}
	return colorShiftAmt;
}

inline double f(double t, double a, double b, double p)  {
	double result = 0;
	result = 1.0/(1.0+a*exp(-pow((b*t),p)));
	return result;
}

deque< deque<double> > Entropy::outputSigmoid(FileData &fd, Size ksize, double a, double b, double p)  {
	String shade, color, pix;
	int shadeIndex=0, colorIndex=0;
	Shades sh;
	Rgb rgb;
	Color c;
	init_2D_Deque(fd.shadeColorCount,g_Shades.size(), allColors.size(),0);
	deque< deque<double> > pShadeColor(allColors.size(),deque<double>(g_Shades.size(),0));
	deque< deque<double> > pEntropy(allColors.size(),deque<double>(g_Shades.size(),0));
	deque< deque<double> > pRatio(allColors.size(),deque<double>(g_Shades2.size(),0));
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
							shadeIndex = sh.getShadeIndex(shade);
							color = c.getMainColor(pix);
							color = c.optimizeColor2(color);
							if(shade.find("Black")!=string::npos || color.find("Black")!=string::npos)
								color = "Black";
							else if(shade=="White" || color.find("White")!=string::npos)
								color = "White";
							colorIndex = rgb.getColorIndex(color);
							if(color!="Zero" || colorIndex>=0)
								++fd.shadeColorCount.at(colorIndex).at(shadeIndex);
						}
						catch(const std::out_of_range& oor) {
							printf("ColorVec.Size: %lux%lu\n",fd.colorVec.size(),fd.colorVec.at(i).size());
							printf("Point(%d,%d)\n",j,i);
							printf("Shade: %s\n",shade.c_str());
							printf("ShadeIndex: %d\n",shadeIndex);
							printf("Color: %s\n",color.c_str());
							printf("ColorIndex: %d\n",colorIndex);
							printf("fd.shadeColorCount.Size: %lu\n",fd.shadeColorCount.size());
							printf("fd.shadeColorCount(%d,%d)\n",i,j);
							exit(1);
						}
					}
				}
				int max=0;
				deque<int> indexes;
				deque< deque<int> > shadeColorIndexes;
				for(unsigned int colorRow=0; colorRow<allColors.size(); colorRow++)  {
					for(unsigned int shadeCol=0; shadeCol<g_Shades.size(); shadeCol++)  {
						if(fd.shadeColorCount.at(colorRow).at(shadeCol)>0 && fd.shadeColorCount.at(colorRow).at(shadeCol)>=max) {
							max = fd.shadeColorCount.at(colorRow).at(shadeCol);
							indexes.push_back(colorRow);
							indexes.push_back(shadeCol);
							shadeColorIndexes.push_back(indexes);
							indexes.clear();
						}
					}
				}
				//loop to get & store majority shadeColor
				for(unsigned int y=0; y<shadeColorIndexes.size(); y++)  {
					colorIndex = shadeColorIndexes.at(y).at(0);
					shadeIndex = shadeColorIndexes.at(y).at(1);
					if(fd.shadeColorCount.at(colorIndex).at(shadeIndex)>=max)
						++pShadeColor.at(colorIndex).at(shadeIndex);
				}
				col+=ksize.width;
				fd.shadeColorCount.clear();
				fd.shadeColorCount.resize(allColors.size(),deque<int>(g_Shades.size(),0));
			}
			row += ksize.height;
			col=0;
		}
		double height = fd.colorVec.size()/ksize.height;
		double width = fd.colorVec.at(0).size()/ksize.width;
		double t=0;
		for(unsigned int colorRow=0; colorRow<allColors.size(); colorRow++) {
			for(unsigned int shadeCol=0; shadeCol<g_Shades.size(); shadeCol++) {
				try {
					if(pShadeColor.at(colorRow).at(shadeCol)!=0) {
						pRatio.at(colorRow).at(shadeCol) = pShadeColor.at(colorRow).at(shadeCol)/(height*width);
						t = pRatio.at(colorRow).at(shadeCol);
						pEntropy.at(colorRow).at(shadeCol) = (2.0*(f(t,a,b,p)-f(0,a,b,p)))/(2.0*(f(1,a,b,p)-f(0,a,b,p)));
					}
				}
				catch (const std::out_of_range &oor) {
					printf("pShadeColor.Rows: %lu\n", pShadeColor.size());
					printf("pShadeColor.Cols: %lu\n", pShadeColor.at(colorRow).size());
					printf("pShadeColor(%d,%d)\n",colorRow,shadeCol);
					printf("pEntropy(%d,%d)\n",colorRow,shadeCol);
					exit(1);
				}
			}
		}
	}
	catch (const std::out_of_range &oor) {
		printf("colorVec.Rows: %lu\n",fd.colorVec.size());
		printf("colorVec.Cols: %lu\n",fd.colorVec.at(row).size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}
	String strSize = toString(ksize.width)+"x"+toString(ksize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	String filename = path+fd.filename+ "_"+ file_ksize+"_Sigmoid_"+strSize+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<g_Shades.size(); i++) {
		fprintf(fp,",%s",g_Shades.at(i).c_str());
	}
	fprintf(fp,"\n");
	for(unsigned int i=0; i<pEntropy.size(); i++)
	{
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<pEntropy.at(i).size(); j++)
		{
			if(j<pEntropy.at(i).size()-1)
				fprintf(fp,"%f,", pEntropy.at(i).at(j));
			else
				fprintf(fp,"%f\n", pEntropy.at(i).at(j));
		}
	}
	fclose(fp);
	return pEntropy;
}

deque< deque<double> > Entropy::outputCombinedSigmoid(FileData &fd, Size ksize, double a, double b, double p)  {
	String shade, color, pix;
	int shadeIndex=0, colorIndex=0;
	Shades sh;
	Rgb rgb;
	Color c;
	Hsl hsl;
	Functions fn;
	double h,s,l;
	init_2D_Deque(fd.shadeColorCount,g_Shades2.size(), allColors.size(),0);
	deque< deque<double> > pShadeColor(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > pEntropy(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > pRatio(allColors.size(),deque<double>(g_Shades2.size(),0));
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
							shade = "Low";
							color = c.combineColors(color);
							if(fd.filename.find("acne")!=string::npos)  {
								if(color=="Violet")  {
									int index=-1;
									h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
									s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
									l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
									color = hsl.getHslColor(h,s,l,index);
									color = hsl.getHslColor(index+1);
									//color = "BrownRed";
								}
							}
							if(fd.filename.find("Psoriasis")!=string::npos)  {
								if(color=="Violet")  {
									int index=-1;
									h = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',1);
									s = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',2);
									l = fn.getDelimitedValuesFromString(fd.hslMat.at(i).at(j),';',3);
									color = hsl.getHslColor(h,s,l,index);
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
								++fd.shadeColorCount.at(colorIndex).at(shadeIndex);
						}
						catch(const std::out_of_range& oor) {
							printf("ColorVec.Size: %lux%lu\n",fd.colorVec.size(),fd.colorVec.at(i).size());
							printf("Point(%d,%d)\n",j,i);
							printf("Shade: %s\n",shade.c_str());
							printf("ShadeIndex: %d\n",shadeIndex);
							printf("Color: %s\n",color.c_str());
							printf("ColorIndex: %d\n",colorIndex);
							printf("fd.shadeColorCount.Size: %lu\n",fd.shadeColorCount.size());
							printf("fd.shadeColorCount(%d,%d)\n",i,j);
							exit(1);
						}
					}
				}
				int max=0;
				deque<int> indexes;
				deque< deque<int> > shadeColorIndexes;
				for(unsigned int colorRow=0; colorRow<allColors.size(); colorRow++)  {
					for(unsigned int shadeCol=0; shadeCol<g_Shades2.size(); shadeCol++)  {
						if(fd.shadeColorCount.at(colorRow).at(shadeCol)>0 && fd.shadeColorCount.at(colorRow).at(shadeCol)>=max) {
							max = fd.shadeColorCount.at(colorRow).at(shadeCol);
							indexes.push_back(colorRow);
							indexes.push_back(shadeCol);
							shadeColorIndexes.push_back(indexes);
							indexes.clear();
						}
					}
				}
				//loop to get & store majority shadeColor
				for(unsigned int y=0; y<shadeColorIndexes.size(); y++)  {
					colorIndex = shadeColorIndexes.at(y).at(0);
					shadeIndex = shadeColorIndexes.at(y).at(1);
					if(fd.shadeColorCount.at(colorIndex).at(shadeIndex)>=max)
						++pShadeColor.at(colorIndex).at(shadeIndex);
				}
				col+=ksize.width;
				fd.shadeColorCount.clear();
				fd.shadeColorCount.resize(allColors.size(),deque<int>(g_Shades2.size(),0));
			}
			row += ksize.height;
			col=0;
		}
		double height = fd.colorVec.size()/ksize.height;
		double width = fd.colorVec.at(0).size()/ksize.width;
		double t=0;
		for(unsigned int colorRow=0; colorRow<allColors.size(); colorRow++) {
			for(unsigned int shadeCol=0; shadeCol<g_Shades2.size(); shadeCol++) {
				try {
					if(pShadeColor.at(colorRow).at(shadeCol)!=0) {
						pRatio.at(colorRow).at(shadeCol) = pShadeColor.at(colorRow).at(shadeCol)/(height*width);
						t = pRatio.at(colorRow).at(shadeCol);
						pEntropy.at(colorRow).at(shadeCol) = (2.0*(f(t,a,b,p)-f(0,a,b,p)))/(2.0*(f(1,a,b,p)-f(0,a,b,p)));
					}
				}
				catch (const std::out_of_range &oor) {
					printf("pShadeColor.Rows: %lu\n", pShadeColor.size());
					printf("pShadeColor.Cols: %lu\n", pShadeColor.at(colorRow).size());
					printf("pShadeColor(%d,%d)\n",colorRow,shadeCol);
					printf("pEntropy(%d,%d)\n",colorRow,shadeCol);
					exit(1);
				}
			}
		}
	}
	catch (const std::out_of_range &oor) {
		printf("colorVec.Rows: %lu\n",fd.colorVec.size());
		printf("colorVec.Cols: %lu\n",fd.colorVec.at(row).size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}
	//Graph gr;
	//gr.graph(pShadeColor,g_Shades2,allColors,fd.filename+"pShadeColor");
	double total=0;
	String strSize = toString(ksize.width)+"x"+toString(ksize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	String filename = path+fd.filename+ "_"+ file_ksize+"_SigmoidCombined_"+strSize+".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<g_Shades2.size(); i++) {
		fprintf(fp,",%s",g_Shades2.at(i).c_str());
	}
	fprintf(fp,"\n");
	for(unsigned int i=0; i<pEntropy.size(); i++)
	{
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<pEntropy.at(i).size(); j++)
		{
			total += pRatio.at(i).at(j);
			if(j<pEntropy.at(i).size()-1)
				fprintf(fp,"%f,", pEntropy.at(i).at(j));
			else {
				total = (2.0*(f(total,a,b,p)-f(0,a,b,p)))/(2.0*(f(1,a,b,p)-f(0,a,b,p)));
				fprintf(fp,"%f,%f\n", pEntropy.at(i).at(j),total);
			}
		}
		total=0;
	}
	fclose(fp);
	return pEntropy;
}

Mat Entropy::showEntropySquares(Mat img, Size ksize)  {
	Mat dst = img.clone();
	Point end;
	String dark,high,low,light,white;
	cout << vecTotal[0] << endl;
	cout << vecTotal[1] << endl;
	cout << vecTotal[2] << endl;
	cout << vecTotal[3] << endl;
	for(int i=0; i<img.rows; i+=ksize.height)  {
		for(int j=0; j<img.cols; j+=ksize.width)  {
			if((i+ksize.width)>img.cols) end = Point(img.cols-1,img.rows-1);
			else end = Point(j+ksize.width,i+ksize.height);

			dark = toString(vec[i/ksize.height][j/ksize.width][0]);
			high = toString(vec[i/ksize.height][j/ksize.width][1]);
			low = toString(vec[i/ksize.height][j/ksize.width][2]);
			light = toString(vec[i/ksize.height][j/ksize.width][3]);
			//white = toString(vec[i/ksize.height][j/ksize.width][4]);
			//dark += " ( "+toString(roundDecimal(vec2[i/ksize.height][j/ksize.width][0],3))+" )";
			//high += " ( "+toString(roundDecimal(vec2[i/ksize.height][j/ksize.width][1],3))+" )";
			//low += " ( "+toString(roundDecimal(vec2[i/ksize.height][j/ksize.width][2],3))+" )";
			//light += " ( "+toString(roundDecimal(vec2[i/ksize.height][j/ksize.width][3],3))+" )";
			putText(dst,dark,Point(j+5,i+10),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,high,Point(j+5,i+20),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,low,Point(j+5,i+30),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,light,Point(j+5,i+40),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			rectangle(dst,Point(j,i),Point(end.x,end.y),Scalar(0,0,255));
		}
	}
	//namedWindow("output",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//imshow("output",dst);
	//waitKey(0);
	return dst;
}

inline double eyeF_fn1(double Y, double a, double b, double p)  {
	double result=0;
	result = 1.0/(1+a*exp(-b*pow(Y,p)));
	return result;
}

void Entropy::eyeFn(FileData &fd, Size ksize,String targetColor)  {
	int height = fd.colorVec.size()/ksize.height;
	int width = fd.colorVec.at(0).size()/ksize.width;
	int innerHeight = allColors.size();
	int innerWidth = g_Shades2.size();
	init_3D_Deque(vec,height,width,innerWidth);
	init_3D_Deque(vec2,height,width,innerWidth);
	init_3D_Deque(gTargetCellCount,height,width,innerWidth);
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
							shade = combineShades(shade);
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
							if(fd.filename.find("Psoriasis")!=string::npos)  {
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
							printf("ColorVec.Size: %lux%lu\n",fd.colorVec.size(),fd.colorVec.at(i).size());
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
							printf("pShadeColor.Cols: %lu\n", pShadeColor.at(colorRow).size());
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
		printf("colorVec.Cols: %lu\n",fd.colorVec.at(row).size());
		printf("colorVec(%d,%d)\n",col,row);
		exit(1);
	}
	double result=0;
	deque<deque<Point> > dnMinPt(allColors.size(),deque<Point>(g_Shades2.size(),Point(0,0)));
	deque<deque<Point> > dnMaxPt(allColors.size(),deque<Point>(g_Shades2.size(),Point(0,0)));
	deque< deque<bool> > dnMinMaxInit(allColors.size(),deque<bool>(g_Shades2.size(),false));
	deque< deque<double> > dnMin(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > dnMax(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<int> > cellCount(allColors.size(),deque<int>(g_Shades2.size(),0));
	deque< deque<int> > targetCellCount(allColors.size(),deque<int>(g_Shades2.size(),0));
	int binSize = 21;
	deque< deque<double> > stdevRatio(allColors.size(),deque<double>(g_Shades2.size(),0));
	deque< deque<double> > totalBins(allColors.size(),deque<double>(g_Shades2.size(),0));
	//deque<deque<deque<double> > > densityBin(allColors.size(),deque<deque<double> >(g_Shades2.size(),deque<double>(binSize,0)));
	deque<deque<deque<deque<double> > > > densityBin(allColors.size(),deque<deque<deque<double> > >(g_Shades2.size(),deque<deque<double> >(binSize,deque<double>(1,-1))));
	int binNum = 0;
	//deque<deque<int> > pt(height,deque<int>(width,0));
	deque<deque<Point> > ratioPtsList(allColors.size(), deque<Point>(1,Point(-1,-1)));
	deque<deque<double> > smoothRatioSingleList(allColors.size(),deque<double>(1,-1));
	deque<deque<deque<deque<double> > > > smoothRatioOutlierRm(height,deque<deque<deque<double> > >(width,deque<deque<double> >(allColors.size(),deque<double>(g_Shades2.size(),0))));
	deque<deque<deque<deque<double> > > > ratioOutlierRm(height,deque<deque<deque<double> > >(width,deque<deque<double> >(allColors.size(),deque<double>(g_Shades2.size(),0))));
	int x1=0,y1=0,minRow=0,minCol=0;
	double count=0, min=0.01;
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
						if(smoothRatioSingleList.at(c).size()>0) {
							if(smoothRatioSingleList.at(c).at(0)==-1) {
								smoothRatioSingleList.at(c).pop_front();
								ratioPtsList.at(c).pop_front();
							}
						}
						smoothRatioSingleList.at(c).push_back(smoothRatio[y1][x1][c][d]);
						ratioPtsList.at(c).push_back(Point(x1,y1));

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
						//stdevRatio.at(c).at(d) += log2(smoothRatio[y1][x1][c][d]+0.902)+0.15;
						/*****************************/
						if(targetColor!="") {
							int index = rgb.getColorIndex(targetColor);
							if(c==index) {
								//pt.at(y1).at(x1) = 1;
								targetCellCount.at(c).at(d) = cellCount.at(c).at(d);
							}
						}
						if(dnMinMaxInit.at(c).at(d)==false) {
							dnMin.at(c).at(d) = dnMax.at(c).at(d) = fnEye.at(c).at(d) * dnEye.at(c).at(d);
							dnMinPt.at(c).at(d) = dnMaxPt.at(c).at(d) = Point(x1,y1);
							dnMinMaxInit.at(c).at(d) = true;
						}
						else {
							result = fnEye.at(c).at(d) * dnEye.at(c).at(d);
							if(result<dnMin.at(c).at(d)) {
								dnMin.at(c).at(d) = result;
								dnMinPt.at(c).at(d) = Point(x1,y1);
							}
							if(result>dnMax.at(c).at(d)) {
								dnMax.at(c).at(d) = result;
								dnMaxPt.at(c).at(d) = Point(x1,y1);
							}
						}
					}
					if(targetColor!="") {
						int index = rgb.getColorIndex(targetColor);
						if(c==index) {
							vec[y1][x1][d] = fnEye.at(c).at(d);
							vec2[y1][x1][d] = dnEye.at(c).at(d);
							gTargetCellCount[y1][x1][d] = targetCellCount.at(c).at(d);
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
	for(int c=0; c<innerHeight; c++) {
		if(smoothRatioSingleList.at(c).size()>0) {
			if(smoothRatioSingleList.at(c).at(0)!=-1) {
				if(sortedList.size()==0) {
					sortedList.push_back(smoothRatioSingleList.at(c).at(0));
					sortedPtsList.push_back(ratioPtsList.at(c).at(0));
				}
				for(unsigned int d=1; d<smoothRatioSingleList.at(c).size(); d++) {
					for(unsigned int e=0; e<sortedList.size(); e++) {
						if((sortedList.size()-e)<=1) {
							if(smoothRatioSingleList.at(c).at(d)>=sortedList.at(e)) {
								sortedList.push_back(smoothRatioSingleList.at(c).at(d));
								sortedPtsList.push_back(ratioPtsList.at(c).at(d));
								break;
							}
							else {
								sortedList.push_front(smoothRatioSingleList.at(c).at(d));
								sortedPtsList.push_front(ratioPtsList.at(c).at(d));
								break;
							}
						}
						else {
							if(smoothRatioSingleList.at(c).at(d)>=sortedList.at(e) && smoothRatioSingleList.at(c).at(d)<=sortedList.at(e+1)) {
								sortedList.insert(sortedList.begin()+e+1,smoothRatioSingleList.at(c).at(d));
								sortedPtsList.insert(sortedPtsList.begin()+e+1,ratioPtsList.at(c).at(d));
								break;
							}
							if(smoothRatioSingleList.at(c).at(d)<=sortedList.at(0)) {
								sortedList.push_front(smoothRatioSingleList.at(c).at(d));
								sortedPtsList.push_front(ratioPtsList.at(c).at(d));
								break;
							}
						}
					}
				}
				double t = sortedList.size()*outlierThresh;
				t = round(t);
				for(int d=0; d<t; d++) {
					ratioOutlierRm[sortedPtsList.front().y][sortedPtsList.front().x][c][2]=0;
					ratioOutlierRm[sortedPtsList.back().y][sortedPtsList.back().x][c][2]=0;
					sortedPtsList.pop_back();
					sortedPtsList.pop_front();
					sortedList.pop_back();
					sortedList.pop_front();
				}
				//smoothRatioSingleList.at(c) = sortedList;
				sortedList.clear();
				sortedPtsList.clear();
				deque<double>().swap(sortedList);
				deque<Point>().swap(sortedPtsList);
			}
		}
	}
	smoothRatioSingleList.clear();
	smoothRatioSingleList.resize(allColors.size(),deque<double>(1,-1));
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
						if(smoothRatioSingleList.at(c).size()>0) {
							if(smoothRatioSingleList.at(c).at(0)==-1) {
								smoothRatioSingleList.at(c).pop_front();
							}
						}
						smoothRatioSingleList.at(c).push_back(smoothRatioOutlierRm[y1][x1][c][d]);
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
		if(smoothRatioSingleList.at(c).size()>0) {
			if(smoothRatioSingleList.at(c).at(0)!=-1) {
				stdevRatio.at(c).at(2) = standardDev(smoothRatioSingleList.at(c));
			}
		}
	}
	//writeSeq2File(smoothRatioSingleList, fd.filename+"_BinOutput");
	String strSize = toString(ksize.width)+"x"+toString(ksize.height);
	String file_ksize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
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

	String filename4 = path+fd.filename+ "_"+ file_ksize+"_YSV_Combined"+strSize+".csv";
	FILE * fp4;
	fp4 = fopen(filename4.c_str(),"w");
	fprintf(fp4,",Y,S,V\n");
	for(unsigned int i=0; i<allColors.size(); i++) {
		fprintf(fp4,"%s,%f,%f,%f\n",allColors.at(i).c_str(),fnEye.at(i).at(2),dnEye.at(i).at(2),stdevRatio.at(i).at(2));
	}
	fclose(fp);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	gRatio = ratio;
	gSmoothRatio = smoothRatio;
	gSmoothRatioRm = smoothRatioOutlierRm;
	gRatioRm = ratioOutlierRm;
}

Mat Entropy::showEyeFnSquares(Mat img, Size ksize, String targetColor)  {
	Mat dst = img.clone();
	Point end;
	String dark,high,low,light,white;
	String low2;
	String data,data2,data3;
	Rgb rgb;
	int indexColor = rgb.getColorIndex(targetColor);

	for(int i=0; i<img.rows; i+=ksize.height)  {
		for(int j=0; j<img.cols; j+=ksize.width)  {
			if((i+ksize.width)>img.cols) end = Point(img.cols-1,img.rows-1);
			else end = Point(j+ksize.width,i+ksize.height);
			//data = toString(roundDecimal(vec[i/ksize.height][j/ksize.width][2],3));
			//data3 = toString(roundDecimal(gTargetCellCount[i/ksize.height][j/ksize.width][2],3));
			//data2 = toString(roundDecimal(vec2[i/ksize.height][j/ksize.width][2],3));
			//dark = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][0],3));
			//high = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][1],3));
			low2 = toString(roundDecimal(gRatio[i/ksize.height][j/ksize.width][indexColor][2],3));
			low = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][2],3));
			data = toString(roundDecimal(gRatioRm[i/ksize.height][j/ksize.width][indexColor][2],3));
			data2 = toString(roundDecimal(gSmoothRatioRm[i/ksize.height][j/ksize.width][indexColor][2],3));
			//light = toString(roundDecimal(gSmoothRatio[i/ksize.height][j/ksize.width][indexColor][3],3));
			//white = toString(vec[i/ksize.height][j/ksize.width][4]);
			//putText(dst,dark,Point(j+5,i+10),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			//putText(dst,high,Point(j+5,i+20),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,data,Point(j+5,i+10),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
			putText(dst,data2,Point(j+5,i+20),FONT_HERSHEY_SIMPLEX,0.3,Scalar(255,0,0));
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

bool Entropy::loadEntropyFiles(String filepath, deque<deque<double> > &dataMat) {
	fstream fs(filepath.c_str());
	if(fs.is_open()) {
		String temp;
		deque<String> vec;
		deque<double> thresh;
		getline(fs,temp);
		while(getline(fs,temp)) {
			getSubstr(temp,',',vec);
			//vec.pop_back(); //removes the weird empty space at the end of deque
			for(unsigned int i=0; i<vec.size(); i++) {
				vec.at(i) = vec.at(i).substr(0,vec.at(i).length());
				if(i>0)
					thresh.push_back(atof(vec.at(i).c_str()));
			}
			dataMat.push_back(thresh);
			thresh.clear();
			vec.clear();
		}
		deque<String>().swap(vec);
		deque<double>().swap(thresh);
		fs.close();
		return true;
	}
	else
		printf("Failed to load Entropy file!\n");
	return false;
}

void Entropy::compareEntropy(deque<deque<double> > vec1, deque<deque<double> > vec2, deque<deque<double> > &corrVec, deque<deque<double> > &matchVec) {
	matchVec.resize(vec1.size(),deque<double>(vec1.at(0).size(),0));
	corrVec.resize(vec1.size(),deque<double>(vec1.at(0).size(),0));
	double Y_HIGH = 49.0;
	double Y_LOW = 15.0;
	double S_HIGH = 0.30;
	double S_LOW = 0.10;
	double V_HIGH = 0.10;
	double V_LOW = 0.04;
	double Y_THRESH = 0.50;
	double S_THRESH = 0.25;
	double V_THRESH = 0.20;
	double Y_DIST = 7;
	double S_DIST = 0.05;
	double V_DIST = 0.05;
	double Y_PERCEPTION = 5.0;
	double S_PERCEPTION = 0.045;
	double ysv1[3];
	double ysv2[3];
	double val=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec1.at(i).size(); j++) {
			ysv1[j] = vec1.at(i).at(j);
			ysv2[j] = vec2.at(i).at(j);
		}
		if(ysv1[0]>Y_PERCEPTION || ysv2[0]>Y_PERCEPTION) {
			if((ysv1[0]>=Y_LOW && ysv1[0]<=Y_HIGH) && (ysv2[0]>=Y_LOW && ysv2[0]<=Y_HIGH)) {
				val = (max(ysv1[0],ysv2[0])-min(ysv1[0],ysv2[0]))/min(ysv1[0],ysv2[0]);
				corrVec.at(i).at(0) = val;
				if(val<=Y_THRESH)
					matchVec.at(i).at(0) = 1.1;
				else
					matchVec.at(i).at(0) = 0.1;
			}
			else if((ysv1[0]>Y_HIGH && ysv2[0]>Y_HIGH)) {
				val = (max(ysv1[0],ysv2[0])-min(ysv1[0],ysv2[0]))/min(ysv1[0],ysv2[0]);
				corrVec.at(i).at(0) = val;
				matchVec.at(i).at(0) = 1.1;
			}
			else if((ysv1[0]>Y_HIGH && ysv2[0]<=Y_HIGH) || (ysv1[0]<=Y_HIGH && ysv2[0]>Y_HIGH)) {
				val = (max(ysv1[0],ysv2[0])-min(ysv1[0],ysv2[0]))/min(ysv1[0],ysv2[0]);
				corrVec.at(i).at(0) = val;
				if(val<=Y_THRESH)
					matchVec.at(i).at(0) = 1.1;
				else
					matchVec.at(i).at(0) = 0.1;
			}
			else if((ysv1[0]<=Y_LOW && ysv2[0]<=Y_LOW)) {
				val = max(ysv1[0],ysv2[0])-min(ysv1[0],ysv2[0]);
				corrVec.at(i).at(0) = val;
				if(val<=Y_DIST)
					matchVec.at(i).at(0) = 1.2;
				else
					matchVec.at(i).at(0) = 0.2;
			}
			else if((ysv1[0]<=Y_LOW && ysv2[0]>Y_LOW) || (ysv1[0]>Y_LOW && ysv2[0]<=Y_LOW)) {
				val = (max(ysv1[0],ysv2[0])-min(ysv1[0],ysv2[0]))/min(ysv1[0],ysv2[0]);
				corrVec.at(i).at(0) = val;
				if(val<=Y_THRESH)
					matchVec.at(i).at(0) = 1.1;
				else
					matchVec.at(i).at(0) = 0.1;
			}
			if(ysv1[1]>S_PERCEPTION || ysv2[1]>S_PERCEPTION) {
				if((ysv1[1]>=S_LOW && ysv1[1]<=S_HIGH) && (ysv2[1]>=S_LOW && ysv2[1]<=S_HIGH)) {
					val = (max(ysv1[1],ysv2[1])-min(ysv1[1],ysv2[1]))/min(ysv1[1],ysv2[1]);
					corrVec.at(i).at(1) = val;
					if(val<=S_THRESH)
						matchVec.at(i).at(1) = 1.1;
					else
						matchVec.at(i).at(1) = 0.1;
				}
				else if((ysv1[1]>S_HIGH && ysv2[1]>S_HIGH)) {
					val = (max(ysv1[1],ysv2[1])-min(ysv1[1],ysv2[1]))/min(ysv1[1],ysv2[1]);
					corrVec.at(i).at(1) = val;
					matchVec.at(i).at(1) = 1.1;
				}
				else if((ysv1[1]>S_HIGH && ysv2[1]<=S_HIGH) || (ysv1[1]<=S_HIGH && ysv2[1]>S_HIGH)) {
					val = (max(ysv1[1],ysv2[1])-min(ysv1[1],ysv2[1]))/min(ysv1[1],ysv2[1]);
					corrVec.at(i).at(1) = val;
					if(val<=S_THRESH)
						matchVec.at(i).at(1) = 1.1;
					else
						matchVec.at(i).at(1) = 0.1;
				}
				else if((ysv1[1]<=S_LOW && ysv2[1]<=S_LOW)) {
					val = max(ysv1[1],ysv2[1])-min(ysv1[1],ysv2[1]);
					corrVec.at(i).at(1) = val;
					if(val<=S_DIST)
						matchVec.at(i).at(1) = 1.2;
					else
						matchVec.at(i).at(1) = 0.2;
				}
				else if((ysv1[1]<=S_LOW && ysv2[1]>S_LOW) || (ysv1[1]>S_LOW && ysv2[1]<=S_LOW)) {
					val = (max(ysv1[1],ysv2[1])-min(ysv1[1],ysv2[1]))/min(ysv1[1],ysv2[1]);
					corrVec.at(i).at(1) = val;
					if(val<=S_THRESH)
						matchVec.at(i).at(1) = 1.1;
					else
						matchVec.at(i).at(1) = 0.1;
				}
			}
			if((ysv1[2]>=V_LOW && ysv1[2]<=V_HIGH) && (ysv2[2]>=V_LOW && ysv2[2]<=V_HIGH)) {
				val = max(ysv1[2],ysv2[2])-min(ysv1[2],ysv2[2]);
				corrVec.at(i).at(2) = val;
				if(val<=V_DIST)
					matchVec.at(i).at(2) = 1.2;
				else
					matchVec.at(i).at(2) = 0.2;
			}
			else if((ysv1[2]>V_HIGH && ysv2[2]>V_HIGH)) {
				val = (max(ysv1[2],ysv2[2])-min(ysv1[2],ysv2[2]))/min(ysv1[2],ysv2[2]);
				corrVec.at(i).at(2) = val;
				if(val<=V_THRESH)
					matchVec.at(i).at(2) = 1.1;
				else
					matchVec.at(i).at(2) = 0.1;
			}
			else if((ysv1[2]>V_HIGH && ysv2[2]<=V_HIGH) || (ysv1[2]<=V_HIGH && ysv2[2]>V_HIGH)) {
				val = (max(ysv1[2],ysv2[2])-min(ysv1[2],ysv2[2]))/min(ysv1[2],ysv2[2]);
				corrVec.at(i).at(2) = val;
				if(val<=V_THRESH)
					matchVec.at(i).at(2) = 1.1;
				else
					matchVec.at(i).at(2) = 0.1;
			}
			else if((ysv1[2]<=V_LOW && ysv2[2]<=V_LOW)) {
				val = max(ysv1[2],ysv2[2])-min(ysv1[2],ysv2[2]);
				corrVec.at(i).at(2) = val;
				if(val<=V_DIST)
					matchVec.at(i).at(2) = 1.2;
				else
					matchVec.at(i).at(2) = 0.2;
			}
			else if((ysv1[2]<=V_LOW && ysv2[2]>V_LOW) || (ysv1[2]>V_LOW && ysv2[2]<=V_LOW)) {
				val = (max(ysv1[2],ysv2[2])-min(ysv1[2],ysv2[2]))/min(ysv1[2],ysv2[2]);
				corrVec.at(i).at(2) = val;
				if(val<=V_THRESH)
					matchVec.at(i).at(2) = 1.1;
				else
					matchVec.at(i).at(2) = 0.1;
			}
		}
	}
}
