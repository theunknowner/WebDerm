
#include "histogram.h"

Mat Histogram::calcHistogram(Mat src) {
	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split( src, bgr_planes );

	/// Establish the number of bins
	int histSize = 255;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 1, 256 } ;
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

	// Draw the histograms for B, G and R
	int hist_w = 700; int hist_h = 512;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
				Scalar( 255, 0, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
				Scalar( 0, 255, 0), 2, 8, 0  );
		line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
				Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
				Scalar( 0, 0, 255), 2, 8, 0  );
	}

    return histImage;
}

void Histogram::equalizeHistogram(Mat src, Mat &dst) {
	String source_window = "Source image";
	String equalized_window = "Equalized Image";
	vector<Mat> bgr_planes;
	cvtColor(src,src,CV_BGR2HLS);
	split( src, bgr_planes );

	/// Apply Histogram Equalization
	equalizeHist( bgr_planes.at(1), bgr_planes.at(1) );

	merge(bgr_planes,src);

	cvtColor(src,dst,CV_HLS2BGR);

	/// Display results
	namedWindow( source_window, CV_WINDOW_AUTOSIZE );
	namedWindow( equalized_window, CV_WINDOW_AUTOSIZE );

	imshow( source_window, src );
	imshow( equalized_window, dst );

	/// Wait until user exits the program
	waitKey(0);
}

/** HSL histogram info **/
void Histogram::hist2SpreadSheet(Mat &src, String name) {
	FILE *fp;
	String filename = path+name+"_hist.csv";
	fp = fopen(filename.c_str(),"w");
	fprintf(fp,"Hue,#,,Sat,#,,Lum,#\n");
	hsl hsl;
	int r,g,b;
	int hue,sat,lum;
	int hueArr[360] = {0};
	int satArr[101] = {0};
	int lumArr[101] = {0};
	for(int i=0; i<src.rows; i++) {
		for(int j=0; j<src.cols; j++) {
			r = src.at<Vec3b>(i,j)[2];
			g = src.at<Vec3b>(i,j)[1];
			b = src.at<Vec3b>(i,j)[0];
			hsl.rgb2hsl(r,g,b);
			hue = hsl.getHue();
			sat = hsl.getSat()*100;
			lum = hsl.getLum()*100;
			if(hue==360) hue=0;
			hueArr[hue]++;
			satArr[sat]++;
			lumArr[lum]++;
		}
	}
	int i=0, j=0, k=0;
	int count1=0, count2=0, count3=0;
	while(i<360 || j<101 || k<101) {
		for(;i<360;i++) {
			if(hueArr[i]>0) {
				fprintf(fp,"%d,%d,,",i,hueArr[i]);
				count1++;
				break;
			}
		}
		i++;
		for(;j<101;j++) {
			if(satArr[j]>0) {
				if(count2>=count1)
					fprintf(fp,",,,");
				fprintf(fp,"%d,%d,,",j,satArr[j]);
				count2++;
				break;
			}
		}
		j++;
		for(;k<101;k++) {
			if(lumArr[k]>0) {
				if(count3>=count2 && count3<count1)
					fprintf(fp,",,,");
				else if(count3>=count2 && count3>=count1)
					fprintf(fp,",,,,,,");
				fprintf(fp,"%d,%d",k,lumArr[k]);
				count3++;
				break;
			}
		}
		k++;
		fprintf(fp,"\n");
	}
}
