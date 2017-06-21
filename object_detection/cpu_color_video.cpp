#include "cpu_color_video.hpp"
#include <iostream>
 
using namespace std;
using namespace cv;

bool BY_RADIUS(Circle first, Circle second) {
	return first.radius <= second.radius;
}

void findLargest(int* num_objects, vector<Circle>* circles, vector<Circle>* key_circles) {
	int size = circles->size();
	*num_objects = min(*num_objects, size);

	for (int i = 1; i <= *num_objects; i++) {
		key_circles->push_back((*circles)[size - i]);
	}
}

int fetchCenters(Point2f centers[], IplImage *frame) {
	IplImage *blobs = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	Mat temp_blobs, red_blobs, lower_red_blobs, upper_red_blobs, green_blobs;
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
	vector<Circle> red_circles(red_count), green_circles(green_count);

	for (int i = 0; i < red_count; i++) {
		approxPolyDP(Mat(red_contours[i]), red_poly[i], 3, true);
		minEnclosingCircle((Mat) red_poly[i], red_circles[i].center, red_circles[i].radius);
	}

	for (int i = 0; i < green_count; i++) {
		approxPolyDP(Mat(green_contours[i]), green_poly[i], 3, true);
		minEnclosingCircle((Mat) green_poly[i], green_circles[i].center, green_circles[i].radius);
	}

	red_count = 5, green_count = 5;
	vector<Circle> key_red_circles(red_count), key_green_circles(green_count);

	sort(red_circles.begin(), red_circles.end(), BY_RADIUS);
	sort(green_circles.begin(), green_circles.end(), BY_RADIUS);

	findLargest(&red_count, &red_circles, &key_red_circles);
	findLargest(&green_count, &green_circles, &key_green_circles);
	int total_count = red_count + green_count;

	cout << "\tKEY RED CIRCLES" << endl << "\t";
	for (int i = 0; i < total_count; i++) {
		if (key_red_circles[i].radius > 20) {
			cout << "[(" << key_red_circles[i].center.x << ", " << key_red_circles[i].center.y << ")\t";
			cout << key_red_circles[i].radius << "]\t";
		}
	}

	cout << endl;

	cout << "\tKEY GREEN CIRCLES" << endl << "\t";
	for (int i = 0; i < total_count; i++) {
		if (key_green_circles[i].radius > 20) {
			cout << "[(" << key_green_circles[i].center.x << ", " << key_green_circles[i].center.y << ")\t";
			cout << key_green_circles[i].radius << "]\t";
		}
	}

	cout << endl;

	// combine red and green arrays into one array and fill rest of array with -1,-1
	int size = sizeof (Point2f*) / sizeof (Point2f);

	for (int i = 0; i < size; i++) {
		if (i < red_count) {
			centers[i] = key_red_circles[i].center;
		} else if (i < total_count) {
			centers[i] = key_green_circles[i - red_count].center;
		} else {
			centers[i] = Point2f(-1, -1);
		}
	}

	// return number of detected objects
	return total_count;
}
