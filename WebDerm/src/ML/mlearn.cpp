/*
 *mlearn.cpp
 *
 *Createdon:Nov14,2014
 *Author:jason
 */

#include "mlearn.h"

bool plotSupportVectors=true;
int numTrainingPoints=200;
int numTestPoints=2000;
int size=200;
int eq=0;

// accuracy
float evaluate ( Mat &predicted , Mat &actual ) {
	assert( predicted.rows == actual.rows );
	int t = 0;
	int f = 0;
	for ( int i = 0; i < actual.rows ; i ++) {
		float p = predicted.at < float >( i ,0) ;
		float a = actual.at < float >( i ,0) ;
		if (( p >= 0.0 && a >= 0.0) || ( p <= 0.0 && a <= 0.0) ) {
			t ++;
		} else {
			f ++;
		}
	}
	return ( t * 1.0) / ( t + f ) ;
}

// plot data and class
void plot_binary ( Mat &data , Mat & classes , string name ) {
	Mat plot ( size , size , CV_8UC3 ) ;
	plot.setTo ( Scalar (255.0 ,255.0 ,255.0) ) ;
	for ( int i = 0; i < data.rows ; i ++) {
		float x = data.at < float >( i ,0) * size ;
		float y = data.at < float >( i ,1) * size ;
		if ( classes.at < float >( i , 0) > 0) {
			circle ( plot , Point (x , y ) , 2 , CV_RGB (255 ,0 ,0) ,1) ;
		} else {
			circle ( plot , Point (x , y ) , 2 , CV_RGB (0 ,255 ,0) ,1) ;
		}
	}
	imshow ( name , plot ) ;
}

// function to learn
int f ( float x , float y , int equation ) {
	switch ( equation ) {
	case 0:
		return y > sin ( x *10) ? -1 : 1;
		break ;
	case 1:
		return y > cos ( x * 10) ? -1 : 1;
		break ;
	case 2:
		return y > 2* x ? -1 : 1;
		break ;
	case 3:
		return y > tan ( x *10) ? -1 : 1;
		break ;
	default :
		return y > cos ( x *10) ? -1 : 1;
	}
}

// label data with equation
Mat labelData ( Mat points , int equation ) {
	Mat labels ( points.rows , 1 , CV_32FC1 ) ;
	for ( int i = 0; i < points.rows ; i ++) {
		float x = points.at < float >( i ,0) ;
		float y = points.at < float >( i ,1) ;
		labels.at < float >( i , 0) = f (x , y , equation ) ;
	}
	return labels ;
}

//Support Vector Machine
void svm ( Mat & trainingData , Mat & trainingClasses , Mat & testData , Mat &
		testClasses ) {
	CvSVMParams param = CvSVMParams () ;
	param.svm_type = CvSVM :: C_SVC ;
	param.kernel_type = CvSVM :: RBF ; // CvSVM :: RBF , CvSVM :: LINEAR ...
	param.degree = 0; // for poly
	param.gamma = 20; // for poly / rbf / sigmoid
	param.coef0 = 0; // for poly / sigmoid
	param.C = 7; // for CV_SVM_C_SVC , CV_SVM _EPS_SVR and CV_SVM_NU_SVR
	param.nu = 0.0; // for CV_SVM_NU_SVC , CV_SVM_ONE_CLASS , and CV_SVM_NU_SVR
	param.p = 0.0; // for CV_SV M_EPS_SV R
	param.class_weights = NULL ; // for CV_SVM_C_SVC
	param.term_crit.type = CV_TERMCRIT_ITER + CV_TERMCRIT_EPS ;
	param.term_crit.max_iter = 1000;
	param.term_crit.epsilon = 1e-6;
	// SVM training ( use train auto for OpenCV >=2.0)
	CvSVM svm ( trainingData , trainingClasses , Mat () , Mat () , param ) ;
	Mat predicted ( testClasses.rows , 1 , CV_32F ) ;
	for ( int i = 0; i < testData.rows ; i ++) {
		Mat sample = testData.row ( i ) ;
		predicted.at<float>( i , 0) = svm.predict ( sample ) ;
	}
	cout << " Accuracy_ { SVM } = " << evaluate ( predicted , testClasses ) << endl ;
	plot_binary ( testData , predicted , " Predictions SVM " ) ;
	// plot support vectors
	if ( plotSupportVectors ) {
		Mat plot_sv ( size , size , CV_8UC3 ) ;
		plot_sv.setTo ( Scalar (255.0 ,255.0 ,255.0) ) ;
		int svec_count = svm.get_support_vector_count() ;
		for ( int vecNum = 0; vecNum < svec_count ; vecNum ++) {
			const float * vec = svm.get_support_vector( vecNum ) ;
			circle ( plot_sv , Point ( vec [0]* size , vec [1]* size ) , 3 , CV_RGB (0 , 0 , 0) ) ;
		}
		imshow ( " Support Vectors " , plot_sv ) ;
	}
}

//Neural Network - Multi-Layer Perceptrons
void mlp ( Mat & trainingData , Mat & trainingClasses , Mat & testData , Mat &
		testClasses ) {
	Mat layers = Mat (4 , 1 , CV_32SC1 ) ;

	layers.row (0) = Scalar (2);
	layers.row (1) = Scalar (10);
	layers.row (2) = Scalar (15);
	layers.row (3) = Scalar (1);
	CvANN_MLP mlp ;
	CvANN_MLP_TrainParams params;
	CvTermCriteria criteria;
	criteria.max_iter = 100;
	criteria.epsilon = 0.00001f;
	criteria.type = CV_TERMCRIT_ITER|CV_TERMCRIT_EPS;
	params.train_method = CvANN_MLP_TrainParams::BACKPROP ;
	params.bp_dw_scale = 0.05f;
	params.bp_moment_scale = 0.05f;
	params.term_crit = criteria ;
	mlp.create ( layers ) ;
	// train
	mlp.train ( trainingData , trainingClasses , Mat () , Mat () , params ) ;
	Mat response (1 , 1 , CV_32FC1 ) ;
	Mat predicted ( testClasses.rows , 1 , CV_32F ) ;
	for ( int i = 0; i < testData.rows ; i ++) {
		Mat response (1 , 1 , CV_32FC1 ) ;
		Mat sample = testData.row ( i ) ;
		mlp.predict ( sample , response ) ;
		predicted.at < float >( i ,0) = response.at < float >(0 ,0) ;
	}
	cout << " Accuracy_ { MLP } = " << evaluate ( predicted , testClasses ) << endl ;
	plot_binary ( testData , predicted , "Predictions Backpropagation" ) ;
}

//K-nearest neighbor
void knn ( Mat & trainingData , Mat & trainingClasses , Mat & testData , Mat &
		testClasses , int K ) {
	CvKNearest knn ( trainingData , trainingClasses , Mat () , false , K ) ;
	Mat predicted ( testClasses.rows , 1 , CV_32F ) ;
	for ( int i = 0; i < testData.rows ; i ++) {
		const Mat sample = testData.row ( i ) ;
		predicted.at < float >( i ,0) = knn.find_nearest ( sample , K ) ;
	}
	cout << " Accuracy_ { KNN } = " << evaluate ( predicted , testClasses ) << endl ;
	plot_binary ( testData , predicted , " Predictions KNN " ) ;
}

//Bayesian Classifier
void bayes ( Mat & trainingData , Mat & trainingClasses , Mat & testData , Mat
		& testClasses ) {
	CvNormalBayesClassifier bayes ( trainingData , trainingClasses ) ;
	Mat predicted ( testClasses.rows , 1 , CV_32F ) ;
	for ( int i = 0; i < testData.rows ; i ++) {
		const Mat sample = testData.row ( i ) ;
		predicted.at < float > (i , 0) = bayes.predict ( sample ) ;
	}
	cout << " Accuracy_ { BAYES } = " << evaluate ( predicted , testClasses ) << endl ;
	plot_binary ( testData , predicted , " Predictions Bayes " ) ;
}

//Decision Tree
void decisiontree ( Mat & trainingData , Mat & trainingClasses , Mat & testData ,
		Mat & testClasses ) {
	CvDTree dtree ;
	Mat var_type (3 , 1 , CV_8U ) ;
	// define attributes as numerical
	var_type.at < unsigned int >(0 ,0) = CV_VAR_NUMERICAL;
	var_type.at < unsigned int >(0 ,1) = CV_VAR_NUMERICAL ;
	// define output node as numerical
	var_type.at < unsigned int >(0 ,2) = CV_VAR_NUMERICAL;
	dtree.train ( trainingData , CV_ROW_SAMPLE , trainingClasses , Mat () , Mat () ,
			var_type , Mat () , CvDTreeParams () ) ;
	Mat predicted ( testClasses.rows , 1 , CV_32F ) ;
	for ( int i = 0; i < testData.rows ; i ++) {
		const Mat sample = testData.row ( i ) ;
		CvDTreeNode * prediction = dtree.predict ( sample ) ;
		predicted.at < float > (i , 0) = prediction->value ;
	}
	cout << " Accuracy_ { TREE } = " << evaluate ( predicted , testClasses ) << endl ;
	plot_binary ( testData , predicted , " Predictions tree " ) ;
}

void MLearn::runML(){
	Mat trainingData ( numTrainingPoints , 2 , CV_32FC1 ) ;
	Mat testData ( numTestPoints , 2 , CV_32FC1 ) ;
	randu ( trainingData ,0 ,1) ;
	randu ( testData ,0 ,1) ;
	Mat trainingClasses = labelData ( trainingData , eq ) ;
	Mat testClasses = labelData ( testData , eq ) ;
	plot_binary ( trainingData , trainingClasses , " Training Data " ) ;
	plot_binary ( testData , testClasses , " Test Data " ) ;
	svm ( trainingData , trainingClasses , testData , testClasses ) ;
	mlp ( trainingData , trainingClasses , testData , testClasses ) ;
	knn ( trainingData , trainingClasses , testData , testClasses , 3) ;
	bayes ( trainingData , trainingClasses , testData , testClasses ) ;
	decisiontree ( trainingData , trainingClasses , testData , testClasses ) ;
	waitKey () ;
}
