/*
 * stdafx.h
 *
 *  Created on: Nov 27, 2013
 *      Author: jason
 */
#ifndef STDAFX_H_INCLUDED
#define STDAFX_H_INCLUDED

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <fstream>
#include <deque>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <cstdarg>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unordered_map>
#include <map>
#include <unistd.h>
#include <memory>

#define BOOST_SYSTEM_NO_DEPRECATED
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include "boost/timer.hpp"

namespace fs = boost::filesystem;

using namespace std;
using namespace cv;

#endif
