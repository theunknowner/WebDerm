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

deque< deque<double> > Entropy::outputCombinedEntropy(FileData &fd, Size ksize) {
	String shade, color, pix;
	int shadeIndex=0, colorIndex=0;
	Shades sh;
	Rgb rgb;
	Color c;
	init_2D_Deque(fd.shadeColorCount,g_Shades2.size(), allColors.size(),0);
	for(unsigned int i=0; i<fd.colorVec.size(); i++) {
		for(unsigned int j=0; j<fd.colorVec.at(i).size(); j++) {
			try {
				pix = fd.colorVec.at(i).at(j);
				shade = sh.extractShade(pix);
				/**temporary testing**/
				if(shade.find("Dark")!=string::npos) shade = "Dark";
				if(shade.find("High")!=string::npos) shade = "High";
				if(shade.find("Low")!=string::npos) shade = "Low";
				if(shade.find("Light")!=string::npos) shade = "Light";
				/************************************/
				shadeIndex = sh.getShadeIndex2(shade);
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
							/**temporary testing**/
							if(shade.find("Dark")!=string::npos) shade = "Dark";
							if(shade.find("High")!=string::npos) shade = "High";
							if(shade.find("Low")!=string::npos) shade = "Low";
							if(shade.find("Light")!=string::npos) shade = "Light";
		/************************************/
							shadeIndex = sh.getShadeIndex2(shade);
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
					for(unsigned int shadeCol=0; shadeCol<g_Shades2.size(); shadeCol++) {
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
	for(unsigned int i=0; i<pEntropy.size(); i++)
	{
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<pEntropy.at(i).size(); j++)
		{
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

void Entropy::importEntropyFiles(String path1, String path2,String name) {
	deque< deque<String> > vec;
	deque< deque<String> > vec2;
	deque< deque<double> > doubleVec1;
	deque< deque<double> > doubleVec2;
	deque<double> tempVec;
	deque<double> tempVec2;
	FileData fd;
	fd.loadFileMatrix(path1,vec);
	fd.loadFileMatrix(path2,vec2);
	for(unsigned int i=1; i<vec.size(); i++) {
		for(unsigned int j=1; j<vec.at(i).size(); j++) {
			tempVec.push_back(atof(vec.at(i).at(j).c_str()));
			tempVec2.push_back(atof(vec2.at(i).at(j).c_str()));
		}
		doubleVec1.push_back(tempVec);
		doubleVec2.push_back(tempVec2);
		tempVec.clear();
		tempVec2.clear();
	}
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
							/**temporary testing**/
							if(shade.find("Dark")!=string::npos) shade = "Dark";
							if(shade.find("High")!=string::npos) shade = "High";
							if(shade.find("Low")!=string::npos) shade = "Low";
							if(shade.find("Light")!=string::npos) shade = "Light";
							/************************************/
							shadeIndex = sh.getShadeIndex2(shade);
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
	Graph gr;
	gr.graph(pShadeColor,g_Shades2,allColors,fd.filename+"pShadeColor");
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
