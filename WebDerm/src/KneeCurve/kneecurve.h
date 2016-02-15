/*
 * kneecurve.h
 *
 *  Created on: Jan 23, 2015
 *      Author: jason
 */

#ifndef KNEECURVE_H_
#define KNEECURVE_H_

#include "../global.h"

class KneeCurve {
	double maxDist;
	int bestIdx;

public:
	int kneeCurvePoint(deque<double> &vec);
	int kneeCurvePoint(vector<double> &vec);
	int kneeCurvePoint(vector<int> &vec);
	double getMaxDist();
	void removeOutliers(deque<double> &vec, double percent);
	void removeOutliers(vector<double> &vec, double percent);
	void removeOutliers(deque<int> &vec, double percent);

	void loadVectorFile(String path, deque<double> &vec);

	Mat filterKneePt(Mat src, double thresh=0.0, double shift=1.0);

	/* save_graph.cpp */
	void save_graph(String out, vector<double> &vec);
	void save_graph(String out, deque<double> &vec);
	void save_graph(String out, vector<int> &vec);
};

#endif /* KNEECURVE_H_ */
