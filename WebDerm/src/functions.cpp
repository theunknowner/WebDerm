/*
 * functions.cpp
 *
 *  Created on: Jan 17, 2014
 *      Author: jason
 */

#include "/home/jason/git/WebDerm/WebDerm/headers/functions.h"

String toString(int val)
{
	ostringstream stringStream;
	stringStream << val;
	return stringStream.str();
}

String toString(double val)
{
	ostringstream stringStream;
	stringStream << val;
	return stringStream.str();
}

double roundDecimal(double num, int places)
{
	double val=0;
	val = num * pow(10,places);
	val = round(val);
	val /= pow(10,places);
	return val;
}

	/* Get Standard Deviation of the Y-axis */
	double stdDevY(Mat img)
	{
		double row[img.rows];
		double rowAvg=0;
		double rowSquare=0;
		double result;

		for(int y=0; y<img.rows; y++)
		{
			row[y] = y;
			rowAvg += row[y];
		}
		rowAvg /= img.rows;
		rowAvg *= rowAvg;

		for(int y=0; y<img.rows; y++)
		{
			row[y] *= row[y];
			rowSquare +=row[y];
		}
		rowSquare /=img.rows;
		result = rowSquare - rowAvg;
		return sqrt(result);
	}

	/* Get Standard Deviation of the X-axtutorialpointis */
	double stdDevX(Mat img)
	{
		double col[img.cols];
		double colAvg=0;
		double colSquare=0;
		double result=0;

		for(int x=0; x<img.cols; x++)
			{
				col[x] = x;
				colAvg += col[x];
			}
			colAvg /= img.cols;
			colAvg *= colAvg;

			for(int x=0; x<img.cols; x++)
			{
				col[x] *= col[x];
				colSquare +=col[x];
			}
			colSquare /=img.cols;
			result = colSquare - colAvg;
			return sqrt(result);
	}

	/* Some equation that goes in function eTotal() for the Gaussian filter formula*/
	double functionE(Point pixLoc, Point currLoc, double stdX, double stdY)
	{
		double result;
		double x=0.0;
		double y=0.0;

		x = pixLoc.x - currLoc.x;
		x *= x;;
		x = x / (2*(stdX*stdX));
		y = pixLoc.y - currLoc.y;
		y *= y;
		y = y / (2*(stdY*stdY));

		result = x + y;
		return result;
	}

	/* Gets the wait of the current pixel at location (X,Y) for the Gaussian Filter formula*/
	double eTotal(double function)
	{
		double result;

		result = exp(-(function));
		return result;
	}

	void kernel(Mat img, Size size, double stdX, double stdY)
	{
		double ksize[size.width][size.height];
		double funcE;
		double eVar;
		int col=size.width/2;
		int row=size.height/2;

				for(int y=0; y<=2*row; y++)
				{
					for(int x=0; x<=2*col; x++)
					{
						funcE = functionE(Point(col,row), Point(x,y), stdX, stdY);
						eVar = eTotal(funcE);
						//cout << eVar << endl;
						ksize[x][y] = eVar;
					}
				}

		for(int i=0; i<size.height; i++)
		{
			for(int j=0; j<size.width; j++)
			{
				cout << "[" << ksize[j][i] << "]" << " ";
			}
			cout << endl;
		}
	}

	double calcSlope(Point pt, Point origin)
	{
		double slopeY, slopeX;
		slopeY = (pt.y - origin.y);
		slopeX = (pt.x - origin.x);
		if(slopeX==0)
			return slopeY;
		else
			return slopeY/slopeX;
	}

	Mat cropImage(Mat input)
	{
		Mat img1, img2;
		int ptX=0, ptY=0;
		int roiWidth=0, roiHeight=0;

		for(int i=0;i<input.cols;i++)
		{
			if(countNonZero(input.col(i))>0)
			{
				 ptX = i;
				 roiWidth = input.cols-ptX;
				 break;
			}
		}
		for(int j=0;j<input.rows;j++)
		{
			if(countNonZero(input.row(j))>0)
			{
				ptY= j;
				roiHeight = input.rows-ptY;
				break;
			}
		}
		img1 = input(Rect(ptX,ptY,roiWidth,roiHeight));

		for(int i=img1.cols-1;i>0;i--)
		{
			if(countNonZero(img1.col(i))>0)
			{
				 roiWidth = i;
				 break;
			}
		}
		for(int j=img1.rows-1;j>0;j--)
		{
			if(countNonZero(img1.row(j))>0)
			{
				roiHeight = j;
				break;
			}
		}
		img2 = img1(Rect(0,0,roiWidth,roiHeight));
		return img2;
	}

	Mat rotateImage(const Mat& source, double angle)
	{
	    Point2f src_center(source.cols/2.0F, source.rows/2.0F);
	    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	    Mat dst;
	    warpAffine(source, dst, rot_mat, source.size());
	    return dst;
	}

	int checkTransitions(Mat img)
	{
		int flag=0;
		int change=0;
		int count=0;
		for(int col=0; col<img.cols; col++)
		{
			if(img.at<uchar>(0,col) == 0)
			{
				flag=0;
				if(flag!=change)
				{
					change=flag;
					++count;
				}
				if(img.at<uchar>(0,col) == 255)
				{
					flag=1;
					if(flag!=change)
					{
						change=flag;
						++count;
					}
				}
			}
		}
		return count;
	}

	Mat fillEdges2(Mat img)
	{
		Mat img2 = img.clone();
		Mat kernel[3] = Mat::zeros(1, img.cols, CV_8U);
		Point origin[3];
		Point pt[2];
		double originSlope[2];
		double avgOriginSlope=0;
		double thresh = 0.05;
		int flag=0, change=0;
		int j=0;
		int y[3];
		int i=0;
		double dist=0;
		double maxSlope=0;

		for(int row=1; row<img.rows-33; row+=16)
		{
			y[0] = row;
			y[1] = row+16;
			y[2] = row+32;
			kernel[0] = img2.row(row);
			kernel[1] = img2.row(row+16);
			kernel[2] = img2.row(row+32);
			for(int r=0; r<3; r++)
			{
				for(int c=0; c<kernel[r].cols; c++)
				{
					if((int)kernel[r].at<uchar>(0,c) == 0)
					{
						flag=0;
						if(flag!=change)
							change = flag;
					}
					if((int)kernel[r].at<uchar>(0,c) == 255)
					{
						flag=1;
						if(flag!=change)
						{
							change = flag;
							if(j<3)
							{
								origin[j] = Point(c,y[j]);
								j++;
								if(j==3)
								{
									originSlope[0] = calcSlope(origin[1], origin[0]);
									originSlope[1] = calcSlope(origin[2], origin[0]);
									avgOriginSlope = (originSlope[0]+originSlope[1])/2;
								}
								break;
							}
							else
							{
								if(i<3)
								{
									origin[i] = Point(c,y[i]);
									i++;
									if(i==3)
									{
										originSlope[0] = calcSlope(origin[1], origin[0]);
										originSlope[1] = calcSlope(origin[2], origin[0]);
										if( abs(originSlope[0]-avgOriginSlope) > abs(originSlope[1]-avgOriginSlope) )
										{
											maxSlope = abs(originSlope[0]-avgOriginSlope);
										}
										else
										{
											maxSlope = abs(originSlope[1]-avgOriginSlope);
										}
										dist = maxSlope / (abs(avgOriginSlope) + 0.00001);
										if(dist<=thresh)
										{
											avgOriginSlope = (originSlope[0]+originSlope[1])/2;
											i=0;
										}
										else
										{
											pt[0] = origin[1];
											pt[1] = origin[2];
											if(avgOriginSlope==0)
											{
												origin[1].x = origin[0].x;
												origin[2].x = origin[0].x;
											}
											else
											{
												origin[1].x = ((origin[1].y - origin[0].y) / (avgOriginSlope)) + origin[0].x;
												origin[2].x = ((origin[2].y - origin[0].y) / (avgOriginSlope)) + origin[0].x;
											}
											for(int a=1; a<=2; a++)
											{
												for(int col=origin[a].x; col<pt[a-1].x; col++)
												{
													img2.at<uchar>(origin[a].y, col) = 255;
												}
											}
											i=0;
										}
									}
									break;
								}
							}
						}
					}
				}
			}
		}
		return img2;
	}

	Mat fillEdges3(Mat img)
	{
		Mat img2 = img.clone();
		Mat kernel[5] = Mat::zeros(1, img.cols, CV_8U);
		Point origin[5];
		Point originPt;
		Point pt;
		int flag=0, change=0;
		int y[5];
		int j=0;
		int i=0;
		int devThresh=3;

		for(int row=1; row<img2.rows-5; row+=4)
		{
			y[0] = row;
			y[1] = row+1;
			y[2] = row+2;
			y[3] = row+3;
			y[4] = row+4;
			kernel[0] = img2.row(y[0]);
			kernel[1] = img2.row(y[1]);
			kernel[2] = img2.row(y[2]);
			kernel[3] = img2.row(y[3]);
			kernel[4] = img2.row(y[4]);
			for(int r=0; r<5; r++)
			{
				for(int c=0; c<kernel[r].cols; c++)
				{
					if( (int)kernel[r].at<uchar>(0,c) == 0)
					{
						flag = 0;
						if(flag!=change)
						{
							change=flag;

						}
					}
					if((int)kernel[r].at<uchar>(0,c) == 255)
					{
						flag=1;
						if(flag!=change)
						{
							change=flag;
							if(j<5)
							{
								origin[r] = Point(c, y[r]);
								originPt = origin[0];
								j++;
								if(j<5) break;
							}
							if(j>=5)
							{
								if(i<5)
								{
									origin[r] = Point(c, y[r]);
									i++;
									if(i==5)
									{
										for(int a=1; a<5; a++)
										{
											if(abs(origin[a].x-origin[0].x) >= devThresh || abs(origin[a].x-originPt.x) >= devThresh)
											{
												img2.at<uchar>(y[a],origin[0].x) = 255;
												for(int col=origin[0].x; col<origin[a].x; col++)
												{
													img2.at<uchar>(y[a],col) = 255;
												}
											}
										}
										originPt = origin[0];
									}
									break;
								}
							}
						}
					}
				}
			}
			i=0;
		}
		return img2;
	}

	void match_color_seq(char colorSequence[], int length, deque<String> strArr, int * seq, FILE *fp)
	{
		String temp;
		String str(colorSequence, length);
		size_t found=0;
		int tempSeq[strArr.size()];
		unsigned int j=0;
		for(unsigned int i=0; i<strArr.size(); i++)
		{
			tempSeq[i] = 0;
		}
		for(unsigned int i=0; i<strArr.size(); i++)
		{
			while(j<str.size())
			{
				found = str.find(strArr.at(i),found);
				if(found!=string::npos)
				{
					seq[i]++;
					tempSeq[i]++;
					found++;
					j=found;
				}
				else {j++;}
			}
			found=0;
			j=0;
			if(tempSeq[i]!=0)
			{
				fprintf(fp, "%s: %d\n", strArr.at(i).c_str(), tempSeq[i]);
			}

		}
	}

	void freqOfList(deque<double> &vec) {
		FILE * fp;
		fp = fopen("Freq.csv","w");
		int arr[255]={0};
		int val=0;
		double total=0;
		double percent=0;
		for(unsigned int i=0; i<vec.size(); i++) {
			val=vec.at(i);
			arr[val]++;
		}
		for(int i=0; i<255; i++) {
			fprintf(fp,"%d,",i);
		}
		fprintf(fp,"\n");
		for(int i=0;i<255; i++) {
			fprintf(fp,"%d,",arr[i]);
			total += arr[i];
		}
		fprintf(fp,"\n");
		for(int i=0; i<255; i++) {
			percent = arr[i]/total;
			fprintf(fp,"%f,",percent);
		}
		cout << total << endl;
	}
