/*
 * hysteresis.cpp
 *
 *  Created on: Feb 14, 2014
 *      Author: jason
 */

#include "hysteresis.h"

void writeSeq2File(vector< vector<String> > &vec, String name)
{
	String filename = name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"[%s],", vec.at(i).at(j).c_str());
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(vector< vector<String> > &vec, String pathname, String name)
{
	String filename = pathname + name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%s,", vec.at(i).at(j).c_str());
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(vector< vector<int> > &vec, String pathname, String name)
{
	String filename = pathname + name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%d,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

void writeSeq2File(vector< vector<double> > &vec, String pathname, String name)
{
	String filename = pathname + name + ".csv";
	FILE * fp;
	fp = fopen(filename.c_str(),"w");
	for(unsigned int i=0; i<vec.size(); i++)
	{
		for(unsigned int j=0; j<vec.at(i).size(); j++)
		{
			fprintf(fp,"%f,", vec.at(i).at(j));
		}
		fprintf(fp, "\n");
	}
}

//hysteresis moving 1 col/row at a time
	void hysteresis(Mat img, Size size, String name)
	{
		rgb rgb;
		//String filename = name + "_seqCheck.csv";
		//FILE * fSeq;
		//fSeq = fopen(filename.c_str(), "w");
		int matchingScans = (size.width*size.height)/2;
		deque<String> pixelColorWindow;
		vector<String> colorWindow;
		vector< vector<String> > windowVec;
		vector<int> index;
		vector<String> color;
		vector<int> colorCount;
		int mainColorIndex[mainColors.size()];
		int mainColorLevels[mainColors.size()];
		double mainColorLevelAvg[mainColors.size()];
		String pix;
		int b,g,r;
		int row=0, col=0;
		fill_n(mainColorIndex,mainColors.size(),0);
		fill_n(mainColorLevelAvg,mainColors.size(),0);
		while(row<=(img.rows-size.height))
		{
			while(col<=(img.cols-size.width))
			{
				if(col==0)
				{
					for(int x=col; x<(col+size.width); x++)
					{
						for(int y=row; y<(row+size.height); y++)
						{
							b = img.at<Vec3b>(y,x)[0];
							g = img.at<Vec3b>(y,x)[1];
							r = img.at<Vec3b>(y,x)[2];
							pix = rgb.checkBlack(r,g,b);
							if(pix=="OTHER")
							{
								pix = rgb.pushColor(r,g,b);
								pixelColorWindow.push_back(pix);
							}
							else
							{
								pixelColorWindow.push_back(pix);
							}
						}
					}
				}
				else
				{
					for(int y=row; y<(row+size.height); y++)
					{
						b = img.at<Vec3b>(y,col+(size.width-1))[0];
						g = img.at<Vec3b>(y,col+(size.width-1))[1];
						r = img.at<Vec3b>(y,col+(size.width-1))[2];
						pix = rgb.checkBlack(r,g,b);
						if(pix=="OTHER")
						{
							pix = rgb.pushColor(r,g,b);
							pixelColorWindow.pop_front();
							pixelColorWindow.push_back(pix);
						}
						else
						{
							pixelColorWindow.pop_front();
							pixelColorWindow.push_back(pix);
						}
					}
				}
				for(unsigned int i=0; i<pixelColorWindow.size(); i++)
				{
					for(unsigned int j=0; j<mainColors.size(); j++)
					{
						if(containsMainColor(pixelColorWindow.at(i),mainColors.at(j))!=0)
							mainColorIndex[j]++;
					}
				}
				for(unsigned int j=0; j<mainColors.size(); j++)
				{
					if(mainColorIndex[j]>matchingScans)
					{
						index.push_back(j);
					}
				}
				for(unsigned int i=0; i<pixelColorWindow.size(); i++)
				{
					for(unsigned int j=0; j<index.size(); j++)
					{
						mainColorLevels[index.at(j)] = rgb.getColorLevel(pixelColorWindow.at(i),
															mainColors.at(index.at(j)));
						mainColorLevelAvg[index.at(j)] += mainColorLevels[index.at(j)];
					}
				}
				for(unsigned int i=0; i <index.size(); i++)
				{
					mainColorLevelAvg[index.at(i)] /= (size.width*size.height);
				}
				if(index.size()!=0)
				{
					pix.clear();
					for(unsigned int i=0; i<index.size(); i++)
					{
						pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
					}
					colorWindow.push_back(pix);
				}
				else
				{
					colorWindow.push_back("NZ");
				}
				fill_n(mainColorIndex,mainColors.size(),0);
				fill_n(mainColorLevelAvg,mainColors.size(),0);
				index.clear();
				++col;
			}//end while col
			windowVec.push_back(colorWindow);
			/*
			for(unsigned int i=0; i<colorWindow.size(); i++)
			{
				if(i==0)
				{
					color.push_back(colorWindow.at(i));
					++count;
				}
				else
				{
					if(colorWindow.at(i)==color.at(color.size()-1))
					{
						++count;
					}
					else
					{
						colorCount.push_back(count);
						color.push_back(colorWindow.at(i));
						count=0;
						++count;
					}
				}
			}
			colorCount.push_back(count);
			for(unsigned int i=0; i<color.size(); i++)
			{
				fprintf(fSeq, "%s(%d),", color.at(i).c_str(),colorCount.at(i));
			}
			fprintf(fSeq,"\n");*/
			//count=0;
			//colorCount.clear();
			//color.clear();
			colorWindow.clear();
			pixelColorWindow.clear();
			col=0; ++row;
		}//end while row
		writeSeq2File(windowVec,name);
		deque<String>().swap(pixelColorWindow);
		//vector<int>().swap(colorCount);
		//vector<String>().swap(color);
		vector<String>().swap(colorWindow);
		vector< vector<String> >().swap(windowVec);
		vector<int>().swap(index);
	}

	void hysteresis(Mat img,String name)
	{
		rgb rgb;
		String filename;
		vector <vector<String> > windowVec;
		vector<int> index;
		vector<String> color;
		vector<int> colorCount;
		int count=0;
		int mainColorIndex[mainColors.size()];
		String pix;
		int b,g,r;
		fill_n(mainColorIndex,mainColors.size(),0);
		for(int row=0; row<img.rows; row++)
		{
			for(int col=0; col<img.cols; col++)
			{
				b = img.at<Vec3b>(row,col)[0];
				g = img.at<Vec3b>(row,col)[1];
				r = img.at<Vec3b>(row,col)[2];
				pix = rgb.checkBlack(r,g,b);
				if(pix=="OTHER")
				{
					pix = rgb.pushColor(r,g,b);
				}
				for(unsigned int i=0; i<mainColors.size(); i++)
				{
					count = containsMainColor(pix, mainColors.at(i));
					mainColorIndex[i]+=count;
				}
				count=0;
				for(unsigned int j=0; j<mainColors.size(); j++)
				{
					if(mainColorIndex[j]>0)
					{
						if(mainColorIndex[j]>1)
						{
							index.push_back(j);
							index.push_back(j);
						}
						else
							index.push_back(j);
					}
				}
				if(index.size()!=0)
				{
					pix.clear();
					if(index.size()==1 && mainColors.at(index[0])=="Light")
					{
						pix = "White";
					}
					else if(index.size()==1 && mainColors.at(index[0])=="Dark")
					{
						pix = "Black";
					}
					else
					{
						for(unsigned int i=0; i<index.size(); i++)
						{
							pix += mainColors.at(index[i]);
						}
					}
					color.push_back(pix);
				}
				else
				{
					color.push_back("NZ");
				}
				fill_n(mainColorIndex,mainColors.size(),0);
				index.clear();
			}
			windowVec.push_back(color);
			color.clear();
		}
		writeSeq2File(windowVec,name);
		vector<String>().swap(color);
		vector< vector<String> >().swap(windowVec);
		vector<int>().swap(index);
	}
