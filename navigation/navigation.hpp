#ifndef NAVIGATION
#define NAVIGATION

#include <iostream>
#include <iomanip>
#include <map>
#include <stdio.h>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "../object_detection/detection_framework.hpp"


void update_heading(double theta, double magnitude);
IplImage* query_image();
double dist_from_org(Point2f* point);
#endif
