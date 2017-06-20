#include <stdio.h>
#include <iostream>
#include <map>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

int findLargest(int size, vector<float>* radii, vector<Point2f>* centers, vector<float>* key_radii, vector<Point2f>* key_centers) {
	int flag = 0, count = 0, min_radius = 20; // min_radius in pixels

	for (int i = 0; i < size; i++) {
		flag = 0;

		for (int j = 0; j < 5; j++) {
			if ((*radii)[i] > (*key_radii)[j] && flag == 0 && (*radii)[i] > min_radius) {
				(*key_radii)[j] = (*radii)[i];
				(*key_centers)[j] = (*centers)[i];
				flag = 1;
				count++;
			}
		}
	}

	return count;
}

int fetchCenters(Point2f centers[], IplImage *frame) {
	// calculate size of array
	IplImage *blobs = NULL;

	// check frame was correctly loaded
	if (blobs == NULL) {
		printf("error");
		return -1;
	}

	blobs = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	Mat temp_blobs, red_blobs, green_blobs, lower_red_blobs, upper_red_blobs;

	// Creating a Mat version of the current frame
	cvarrToMat(blobs).copyTo(temp_blobs);	

	// Finding the color blobs (needs to be a colored image) using Inrange
	inRange(temp_blobs, Scalar(0, 150, 150), Scalar(25, 250, 250), lower_red_blobs);
	inRange(temp_blobs, Scalar(155, 70, 70), Scalar(180, 200, 200), upper_red_blobs);
	inRange(temp_blobs, Scalar(50, 80, 80), Scalar(130, 255, 255), green_blobs);

	bitwise_or(lower_red_blobs, upper_red_blobs, red_blobs);

	vector<vector<Point>> red_contours, green_contours;
	vector<Vec4i> red_hierarchy, green_hierarchy;
	findContours(red_blobs, red_contours, red_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	findContours(green_blobs, green_contours, green_hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// calculate min enclosing circles for every red and green contour
	int red_count = red_contours.size(), green_count = green_contours.size();
	vector<vector<Point>> red_poly(red_count), green_poly(green_count);
	vector<Point2f> red_centers(red_count), green_centers(green_count);
	vector<float> red_radii(red_count), green_radii(green_count);

	for (int i = 0; i < red_count; i++) {
		approxPolyDP(Mat(red_contours[i]), red_poly[i], 3, true);
		minEnclosingCircle((Mat) red_poly[i], red_centers[i], red_radii[i]);
	}

	for (int i = 0;i < green_count; i++) {
		approxPolyDP(Mat(green_contours[i]), green_poly[i], 3, true);
		minEnclosingCircle((Mat) green_poly[i], green_centers[i], green_radii[i]);
	}

	vector<float> key_red_radii(5), key_green_radii(5);
	vector<Point2f> key_red_centers(5), key_green_centers(5);

	red_count = findLargest(red_count, &red_radii, &red_centers, &key_red_radii, &key_red_centers);
	green_count = findLargest(green_count, &green_radii, &green_centers, &key_green_radii, &key_green_centers);
	int total_count = red_count + green_count;

	// combine red and green arrays into one array and fill rest of array with -1,-1
	int size = sizeof (centers) / sizeof (Point2f);

	for (int i = 0; i < size; i++) {
		if (i < red_count) {
			centers[i] = key_red_centers[i];
		} else if (i < total_count) {
			centers[i] = key_green_centers[i - red_count];
		} else {
			centers[i] = Point2f(-1, -1);
		}
	}

	// return number of detected objects
	return total_count;
}
