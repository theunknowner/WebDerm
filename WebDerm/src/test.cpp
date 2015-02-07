/*
 * test.cpp
 *
 *  Created on: Feb 6, 2014
 *      Author: jason
 */

#include "test.h"

//! filter using luminance
//! if there is no knee then don't process using this method
void testAlgo1() {
	Rgb rgb;
	Hsl hsl;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5;
	img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/psoriasis19a.jpg",Size(140,140),0);
	//img = runResizeImage("/home/jason/Desktop/Programs/Color Normalized/acne12-2.png",Size(140,140),0);
	//img3 = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/clp4jpg",Size(700,700),0);
	//namedWindow("img",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	//namedWindow("img2",CV_WINDOW_FREERATIO | CV_GUI_EXPANDED);
	ShapeMorph sm;
	Size size(3,3);
	img = runColorNormalization(img);
	cvtColor(img,img,CV_BGR2GRAY);

	img = sm.prepareImage(img);
	imgshow(img);
	KneeCurve kc;
	vector<double> vec;
	Mat imgEdgeRemoval(img.rows, img.cols, CV_8U, Scalar(255));

	Mat map(img.rows, img.cols, CV_8U, Scalar(0));
	for(int i=0; i<imgEdgeRemoval.rows; i++) {
		for(int j=0; j<imgEdgeRemoval.cols; j++) {
			if(i>=0 && i<=32 && j>=0 && j<=23) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	//imgshow(map);
	vector<double> vec2;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			int val = map.at<uchar>(i,j);
			if(val!=255) {
				vec2.push_back(lum);
			}
		}
	}
	kc.removeOutliers(vec2,0.025);
	Poly poly;
	vector<double> xVec;
	sort(vec2.begin(),vec2.end());
	for(unsigned int i=0; i<vec2.size(); i++) {
		xVec.push_back((double)i);
	}
	vector<double> p = poly.polyfit(xVec,vec2,1);
	vector<double> y1 = poly.polyval(p,xVec);
	//MSE
	double sum=0;
	for(unsigned int i=0; i<y1.size(); i++) {
		double val = (vec2.at(i)-y1.at(i))/vec2.at(i);
		val = pow(val,2);
		sum += val;
	}
	sum = sqrt(sum);
	cout << "Sum2: " << sum << endl;
	vector<double> _vec2 = vec2;
	int bestIdx=0;
	if(sum<6.0) {
		//_vec2.erase(_vec2.begin(),_vec2.begin()+(vec2.size()/2));
		bestIdx = round(_vec2.size()*0.85);
	}
	else {
		bestIdx = kc.kneeCurvePoint(_vec2);
	}
	//int bestIdx = kc.kneeCurvePoint(_vec2);
	cout << bestIdx << endl;
	writeSeq2File(vec2,"vec2");
	writeSeq2File(_vec2,"_vec2");
	double thresh = _vec2.at(bestIdx);
	cout << thresh << endl;

	Mat result=img.clone();
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			int val = map.at<uchar>(i,j);
			if(val==255 || lum<thresh)
				result.at<uchar>(i,j) = 0;
		}
	}
	imgshow(result);
	/*
	img4 = sm.densityDetection(result);
	imgshow(img4);
	vector<Mat> featureVec = sm.liquidFeatureExtraction(img4);
	int countPix=0, idx=0;
	deque<int> countVec;
	deque<int> idxVec;
	for(unsigned int i=0; i<featureVec.size(); i++) {
		countPix = countNonZero(featureVec.at(i));
		idx = i;
		countVec.push_back(countPix);
	}
	jaysort(countVec,idxVec);
	vector<Mat> matVec;
	Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
	while(true) {
		static unsigned int n=1;
		//matVec.push_back(featureVec.at(idxVec.at(idxVec.size()-n)));
		morphologyEx(featureVec.at(idxVec.at(idxVec.size()-n)),img5,MORPH_CLOSE,element);
		//img5 = sm.dilation(featureVec.at(idxVec.at(idxVec.size()-n)),Size(3,3));
		matVec.push_back(img5.clone());
		imgshow(matVec.at(matVec.size()-1));
		//imwrite("img"+toString(n)+".png",matVec.at(matVec.size()-1));
		n++;
		if(matVec.size()>=5) break;
		if(n>idxVec.size()) break;
	}
	/**/
	/*
	TestML ml;
	vector<vector<double> > trainingData;
	vector<vector<double> > trainingLabels;
	ml.importCsvData("/home/jason/git/Samples/Samples/training_set.csv",trainingData,trainingLabels);
	int sampleSize = trainingData.size();
	int inputSize = trainingData.at(0).size();
	int outputSize = trainingLabels.at(0).size();
	int hiddenNodes = 20;
	ml.setLayerParams(inputSize,hiddenNodes,outputSize);
	Mat training_set(sampleSize,ml.inputSize,CV_32F);
	Mat training_labels(sampleSize,ml.outputSize,CV_32F);
	ml.vecToMat(trainingData,trainingLabels,training_set,training_labels);
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);
	/*
	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/jason/git/Samples/Samples/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	//TestML ml;
	vector<Mat> sampleVec;
	Mat sample;
	for(unsigned int i=0; i<matVec.size(); i++) {
		sample = ml.prepareImage(matVec.at(i));
		sampleVec.push_back(sample);
		sample.release();
	}
	Mat results;
	ml.setLayerParams(400,20,5);
	Mat sample_set = ml.prepareMatSamples(sampleVec);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(sample_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
		//printf("%s: ",nameVec.at(i).c_str());
		//cout << results.row(i) << " - ";
		//cout << labels.at(i) << endl;
	}
	waitKey(0);
	/*
	vector<vector<double> > data;
	vector<vector<double> > labels;
	ml.importCsvData("/home/jason/git/Samples/Samples/test_set.csv",data,labels);
	sampleSize = data.size();
	Mat test_set(sampleSize,inputSize,CV_32F);
	Mat test_labels(sampleSize,outputSize,CV_32F);
	Mat results(sampleSize,outputSize,CV_32F);
	ml.vecToMat(data,labels,test_set,test_labels);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(test_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}
	 */
}

//close - I
void testAlgo2() {
	Rgb rgb;
	Hsl hsl;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5;
	img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/psoriasis19a.jpg",Size(140,140),0);
	ShapeMorph sm;
	Size size(3,3);
	img = runColorNormalization(img);
	cvtColor(img,img,CV_BGR2GRAY);

	Mat element = getStructuringElement(MORPH_RECT,Size(17,17));
	morphologyEx(img,img2,MORPH_CLOSE,element);
	Mat dil = sm.dilation(img,Size(17,17));
	img2 = sm.erosion(dil,Size(17,17));
	img3 = img2 - img;
	imgshow(img);
	imgshow(dil);
	imgshow(img2);
	imgshow(img3);

	KneeCurve kc;
	vector<double> vec;
	Mat imgEdgeRemoval(img.rows, img.cols, CV_8U, Scalar(255));
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img3.at<uchar>(i,j);
			if(lum>5)
				vec.push_back(lum);
		}
	}
	writeSeq2File(vec,"vec1");
	Poly poly;
	/*
	vector<double> xVec;
	sort(vec.begin(),vec.end());
	for(unsigned int i=0; i<vec.size(); i++) {
		xVec.push_back((double)i);
	}
	vector<double> p = poly.polyfit(xVec,vec,1);
	vector<double> y1 = poly.polyval(p,xVec);
	//MSE
	double sum=0;
	for(unsigned int i=0; i<y1.size(); i++) {
		//cout << y1.at(i) << endl;
		double val = (vec.at(i)-y1.at(i))/vec.at(i);
		val = pow(val,2);
		sum += val;
	}
	sum = sqrt(sum);
	vector<double> _vec2 = vec;
	if(sum<6.0) {
		_vec2.erase(_vec2.begin(),_vec2.begin()+(vec.size()/2));
	}
	int bestIdx = kc.kneeCurvePoint(_vec2);
	if(_vec2.size()!=vec.size())
		bestIdx += (_vec2.size()/2);
	double thresh = vec.at(bestIdx);
	//writeSeq2File(vec,"vec");
	cout << bestIdx << endl;
	cout << thresh << endl;

	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			if(lum<thresh)
				imgEdgeRemoval.at<uchar>(i,j) = 0;
		}
	}
	Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(imgEdgeRemoval,imgEdgeRemoval,MORPH_CLOSE,element);
	//imgshow(imgEdgeRemoval);
	 */
	Mat map(img.rows, img.cols, CV_8U, Scalar(0));
	for(int i=0; i<imgEdgeRemoval.rows; i++) {
		for(int j=0; j<imgEdgeRemoval.cols; j++) {
			if(i>=0 && i<=32 && j>=0 && j<=23) {
				map.at<uchar>(i,j) = 255;
			}
		}
	}
	vector<double> vec2;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img3.at<uchar>(i,j);
			int val = map.at<uchar>(i,j);
			if(val!=255 && lum>5) {
				vec2.push_back(lum);
			}
		}
	}
	kc.removeOutliers(vec2,0.025);
	vector<double> xVec;
	sort(vec2.begin(),vec2.end());
	for(unsigned int i=0; i<vec2.size(); i++) {
		xVec.push_back((double)i);
	}
	vector<double> p = poly.polyfit(xVec,vec2,1);
	vector<double> y1 = poly.polyval(p,xVec);
	//MSE
	double sum=0;
	for(unsigned int i=0; i<y1.size(); i++) {
		double val = (vec2.at(i)-y1.at(i))/vec2.at(i);
		val = pow(val,2);
		sum += val;
	}
	sum = sqrt(sum);
	cout << "Sum2: " << sum << endl;
	vector<double> _vec2 = vec2;
	if(sum<6.0) {
		_vec2.erase(_vec2.begin(),_vec2.begin()+(vec2.size()/2));
	}
	int bestIdx = kc.kneeCurvePoint(_vec2);
	cout << bestIdx << endl;
	writeSeq2File(vec2,"vec2");
	writeSeq2File(_vec2,"_vec2");
	double thresh = _vec2.at(bestIdx);
	cout << thresh << endl;
	Mat result=img3.clone();
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img3.at<uchar>(i,j);
			int val = map.at<uchar>(i,j);
			if(lum<thresh || val==255)
				result.at<uchar>(i,j) = 0;
		}
	}
	imgshow(result);
	/*
	img4 = sm.densityDetection(result);
	vector<Mat> featureVec = sm.liquidFeatureExtraction(img4);
	int countPix=0, idx=0;
	deque<int> countVec;
	deque<int> idxVec;
	for(unsigned int i=0; i<featureVec.size(); i++) {
		countPix = countNonZero(featureVec.at(i));
		idx = i;
		countVec.push_back(countPix);
	}
	jaysort(countVec,idxVec);
	vector<Mat> matVec;
	Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
	while(true) {
		static unsigned int n=1;
		//matVec.push_back(featureVec.at(idxVec.at(idxVec.size()-n)));
		morphologyEx(featureVec.at(idxVec.at(idxVec.size()-n)),img5,MORPH_CLOSE,element);
		//img5 = sm.dilation(featureVec.at(idxVec.at(idxVec.size()-n)),Size(3,3));
		matVec.push_back(img5.clone());
		//imgshow(matVec.at(matVec.size()-1));
		//imwrite("img"+toString(n)+".png",matVec.at(matVec.size()-1));
		n++;
		if(matVec.size()>=5) break;
		if(n>idxVec.size()) break;
	}
	imgshow(img4);
	/**/

	/*
	TestML ml;
	vector<vector<double> > trainingData;
	vector<vector<double> > trainingLabels;
	ml.importCsvData("/home/jason/git/Samples/Samples/training_set.csv",trainingData,trainingLabels);
	int sampleSize = trainingData.size();
	int inputSize = trainingData.at(0).size();
	int outputSize = trainingLabels.at(0).size();
	int hiddenNodes = 20;
	ml.setLayerParams(inputSize,hiddenNodes,outputSize);
	Mat training_set(sampleSize,ml.inputSize,CV_32F);
	Mat training_labels(sampleSize,ml.outputSize,CV_32F);
	ml.vecToMat(trainingData,trainingLabels,training_set,training_labels);
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);
/*
	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/jason/git/Samples/Samples/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	//TestML ml;
	vector<Mat> sampleVec;
	Mat sample;
	for(unsigned int i=0; i<matVec.size(); i++) {
		sample = ml.prepareImage(matVec.at(i));
		sampleVec.push_back(sample);
		sample.release();
	}
	Mat results;
	ml.setLayerParams(400,20,5);
	Mat sample_set = ml.prepareMatSamples(sampleVec);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(sample_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
		//printf("%s: ",nameVec.at(i).c_str());
		//cout << results.row(i) << " - ";
		//cout << labels.at(i) << endl;
	}
	waitKey(0);
	/*
	vector<vector<double> > data;
	vector<vector<double> > labels;
	ml.importCsvData("/home/jason/git/Samples/Samples/test_set.csv",data,labels);
	sampleSize = data.size();
	Mat test_set(sampleSize,inputSize,CV_32F);
	Mat test_labels(sampleSize,outputSize,CV_32F);
	Mat results(sampleSize,outputSize,CV_32F);
	ml.vecToMat(data,labels,test_set,test_labels);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(test_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}
	/**/
}

//! 2/5/15 algo - working **Don't touch**
void testAlgo3() {
	Rgb rgb;
	Hsl hsl;
	Color c;
	Functions fn;
	Shades sh;
	rgb.importThresholds();
	hsl.importHslThresholds();
	sh.importThresholds();
	Mat img, img2,img3, img4, img5;
	img = runResizeImage("/home/jason/Desktop/Programs/Looks_Like/psoriasis1.jpg",Size(140,140),0);
	ShapeMorph sm;
	Size size(3,3);
	img = runColorNormalization(img);
	cvtColor(img,img,CV_BGR2GRAY);

	img = sm.prepareImage(img);
	imgshow(img);
	KneeCurve kc;
	deque<double> vec;
	Mat imgEdgeRemoval = img.clone();
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			vec.push_back(lum);
		}
	}
	int bestIdx = kc.kneeCurvePoint(vec);
	double thresh = vec.at(bestIdx);
	cout << bestIdx << endl;
	cout << thresh << endl;
	writeSeq2File(vec,"vec");
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			if(lum<thresh)
				imgEdgeRemoval.at<uchar>(i,j) = 0;
		}
	}
	Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
	morphologyEx(imgEdgeRemoval,imgEdgeRemoval,MORPH_CLOSE,element);
	imgshow(imgEdgeRemoval);
	Mat map(img.rows, img.cols, CV_8U, Scalar(0));
	for(int i=0; i<imgEdgeRemoval.rows; i++) {
		int leftFlag=0, rightFlag=0;
		int j2=imgEdgeRemoval.cols-1;
		for(int j=0; j<imgEdgeRemoval.cols; j++) {
			double lum = imgEdgeRemoval.at<uchar>(i,j);
			double lum2 = imgEdgeRemoval.at<uchar>(i,j2);
			if(lum>5 && leftFlag==0) {
				map.at<uchar>(i,j) = 255;
			}
			else
				leftFlag=1;
			if(lum2>5 && rightFlag==0) {
				map.at<uchar>(i,j2) = 255;
				j2--;
			}
			else
				rightFlag=1;
			if(leftFlag==1 && rightFlag==1)
				break;
		}
	}
	imgshow(map);
	deque<double> vec2;
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			int val = map.at<uchar>(i,j);
			if(val!=255) {
				vec2.push_back(lum);
			}
		}
	}

	kc.removeOutliers(vec2,0.025);
	bestIdx = kc.kneeCurvePoint(vec2);
	writeSeq2File(vec2,"data");
	cout << bestIdx << endl;
	thresh = vec2.at(bestIdx);
	cout << thresh << endl;
	Mat result=img.clone();
	for(int i=0; i<img.rows; i++) {
		for(int j=0; j<img.cols; j++) {
			double lum = img.at<uchar>(i,j);
			int val = map.at<uchar>(i,j);
			if(lum<thresh || val==255)
				result.at<uchar>(i,j) = 0;
		}
	}
	imgshow(result);
	/*
	img2 = sm.prepareImage(img);
	img3 = sm.lumFilter(img2);
	imgshow(img2);
	imgshow(img3);
/**/
	/*
	GridDisplay gd;
	gd.setImage(result);
	gd.setGridSize(Size(28,28));
	gd.drawGrid();
	gd.displayGrid();
	 */

	img4 = sm.densityDetection(result);
	vector<Mat> featureVec = sm.liquidFeatureExtraction(img4);
	int countPix=0, idx=0;
	deque<int> countVec;
	deque<int> idxVec;
	for(unsigned int i=0; i<featureVec.size(); i++) {
		countPix = countNonZero(featureVec.at(i));
		idx = i;
		countVec.push_back(countPix);
	}
	jaysort(countVec,idxVec);
	vector<Mat> matVec;
	element = getStructuringElement(MORPH_RECT,Size(3,3));
	while(true) {
		static unsigned int n=1;
		//matVec.push_back(featureVec.at(idxVec.at(idxVec.size()-n)));
		morphologyEx(featureVec.at(idxVec.at(idxVec.size()-n)),img5,MORPH_CLOSE,element);
		//img5 = sm.dilation(featureVec.at(idxVec.at(idxVec.size()-n)),Size(3,3));
		matVec.push_back(img5.clone());
		//imgshow(matVec.at(matVec.size()-1));
		//imwrite("img"+toString(n)+".png",matVec.at(matVec.size()-1));
		n++;
		if(matVec.size()>=5) break;
		if(n>idxVec.size()) break;
	}
	imgshow(img4);
	/**/

	/*
	TestML ml;
	vector<vector<double> > trainingData;
	vector<vector<double> > trainingLabels;
	ml.importCsvData("/home/jason/git/Samples/Samples/training_set.csv",trainingData,trainingLabels);
	int sampleSize = trainingData.size();
	int inputSize = trainingData.at(0).size();
	int outputSize = trainingLabels.at(0).size();
	int hiddenNodes = 20;
	ml.setLayerParams(inputSize,hiddenNodes,outputSize);
	Mat training_set(sampleSize,ml.inputSize,CV_32F);
	Mat training_labels(sampleSize,ml.outputSize,CV_32F);
	ml.vecToMat(trainingData,trainingLabels,training_set,training_labels);
	Mat layers(3,1,CV_32S);
	layers.at<int>(0,0) = inputSize;
	layers.at<int>(1,0) = hiddenNodes;
	layers.at<int>(2,0) = outputSize;
	CvANN_MLP ann(layers,CvANN_MLP::SIGMOID_SYM,0.6,1);
/*
	TermCriteria criteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
	CvANN_MLP_TrainParams params(criteria,CvANN_MLP_TrainParams::BACKPROP,0.1,0.1);
	int iter = ann.train(training_set,training_labels,Mat(),Mat(),params);
	cout << "Iterations: " << iter << endl;
	CvFileStorage* storage = cvOpenFileStorage("/home/jason/git/Samples/Samples/param.xml", 0, CV_STORAGE_WRITE );
	ann.write(storage,"shapeML");
	cvReleaseFileStorage(&storage);
/**/
	/*
	//TestML ml;
	vector<Mat> sampleVec;
	Mat sample;
	for(unsigned int i=0; i<matVec.size(); i++) {
		sample = ml.prepareImage(matVec.at(i));
		sampleVec.push_back(sample);
		sample.release();
	}
	Mat results;
	ml.setLayerParams(400,20,5);
	Mat sample_set = ml.prepareMatSamples(sampleVec);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(sample_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
		//printf("%s: ",nameVec.at(i).c_str());
		//cout << results.row(i) << " - ";
		//cout << labels.at(i) << endl;
	}
	waitKey(0);
	/*
	vector<vector<double> > data;
	vector<vector<double> > labels;
	ml.importCsvData("/home/jason/git/Samples/Samples/test_set.csv",data,labels);
	sampleSize = data.size();
	Mat test_set(sampleSize,inputSize,CV_32F);
	Mat test_labels(sampleSize,outputSize,CV_32F);
	Mat results(sampleSize,outputSize,CV_32F);
	ml.vecToMat(data,labels,test_set,test_labels);
	ann.load("/home/jason/git/Samples/Samples/param.xml");
	ann.predict(test_set,results);
	for(int i=0; i<results.rows; i++) {
		printf("Sample: %d, ",i+1);
		cout << results.row(i) << endl;
	}
	/**/
}

String testHysteresis(Mat &img, int row, int col, Size size)
{
	Rgb rgb;
	Hsl hsl;
	Intensity in;
	Color colorObj;
	int b,g,r;
	int ind=0;
	double dist=0;
	double grayLevel=0;
	double* HSL;
	int hue=0;
	double hslAvg[3]={0};
	deque<double> hueWindow;
	deque<double> satWindow;
	deque<double> lumWindow;
	String hslStr;
	int count=0; //stores the amount of pixels hit
	deque<int> index;
	String pix;
	deque<String> pixelColorWindow;
	int colorIndex[rgbColors.size()];
	int mainColorIndex[mainColors.size()];
	double mainColorLevels[mainColors.size()];
	double mainColorLevelAvg[mainColors.size()];
	int dimension = size.width*size.height;
	int matchingScans = dimension/2;
	int windowFlags[dimension];
	fill_n(windowFlags,dimension,0);
	fill_n(colorIndex,rgbColors.size(),0);
	fill_n(mainColorIndex,mainColors.size(),0);
	fill_n(mainColorLevelAvg,mainColors.size(),0);
	for(int x=(col-1); x<(col+size.width-1); x++)
	{
		for(int y=(row-1); y<(row+size.height-1); y++)
		{
			b = img.at<Vec3b>(y,x)[0];
			g = img.at<Vec3b>(y,x)[1];
			r = img.at<Vec3b>(y,x)[2];
			pix = rgb.checkBlack(r,g,b);
			if(pix=="OTHER")
			{
				pix = rgb.calcColor(r,g,b,dist,ind);
			}
			HSL = hsl.rgb2hsl(r,g,b);
			HSL[1] = roundDecimal(HSL[1],2);
			HSL[2] = roundDecimal(HSL[2],2);
			hueWindow.push_back(HSL[0]);
			satWindow.push_back(HSL[1]);
			lumWindow.push_back(HSL[2]);
			pixelColorWindow.push_back(pix);
			cout << pix << img.at<Vec3b>(y,x) << ";";
			printf("HSL(%.0f,%.2f,%.2f)\n",HSL[0],HSL[1],HSL[2]);
		}
	}
	for(unsigned int i=0; i<pixelColorWindow.size(); i++)
	{
		for(unsigned int j=0; j<mainColors.size(); j++)
		{
			if(colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j))!=0)
				mainColorIndex[j]++;
		}
	}
	for(unsigned int j=0; j<mainColors.size(); j++)
	{
		if(mainColorIndex[j]!=0)
		{
			cout << mainColors.at(j) << ":"<< mainColorIndex[j] << endl;
		}
		if(mainColorIndex[j]>=round(matchingScans))
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
			if(pixelColorWindow.at(i).find(mainColors.at(index.at(j)))!=string::npos) {
				windowFlags[i] = 1;
			}
		}
	}
	for(int i=0; i<dimension; i++) {
		if(windowFlags[i]==true) {
			grayLevel += rgb.getGrayLevel1(pixelColorWindow.at(i));
			hue =  hueWindow.at(i) + 180;
			if(hue>=360) hue %= 360;
			hslAvg[0] += hue;
			hslAvg[1] += satWindow.at(i);
			hslAvg[2] += lumWindow.at(i);
			count++;
		}
	}
	for(unsigned int i=0; i <index.size(); i++)
	{
		mainColorLevelAvg[index.at(i)] /= mainColorIndex[index.at(i)];
	}
	double h = 0, s = 0, l = 0;
	if(index.size()!=0)
	{
		pix.clear();
		for(unsigned int i=0; i<index.size(); i++)
		{
			if(mainColors.at(index[i])=="Zero") {
				pix = mainColors.at(index[i]);
				break;
			}
			else if(mainColors.at(index[i])=="Black" || mainColors.at(index[i])=="White") {
				pix = mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
				break;
			}
			else
				pix += mainColors.at(index[i]) + toString(round(mainColorLevelAvg[index.at(i)]));
		}
		grayLevel = round(grayLevel/count);
		hslAvg[0] = round(hslAvg[0]/count);
		hslAvg[1] = roundDecimal(hslAvg[1]/count,2);
		hslAvg[2] = roundDecimal(hslAvg[2]/count,2);
		hslAvg[0] -= 180;
		if(hslAvg[0]<0) hslAvg[0] += 360;
		h = hslAvg[0];
		s = hslAvg[1];
		l = hslAvg[2];
		pix = colorObj.fixColors(pix,r,g,b);
		if(pix!="Zero")
			pix = toString(grayLevel) + pix;
		else {
			h=0; s=0; l=0;
		}
	}
	else pix = "NOISE";

	hslStr = toString(h)+";"+toString(s)+";"+toString(l);
	cout << "HSL_Avg: " << hslStr << endl;
	//cout << "COLORINT:" << in.calcIntensity(pix) << endl;
	deque<int>().swap(index);
	deque<String>().swap(pixelColorWindow);
	//img.release();
	return pix;
}

//hysteresis with info output to image window using vectors
String testMouseHysteresis(Mat &img, int row, int col, Size size,
		deque<Vec3b> &vec, deque<String> &colorVec)
{
	Rgb rgb;
	Color colorObj;
	int b,g,r;
	deque<int> index;
	String pix;
	deque<String> pixelColorWindow;
	int colorIndex[rgbColors.size()];
	int mainColorIndex[mainColors.size()];
	int matchingScans = (size.width*size.height)/2;
	fill_n(colorIndex,rgbColors.size(),0);
	fill_n(mainColorIndex,mainColors.size(),0);
	for(int x=(col-1); x<(col+size.width-1); x++)
	{
		for(int y=(row-1); y<(row+size.height-1); y++)
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
			vec.push_back(img.at<Vec3b>(y,x));
			colorVec.push_back(pix);
		}
	}
	int count=0;
	for(unsigned int i=0; i<pixelColorWindow.size(); i++)
	{
		for(unsigned int j=0; j<mainColors.size(); j++)
		{
			count = colorObj.containsMainColor(pixelColorWindow.at(i),mainColors.at(j));
			mainColorIndex[j]+=count;
		}
	}
	for(unsigned int j=0; j<mainColors.size(); j++)
	{
		if(mainColorIndex[j]>matchingScans)
		{
			if(mainColorIndex[j]>(matchingScans*size.height))
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
	}
	else pix = "NOISE";

	deque<int>().swap(index);
	deque<String>().swap(pixelColorWindow);
	//img.release();
	return pix;
}

void testColorIndex(Mat &img, int index)
{
	Rgb rgb;
	int r,g,b;
	int ind=0;
	double dist=0;
	int count=0;
	Mat mask = mask.zeros(img.size(), CV_8U);
	Mat img2;
	for(int row=0; row<img.rows; row++)
	{
		for(int col=0; col<img.cols; col++)
		{
			r = img.at<Vec3b>(row,col)[2];
			g = img.at<Vec3b>(row,col)[1];
			b = img.at<Vec3b>(row,col)[0];
			rgb.pushColor(r,g,b,ind,dist);
			if((ind+2)==index)
			{
				//printf("%d,%d\n", col,row);
				mask.at<uchar>(row,col) = 255;
				++count;
			}
		}
	}
	cout << count << endl;
	img.copyTo(img2,mask);
	imshow("Img", img);
	imshow("Img2",img2);
	waitKey(0);
}

String testColorAtLoc(Mat &img, Point pt, double &h) {
	Rgb rgb;
	Hsl hsl;
	int r,g,b;
	int ind= -3;
	double dist=0;
	int hue=0;
	double * HSL;
	String pix;
	r = img.at<Vec3b>(pt.y,pt.x)[2];
	g = img.at<Vec3b>(pt.y,pt.x)[1];
	b = img.at<Vec3b>(pt.y,pt.x)[0];
	pix = rgb.checkBlack(r,g,b);
	HSL = hsl.rgb2hsl(r,g,b);
	HSL[1] = roundDecimal(HSL[1],2);
	HSL[2] = roundDecimal(HSL[2],2);
	printf("RGB(%d,%d,%d)\n",r,g,b);
	printf("HSL(%.f,%.2f,%.2f)\n",HSL[0],HSL[1],HSL[2]);
	if(pix=="OTHER") {
		pix = rgb.calcColor2(r,g,b);
		if(pix=="OTHER") {
			pix = rgb.pushColor(r,g,b,ind,dist);
		}
	}
	hue = (hsl.getHue()+180);
	hue %= 360;
	hue /= 360;
	h = hue;
	//cout << pix << img.at<Vec3b>(pt.y,pt.x) << ";" << ind+2 << ";" << hue<< endl;
	return pix;
}
