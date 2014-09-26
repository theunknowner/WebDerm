/*
 * Entropy.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: jason
 */

#include "entropy.h"

void Entropy::outputEntropy(FileData &fd, Size ksize) {
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
				/**temporary testing**/
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
					//shade = "Dark1";
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
			/**temporary testing**/
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
	String strSize = toString(fd.ksize.width)+"x"+toString(fd.ksize.height);
	String filename = path+fd.filename + "_Entropy_"+strSize+".csv";
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
	compareEntropy(doubleVec1,doubleVec2,name);
}

/** shifts the vector elements left n_times **/
void shiftLeft(deque<double> &vec, int n_times) {
	for(int i=0; i<n_times; i++) {
		vec.pop_front();
		vec.push_back(0);
	}
}

void Entropy::compareEntropy(deque< deque<double> > &vec1, deque< deque<double> > &vec2,String name) {
	deque< deque<double> > distVec(vec1.size(), deque<double>(vec1.at(0).size(),0));
	int flag=0;
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

	}
}
