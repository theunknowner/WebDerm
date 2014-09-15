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
	rgb rgb;
	Color c;
	for(unsigned int i=0; i<fd.colorVec.size(); i++) {
		for(unsigned int j=0; j<fd.colorVec.at(i).size(); j++) {
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

	String filename = path+fd.filename + "_Entropy.csv";
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
	outputEntropyDistance(doubleVec1,doubleVec2,name);
}

void Entropy::outputEntropyDistance(deque< deque<double> > &vec1, deque< deque<double> > &vec2,String name) {
	deque< deque<double> > distVec(vec1.size(), deque<double>(vec1.at(0).size(),0));
	int cumDist[20] = {0};
	int dist=0;
	for(unsigned int i=0; i<vec1.size(); i++) {
		for(unsigned int j=0; j<vec1.at(i).size(); j++) {
			distVec.at(i).at(j) = abs(vec1.at(i).at(j)-vec2.at(i).at(j));
			dist = floor(distVec.at(i).at(j));
			++cumDist[dist];
		}
	}

	String filename = path+name + "_EntropyDistance.csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<g_Shades.size(); i++) {
		fprintf(fp,",%s",g_Shades.at(i).c_str());
	}
	fprintf(fp,"\n");
	for(unsigned int i=0; i<distVec.size(); i++)
	{
		fprintf(fp,"%s,",allColors.at(i).c_str());
		for(unsigned int j=0; j<distVec.at(i).size(); j++)
		{
			if(j<distVec.at(i).size()-1)
				fprintf(fp,"%f,", distVec.at(i).at(j));
			else
				fprintf(fp,"%f\n", distVec.at(i).at(j));
		}
	}
	for(int i=0; i<20; i++) {
		if(cumDist[i]!=0) {
			fprintf(fp,"%d,%d\n",i,cumDist[i]);
		}
	}
	fclose(fp);
}
